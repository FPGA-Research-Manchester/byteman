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

add_test(NAME Test_XUS_Kintex_025 COMMAND byteman xus -d xcku025 -b 0xff -i xcku025.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_035 COMMAND byteman xus -d xcku035 -b 0xff -i xcku035.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_040 COMMAND byteman xus -d xcku040 -b 0xff -i xcku040.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_060 COMMAND byteman xus -d xcku060 -b 0xff -i xcku060.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_085 COMMAND byteman xus -d xcku085 -b 0xff -i xcku085.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_095 COMMAND byteman xus -d xcku095 -b 0xff -i xcku095.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Kintex_115 COMMAND byteman xus -d xcku115 -b 0xff -i xcku115.bit -test notequal 0xffffffff)

add_test(NAME Test_XUS_Virtex_065 COMMAND byteman xus -d xcvu065 -b 0xff -i xcvu065.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_080 COMMAND byteman xus -d xcvu080 -b 0xff -i xcvu080.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_095 COMMAND byteman xus -d xcvu095 -b 0xff -i xcvu095.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_125 COMMAND byteman xus -d xcvu125 -b 0xff -i xcvu125.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_160 COMMAND byteman xus -d xcvu160 -b 0xff -i xcvu160.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_190 COMMAND byteman xus -d xcvu190 -b 0xff -i xcvu190.bit -test notequal 0xffffffff)
add_test(NAME Test_XUS_Virtex_440 COMMAND byteman xus -d xcvu440 -b 0xff -i xcvu440.bit -test notequal 0xffffffff)

#The following tests test merging
#first they set main buffer to 0x55555555s everywhere.
#then the second bitstream is all initialized to 0xFCFCFCFC
#The first bitstream is tested to != 0xA5A5A5A5
#The second bitstream is merged into the first one using XOR merging
#The first bitstream is tested to == 0xA5A5A5A5
add_test(NAME Test_XUS_Merge_XOR_Kintex_025 COMMAND byteman xus -d first,second xcku025 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_035 COMMAND byteman xus -d first,second xcku035 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_040 COMMAND byteman xus -d first,second xcku040 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_060 COMMAND byteman xus -d first,second xcku060 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_085 COMMAND byteman xus -d first,second xcku085 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_095 COMMAND byteman xus -d first,second xcku095 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Kintex_115 COMMAND byteman xus -d first,second xcku115 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUS_Merge_XOR_Virtex_065 COMMAND byteman xus -d first,second xcvu065 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_080 COMMAND byteman xus -d first,second xcvu080 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_095 COMMAND byteman xus -d first,second xcvu095 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_125 COMMAND byteman xus -d first,second xcvu125 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_160 COMMAND byteman xus -d first,second xcvu160 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_190 COMMAND byteman xus -d first,second xcvu190 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUS_Merge_XOR_Virtex_440 COMMAND byteman xus -d first,second xcvu440 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

