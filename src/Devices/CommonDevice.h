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

#ifndef COMMONDEVICE_H
#define COMMONDEVICE_H

#include<iostream>
#include<string>
#include<time.h>



class CommonDevice
{
	public:
		CommonDevice(){
			enableWarn = 1;
			enableLog = 0;
		};
		virtual ~CommonDevice(){
			
		};
		std::string instanceName;
		int enableWarn;
		int enableLog;
		inline void printMessage(std::string message){
			time_t timestamp = time(0);
			struct tm  tstruct;
			char	   timeChars[80];
			tstruct = *localtime(&timestamp);
			strftime(timeChars, sizeof(timeChars), "%d %b %Y %H:%M:%S @ ", &tstruct);
			std::cout << timeChars << instanceName << message << std::endl;
		}
		inline void warn(std::string message) {
			#ifdef ENABLEWARN
				if(enableWarn)
					CommonDevice::printMessage(std::string(" warning: ").append(message));
			#endif
		}
		inline void log(std::string message) {
			#ifdef ENABLELOGS
				if(enableLog)
					CommonDevice::printMessage(std::string("  info  : ").append(message));
			#endif
		}
};

#endif //COMMONDEVICE_H