/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                             */
/*   This file is part of the C++-library FunG.                              */
/*   Copyright 2015 Lars Lubkoll                                               */
/*                                                                             */
/*   FunG is free software: you can redistribute it and/or modify            */
/*   it under the terms of the GNU General Public License as published by      */
/*   the Free Software Foundation, either version 3 of the License, or         */
/*   (at your option) any later version.                                       */
/*                                                                             */
/*   FunG is distributed in the hope that it will be useful,                 */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/*   GNU General Public License for more details.                              */
/*                                                                             */
/*   You should have received a copy of the GNU General Public License         */
/*   along with FunG.  If not, see <http://www.gnu.org/licenses/>.           */
/*                                                                             */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FUNG_UTIL_EXCEPTIONS_HH
#define FUNG_UTIL_EXCEPTIONS_HH

#include <stdexcept>
#include <string>
#include <type_traits>

namespace FunG
{
  /**
   * \ingroup exceptions
   * \brief Exception for scalar function arguments that are outside the domain of the function.
   *
   * Example:
   * \code
   * if( x < 0 )
   *   throw OutOfDomainException("[0,inf[","Sqrt" x,__FILE__,__LINE__);
   * \endcode
   */
  class OutOfDomainException : public std::runtime_error
  {
  public:
    /**
     * \brief Constructor.
     *
     * \param range std::string that contains the mathematical expression for the valid range
     * \param function name of the function throwing this exception
     * \param value value outside range
     * \param file file containing the throwing code
     * \param line line containing the throwing code
     */
    template <class Value, class = std::enable_if_t<std::is_arithmetic<Value>::value> >
    OutOfDomainException(const std::string& function, const std::string& range, const Value& value, const std::string& file, const int line) :
      std::runtime_error(std::string("OutOfDomainException in ") + file + " at line " + std::to_string(line) + ".\n           " + function + ": Argument " + std::to_string(value) + " is outside range " + range + ".\n")
    {}
  };

  /**
   * \ingroup exceptions
   * \brief Exception for non-symmetric matrices if symmetric matrices are required
   */
  class NonSymmetricMatrixException : public std::runtime_error{
  public:
    /**
     * \brief Constructor.
     *
     * \param function name of the function throwing this exception
     * \param rows number of rows of matrix
     * \param cols number of columns of matrix
     * \param file file containing the throwing code
     * \param line line containing the throwing code
     */
    template <class Value, class = std::enable_if_t<std::is_arithmetic<Value>::value> >
    NonSymmetricMatrixException(const std::string& function, const Value& rows, const Value& cols, const std::string& file, const int line) :
      std::runtime_error(std::string("NonSymmetricMatrixException in ") + file + " at line " + std::to_string(line) + ".\n           " + function + ": Matrix of dimension " + std::to_string(rows) +
                         "x" + std::to_string(cols) + " is not symmetric.\n")
    {}
  };
}

#endif // FUNG_UTIL_EXCEPTIONS_HH
