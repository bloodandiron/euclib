/*
 *	Copyright (C) 2010 Jonathan Marini
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#define GNUPLOT // I am testing this w/ gnuplot, so it should be in that format

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <functional>
#include <time.h>

#include "point.hpp"
#include "vector.hpp"

using namespace euclib;
using namespace std;

// Takes an optional seed as an argument (to recreate bugs)
int main( int argc, char *argv[] ) {
	int seed = time(NULL); // default seed
	if( argc == 2 ) {
		seed = atoi(argv[1]);
	}

	// min and max values when plotting
	int max = 10;

	// random number unif_gen
	uniform_real_distribution<float> unif_distr(0, max);
	normal_distribution<float> norm_distr( max/2, max/10 );
	mt19937 engine( seed );
	auto unif = bind(unif_distr, engine);
	auto norm = bind(norm_distr, engine);

	cout << "seed=" << seed << "\n";

	//////////////////////////////////////////
	//  Testing

	point2f pt1;				// default
	point2f pt2 { 1.f, 3.f };	// list, same length
	point2f pt3 { pt2 };		// copy
	point2f pt4 { 2.f };		// list, too short
	pt1 = 3.f * ( pt3 + pt4 );	// expression
	cout << "=== points ===\n"
	     << "pt1: " << pt1[0] << ", " << pt1[1] << "\n"
	     << "pt2: " << pt2[0] << ", " << pt2[1] << "\n"
	     << "pt3: " << pt3[0] << ", " << pt3[1] << "\n"
	     << "pt4: " << pt4[0] << ", " << pt4[1] << "\n";

	vector2f v1;				// default
	vector2f v2 { 5.f, 6.f };	// list, same length
	vector2f v3 { v2 };			// copy
	vector2f v4 { 1.f };		// list, too short
	v1 = 2.f * ( v3 + v4 );		// expression
	cout << "=== vector ===\n"
	     << "v1:  " << v1[0] << ", " << v1[1] << "\n"
	     << "v2:  " << v2[0] << ", " << v2[1] << "\n"
	     << "v3:  " << v3[0] << ", " << v3[1] << "\n"
	     << "v4:  " << v4[0] << ", " << v4[1] << "\n";

	vector2f v5 { pt1 };					// vector from point
	vector2f v6 { 3.f * ( pt3 + pt4 ) };	// vector from point expression
	point2f pt5 { v1 };						// point from vector
	point2f pt6 { 2.f * ( v3 + v4 ) };		// point from vector expression
	cout << "=== mixed ===\n"
	     << "v5:  " << v5[0]  << ", " << v5[1]  << "\n"
	     << "v6:  " << v6[0]  << ", " << v6[1]  << "\n"
	     << "pt5: " << pt5[0] << ", " << pt5[1] << "\n"
	     << "pt6: " << pt6[0] << ", " << pt6[1] << "\n";

	return 0;
}

