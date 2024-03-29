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

add_test(NAME Test_XUSP_Artix_10 COMMAND byteman xusp -d xcau10p -b 0xff -test equal 0xffffffff -i xcau10p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_15 COMMAND byteman xusp -d xcau15p -b 0xff -test equal 0xffffffff -i xcau15p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_20 COMMAND byteman xusp -d xcau20p -b 0xff -test equal 0xffffffff -i xcau20p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_25 COMMAND byteman xusp -d xcau25p -b 0xff -test equal 0xffffffff -i xcau25p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Zynq_1 COMMAND byteman xusp -d xczu1 -b 0xff -test equal 0xffffffff -i xczu1.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_2 COMMAND byteman xusp -d xczu2 -b 0xff -test equal 0xffffffff -i xczu2.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_3 COMMAND byteman xusp -d xczu3 -b 0xff -test equal 0xffffffff -i xczu3.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_4 COMMAND byteman xusp -d xczu4 -b 0xff -test equal 0xffffffff -i xczu4.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_5 COMMAND byteman xusp -d xczu5 -b 0xff -test equal 0xffffffff -i xczu5.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_6 COMMAND byteman xusp -d xczu6 -b 0xff -test equal 0xffffffff -i xczu6.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_7 COMMAND byteman xusp -d xczu7 -b 0xff -test equal 0xffffffff -i xczu7.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_9 COMMAND byteman xusp -d xczu9 -b 0xff -test equal 0xffffffff -i xczu9.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_11 COMMAND byteman xusp -d xczu11 -b 0xff -test equal 0xffffffff -i xczu11.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_15 COMMAND byteman xusp -d xczu15 -b 0xff -test equal 0xffffffff -i xczu15.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_17 COMMAND byteman xusp -d xczu17 -b 0xff -test equal 0xffffffff -i xczu17.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_19 COMMAND byteman xusp -d xczu19 -b 0xff -test equal 0xffffffff -i xczu19.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_RFSoC_21 COMMAND byteman xusp -d xczu21dr -b 0xff -test equal 0xffffffff -i xczu21dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_25 COMMAND byteman xusp -d xczu25dr -b 0xff -test equal 0xffffffff -i xczu25dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_27 COMMAND byteman xusp -d xczu27dr -b 0xff -test equal 0xffffffff -i xczu27dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_28 COMMAND byteman xusp -d xczu28dr -b 0xff -test equal 0xffffffff -i xczu28dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_29 COMMAND byteman xusp -d xczu29dr -b 0xff -test equal 0xffffffff -i xczu29dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_39 COMMAND byteman xusp -d xczu39dr -b 0xff -test equal 0xffffffff -i xczu39dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_42 COMMAND byteman xusp -d xczu42dr -b 0xff -test equal 0xffffffff -i xczu42dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_43 COMMAND byteman xusp -d xczu43dr -b 0xff -test equal 0xffffffff -i xczu43dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_46 COMMAND byteman xusp -d xczu46dr -b 0xff -test equal 0xffffffff -i xczu46dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_47 COMMAND byteman xusp -d xczu47dr -b 0xff -test equal 0xffffffff -i xczu47dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_48 COMMAND byteman xusp -d xczu48dr -b 0xff -test equal 0xffffffff -i xczu48dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_49 COMMAND byteman xusp -d xczu49dr -b 0xff -test equal 0xffffffff -i xczu49dr.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Kintex_3 COMMAND byteman xusp -d xcku3p -b 0xff -test equal 0xffffffff -i xcku3p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_5 COMMAND byteman xusp -d xcku5p -b 0xff -test equal 0xffffffff -i xcku5p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_9 COMMAND byteman xusp -d xcku9p -b 0xff -test equal 0xffffffff -i xcku9p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_11 COMMAND byteman xusp -d xcku11p -b 0xff -test equal 0xffffffff -i xcku11p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_13 COMMAND byteman xusp -d xcku13p -b 0xff -test equal 0xffffffff -i xcku13p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_15 COMMAND byteman xusp -d xcku15p -b 0xff -test equal 0xffffffff -i xcku15p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_19 COMMAND byteman xusp -d xcku19p -b 0xff -test equal 0xffffffff -i xcku19p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Virtex_3 COMMAND byteman xusp -d xcvu3p -b 0xff -test equal 0xffffffff -i xcvu3p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_5 COMMAND byteman xusp -d xcvu5p -b 0xff -test equal 0xffffffff -i xcvu5p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_7 COMMAND byteman xusp -d xcvu7p -b 0xff -test equal 0xffffffff -i xcvu7p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_9 COMMAND byteman xusp -d xcvu9p -b 0xff -test equal 0xffffffff -i xcvu9p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_11 COMMAND byteman xusp -d xcvu11p -b 0xff -test equal 0xffffffff -i xcvu11p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_13 COMMAND byteman xusp -d xcvu13p -b 0xff -test equal 0xffffffff -i xcvu13p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_19 COMMAND byteman xusp -d xcvu19p -b 0xff -test equal 0xffffffff -i xcvu19p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_23 COMMAND byteman xusp -d xcvu23p -b 0xff -test equal 0xffffffff -i xcvu23p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_27 COMMAND byteman xusp -d xcvu27p -b 0xff -test equal 0xffffffff -i xcvu27p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_29 COMMAND byteman xusp -d xcvu29p -b 0xff -test equal 0xffffffff -i xcvu29p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_31 COMMAND byteman xusp -d xcvu31p -b 0xff -test equal 0xffffffff -i xcvu31p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_33 COMMAND byteman xusp -d xcvu33p -b 0xff -test equal 0xffffffff -i xcvu33p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_35 COMMAND byteman xusp -d xcvu35p -b 0xff -test equal 0xffffffff -i xcvu35p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_37 COMMAND byteman xusp -d xcvu37p -b 0xff -test equal 0xffffffff -i xcvu37p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_45 COMMAND byteman xusp -d xcvu45p -b 0xff -test equal 0xffffffff -i xcvu45p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_47 COMMAND byteman xusp -d xcvu47p -b 0xff -test equal 0xffffffff -i xcvu47p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_57 COMMAND byteman xusp -d xcvu57p -b 0xff -test equal 0xffffffff -i xcvu57p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Kria COMMAND byteman xusp -d Kria26C -b 0xff -test equal 0xffffffff -i xck26.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Alveo_25 COMMAND byteman xusp -d AlveoU25 -b 0xff -test equal 0xffffffff -i xcu25.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_26 COMMAND byteman xusp -d AlveoU26 -b 0xff -test equal 0xffffffff -i xcu26.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_30 COMMAND byteman xusp -d AlveoU30 -b 0xff -test equal 0xffffffff -i xcu30.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_55N COMMAND byteman xusp -d AlveoU55N -b 0xff -test equal 0xffffffff -i xcu55n.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_200 COMMAND byteman xusp -d AlveoU200 -b 0xff -test equal 0xffffffff -i xcu200.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_250 COMMAND byteman xusp -d AlveoU250 -b 0xff -test equal 0xffffffff -i xcu250.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_280 COMMAND byteman xusp -d AlveoU280 -b 0xff -test equal 0xffffffff -i xcu280.bit -test notequal 0xffffffff)

