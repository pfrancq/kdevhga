/*

	GALILEI Project

	GNodeInfos.cpp

	Node representing a set of words - Implementation.

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



//------------------------------------------------------------------------------
// include files for current application
#include <gnodeinfos.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
GNodeInfosData::GNodeInfosData(unsigned int max) : MaxAttr(max)
{
}


//------------------------------------------------------------------------------
GNodeInfos::GNodeInfos(RNodesGA<GNodeInfos,RObjH,GNodeInfosData,GChromoH>* owner,unsigned id,GNodeInfosData* data)
	: RNodeGA<GNodeInfos,RObjH,GNodeInfosData,GChromoH>(owner,id,data)
{
}


//------------------------------------------------------------------------------
GNodeInfos::GNodeInfos(const GNodeInfos* w)
	: RNodeGA<GNodeInfos,RObjH,GNodeInfosData,GChromoH>(w)
{
}


//------------------------------------------------------------------------------
int GNodeInfos::Compare(const GNodeInfos* n) const
{
	return(Id-n->Id);
}


//------------------------------------------------------------------------------
int GNodeInfos::Compare(const GNodeInfos& n) const
{
	return(Id-n.Id);
}


//------------------------------------------------------------------------------
void GNodeInfos::Evaluate(double& val, double nbchoices)
{
	GNodeInfos** ptr;
	unsigned int i;

	// Update count for this level
	nbchoices+=NbSubNodes+NbSubObjects;

	// For each objects add number of choices to get there
	val+=NbSubObjects*nbchoices;

	// Go through each subnodes in order to continue computing the value
	for(i=NbSubNodes+1,ptr=GetNodes();--i;ptr++)
		(*ptr)->Evaluate(val,nbchoices);
}


//------------------------------------------------------------------------------
GNodeInfos& GNodeInfos::operator=(const GNodeInfos& w)
{
	RNodeGA<GNodeInfos,RObjH,GNodeInfosData,GChromoH>::operator=(w);
	return(*this);
}
