/*

	R Project Library

	gawords.h

	Description - Header.

	(C) 2001 by Pascal Francq

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


//-----------------------------------------------------------------------------
#ifndef RGAWordsH
#define RGAWordsH


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rhga/robjh.h>
#include <rhga/rnodesga.h>
using namespace R;


//-----------------------------------------------------------------------------
// forward class declaration
class RNodeWords;


//-----------------------------------------------------------------------------
class RNodeWordsData
{
public:
	RNodeWordsData(unsigned int max) : MaxAttr(max) {}
	unsigned int MaxAttr;
};


//-----------------------------------------------------------------------------
class RNodeWords : public RNodeGA<RNodeWords,RObjH,RNodeWordsData>
{
public:
	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identificator of the node.
	* @param data           Data used to construct the node.
	*/
	RNodeWords(RNodesGA<RNodeWords,RObjH,RNodeWordsData>* owner,unsigned id,RNodeWordsData* data)
		: RNodeGA<RNodeWords,RObjH,RNodeWordsData>(owner,id,data) {}
	int Compare(const RNodeWords* n) {return(Id-n->Id);}
};


//-----------------------------------------------------------------------------
#endif
