/*

	R Project Library

	gawords.cpp

	Description - Implementation.

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



//------------------------------------------------------------------------------
// include files for current application
#include <gawords.h>
using namespace R;
using namespace GALILEI;


//------------------------------------------------------------------------------
GNodeWordsData::GNodeWordsData(unsigned int max) : MaxAttr(max)
{
}


//------------------------------------------------------------------------------
GNodeWords::GNodeWords(RNodesGA<GNodeWords,RObjH,GNodeWordsData,GChromoH>* owner,unsigned id,GNodeWordsData* data)
	: RNodeGA<GNodeWords,RObjH,GNodeWordsData,GChromoH>(owner,id,data)
{
}


//------------------------------------------------------------------------------
GNodeWords::GNodeWords(const GNodeWords* w)
	: RNodeGA<GNodeWords,RObjH,GNodeWordsData,GChromoH>(w)
{
}


//------------------------------------------------------------------------------
int GNodeWords::Compare(const GNodeWords* n) const
{
	return(Id-n->Id);
}


//------------------------------------------------------------------------------
int GNodeWords::Compare(const GNodeWords& n) const
{
	return(Id-n.Id);
}


//------------------------------------------------------------------------------
void GNodeWords::Evaluate(double& val, double nbchoices)
{
	GNodeWords** ptr;
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
GNodeWords& GNodeWords::operator=(const GNodeWords& w)
{
	RNodeGA<GNodeWords,RObjH,GNodeWordsData,GChromoH>::operator=(w);
	return(*this);
}
