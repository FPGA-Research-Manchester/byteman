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

#include "XilinxConfigurationAccessPort.h"
#include <time.h>

XilinxConfigurationAccessPort::XilinxConfigurationAccessPort(){
	designName = "";
    partName = "";
}
XilinxConfigurationAccessPort::~XilinxConfigurationAccessPort(){
}

void XilinxConfigurationAccessPort::updateDateAndTime(){
	time_t timestamp = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&timestamp);
	strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
	fileDate = string(buf);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	fileTime = string(buf);
}