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

#include "CommonDevice2D.h"

using namespace std;

void CommonDevice2D::region(string params, Rect2D cmdRect){
	if(params.find("clear") != string::npos) //first do the clearing
		regionSelection.clear();
	if(params.find("add") != string::npos){ //before potential adding
		if(cmdRect.size.row <= 0 || cmdRect.size.col <= 0)
			throw runtime_error("Regions need to be of positive size.");
		regionSelection.push_back(cmdRect);
	}
}
