#/bin/bash

header="src/version.h"

set -u
set -e
# set -x

if [ -d .git ]; then
  branch=`git status -bsu no`
  revisioncount=`git log --oneline | wc -l`
  projectversion=`git describe --tags --long --always`
  # generate
  echo "const char* kVersion = \"${branch}-${projectversion%%-*}\";" \
    > "${header}.$$"
  # diff and mv if needed
  cmp "${header}.$$" "${header}" ||
    mv "${header}.$$" "${header}"
  # cleanup
  rm -f "${header}.$$"
fi

