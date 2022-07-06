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
string(REGEX MATCH "#define VERSION \"([0-9]+).([0-9]+)\"" FIRST_LINE "${BytemanVersionHeader}")
set(NEW_VER_MAJOR ${CMAKE_MATCH_1})
set(NEW_VER_MINOR ${CMAKE_MATCH_2})
string(REGEX MATCH "#define VERSION_BUILD \"([0-9]+)\"" FIRST_LINE "${BytemanVersionHeader}")
set(NEW_VER_BUILD ${CMAKE_MATCH_1})

file(READ "helpOutput.log" HelpOutputLog)
file(READ "helpAssemblyOutput.log" HelpAssemblyOutputLog)
file(READ "helpBlankOutput.log" HelpBlankOutputLog)
file(READ "helpChangeOutput.log" HelpChangeOutputLog)
file(READ "helpDeviceOutput.log" HelpDeviceOutputLog)
file(READ "helpExitOutput.log" HelpExitOutputLog)
file(READ "helpHelpOutput.log" HelpHelpOutputLog)
file(READ "helpInputOutput.log" HelpInputOutputLog)
file(READ "helpMergeOutput.log" HelpMergeOutputLog)
file(READ "helpOutputOutput.log" HelpOutputOutputLog)
file(READ "helpRegionOutput.log" HelpRegionOutputLog)
file(READ "helpStdinOutput.log" HelpStdinOutputLog)
file(READ "helpVerboseOutput.log" HelpVerboseOutputLog)
file(READ "helpWarnOutput.log" HelpWarnOutputLog)
file(READ "README.md" ReadMe)
string(REGEX REPLACE "\$> byteman.exe -help[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help\r\n```\r\n${HelpOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help assembly[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help assembly\r\n```\r\n${HelpAssemblyOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help blank[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help blank\r\n```\r\n${HelpBlankOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help change[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help change\r\n```\r\n${HelpChangeOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help device[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help device\r\n```\r\n${HelpDeviceOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help exit[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help exit\r\n```\r\n${HelpExitOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help help[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help help\r\n```\r\n${HelpHelpOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help input[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help input\r\n```\r\n${HelpInputOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help merge[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help merge\r\n```\r\n${HelpMergeOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help output[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help output\r\n```\r\n${HelpOutputOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help region[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help region\r\n```\r\n${HelpRegionOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help stdin[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help stdin\r\n```\r\n${HelpStdinOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help verbose[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help verbose\r\n```\r\n${HelpVerboseOutputLog}\r\n```" ReadMe "${ReadMe}")
string(REGEX REPLACE "\$> byteman.exe -help warn[\r\n|\r|\n]```[\r\n|\r|\n]([\s\S]*)[\r\n|\r|\n]```" "\$> byteman.exe -help warn\r\n```\r\n${HelpWarnOutputLog}\r\n```" ReadMe "${ReadMe}")
file(WRITE "README.md" "${ReadMe}")
file(WRITE commitHelpMessage
"#!/usr/bin/env bash
git config --local user.email \"action@github.com\"
git config --local user.name \"GitHub Action\"
git add README.md
git commit -m \"Usage #${NEW_VER_BUILD}\"        
until git push &> /dev/null
do
  until git pull --rebase &> /dev/null
  do
	sleep 1
	git status
  done
done
")
