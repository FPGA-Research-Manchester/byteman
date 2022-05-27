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

file(READ "CMakeLists.txt" CMakeLists)
string(REGEX MATCH "byteman VERSION ([0-9]+).([0-9]+).([0-9]+)" FIRST_LINE "${CMakeLists}")
set(NEW_VER_MAJOR ${CMAKE_MATCH_1})
set(NEW_VER_MINOR ${CMAKE_MATCH_2})
math(EXPR NEW_VER_BUILD "${CMAKE_MATCH_3}+1")
string(REGEX REPLACE "byteman VERSION ([0-9]+).([0-9]+).([0-9]+)" "byteman VERSION ${NEW_VER_MAJOR}.${NEW_VER_MINOR}.${NEW_VER_BUILD}" CMakeLists "${CMakeLists}")
file(WRITE "CMakeLists.txt" "${CMakeLists}")
file(READ "src/byteman.h" BytemanHeader)
string(REGEX REPLACE "#define VERSION \"([0-9]+).([0-9]+)\"" "#define VERSION \"${NEW_VER_MAJOR}.${NEW_VER_MINOR}\"" BytemanHeader "${BytemanHeader}")
string(REGEX REPLACE "#define VERSION_BUILD \"([0-9]+)\"" "#define VERSION_BUILD \"${NEW_VER_BUILD}\"" BytemanHeader "${BytemanHeader}")
file(WRITE "src/byteman.h" "${BytemanHeader}")
file(READ "Resources/Doxyfile.in" DoxyFile)
string(REGEX REPLACE "PROJECT_NUMBER = \"([0-9]+).([0-9]+) \(Build #([0-9]+)\)\"" "PROJECT_NUMBER = \"${NEW_VER_MAJOR}.${NEW_VER_MINOR} (Build #${NEW_VER_BUILD})\"" DoxyFile "${DoxyFile}")
file(WRITE "Resources/Doxyfile.in" "${DoxyFile}")
file(READ "README.md" ReadMe)
string(REGEX REPLACE "byteman-v([0-9]+).([0-9]+)" "byteman-v${NEW_VER_MAJOR}.${NEW_VER_MINOR}" ReadMe "${ReadMe}")
string(REGEX REPLACE "Build%20([0-9]+)-" "Build%20${NEW_VER_BUILD}-" ReadMe "${ReadMe}")
file(WRITE "README.md" "${ReadMe}")
file(WRITE commitVersion
"#!/usr/bin/env bash
git config --local user.email \"action@github.com\"
git config --local user.name \"GitHub Action\"
git add src/byteman.h
git add CMakeLists.txt
git add Resources/Doxyfile.in
git add README.md
git commit -m \"Build #${NEW_VER_BUILD}\"        
until git push &> /dev/null
do
  until git pull --rebase &> /dev/null
  do
	sleep 1
	git status
  done
done
")
