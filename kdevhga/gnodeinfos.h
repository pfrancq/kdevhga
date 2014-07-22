/*

	GALILEI Project

	GNodeInfos.h

	Node representing a set of words. - Header.

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
#include <robjh.h>
#include <rnodesga.h>
#include <rnodega.h>
#include <ghga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class GNodeInfos : public R::RNodeGA<GNodeInfos,R::RObjH,GChromoH>
{
public:
	/**
	* Construct the node.
	* @param id             Identificator of the node.
	*/
	GNodeInfos(size_t id,size_t max);

	int Compare(const GNodeInfos& n) const;
	int Compare(const GNodeInfos* n) const;

	void Evaluate(double& val, double nbchoices);
	virtual void CopyInfos(const GNodeInfos& w);
};


}//------- End of namespace GALILEI --------------------------------------------


//-----------------------------------------------------------------------------
#endif
