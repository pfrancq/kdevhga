/*

	GALILEI Project

	GNodeInfos.h

	Node representing a set of words. - Header.

	Copyright 1998-2004 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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


//-----------------------------------------------------------------------------
#ifndef RNodeWordsH
#define RNodeWordsH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rhga/robjh.h>
#include <rhga/rnodesga.h>
#include <rhga/rnodega.h>
#include <ghga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class GNodeInfosData
{
public:
	unsigned int MaxAttr;

	GNodeInfosData(unsigned int max);
};


//-----------------------------------------------------------------------------
class GNodeInfos : public R::RNodeGA<GNodeInfos,R::RObjH,GNodeInfosData,GChromoH>
{
public:
	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identificator of the node.
	* @param data           Data used to construct the node.
	*/
	GNodeInfos(R::RNodesGA<GNodeInfos,R::RObjH,GNodeInfosData,GChromoH>* owner,unsigned int id,GNodeInfosData* data);

	GNodeInfos(const GNodeInfos* w);

	int Compare(const GNodeInfos* n) const;

	int Compare(const GNodeInfos& n) const;

	void Evaluate(double& val, double nbchoices);
	GNodeInfos& operator=(const GNodeInfos& w);
};


}//------- End of namespace GALILEI --------------------------------------------


//-----------------------------------------------------------------------------
#endif
