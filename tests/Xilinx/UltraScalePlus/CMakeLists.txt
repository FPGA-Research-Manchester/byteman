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

add_test(NAME Test_XUSP_Artix_10 COMMAND byteman xs7 -d xcau10p -b 0xff -i xckau10p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_15 COMMAND byteman xs7 -d xcau15p -b 0xff -i xckau15p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_20 COMMAND byteman xs7 -d xcau20p -b 0xff -i xckau20p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Artix_25 COMMAND byteman xs7 -d xcau25p -b 0xff -i xckau25p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Zynq_1 COMMAND byteman xs7 -d xczu1 -b 0xff -i xczu1.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_2 COMMAND byteman xs7 -d xczu2 -b 0xff -i xczu2.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_3 COMMAND byteman xs7 -d xczu3 -b 0xff -i xczu3.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_4 COMMAND byteman xs7 -d xczu4 -b 0xff -i xczu4.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_5 COMMAND byteman xs7 -d xczu5 -b 0xff -i xczu5.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_6 COMMAND byteman xs7 -d xczu6 -b 0xff -i xczu6.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_7 COMMAND byteman xs7 -d xczu7 -b 0xff -i xczu7.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Zynq_17 COMMAND byteman xs7 -d xczu17 -b 0xff -i xczu17.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_RFSoC_21 COMMAND byteman xs7 -d xczu21dr -b 0xff -i xczu21dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_25 COMMAND byteman xs7 -d xczu25dr -b 0xff -i xczu25dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_27 COMMAND byteman xs7 -d xczu27dr -b 0xff -i xczu27dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_28 COMMAND byteman xs7 -d xczu28dr -b 0xff -i xczu28dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_29 COMMAND byteman xs7 -d xczu29dr -b 0xff -i xczu29dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_39 COMMAND byteman xs7 -d xczu39dr -b 0xff -i xczu39dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_42 COMMAND byteman xs7 -d xczu42dr -b 0xff -i xczu42dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_43 COMMAND byteman xs7 -d xczu43dr -b 0xff -i xczu43dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_46 COMMAND byteman xs7 -d xczu46dr -b 0xff -i xczu46dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_47 COMMAND byteman xs7 -d xczu47dr -b 0xff -i xczu47dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_48 COMMAND byteman xs7 -d xczu48dr -b 0xff -i xczu48dr.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_RFSoC_49 COMMAND byteman xs7 -d xczu49dr -b 0xff -i xczu49dr.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Kintex_3 COMMAND byteman xs7 -d xcku3p -b 0xff -i xcku3p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_5 COMMAND byteman xs7 -d xcku5p -b 0xff -i xcku5p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_9 COMMAND byteman xs7 -d xcku9p -b 0xff -i xcku9p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_11 COMMAND byteman xs7 -d xcku11p -b 0xff -i xcku11p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_13 COMMAND byteman xs7 -d xcku13p -b 0xff -i xcku13p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_15 COMMAND byteman xs7 -d xcku15p -b 0xff -i xcku15p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Kintex_19 COMMAND byteman xs7 -d xcku19p -b 0xff -i xcku19p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Virtex_3 COMMAND byteman xs7 -d xcvu3p -b 0xff -i xcvu3p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_5 COMMAND byteman xs7 -d xcvu5p -b 0xff -i xcvu5p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_7 COMMAND byteman xs7 -d xcvu7p -b 0xff -i xcvu7p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_9 COMMAND byteman xs7 -d xcvu9p -b 0xff -i xcvu9p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_11 COMMAND byteman xs7 -d xcvu11p -b 0xff -i xcvu11p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_13 COMMAND byteman xs7 -d xcvu13p -b 0xff -i xcvu13p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_19 COMMAND byteman xs7 -d xcvu19p -b 0xff -i xcvu19p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_23 COMMAND byteman xs7 -d xcvu23p -b 0xff -i xcvu23p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_27 COMMAND byteman xs7 -d xcvu27p -b 0xff -i xcvu27p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_29 COMMAND byteman xs7 -d xcvu29p -b 0xff -i xcvu29p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_31 COMMAND byteman xs7 -d xcvu31p -b 0xff -i xcvu31p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_33 COMMAND byteman xs7 -d xcvu33p -b 0xff -i xcvu33p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_35 COMMAND byteman xs7 -d xcvu35p -b 0xff -i xcvu35p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_37 COMMAND byteman xs7 -d xcvu37p -b 0xff -i xcvu37p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_45 COMMAND byteman xs7 -d xcvu45p -b 0xff -i xcvu45p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_47 COMMAND byteman xs7 -d xcvu47p -b 0xff -i xcvu47p.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Virtex_57 COMMAND byteman xs7 -d xcvu57p -b 0xff -i xcvu57p.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Kria COMMAND byteman xs7 -d xck26 -b 0xff -i xck26.bit -test notequal 0xffffffff)

add_test(NAME Test_XUSP_Alveo_25 COMMAND byteman xs7 -d Alveo U25 -b 0xff -i xcu25.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_26 COMMAND byteman xs7 -d Alveo U26 -b 0xff -i xcu26.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_30 COMMAND byteman xs7 -d Alveo U30 -b 0xff -i xcu30.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_55N COMMAND byteman xs7 -d Alveo U55N -b 0xff -i xcu55n.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_200 COMMAND byteman xs7 -d Alveo U200 -b 0xff -i xcu200.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_250 COMMAND byteman xs7 -d Alveo U250 -b 0xff -i xcu250.bit -test notequal 0xffffffff)
add_test(NAME Test_XUSP_Alveo_280 COMMAND byteman xs7 -d Alveo U280 -b 0xff -i xcu280.bit -test notequal 0xffffffff)