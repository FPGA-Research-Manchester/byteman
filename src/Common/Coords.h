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

#ifndef COORDS_H
#define COORDS_H

#include<algorithm>   // min,max

using namespace std;

struct Coord2D
{
	int row;
	int col;
};

struct Rect2D
{
	Coord2D position;
	Coord2D size;
};
namespace rect {
	inline Rect2D getOverlap(Rect2D r1, Rect2D r2){
		Rect2D retRect;
		retRect.position.col = max(r1.position.col, r2.position.col);
		retRect.size.col = min((r1.position.col+r1.size.col), (r2.position.col+r2.size.col)) - retRect.position.col;
		if(0 > retRect.size.col)
			retRect.size.col = 0;
		
		retRect.position.row = max(r1.position.row, r2.position.row);
		retRect.size.row = min((r1.position.row+r1.size.row), (r2.position.row+r2.size.row)) - retRect.position.row;
		if(0 > retRect.size.row)
			retRect.size.row = 0;
		
		return retRect;
	}
	inline bool empty(Rect2D r){
		return (0 >= r.size.row || 0 >= r.size.col);
	}
}

#endif //COORDS_H