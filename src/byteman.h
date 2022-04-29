/******************************************************************************
 * Copyright 2022 Kristiyan Manev (University of Manchester)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#ifndef BYTEMAN_H
#define BYTEMAN_H
#include <iostream>
using namespace std;
//Any changes to version numbers should be done only in CMakeLists.txt, not here!
#define VERSION "0.2"
#define VERSION_BUILD "79"
enum Arch{
	ARCH_NULL = 0,
	XIL_SERIES6,
	XIL_SERIES7,
	XIL_US,
	XIL_USP,
	XIL_VERSAL
};
void printDeviceList();
void parseHelp(string, int=0, int=0);
void usage();
void usageArg(string);
void usage_verbose();
void usage_warn();
void usage_region();
void usage_blank();
void usage_change();
void usage_device();
void usage_help();
void usage_input();
void usage_merge();
void usage_output();
void usage_assembly();
void usage_end();
void usage_stdin();

#endif //BYTEMAN_H
