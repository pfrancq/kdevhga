/*

	R Project Library

	RInsth.h

	Class representing an instance of a HGA - Header

	Copyright 1998-2003 by the Universit� Libre de Bruxelles.

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
#ifndef GInstH_H
#define GInstH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rhga/rinsth.h>


//------------------------------------------------------------------------------
// include files for GALILEI
#include <ghga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the GA.
* @author Pascal Francq
* @short HGA "thread-dependent" Data.
*/
class GThreadDataH : public R::RThreadDataH<GInstH,GChromoH,GNodeWords,R::RObjH,GNodeWordsData>
{
public:
	/**
	* Construct the data.
	* @param data           Owner of the data.
	*/
	GThreadDataH(GInstH *owner) throw(bad_alloc);

	/**
	* Destruct the data.
	*/
	virtual ~GThreadDataH(void);
};


//------------------------------------------------------------------------------
class GInstH : public R::RInstH<GInstH,GChromoH,GFitnessH,GThreadDataH,GNodeWords,R::RObjH,GNodeWordsData>
{
public:

	/**
	* Maximal number of Generations.
	*/
	unsigned int MaxGen;

	/**
	* Construct the instance.
	* @param max            Maximal number of generations.
	* @param popsize        Size of the population.
	* @param objs           Objects to place in the tree.
	* @param h              The heuristic that has to be used.
	* @param debug          Debugger.
	*/
	GInstH(unsigned int max,unsigned int popsize,R::RObjs<R::RObjH>* objs,R::HeuristicType h,R::RDebug* debug=0) throw(bad_alloc);

	/**
	* Initialisation of the instance.
	*/
	virtual void Init(GNodeWordsData* hdata) throw(bad_alloc);

	/**
	* This function determines if the GA must stop. It is called after
	* each generation. This function is a pure one and has to be implement.
	* @return When the function returns true, the GA is stop.
	*/
	virtual bool StopCondition(void);

	/**
	* Destruct the instance.
	*/
	virtual ~GInstH(void);
};


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
