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

#ifndef ASSERT_H
#define ASSERT_H

#include<stdexcept>
/*
	throwingAssertPrintVar_{N} takes {N} additional arguments and prints their values if assert fails
*/
#ifdef NDEBUG
	#define throwingAssert(left, operator, right) {;}
	#define throwingAssertPrintVar_1(left, operator, right, var1) {;}
	#define throwingAssertPrintVar_2(left, operator, right, var1, var2) {;}
	#define throwingAssertPrintVar_3(left, operator, right, var1, var2, var3) {;}
	#define throwingAssertPrintVar_4(left, operator, right, var1, var2, var3, var4) {;}
	#define throwingAssertPrintVar_5(left, operator, right, var1, var2, var3, var4, var5) {;}
#else
    #define throwingAssert(left, operator, right) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)) ); }}
    #define throwingAssertPrintVar_1(left, operator, right, var1) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)).append("\n\t\t").append(#var1).append(" = ").append(std::to_string(var1)) ); }}
    #define throwingAssertPrintVar_2(left, operator, right, var1, var2) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)).append("\n\t\t").append(#var1).append(" = ").append(std::to_string(var1)).append("\n\t\t").append(#var2).append(" = ").append(std::to_string(var2)) ); }}
    #define throwingAssertPrintVar_3(left, operator, right, var1, var2, var3) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)).append("\n\t\t").append(#var1).append(" = ").append(std::to_string(var1)).append("\n\t\t").append(#var2).append(" = ").append(std::to_string(var2)).append("\n\t\t").append(#var3).append(" = ").append(std::to_string(var3)) ); }}
    #define throwingAssertPrintVar_4(left, operator, right, var1, var2, var3, var4) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)).append("\n\t\t").append(#var1).append(" = ").append(std::to_string(var1)).append("\n\t\t").append(#var2).append(" = ").append(std::to_string(var2)).append("\n\t\t").append(#var3).append(" = ").append(std::to_string(var3)).append("\n\t\t").append(#var4).append(" = ").append(std::to_string(var4)) ); }}
    #define throwingAssertPrintVar_5(left, operator, right, var1, var2, var3, var4, var5) {if(!((left) operator (right))){ throw std::runtime_error(std::string("Assertion failed in ").append(__FILE__).append(":").append(std::to_string(__LINE__)).append(":\n\tassert(").append(#left).append(" ").append(#operator).append(" ").append(#right).append(")\n\twith variable values:\n\t\t").append(#left).append(" = ").append(std::to_string(left)).append("\n\t\t").append(#right).append(" = ").append(std::to_string(right)).append("\n\t\t").append(#var1).append(" = ").append(std::to_string(var1)).append("\n\t\t").append(#var2).append(" = ").append(std::to_string(var2)).append("\n\t\t").append(#var3).append(" = ").append(std::to_string(var3)).append("\n\t\t").append(#var4).append(" = ").append(std::to_string(var4)).append("\n\t\t").append(#var5).append(" = ").append(std::to_string(var5)) ); }}
#endif

#endif//ASSERT_H