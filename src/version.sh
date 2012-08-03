#/bin/bash

# Copyright 2012 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Update the git branch name at version header if possible
# It parse and update the c header file with following contents:
#   const char* kVersion = "ninja V0.1.3 ## develop";
# If the contents is not changed, the header file is not touched

header="src/version.h"

set -u  # undefined is an error
set -e  # exit on error
###XXX set -x   # be verbose

type git || exit 0  # OK, noop

if [ -d .git ]; then
  # get the branch name from git
  branch=$(git status -bsu no)
  # try to get the most recent tag that is reachable from a git commit
  projectversion=$(git describe --tags --dirty --always)

  # parse the current release version from header file
  version=$(awk '/kVersion/ {print $6}' "${header}")

  # write a the temporary header file
  echo "const char* kVersion = \"ninja ${version} ${branch%%.*}-${projectversion%%-*}\";"
  echo "const char* kVersion = \"ninja ${version} ${branch%%.*}\";" \
    > "${header}.$$"

  # diff with current header and mv if needed
  cmp -s "${header}.$$" "${header}" ||
    mv "${header}.$$" "${header}"

  # cleanup
  rm -f "${header}.$$"
  cat "${header}"
fi

