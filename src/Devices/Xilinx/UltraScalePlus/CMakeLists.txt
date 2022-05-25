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
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlusInput.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlusOutput.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlusDevices.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlusAssembler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/XilinxUltraScalePlusConfigurationAccessPort.cpp)

#build byteman natively (without the library)
target_sources(byteman PRIVATE ${XUSP_SRC})

#Create a Xilinx US+ library, disabled because the github cross compilation can't link them and throws errors
#add_library(XUSP_LIB STATIC ${XUSP_SRC} ${XILINX_SHARED_SRC} ${DEVICES_SHARED_SRC})
#target_compile_definitions(XUSP_LIB PUBLIC XUSP_LIB)

