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

add_test(NAME Test_XS7_Spartan_6 COMMAND byteman xs7 -d xc7s6 -b 0xff -test equal 0xffffffff -i xc7s6.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Spartan_15 COMMAND byteman xs7 -d xc7s15 -b 0xff -test equal 0xffffffff -i xc7s15.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Spartan_25 COMMAND byteman xs7 -d xc7s25 -b 0xff -test equal 0xffffffff -i xc7s25.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Spartan_50 COMMAND byteman xs7 -d xc7s50 -b 0xff -test equal 0xffffffff -i xc7s50.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Spartan_75 COMMAND byteman xs7 -d xc7s75 -b 0xff -test equal 0xffffffff -i xc7s75.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Spartan_100 COMMAND byteman xs7 -d xc7s100 -b 0xff -test equal 0xffffffff -i xc7s100.bit -test notequal 0xffffffff)

add_test(NAME Test_XS7_Artix_12 COMMAND byteman xs7 -d xc7a12 -b 0xff -test equal 0xffffffff -i xc7a12t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_15 COMMAND byteman xs7 -d xc7a15 -b 0xff -test equal 0xffffffff -i xc7a15t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_25 COMMAND byteman xs7 -d xc7a25 -b 0xff -test equal 0xffffffff -i xc7a25t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_35 COMMAND byteman xs7 -d xc7a35 -b 0xff -test equal 0xffffffff -i xc7a35t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_50 COMMAND byteman xs7 -d xc7a50 -b 0xff -test equal 0xffffffff -i xc7a50t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_75 COMMAND byteman xs7 -d xc7a75 -b 0xff -test equal 0xffffffff -i xc7a75t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_100 COMMAND byteman xs7 -d xc7a100 -b 0xff -test equal 0xffffffff -i xc7a100t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Artix_200 COMMAND byteman xs7 -d xc7a200 -b 0xff -test equal 0xffffffff -i xc7a200t.bit -test notequal 0xffffffff)

add_test(NAME Test_XS7_Zynq_007 COMMAND byteman xs7 -d xc7z007 -b 0xff -test equal 0xffffffff -i xc7z007.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_010 COMMAND byteman xs7 -d xc7z010 -b 0xff -test equal 0xffffffff -i xc7z010.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_012 COMMAND byteman xs7 -d xc7z012 -b 0xff -test equal 0xffffffff -i xc7z012.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_014 COMMAND byteman xs7 -d xc7z014 -b 0xff -test equal 0xffffffff -i xc7z014.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_015 COMMAND byteman xs7 -d xc7z015 -b 0xff -test equal 0xffffffff -i xc7z015.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_020 COMMAND byteman xs7 -d xc7z020 -b 0xff -test equal 0xffffffff -i xc7z020.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_030 COMMAND byteman xs7 -d xc7z030 -b 0xff -test equal 0xffffffff -i xc7z030.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_035 COMMAND byteman xs7 -d xc7z035 -b 0xff -test equal 0xffffffff -i xc7z035.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_045 COMMAND byteman xs7 -d xc7z045 -b 0xff -test equal 0xffffffff -i xc7z045.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Zynq_100 COMMAND byteman xs7 -d xc7z100 -b 0xff -test equal 0xffffffff -i xc7z100.bit -test notequal 0xffffffff)

add_test(NAME Test_XS7_Kintex_70 COMMAND byteman xs7 -d xc7k70 -b 0xff -test equal 0xffffffff -i xc7k70t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_160 COMMAND byteman xs7 -d xc7k160 -b 0xff -test equal 0xffffffff -i xc7k160t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_325 COMMAND byteman xs7 -d xc7k325 -b 0xff -test equal 0xffffffff -i xc7k325t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_355 COMMAND byteman xs7 -d xc7k355 -b 0xff -test equal 0xffffffff -i xc7k355t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_410 COMMAND byteman xs7 -d xc7k410 -b 0xff -test equal 0xffffffff -i xc7k410t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_420 COMMAND byteman xs7 -d xc7k420 -b 0xff -test equal 0xffffffff -i xc7k420t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Kintex_480 COMMAND byteman xs7 -d xc7k480 -b 0xff -test equal 0xffffffff -i xc7k480t.bit -test notequal 0xffffffff)

