/*

	R Project Library

	RChromoH.h

	Class representing a tree (chromosome) - Header

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
#ifndef GChromoH_H
#define GChromoH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rhga/rhga.h>
#include <rhga/rchromoh.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ghga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
class GChromoH : public R::RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,R::RObjH,GNodeWordsData>
{
public:

	/**
	* Construct the chromosome.
	* @param inst           The instance.
	* @param id             Identificator of the chromosome.
	*/
	GChromoH(GInstH* inst,unsigned int id) throw(bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the
	*                       chromosome.
	*/
	virtual void Init(GThreadDataH* thData) throw(bad_alloc);

	/**
	* Evaluate the quality of the solution
	*/
	void Evaluate(void) throw(R::eGA);

	/**
	* Destruct the chromosome.
	*/
	virtual ~GChromoH(void);
};


}//------- End of namespace GALILEI --------------------------------------------


//------------------------------------------------------------------------------
#endif
