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
#include <rhga/rnodega.h>
#include <ghga.h>


//------------------------------------------------------------------------------
namespace GALILEI{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class GNodeWordsData
{
public:
	unsigned int MaxAttr;

	GNodeWordsData(unsigned int max);
};


//-----------------------------------------------------------------------------
class GNodeWords : public R::RNodeGA<GNodeWords,R::RObjH,GNodeWordsData,GChromoH>
{
public:
	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identificator of the node.
	* @param data           Data used to construct the node.
	*/
	GNodeWords(R::RNodesGA<GNodeWords,R::RObjH,GNodeWordsData,GChromoH>* owner,unsigned int id,GNodeWordsData* data);

	GNodeWords(const GNodeWords* w);

	int Compare(const GNodeWords* n) const;

	int Compare(const GNodeWords& n) const;

	void Evaluate(double& val, double nbchoices);
	GNodeWords& operator=(const GNodeWords& w);
};


}//------- End of namespace GALILEI --------------------------------------------


//-----------------------------------------------------------------------------
#endif
