/*

	QTreeInfos.cpp

	Widget representing a tree of vectors - Implementation.

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
#include <gchromoh.h>
using namespace R;
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt / KDE
#include <qpixmap.h>
#include <kiconloader.h>
#include <kglobal.h>


//-----------------------------------------------------------------------------
#include <qtreeinfos.h>
#include "kdevhgadoc.h"
#include "khgaheuristicview.h"



//-----------------------------------------------------------------------------
//
// class QTreeInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QTreeInfos::QTreeInfos(KDevHGADoc* pDoc,QWidget* parent)
	: QListView(parent,"Results"), Doc(pDoc), Chromos(0), Nodes(0)
{
	addColumn("Resulting Tree");
	setRootIsDecorated(true);
	setSorting(-1);
}


//-----------------------------------------------------------------------------
QString QTreeInfos::constAttr(const RAttrList* attr)
{
	QString str;
	unsigned int i;

	str="(";
	for(i=0;i<attr->GetNbAttr();i++)
	{
		if(i>0)
			str+=",";
		str+=QString("\"")+Doc->Words.GetPtr<unsigned int>((*attr)[i],false)->W.Latin1()+"\"";
	}
	str+=")";
	return(str);
}


//-----------------------------------------------------------------------------
void QTreeInfos::constObjs(RObjH** objs,unsigned int nb,QListViewItem* item)
{
	QString str;
	QListViewItem* item2=0;

	for(nb++;--nb;objs++)
	{
		str=QString("Objects ")+(*objs)->GetName().Latin1()+" "+constAttr((*objs)->GetAttr());
		item2=new QListViewItem(item,item2,str);
		item2->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("document",KIcon::Small)));
	}
}


//-----------------------------------------------------------------------------
void QTreeInfos::constNode(QListViewItem* p,QListViewItem*& cur,GNodeInfos* n)
{
	QString str;
	unsigned int i;
	GNodeInfos** N;
	QListViewItem* item2=0;
	QListViewItem* item=0;

	str="Node: "+QString::number(n->GetId())+" "+constAttr(n->GetAttr());
	item=cur;
	if(p)
		item=new QListViewItem(p,item,str);
	else
		item=new QListViewItem(this,str);
	item->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("folder",KIcon::Small)));

	// Continue the tree with the subnodes.
	for(i=n->GetNbNodes()+1,N=n->GetNodes();--i;N++)
	{
		constNode(item,item2,*N);
	}

	// Continue the tree with the subobjects.
	constObjs(n->GetObjects(),n->GetNbObjs(),item);
}


//-----------------------------------------------------------------------------
void QTreeInfos::constNode(QListViewItem* p,QListViewItem*& cur,MyNode* n)
{
	QString str;
	unsigned int i;
	MyNode** N;
	QListViewItem* item2=0;
	QListViewItem* item=0;

	str="Node: "+QString::number(n->GetId())+" "+constAttr(n->GetAttr());
	item=cur;
	if(p)
		item=new QListViewItem(p,item,str);
	else
		item=new QListViewItem(this,str);
	item->setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("folder",KIcon::Small)));

	// Continue the tree with the subnodes.
	for(i=n->GetNbNodes()+1,N=n->GetNodes();--i;N++)
	{
		constNode(item,item2,*N);
	}

	// Continue the tree with the subobjects.
	constObjs(n->GetObjects(),n->GetNbObjs(),item);
}


//-----------------------------------------------------------------------------
void QTreeInfos::setNodes(GChromoH* chromos)
{
	unsigned int i;
	GNodeInfos** N;
	QListViewItem* cur=0;

	Chromos=chromos;
	Nodes=0;
	clear();
	for(i=Chromos->Top->GetNbNodes()+1,N=Chromos->Top->GetNodes();--i;N++)
		constNode(0,cur,*N);
	repaint();
}


//-----------------------------------------------------------------------------
void QTreeInfos::setNodes(RNodesGA<MyNode,RObjH,GNodeInfosData,KHGAHeuristicView>* nodes)
{
	unsigned int i;
	MyNode** N;
	QListViewItem* cur=0;

	Nodes=nodes;
	Chromos=0;
	clear();
	for(i=Nodes->Top->GetNbNodes()+1,N=Nodes->Top->GetNodes();--i;N++)
		constNode(0,cur,*N);
	repaint();
}


//-----------------------------------------------------------------------------
QTreeInfos::~QTreeInfos(void)
{
}
