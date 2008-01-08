/*

	QTreeInfos.cpp

	Widget representing a tree of vectors - Implementation.

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
// include files for R Project
#include <rqt.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for GALILEI Project
#include <gchromoh.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qpixmap.h>
#include <qpopupmenu.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kiconloader.h>
#include <kglobal.h>


//-----------------------------------------------------------------------------
// include files for current applications
#include <qtreeinfos.h>
#include "kdevhgadoc.h"
#include "khgaheuristicview.h"
#include "kdevhga.h"



//-----------------------------------------------------------------------------
//
// class MyItem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyItem : public QListViewItem
{
public:
	RObjH* Obj;
	GNodeInfos* Node;
	KDevHGADoc* Doc;
	
	MyItem(KDevHGADoc* doc,QListViewItem* parent,QListViewItem* item,RObjH* obj);
	MyItem(KDevHGADoc* doc,QListViewItem* parent,QListViewItem* item,GNodeInfos* node);
	MyItem(KDevHGADoc* doc,QListView* parent,GNodeInfos* node);
};


//-----------------------------------------------------------------------------
MyItem::MyItem(KDevHGADoc* doc,QListViewItem* parent,QListViewItem* item,RObjH* obj)
	: QListViewItem(parent,item,ToQString(obj->GetName())), Obj(obj), Node(0), Doc(doc)
{
	setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("document",KIcon::Small)));
}


//-----------------------------------------------------------------------------
MyItem::MyItem(KDevHGADoc* doc,QListViewItem* parent,QListViewItem* item,GNodeInfos* node)
	: QListViewItem(parent,item), Obj(0), Node(node), Doc(doc)
{
	QString str;
	unsigned int i;

	RAttrList View(20);
	if(theApp->MustDisplayFull())
		View=node->GetAttr();
	else
	{
		View.Diff(node->GetParent()->GetAttr(),node->GetAttr());
	}
	for(i=0;i<View.GetNbAttr();i++)
	{
		if(i>0)
			str+=",";
		str+=ToQString(Doc->GetWord(View[i]));
	}
	setText(0,str);
	setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("folder",KIcon::Small)));
}


//-----------------------------------------------------------------------------
MyItem::MyItem(KDevHGADoc* doc,QListView* parent,GNodeInfos* node)
	: QListViewItem(parent), Obj(0), Node(node), Doc(doc)
{
	QString str;
	unsigned int i;

	RAttrList View(20);
	if(theApp->MustDisplayFull())
		View=node->GetAttr();
	else
	{
		View.Diff(node->GetParent()->GetAttr(),node->GetAttr());
	}
	for(i=0;i<View.GetNbAttr();i++)
	{
		if(i>0)
			str+=",";
		str+=ToQString(Doc->GetWord(View[i]));
	}
	setText(0,str);
	setPixmap(0,QPixmap(KGlobal::iconLoader()->loadIcon("folder",KIcon::Small)));
}



//------------------------------------------------------------------------------
//
// QInfoBox
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The QInfoBox class provides a popupmenu that display informartion about a
* specific object and geometric information.
* @author Pascal Francq
* @short Popup object information
*/
class QInfoBox : public QPopupMenu
{
	/**
	* Widget that must have the focus after.
	*/
	QWidget* afterFocus;

public:

	/**
	* Constructor of the popup.
	* @param parent        Parent of the widget.
	* @param item          Item
	*/
	QInfoBox(KDevHGADoc* doc,QWidget* parent,MyItem* item);

protected:

	/**
	* Mouse release event method. When the mouse bouton is released, the popup
	* is closed.
	*/
	virtual void mouseReleaseEvent(QMouseEvent*);
};


//------------------------------------------------------------------------------
QInfoBox::QInfoBox(KDevHGADoc* doc,QWidget* parent,MyItem* item)
	: QPopupMenu(0,"Info Box")
{
	RAttrList attr(40);
	
	// Node?
	if(item->Node)
	{
		insertItem("Node "+QString::number(item->Node->GetId()));
		attr=item->Node->GetAttr();
	}
	
	// Object?
	if(item->Obj)
	{
		insertItem(ToQString(item->Obj->GetName())+" ("+QString::number(item->Obj->GetId())+")");
		attr=item->Obj->GetAttr();
	}

	// Put the attributes
	insertItem("Attributes:");
	for(size_t i=0;i<attr.GetNbAttr();i++)
		insertItem("  "+ToQString(doc->GetWord(attr[i]))+" ("+QString::number(attr[i])+")");
	
	// Prepare end
	afterFocus=parent;
	afterFocus->parentWidget()->setFocus();
}


//------------------------------------------------------------------------------
void QInfoBox::mouseReleaseEvent(QMouseEvent*)
{
	afterFocus->parentWidget()->setFocus();
	delete(this);
}



//-----------------------------------------------------------------------------
//
// class QTreeInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QTreeInfos::QTreeInfos(KDevHGADoc* pDoc,QWidget* parent)
	: QListView(parent,"Results"), Doc(pDoc), Chromos(0)
{
	addColumn("Resulting Tree");
	setRootIsDecorated(true);
	setSorting(-1);
	setSortColumn(-1);
	connect(this,SIGNAL(rightButtonPressed( QListViewItem *, const QPoint &, int )),this,SLOT(slotPressEvent( QListViewItem *, const QPoint &, int )));
	connect(theApp,SIGNAL(redrawTrees()),this,SLOT(redraw()));
}


//-----------------------------------------------------------------------------
void QTreeInfos::constObjs(RCursor<RObjH> objs,QListViewItem* item)
{
	QListViewItem* item2=0;

	for(objs.Start();!objs.End();objs.Next())
		item2=new MyItem(Doc,item,item2,objs());
}


//-----------------------------------------------------------------------------
void QTreeInfos::constNode(QListViewItem* p,QListViewItem*& cur,GNodeInfos* n)
{
	QListViewItem* item(0);
	
	if(p&&(!n->GetNbNodes())&&(!theApp->MustDisplayTerminals()))
	{
		item=p;
	}
	else
	{
		QListViewItem* item2=0;

		item=cur;
		if(p)
			item=new MyItem(Doc,p,item,n);
		else
			item=new MyItem(Doc,this,n);

		// Continue the tree with the subnodes.
		RCursor<GNodeInfos> Cur(Chromos->GetNodes(n));
		for(Cur.Start();!Cur.End();Cur.Next())
			constNode(item,item2,Cur());
	}

	// Continue the tree with the subobjects.
	if(theApp->MustDisplayObjects())
		constObjs(Chromos->GetObjs(n),item);
}


//-----------------------------------------------------------------------------
void QTreeInfos::setNodes(GChromoH* chromos)
{
	Chromos=chromos;
	redraw();
}


//------------------------------------------------------------------------------
void QTreeInfos::slotPressEvent(QListViewItem* item, const QPoint& pos,int)
{
	QInfoBox* InfoBox=new QInfoBox(Doc,this,dynamic_cast<MyItem*>(item));
	InfoBox->popup(pos);
}


//------------------------------------------------------------------------------
void QTreeInfos::redraw(void)
{
	QListViewItem* cur=0;

	clear();
	RCursor<GNodeInfos> Cur(Chromos->GetNodes(Chromos->GetTop()));
	for(Cur.Start();!Cur.End();Cur.Next())
		constNode(0,cur,Cur());
	repaint();
}


//-----------------------------------------------------------------------------
QTreeInfos::~QTreeInfos(void)
{
}
