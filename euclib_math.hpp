/*
 *	Copyright (C) 2010-2011 Jonathan Marini
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU Lesser General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU Lesser General Public License for more details.
 *
 *	You should have received a copy of the GNU Lesser General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EUBLIB_MATH_HPP
#define EUBLIB_MATH_HPP

#include <limits>
#include <complex>
#include <cassert>
#include <cmath>

#include "type_traits.hpp"

namespace euclib {

////////////////////////////////////////
// Get definitions from math.h
//   or define our own versions

#ifdef M_PI	// pi
#	define EUCLIB_PI M_PI
#else
#	define EUCLIB_PI 3.14159265358979323846
#endif

#ifdef M_PI_2	// pi/2
#	define EUCLIB_PI_2 M_PI_2
#else
#	define EUCLIB_PI_2 1.57079632679489661923
#endif

#ifdef M_PI_4	// pi/4
#	define EUCLIB_PI_4 M_PI_4
#else
#	define EUCLIB_PI_4 0.78539816339744830962
#endif

#ifdef M_1_PI	// 1/pi
#	define EUCLIB_1_PI M_1_PI
#else
#	define EUCLIB_1_PI 0.31830988618379067154
#endif

#ifdef M_2_PI	// 2/pi
#	define EUCLIB_2_PI M_2_PI
#else
#	define EUCLIB_2_PI 0.63661977236758134308
#endif

#ifdef M_2_SQRTPI	// 2/sqrt(pi)
#	define EUCLIB_2_SQRTPI M_2_SQRTPI
#else
#	define EUCLIB_2_SQRTPI 1.12837916709551257390
#endif


////////////////////////////////////////
// some new definitions

#define EUCLIB_PI_180  0.01745329251994329576	// pi/180
#define EUCLIB_180_PI 57.29577951308232087679	// 180/pi
#define EUCLIB_2PI     6.28318530717958647692	// 2*pi


////////////////////////////////////////
// Helper functions for comparisons, because no assumptions
//   can be made about the exactness of type T

namespace detail {

template<typename T>
inline bool equal( T lhs, T rhs );

	template<typename T>
	inline bool equal( T lhs, T rhs, mpl::inaccurate_tag ) {
		return std::abs( lhs - rhs ) <= std::numeric_limits<T>::epsilon( ) *
		                                ( std::abs(lhs) + std::abs(rhs) + 1.0 );
	}

	template<typename T>
	inline bool equal( T lhs, T rhs, mpl::accurate_tag ) {
		return lhs == rhs;
	}

	template<typename T>
	inline bool equal( T lhs, T rhs ) {
		return equal( lhs, rhs, typename mpl::accuracy_traits<T>::category_t( ) );
	}


template<typename T>
inline bool not_equal( T lhs, T rhs ) {
	return !(lhs == rhs);
}

template<typename T>
inline bool less_than( T lhs, T rhs );

	template<typename T>
	inline bool less_than( T lhs, T rhs, mpl::inaccurate_tag ) {
		return rhs - lhs > std::numeric_limits<T>::epsilon( ) *
		                   ( std::abs(lhs) + std::abs(rhs) + 1.0 );
	}

	template<typename T>
	inline bool less_than( T lhs, T rhs, mpl::accurate_tag ) {
		return lhs < rhs;
	}

	template<typename T>
	inline bool less_than( T lhs, T rhs ) {
		return less_than( lhs, rhs, typename mpl::accuracy_traits<T>::category_t( ) );
	}


template<typename T>
inline bool greater_than( T lhs, T rhs ) {
	return rhs < lhs;
}

template<typename T>
inline bool less_than_eq( T lhs, T rhs ) {
	return !(rhs < lhs);
}

template<typename T>
inline bool greater_than_eq( T lhs, T rhs ) {
	return !(lhs < rhs);
}


// TODO: change to two versions (floating/decimal or integer)
template<typename T>
void round_nearest( long double& value ) {
	if( std::numeric_limits<T>::is_integer ) { // make sure it rounds to nearest when cast to T
		switch( std::numeric_limits<T>::round_style ) {
			case 0:  // round to 0
				if( less_than( value, 0.0L ) ) { value -= 0.5L; }
				else { value += 0.5L; }
				break;
			case 2:  // round to inf
				value -= 0.5L;
				break;
			case 3:  // round to -inf
				value += 0.5L;
				break;
			case -1: // indeterminate
			default:
				assert(false);
			case 1:  // round toward nearest
				break;
		}
	}
}

template<typename T>
void round_nearest( double& value ) {
	if( std::numeric_limits<T>::is_integer ) { // make sure it rounds to nearest when cast to T
		switch( std::numeric_limits<T>::round_style ) {
			case 0:  // round to 0
				if( less_than( value, 0.0 ) ) { value -= 0.5; }
				else { value += 0.5; }
				break;
			case 2:  // round to inf
				value -= 0.5;
				break;
			case 3:  // round to -inf
				value += 0.5;
				break;
			case -1: // indeterminate
			default:
				assert(false);
			case 1:  // round toward nearest
				break;
		}
	}
}

template<typename T>
void round_nearest( float& value ) {
	if( std::numeric_limits<T>::is_integer ) { // make sure it rounds to nearest when cast to T
		switch( std::numeric_limits<T>::round_style ) {
			case 0:  // round to 0
				if( less_than( value, 0.f ) ) { value -= 0.5f; }
				else { value += 0.5f; }
				break;
			case 2:  // round to inf
				value -= 0.5f;
				break;
			case 3:  // round to -inf
				value += 0.5f;
				break;
			case -1: // indeterminate
			default:
				assert(false);
			case 1:  // round toward nearest
				break;
		}
	}
}

} // End namespace detail

} // End namespace euclib

#endif // EUCLIB_MATH_HPP
