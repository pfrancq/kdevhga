/*

	RInsth.hh

	Class representing an instance of a HGA - Implementation

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
#include <ginsth.h>
#include <gchromoh.h>
#include <gawords.h>
using namespace R;
using namespace GALILEI;



//------------------------------------------------------------------------------
//
// GThreadDataH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GThreadDataH::GThreadDataH(GInstH *owner) throw(bad_alloc)
	: RThreadDataH<GInstH,GChromoH,GNodeWords,RObjH,GNodeWordsData>(owner)
{
}


//------------------------------------------------------------------------------
GThreadDataH::~GThreadDataH(void)
{
}



//------------------------------------------------------------------------------
//
// GInstH
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
GInstH::GInstH(unsigned int max,unsigned int popsize,RObjs<RObjH>* objs,HeuristicType h,RDebug* debug) throw(bad_alloc)
	: RInstH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,RObjH,GNodeWordsData>(popsize,objs,h,debug),
	  MaxGen(max)
{
}


//------------------------------------------------------------------------------
void GInstH::Init(GNodeWordsData* hdata) throw(bad_alloc)
{
	RInstH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,RObjH,GNodeWordsData>::Init(hdata);
}


//------------------------------------------------------------------------------
bool GInstH::StopCondition(void)
{
	return(Gen==MaxGen);
}


//------------------------------------------------------------------------------
GInstH::~GInstH(void)
{
}
