/**
BSD 2-Clause License

Copyright (c) 2026, Andrey Kudryavtsev (andrewkoudr@hotmail.com)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>
#include <vector>
#include <assert.h>

/** Stretch points by Bresenham. */
int stretchBresenhamPoints(const int x1, const int y1, const int x2, const int y2, 
  std::vector<int> &points)
{
  points.clear();

  // use Bresenham to move from 
  // point 1 to point 2
  int x = x1; int y = y1;

  // argument/function ranges
  int dx = x2 - x1; int dy = y2 - y1;

  // parameters for Bresenham algorithm
  int s1 = (dx >= 0) ? 1 : -1; 
  int s2 = (dy >= 0) ? 1 : -1;
  dx = std::abs(dx); 
  dy = std::abs(dy); 

  // allocate memory
  points.resize(dx + 1);

  // just single point
	if (dx == 0)
  {
    points[0] = y1;
    return int(points.size());
  }

  // other parameters for Bresenham algorithm
  int dx2 = dx + dx;
  int dy2 = dy + dy;
  int e = dy2 - dx;

  // incrementing pointer instead of [] operator (actually a function call)
  // makes even an optimised code 5% faster
	int *p = &(points[0]);
  for (int i = 0; i <= dx; i++)
  {
    // fill array
		*p = y;             
		p++;

    // correct errors   
    while (e >= 0)
    {
      y += s2;
      e -= dx2;
    }
    x += s1;

    e += dy2;
  }

  // return number of generated y points
	return int(points.size());
}

/** Decimate points. Not only decimate, expand as well. */
template <class T> void decimatePoints(std::vector<T> &points, int numpoints)
{
  std::vector<T> newpoints;

  std::vector<int> ranges;
  stretchBresenhamPoints(0,0,numpoints - 1,int(points.size()) - 1,ranges);

  for (int i = 0; i < int(ranges.size()); i++)
  {
    newpoints.push_back(points[ranges[i]]);
  }

  assert(points.front() == newpoints.front());
  assert(points.back() == newpoints.back());

  points.swap(newpoints);
}

/** Get ranges for threads. n == size(). */
void getRanges(int n, int numthreads, std::vector<int> &ranges)
{
  ranges.clear();
  stretchBresenhamPoints(0,0,numthreads,n,ranges);
}

int main()
{
  // EXAMPLE 1 : draw a line between (330,8) to (206,33), y = line[330 + x]
  std::vector<int> line;
  stretchBresenhamPoints(330,8,206,33,line);

  // EXAMPLE 2 : decimate points 276 -> 201
  #define T double
  std::vector<T> points;
  for (int i = 0; i < 276; i++)
  {
    points.push_back(T(i));
  }
  decimatePoints<T>(points,201);

  // EXAMPLE 3 : ranges for 3 threads, distribute 55 elements across 3 threads
  int numthreads = 3;
  std::vector<int> ranges;
  getRanges(55,numthreads,ranges);

  //...
  //for (int t = 0; t < numthreads; t++)
  //{
  //  int j1 = ranges[t];
  //  int j2 = ranges[t + 1] - 1; //!!! 54 is the last

  //  std::thread th;
  //  th = std::thread(...

}
