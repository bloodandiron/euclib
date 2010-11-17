/*	rect.hpp  v 0.1.1.10.1117
 *
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

#ifndef EUBLIB_RECT_HPP
#define EUBLIB_RECT_HPP

#include <ostream>
#include <limits>
#include "line.hpp"
#include "point.hpp"

namespace euclib {

template<typename T>
class rect2 {
// Typedefs
protected:

	typedef std::numeric_limits<T> lim;

	// This class can only be used with numeric types
	static_assert( lim::is_specialized,
	               "type not compatible with std::numeric_limits" );

// Variables
public:

	T l, r, t, b;


// Constructors
public:

	rect2( ) { set_null( ); }
	rect2( const rect2<T>& rect ) { *this = rect; }
	rect2( rect2<T>&& rect ) { *this = std::move( rect ); }
	rect2( T left, T right, T top, T bottom ) :
		l( left ),
		r( right ),
		t( top ),
		b( bottom ) {
		check_valid( );
	}
	rect2( const point2<T>& location, T width, T height ) :
		l( location.x ),
		r( location.x + width ),
		t( location.y ),
		b( location.y + height ) {
		check_valid( );
	}


// Methods
public:

	// Returns a null rect, defined as inf/max for all
	static rect2<T> null( ) {
		if( lim::has_infinity ) {
			return rect2<T>( lim::infinity( ), lim::infinity( ),
			                 lim::infinity( ), lim::infinity( ) );
		}
		else {
			return rect2<T>( lim::max( ), lim::max( ),
			                 lim::max( ), lim::max( ) );
		}
		
	}

	T  width( )  const { return r - l; }
	T  height( ) const { return b - t; }
	
	point2<T> tl( ) const { return point2<T>( l, t ); }
	point2<T> tr( ) const { return point2<T>( r, t ); }
	point2<T> br( ) const { return point2<T>( r, b ); }
	point2<T> bl( ) const { return point2<T>( l, b ); }
	
	line2<T> left( )   const { return line2<T>( tl( ), bl( ) ); }
	line2<T> right( )  const { return line2<T>( tr( ), br( ) ); }
	line2<T> top( )    const { return line2<T>( tl( ), tr( ) ); }
	line2<T> bottom( ) const { return line2<T>( bl( ), br( ) ); }
	
	T area( )      const { return width( ) * height( ); }
	T perimeter( ) const { return 2 * width( ) + 2 * height( ); }
	
	void print( std::ostream& stream, bool newline = false ) const {
		stream << l << " " << r << " " << t << " " << b;
		if( newline ) { stream << "\n"; }
	}
	
	// TODO: debugging only
	void gnuplot( std::ostream& stream ) const {
		stream << l << " " << t << "\n"
		       << r << " " << t << "\n"
		       << r << " " << b << "\n"
		       << l << " " << b << "\n"
		       << l << " " << t << "\n"
		       << "e\n";
	}

private:

	void check_valid( ) {
		// check inf
		if( lim::has_infinity &&
		    ( l == lim::infinity( ) || r == lim::infinity( ) ||
			  t == lim::infinity( ) || b == lim::infinity( ) )
		  ) {
			set_null( );
		}
		// check max
		else if( l == lim::max( ) || r == lim::max( ) ||
		         t == lim::max( ) || b == lim::max( )
		       ) {
			set_null( );				
		}
		// is l > r or t > b
		else if( l - r >= lim::epsilon( ) || t - b >= lim::epsilon( ) ) {
			set_null( );
		}
	}
	
	void set_null( ) {
		if( lim::has_infinity ) {
			l = r = t = b = lim::infinity( );
		}
		else {
			l = r = t = b = lim::max( );
		}
	}

// Operators
public:

	rect2<T>& operator = ( const rect2<T>& rect ) {
		l = rect.l;
		r = rect.r;
		t = rect.t;
		b = rect.b;
		check_valid( );
		return *this;
	}
	
	rect2<T>& operator = ( rect2<T>&& rect ) {
		l = rect.l;
		r = rect.r;
		t = rect.t;
		b = rect.b;
		rect.l = rect.r = rect.t = rect.b = 0; // rect will be deleted
		check_valid( );
		return *this;
	}
	
	// TODO: wow this is ugly...
	bool operator == ( const rect2<T>& rect ) const {
		if( l == rect.l && r == rect.r && t == rect.t && b == rect.b ) {
			return true;
		}
		
		// checking for null equality
		// a rect is null is any value is inf/max or if l > r or t > b
		// check inf
		if( lim::has_infinity &&
		    ( ( l == lim::infinity( ) || r == lim::infinity( ) ||
		        t == lim::infinity( ) || b == lim::infinity( ) ) ||
		      ( l - r >= lim::epsilon( ) || t - b >= lim::epsilon( ) ) ) &&
		    ( ( rect.l == lim::infinity( ) || rect.r == lim::infinity( ) ||
		        rect.t == lim::infinity( ) || rect.b == lim::infinity( ) ) ||
		      ( rect.l - rect.r >= lim::epsilon( ) ||
		        rect.t - rect.b >= lim::epsilon( ) ) )
		  ) {
		  	return true;
		}
		// check max
		else if(
		    ( ( l == lim::max( ) || r == lim::max( ) ||
		        t == lim::max( ) || b == lim::max( ) ) ||
		      ( l - r >= lim::epsilon( ) || t - b >= lim::epsilon( ) ) ) &&
		    ( ( rect.l == lim::max( ) || rect.r == lim::max( ) ||
		        rect.t == lim::max( ) || rect.b == lim::max( ) ) ||
		      ( rect.l - rect.r >= lim::epsilon( ) ||
		        rect.t - rect.b >= lim::epsilon( ) ) )
		       ) {
			return true;
		}
		return false;
	}
	
	bool operator != ( const rect2<T>& rect ) const {
		return !(*this == rect);
	}


}; // End class rect2<T>

// useful typedefs
typedef rect2<int>           rect2i;
typedef rect2<float>         rect2f;
typedef rect2<unsigned int>  rect2u;

}  // End namespace euclib

#endif // EUBLIB_RECT_HPP

