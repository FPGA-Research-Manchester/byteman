###############################################################################
# Copyright 2022 Kristiyan Manev, University of Manchester
#
# Licensed under the Apache License, Version 2.0(the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###############################################################################

file(READ "VERSION" BytemanVersionHeader)
string(REGEX MATCH "#define VERSION_BUILD \"([0-9]+)\"" FIRST_LINE "${BytemanVersionHeader}")
set(NEW_VER_BUILD ${CMAKE_MATCH_1})

file(WRITE commitBinary
"#!/usr/bin/env bash
git config --local user.email \"action@github.com\"
git config --local user.name \"GitHub Action\"
git add ${CMAKE_ARGV3}
git commit -m \"Binaries #${NEW_VER_BUILD}\"        
until git push &> /dev/null
do
  until git pull --rebase &> /dev/null
  do
	sleep 1
	git status
  done
done
")
