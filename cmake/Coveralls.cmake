function(coveralls_setup _COVERAGE_SRCS _COVERALLS_UPLOAD)

	if (ARGC GREATER 2)
		set(_CMAKE_SCRIPT_PATH ${ARGN})
		message("Coveralls: Using alternate CMake script dir:${CMAKE_CURRENT_SOURCE_DIR}/cmake")
	else()
		set(_CMAKE_SCRIPT_PATH ${PROJECT_SOURCE_DIR}/cmake)
	endif()

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsClear.cmake")
		message(FATAL_ERROR "Coveralls: Missing${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsClear.cmake")
	endif()

	if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsGenerateGcov.cmake")
		message(FATAL_ERROR "Coveralls: Missing${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsGenerateGcov.cmake")
	endif()

	# When passing a CMake list to an external process, the list
	# will be converted from the format "1;2;3" to "1 2 3".
	# This means the script we're calling won't see it as a list
	# of sources, but rather just one long path. We remedy this
	# by replacing ";" with "*" and then reversing that in the script
	# that we're calling.
	# http://cmake.3232098.n2.nabble.com/Passing-a-CMake-list-quot-as-is-quot-to-a-custom-target-td6505681.html
	set(COVERAGE_SRCS_TMP ${_COVERAGE_SRCS})
	set(COVERAGE_SRCS "")
	foreach (COVERAGE_SRC ${COVERAGE_SRCS_TMP})
		set(COVERAGE_SRCS "${COVERAGE_SRCS}*${COVERAGE_SRC}")
	endforeach()

	#message("Coverage sources: ${COVERAGE_SRCS}")
	set(COVERALLS_FILE ${PROJECT_BINARY_DIR}/coveralls.json)

	add_custom_target(coveralls_generate

		# Zero the coverage counters.
		COMMAND ${CMAKE_COMMAND}
				-P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsClear.cmake"

		# Run regress tests.
		COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure

		# Generate Gcov and translate it into coveralls JSON.
		# We do this by executing an external CMake script.
		# (We don't want this to run at CMake generation time, but after compilation and everything has run).
		COMMAND ${CMAKE_COMMAND}
				-DCOVERAGE_SRCS="${COVERAGE_SRCS}" # TODO: This is passed like: "a b c", not "a;b;c"
				-DCOVERALLS_OUTPUT_FILE="${COVERALLS_FILE}"
				-DCOV_PATH="${PROJECT_BINARY_DIR}"
				-DPROJECT_ROOT="${PROJECT_SOURCE_DIR}"
				-P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CoverallsGenerateGcov.cmake"

		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		COMMENT "Generating coveralls output..."
		)

	if (_COVERALLS_UPLOAD)
		message("COVERALLS UPLOAD: ON")

		find_program(CURL_EXECUTABLE curl)

		if (NOT CURL_EXECUTABLE)
			message(FATAL_ERROR "Coveralls: curl not found! Aborting")
		endif()

		add_custom_target(coveralls_upload
			# Upload the JSON to coveralls.
			COMMAND ${CURL_EXECUTABLE}
					-S -F json_file=@${COVERALLS_FILE}
					https://coveralls.io/api/v1/jobs

			DEPENDS coveralls_generate

			WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
			COMMENT "Uploading coveralls output...")

		add_custom_target(coveralls DEPENDS coveralls_upload)
	else()
		message("COVERALLS UPLOAD: OFF")
		add_custom_target(coveralls DEPENDS coveralls_generate)
	endif()

endfunction()

macro(coveralls_turn_on_coverage)
	if(NOT (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		AND (NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
		message(FATAL_ERROR "Coveralls: Compiler ${CMAKE_C_COMPILER_ID} is not GNU gcc! Aborting... You can set this on the command line using CC=/usr/bin/gcc CXX=/usr/bin/g++ cmake <options> ..")
	endif()

	if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
		message(FATAL_ERROR "Coveralls: Code coverage results with an optimised (non-Debug) build may be misleading! Add -DCMAKE_BUILD_TYPE=Debug")
	endif()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endmacro()
