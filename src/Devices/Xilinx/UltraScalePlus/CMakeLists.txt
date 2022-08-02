###############################################################################
# Copyright 2022 Kristiyan Manev (University of Manchester)
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

#Add all sources for Xilinx US+
set(XUSP_SRC
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlus.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Fabric.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Input.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Output.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Merge.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Blank.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Devices.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Assembler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Change.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XUSP_Test.cpp
)

#build byteman natively (without the library)
target_sources(byteman PRIVATE ${XUSP_SRC})

#Create a Xilinx US+ library, disabled because the github cross compilation can't link them and throws errors
#add_library(XUSP_LIB STATIC ${XUSP_SRC} ${DEVICES_SHARED_SRC})
#target_compile_definitions(XUSP_LIB PUBLIC XUSP_LIB)

