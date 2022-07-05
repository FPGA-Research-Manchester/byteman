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

#if !defined(NDEBUG)
#include "XilinxSeries7.h"

void XilinxSeries7::test(bool usableFramesOnly, bool testValueEqual, uint32_t testValue){
	log("Testing bitstream against value" + to_string(testValue) + ".");
	ensureInitializedBitstreamArrays();
	ensureFramesAre(usableFramesOnly, testValueEqual, testValue);
	log("Testing finished successfully.");
}
#endif