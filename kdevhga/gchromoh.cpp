/*

	RChromoh.hh

	Class representing a tree (chromosome) - Inline implementation

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
// include files for GALILEI
#include <rhga/robjh.h>
#include <gchromoh.h>
#include <ginsth.h>
#include <gawords.h>
using namespace GALILEI;
using namespace R;



//------------------------------------------------------------------------------
//
// class GChromoH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GChromoH::GChromoH(GInstH* inst,unsigned id) throw(bad_alloc)
	: RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,RObjH,GNodeWordsData>(inst,id)
{
	(*Fitness)=100.0;
}


//------------------------------------------------------------------------------
void GChromoH::Init(GThreadDataH* thData) throw(bad_alloc)
{
	RChromoH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,RObjH,GNodeWordsData>::Init(thData);
}


//------------------------------------------------------------------------------
void GChromoH::Evaluate(void) throw(eGA)
{
	double val=0.0;

	Top->Evaluate(val,0.0);
	(*Fitness)=val/static_cast<double>(Objs->GetNb());
}


//------------------------------------------------------------------------------
GChromoH::~GChromoH(void)
{
}
