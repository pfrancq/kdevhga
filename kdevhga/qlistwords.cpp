/*

	R Project Library

	qlistwords.cpp

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


//-----------------------------------------------------------------------------
#include <gchromoh.h>
using namespace R;
using namespace GALILEI;

//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>




//-----------------------------------------------------------------------------
#include "qlistwords.h"
#include "kdevhgadoc.h"
#include "khgaheuristicview.h"



//-----------------------------------------------------------------------------
//
// class QListWords
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QListWords::QListWords(KDevHGADoc* pDoc,QWidget* parent)
	: QListView(parent,"Results"), Doc(pDoc), Chromos(0), Nodes(0)
{
	addColumn("Resulting Tree");
	setRootIsDecorated(true);
	setSorting(-1);
}


//-----------------------------------------------------------------------------
void QListWords::ConstNode(QListViewItem* p,QListViewItem*& cur,GNodeWords* n)
{
	static char tmp[200],tmp2[100];
	unsigned int i,j;
	const unsigned int* ptr;
	GNodeWords** N;
	RObjH** O;
	QListViewItem* item2;
	QListViewItem* item;

	sprintf(tmp,"Node: %u (",n->GetId());
	for(i=n->GetAttr()->NbAttr+1,ptr=n->GetAttr()->List;--i;ptr++)
	{
		sprintf(tmp2,"\"%s\"",Doc->Words.GetPtr<unsigned int>(*ptr,false)->W());
		strcat(tmp,tmp2);
		if(i>1)
			strcat(tmp,",");
	}
	strcat(tmp,")");
	item=cur;
	item2=0;
	if(p)
		item=new QListViewItem(p,item,tmp);
	else
		item=new QListViewItem(this,tmp);
	item->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/filesystems/folder.png"));

	// Continue the tree with the subnodes.
	for(i=n->GetNbNodes()+1,N=n->GetNodes();--i;N++)
	{
		ConstNode(item,item2,*N);
	}

	// Continue the tree with the subobjects.
	for(i=n->GetNbObjs()+1,O=n->GetObjects();--i;O++)
	{
		sprintf(tmp,"Object %s (",(*O)->GetName()());
		for(j=(*O)->GetAttr()->NbAttr+1,ptr=(*O)->GetAttr()->List;--j;ptr++)
		{
			sprintf(tmp2,"\"%s\"",Doc->Words.GetPtr<unsigned int>(*ptr,false)->W());
			strcat(tmp,tmp2);
			if(j>1)
				strcat(tmp,",");
		}
		strcat(tmp,")");
		item2=new QListViewItem(item,item2,tmp);
		item2->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/document.png"));
	}
}


//-----------------------------------------------------------------------------
void QListWords::ConstNode(QListViewItem* p,QListViewItem*& cur,MyNode* n)
{
	static char tmp[200],tmp2[100];
	unsigned int i,j;
	const unsigned int* ptr;
	MyNode** N;
	RObjH** O;
	QListViewItem* item2;
	QListViewItem* item;

	sprintf(tmp,"Node: %u (",n->GetId());
	for(i=n->GetAttr()->NbAttr+1,ptr=n->GetAttr()->List;--i;ptr++)
	{
		sprintf(tmp2,"\"%s\"",Doc->Words.GetPtr<unsigned int>(*ptr,false)->W());
		strcat(tmp,tmp2);
		if(i>1)
			strcat(tmp,",");
	}
	strcat(tmp,")");
	item=cur;
	item2=0;
	if(p)
		item=new QListViewItem(p,item,tmp);
	else
		item=new QListViewItem(this,tmp);
	item->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/filesystems/folder.png"));

	// Continue the tree with the subnodes.
	for(i=n->GetNbNodes()+1,N=n->GetNodes();--i;N++)
	{
		ConstNode(item,item2,*N);
	}

	// Continue the tree with the subobjects.
	for(i=n->GetNbObjs()+1,O=n->GetObjects();--i;O++)
	{
		sprintf(tmp,"Object %s (",(*O)->GetName()());
		for(j=(*O)->GetAttr()->NbAttr+1,ptr=(*O)->GetAttr()->List;--j;ptr++)
		{
			sprintf(tmp2,"\"%s\"",Doc->Words.GetPtr<unsigned int>(*ptr,false)->W());
			strcat(tmp,tmp2);
			if(j>1)
				strcat(tmp,",");
		}
		strcat(tmp,")");
		item2=new QListViewItem(item,item2,tmp);
		item2->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/document.png"));
	}
}


//-----------------------------------------------------------------------------
void QListWords::setNodes(GChromoH* chromos)
{
	unsigned int i;
	GNodeWords** N;
	QListViewItem* cur=0;

	Chromos=chromos;
	Nodes=0;
	clear();
	for(i=Chromos->Top->GetNbNodes()+1,N=Chromos->Top->GetNodes();--i;N++)
		ConstNode(0,cur,*N);
	repaint();
}


//-----------------------------------------------------------------------------
void QListWords::setNodes(RNodesGA<MyNode,RObjH,GNodeWordsData,KHGAHeuristicView>* nodes)
{
	unsigned int i;
	MyNode** N;
	QListViewItem* cur=0;

	Nodes=nodes;
	Chromos=0;
	clear();
	for(i=Nodes->Top->GetNbNodes()+1,N=Nodes->Top->GetNodes();--i;N++)
		ConstNode(0,cur,*N);
	repaint();
}


//-----------------------------------------------------------------------------
QListWords::~QListWords(void)
{
}
