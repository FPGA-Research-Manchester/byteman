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

inline void printResourceStringInfo(){
	cout << "Brakdown of X coordinates in device " + partName + ":" << endl;
	for(int r = 0 ; r < numberOfRows ; r++){
		cout << "\tRow " << r << " (counting from bottom up) is represented by Y coordinate range " << r * CLB_PER_CLOCK_REGION << "-" << ((r + 1) * CLB_PER_CLOCK_REGION - 1) << "." << endl;
		cout << "\tRow " << r << " has the following X coordinates with respective column resources:" << endl;
		for(int c = 0 ; ; c++){
			uint8_t resourceLetter = (uint8_t)resourceString[r][c];
			if(resourceLetter <= 1)
				break;
			if(LUT_isFrameUnusedForResourceLetter[resourceLetter])
				cout << "\t\t" << c << ": Unused/hidden resource" << endl;
			else
				cout << "\t\t" << c << ": " << LUT_typeOfFrameForResourceLetter[resourceLetter] << endl;
		}
		cout << endl;
	}
}

