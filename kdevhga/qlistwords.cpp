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
// include files for Qt
#include <qpixmap.h>


//-----------------------------------------------------------------------------
#include "qlistwords.h"
#include "kdevhgadoc.h"



//-----------------------------------------------------------------------------
//
// class QListWords
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QListWords::QListWords(KDevHGADoc* pDoc,QWidget* parent)
	: QListView(parent,"Results"), Doc(pDoc)
{
	addColumn("Resulting Tree");
	setRootIsDecorated(true);
	setSorting(-1);
}


//-----------------------------------------------------------------------------
void QListWords::ConstNode(QListViewItem* p,QListViewItem*& cur,RNodeWords* n)
{
	static char tmp[200],tmp2[100];
	unsigned int i,j;
	const unsigned int* ptr;
	RNodeWords** N;
	RObjH** O;
	QListViewItem* item2;
	QListViewItem* item;

//	sprintf(tmp,"Node: %u (",n->GetId());
	tmp[0]=0;
	for(i=n->GetAttr()->NbAttr+1,ptr=n->GetAttr()->List;--i;ptr++)
	{
		sprintf(tmp2,"\"%s\"",Doc->Words.GetPtr<unsigned int>(*ptr,false)->W());
		strcat(tmp,tmp2);
		if(i>1)
			strcat(tmp,",");
	}
	//strcat(tmp,")");
	item=cur;
	item2=0;
	if(p)
		item=new QListViewItem(p,item,tmp);
	else
		item=new QListViewItem(this,tmp);
	item->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/filesystems/folder.png"));

	// Continue the tree with the subnodes.
	for(i=n->GetNbSubNodes()+1,N=&Nodes->Used.Tab[n->GetSubNodes()];--i;N++)
	{
		ConstNode(item,item2,*N);
	}

	// Continue the tree with the subobjects.
	for(i=n->GetNbSubObjects()+1,O=&Nodes->ObjsAss.Tab[n->GetSubObjects()];--i;O++)
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
void QListWords::setNodes(RNodesGA<RNodeWords,RObjH,RNodeWordsData>* nodes)
{
	static char tmp[200],tmp2[100];
	unsigned int i,j;
	const unsigned int* ptr;
	RNodeWords** N;
	QListViewItem* cur=0;
	RObjH** O;

	Nodes=nodes;
	clear();

	// Construct the tree
	for(i=Nodes->Top->GetNbSubNodes()+1,N=&Nodes->Used.Tab[Nodes->Top->GetSubNodes()];--i;N++)
		ConstNode(0,cur,*N);

	// Continue the tree with the subobjects.
	for(i=Nodes->Top->GetNbSubObjects()+1,O=&Nodes->ObjsAss.Tab[Nodes->Top->GetSubObjects()];--i;O++)
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
		cur=new QListViewItem(this,cur,tmp);
		cur->setPixmap(0,QPixmap("/usr/share/icons/hicolor/16x16/mimetypes/document.png"));
	}

	repaint();
}


//-----------------------------------------------------------------------------
QListWords::~QListWords(void)
{
}