#The following tests test merging
#first they set main buffer to 0x55555555s everywhere.
#then the second bitstream is all initialized to 0xFCFCFCFC
#The first bitstream is tested to != 0xA5A5A5A5
#The second bitstream is merged into the first one using XOR merging
#The first bitstream is tested to == 0xA5A5A5A5
add_test(NAME Test_XUSP_Merge_XOR_Artix_10 COMMAND byteman xusp -d first,second xcau10p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Artix_15 COMMAND byteman xusp -d first,second xcau15p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Artix_20 COMMAND byteman xusp -d first,second xcau20p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Artix_25 COMMAND byteman xusp -d first,second xcau25p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_Zynq_1 COMMAND byteman xusp -d first,second xczu1 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_2 COMMAND byteman xusp -d first,second xczu2 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_3 COMMAND byteman xusp -d first,second xczu3 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_4 COMMAND byteman xusp -d first,second xczu4 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_5 COMMAND byteman xusp -d first,second xczu5 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_6 COMMAND byteman xusp -d first,second xczu6 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_7 COMMAND byteman xusp -d first,second xczu7 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_9 COMMAND byteman xusp -d first,second xczu9 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_11 COMMAND byteman xusp -d first,second xczu11 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_15 COMMAND byteman xusp -d first,second xczu15 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_17 COMMAND byteman xusp -d first,second xczu17 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Zynq_19 COMMAND byteman xusp -d first,second xczu19 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_RFSoC_21 COMMAND byteman xusp -d first,second xczu21dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_25 COMMAND byteman xusp -d first,second xczu25dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_27 COMMAND byteman xusp -d first,second xczu27dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_28 COMMAND byteman xusp -d first,second xczu28dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_29 COMMAND byteman xusp -d first,second xczu29dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_39 COMMAND byteman xusp -d first,second xczu39dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_42 COMMAND byteman xusp -d first,second xczu42dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_43 COMMAND byteman xusp -d first,second xczu43dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_46 COMMAND byteman xusp -d first,second xczu46dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_47 COMMAND byteman xusp -d first,second xczu47dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_48 COMMAND byteman xusp -d first,second xczu48dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_RFSoC_49 COMMAND byteman xusp -d first,second xczu49dr -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_Kintex_3 COMMAND byteman xusp -d first,second xcku3p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_5 COMMAND byteman xusp -d first,second xcku5p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_9 COMMAND byteman xusp -d first,second xcku9p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_11 COMMAND byteman xusp -d first,second xcku11p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_13 COMMAND byteman xusp -d first,second xcku13p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_15 COMMAND byteman xusp -d first,second xcku15p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Kintex_19 COMMAND byteman xusp -d first,second xcku19p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_Virtex_3 COMMAND byteman xusp -d first,second xcvu3p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_5 COMMAND byteman xusp -d first,second xcvu5p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_7 COMMAND byteman xusp -d first,second xcvu7p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_9 COMMAND byteman xusp -d first,second xcvu9p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_11 COMMAND byteman xusp -d first,second xcvu11p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_13 COMMAND byteman xusp -d first,second xcvu13p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_19 COMMAND byteman xusp -d first,second xcvu19p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_23 COMMAND byteman xusp -d first,second xcvu23p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_27 COMMAND byteman xusp -d first,second xcvu27p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_29 COMMAND byteman xusp -d first,second xcvu29p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_31 COMMAND byteman xusp -d first,second xcvu31p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_33 COMMAND byteman xusp -d first,second xcvu33p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_35 COMMAND byteman xusp -d first,second xcvu35p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_37 COMMAND byteman xusp -d first,second xcvu37p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_45 COMMAND byteman xusp -d first,second xcvu45p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_47 COMMAND byteman xusp -d first,second xcvu47p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Virtex_57 COMMAND byteman xusp -d first,second xcvu57p -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_Kria COMMAND byteman xusp -d first,second Kria26C -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)

add_test(NAME Test_XUSP_Merge_XOR_Alveo_25 COMMAND byteman xusp -d first,second AlveoU25 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_26 COMMAND byteman xusp -d first,second AlveoU26 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_30 COMMAND byteman xusp -d first,second AlveoU30 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_55N COMMAND byteman xusp -d first,second AlveoU55N -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_200 COMMAND byteman xusp -d first,second AlveoU200 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_250 COMMAND byteman xusp -d first,second AlveoU250 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
add_test(NAME Test_XUSP_Merge_XOR_Alveo_280 COMMAND byteman xusp -d first,second AlveoU280 -b 0x55 -b second 0xF0 -test notEqual 0xA5A5A5A5 -merge full,xor -test equal 0xA5A5A5A5)
