#!/bin/bash -e

REPO_PATH=https://github.com/lubkoll/FunG
DOC_PATH=build/doc
HTML_PATH=build/doc/html
COMMIT_USER="Documentation Generator"
COMMIT_EMAIL="lars.lubkoll@posteo.de"
CHANGESET=$(git rev-parse --verify HEAD)

# Get a clean version of the HTML documentation repo.
rm -rf ${HTML_PATH}
mkdir -p ${DOC_PATH}
git clone -b gh-pages "${REPO_PATH}" --single-branch ${DOC_PATH}
# rm all the files through git to prevent stale files.
#mkdir -p ${HTML_PATH}
cd ${HTML_PATH}
git rm -rf .
cd -
# Generate the HTML documentation.
doxygen

# Create and commit the documentation repo.
cd ${HTML_PATH}
git add --ignore-removal .
git config user.name "${COMMIT_USER}"
git config user.email "${COMMIT_EMAIL}"
git commit -m "Automated documentation build for changeset ${CHANGESET}."
git push origin gh-pages
cd -
