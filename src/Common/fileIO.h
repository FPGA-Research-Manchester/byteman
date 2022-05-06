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
#ifndef FILEIO_H
#define FILEIO_H
#include <stdint.h>
#include <fstream>
#include "endianess.h"
using namespace std;


#define fileReadBE(var,istream,bits) istream.read(reinterpret_cast<char*>(&var), sizeof(var));var=htobe##bits(var);
#define fileReadAndDefineBE(var,istream,bits) uint##bits##_t var;fileReadBE(var,istream,bits)

#define fileReadLE(var,istream,bits) istream.read(reinterpret_cast<char*>(&var), sizeof(var));var=htole##bits(var);
#define fileReadAndDefineLE(var,istream,bits) uint##bits##_t var;fileReadLE(var,istream,bits)

#define fileReadChars(var,istream,num) istream.read(var, num);var[num]='\0';
#define fileReadAndMallocChars(var,istream,num) var=new char[num+1];fileReadChars(var,istream,num)
#define fileReadAndDefineChars(var,istream,num) char* var=new char[num+1];fileReadChars(var,istream,num)

#define fileWriteBE(ostream,bits,var) {uint##bits##_t tmp=var;tmp=htobe##bits(tmp);ostream.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));}
#define fileWriteLE(ostream,bits,var) {uint##bits##_t tmp=var;tmp=htole##bits(tmp);ostream.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));}
#define fileWriteString(ostream,var) ostream.write(var.c_str(), var.size()+1);

#define fileExtensionIs(filename, extension) ((filename.length() >= sizeof(extension))?(0 == filename.compare (filename.length() - sizeof(extension), sizeof(extension), "."extension)):0)

#endif // FILEIO_H