add_test(NAME Test_XS7_Virtex_585 COMMAND byteman xs7 -d xc7v585 -b 0xff -test equal 0xffffffff -i xc7v585t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_2000 COMMAND byteman xs7 -d xc7v2000 -b 0xff -test equal 0xffffffff -i xc7v2000t.bit -test notequal 0xffffffff)
#add_test(NAME Test_XS7_Virtex_H580 COMMAND byteman xs7 -d xc7vh580 -b 0xff -test equal 0xffffffff -i xc7vh580t.bit -test notequal 0xffffffff)
#add_test(NAME Test_XS7_Virtex_H870 COMMAND byteman xs7 -d xc7vh870 -b 0xff -test equal 0xffffffff -i xc7vh870t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X330 COMMAND byteman xs7 -d xc7vx330 -b 0xff -test equal 0xffffffff -i xc7vx330t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X415 COMMAND byteman xs7 -d xc7vx415 -b 0xff -test equal 0xffffffff -i xc7vx415t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X485 COMMAND byteman xs7 -d xc7vx485 -b 0xff -test equal 0xffffffff -i xc7vx485t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X550 COMMAND byteman xs7 -d xc7vx550 -b 0xff -test equal 0xffffffff -i xc7vx550t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X690 COMMAND byteman xs7 -d xc7vx690 -b 0xff -test equal 0xffffffff -i xc7vx690t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X980 COMMAND byteman xs7 -d xc7vx980 -b 0xff -test equal 0xffffffff -i xc7vx980t.bit -test notequal 0xffffffff)
add_test(NAME Test_XS7_Virtex_X1140 COMMAND byteman xs7 -d xc7vx1140 -b 0xff -test equal 0xffffffff -i xc7vx1140t.bit -test notequal 0xffffffff)

#The following tests test merging
#first they set main buffer to 0x55555555s everywhere.
#then the second bitstream is all initialized to 0xFCFCFCFC
#The first bitstream is tested to != 0xA5A5A5A5
#The second bitstream is merged into the first one using XOR merging
#The first bitstream is tested to == 0xA5A5A5A5
add_test(NAME Test_XS7_Merge_XOR_Spartan_6 COMMAND byteman xs7 -d first,second xc7s6 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Spartan_15 COMMAND byteman xs7 -d first,second xc7s15 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Spartan_25 COMMAND byteman xs7 -d first,second xc7s25 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Spartan_50 COMMAND byteman xs7 -d first,second xc7s5 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Spartan_75 COMMAND byteman xs7 -d first,second xc7s75 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Spartan_100 COMMAND byteman xs7 -d first,second xc7s100 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XS7_Merge_XOR_Artix_12 COMMAND byteman xs7 -d first,second xc7a12 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_15 COMMAND byteman xs7 -d first,second xc7a15 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_25 COMMAND byteman xs7 -d first,second xc7a25 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_35 COMMAND byteman xs7 -d first,second xc7a35 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_50 COMMAND byteman xs7 -d first,second xc7a50 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_75 COMMAND byteman xs7 -d first,second xc7a75 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_100 COMMAND byteman xs7 -d first,second xc7a100 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Artix_200 COMMAND byteman xs7 -d first,second xc7a200 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XS7_Merge_XOR_Zynq_007 COMMAND byteman xs7 -d first,second xc7z007 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_010 COMMAND byteman xs7 -d first,second xc7z010 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_012 COMMAND byteman xs7 -d first,second xc7z012 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_014 COMMAND byteman xs7 -d first,second xc7z014 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_015 COMMAND byteman xs7 -d first,second xc7z015 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_020 COMMAND byteman xs7 -d first,second xc7z020 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_030 COMMAND byteman xs7 -d first,second xc7z030 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_035 COMMAND byteman xs7 -d first,second xc7z035 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_045 COMMAND byteman xs7 -d first,second xc7z045 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Zynq_100 COMMAND byteman xs7 -d first,second xc7z100 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XS7_Merge_XOR_Kintex_70 COMMAND byteman xs7 -d first,second xc7k70 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_160 COMMAND byteman xs7 -d first,second xc7k160 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_325 COMMAND byteman xs7 -d first,second xc7k325 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_355 COMMAND byteman xs7 -d first,second xc7k355 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_410 COMMAND byteman xs7 -d first,second xc7k410 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_420 COMMAND byteman xs7 -d first,second xc7k420 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Kintex_480 COMMAND byteman xs7 -d first,second xc7k480 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XS7_Merge_XOR_Virtex_585 COMMAND byteman xs7 -d first,second xc7v585 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_2000 COMMAND byteman xs7 -d first,second xc7v2000 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
#add_test(NAME Test_XS7_Merge_XOR_Virtex_H580 COMMAND byteman xs7 -d first,second xc7vh580 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
#add_test(NAME Test_XS7_Merge_XOR_Virtex_H870 COMMAND byteman xs7 -d first,second xc7vh870 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X330 COMMAND byteman xs7 -d first,second xc7vx330 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X415 COMMAND byteman xs7 -d first,second xc7vx415 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X485 COMMAND byteman xs7 -d first,second xc7vx485 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X550 COMMAND byteman xs7 -d first,second xc7vx550 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X690 COMMAND byteman xs7 -d first,second xc7vx690 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X980 COMMAND byteman xs7 -d first,second xc7vx980 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XS7_Merge_XOR_Virtex_X1140 COMMAND byteman xs7 -d first,second xc7vx1140 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)