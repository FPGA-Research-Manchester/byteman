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

# min cmake version is 3.10 because "target_sources" is used.
cmake_minimum_required(VERSION 3.10)

#next line has to be in the format ..."byteman VERSION X.Y.Z"... for the autoversioning
project(byteman VERSION 1.1.146)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_definitions(-DENABLEWARN) #enables -warn (warnings)
add_definitions(-DENABLELOGS) #enables -verbose (logging)

add_definitions(-DXS7) #Add Xilinx Series 7 devices in byteman
add_definitions(-DXS7SPARTAN)
add_definitions(-DXS7ARTIX)
add_definitions(-DXS7ZYNQ)
add_definitions(-DXS7KINTEX)
add_definitions(-DXS7VIRTEX)
add_definitions(-DXS7BOARDS)

add_definitions(-DXUS) #Add Xilinx UltraScale devices in byteman
add_definitions(-DXUSKINTEX)
add_definitions(-DXUSVIRTEX)

add_definitions(-DXUSP) #Add Xilinx UltraScale+ devices in byteman
add_definitions(-DXUSPARTIX)
add_definitions(-DXUSPKINTEX)
add_definitions(-DXUSPVIRTEX)
add_definitions(-DXUSPZYNQRF)
add_definitions(-DXUSPZYNQMP)
add_definitions(-DXUSPALVEO)
add_definitions(-DXUSPBOARDS)

set(BYTEMAN_ICON ${CMAKE_SOURCE_DIR}/Resources/icon.rc)
add_executable(byteman ${BYTEMAN_ICON})

add_subdirectory(src)

if(MSVC)
	set_target_properties(byteman PROPERTIES LINK_FLAGS "/STACK:4194304")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	#set_target_properties(byteman PROPERTIES LINK_FLAGS "/PROFILE") #If profiling is needed (again)
else()
	set_target_properties(byteman PROPERTIES LINK_FLAGS "-Wl,-z,stack-size=4194304")
endif()
