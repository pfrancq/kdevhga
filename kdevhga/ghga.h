/*

	R Project Library

	HGA.h

	Hierarchical Genetic Algorithms - Header.

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef GHGAH
#define GHGAH


//------------------------------------------------------------------------------
// include files for R Project
#include <rga/rfitness.h>
#include <rhga/robjh.h>
#include <rhga/rhga.h>
#include <rhga/rnodega.h>
#include <rhga/rnodesga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class GNodeWordsData;
class GNodeWords;
class GThreadDataH;
class GChromoH;
class GInstH;


//------------------------------------------------------------------------------
/**
* This class represent the fitness for the GALILEI HGA.
* @author Pascal Francq
* @short GALILEI HGA Fitness.
*/
class GFitnessH : public R::RFitness<double,false>
{
public:

	/**
	* Construct the fitness.
	*/
	GFitnessH(void) : R::RFitness<double,false>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	GFitnessH& operator=(const GFitnessH &f)
	{
		R::RFitness<double,false>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	GFitnessH& operator=(const double val)
	{
		R::RFitness<double,false>::operator=(val);
		return(*this);
	}
};


}//------- End of namespace GALILEI --------------------------------------------


//------------------------------------------------------------------------------
#endif
