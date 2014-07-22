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
// include files for Qt/KDE
#include <qmenu.h>
#include <QtGui/QMouseEvent>
#include <kiconloader.h>


//-----------------------------------------------------------------------------
// include files for current applications
#include <qtreeinfos.h>
#include <ui_qtreeinfos.h>
#include <kdevhga.h>



//-----------------------------------------------------------------------------
//
// class MyItem
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class MyItem : public QTreeWidgetItem, public QObject
{
public:
	RObjH* Obj;
	GNodeInfos* Node;

	MyItem(R::iRProblemH<R::RObjH>* problem,QTreeWidgetItem* parent/*,QTreeWidgetItem* item*/,RObjH* obj);
	MyItem(R::iRProblemH<R::RObjH>* problem,QTreeWidgetItem* parent,QTreeWidgetItem* item,GNodeInfos* node);
	MyItem(R::iRProblemH<R::RObjH>* problem,QTreeWidget* parent,GNodeInfos* node);
};


//-----------------------------------------------------------------------------
MyItem::MyItem(R::iRProblemH<R::RObjH>*,QTreeWidgetItem* parent,/*QTreeWidgetItem* item,*/RObjH* obj)
	: QTreeWidgetItem(parent), Obj(obj), Node(0)
{
/*	if(obj->GetId()==2)
		cout<<"Debug"<<endl;*/
//	cout<<"  Paint Obj "<<obj->GetId()<<endl;
	setText(0,ToQString(obj->GetName()));
	setIcon(0,QPixmap(KIconLoader::global()->loadIcon("document",KIconLoader::Small)));
}


//-----------------------------------------------------------------------------
MyItem::MyItem(R::iRProblemH<R::RObjH>* problem,QTreeWidgetItem* parent,QTreeWidgetItem* item,GNodeInfos* node)
	: QTreeWidgetItem(parent,item), Obj(0), Node(node)
{
	QString str;
	unsigned int i;

	RAttrList View(20);
	if(!Node->GetParent()) // theApp->MustDisplayFull())
		View=node->GetAttr();
	else
	{

		View.Diff(node->GetParent()->GetAttr(),node->GetAttr());
	}
	for(i=0;i<View.GetNbAttr();i++)
	{
		if(i>0)
			str+=",";
		str+=ToQString(problem->GetAttrLabel(View[i]));
	}
	setText(0,str);
	setIcon(0,QPixmap(KIconLoader::global()->loadIcon("folder",KIconLoader::Small)));
}


//-----------------------------------------------------------------------------
MyItem::MyItem(R::iRProblemH<R::RObjH>* problem,QTreeWidget* tree,GNodeInfos* node)
	: QTreeWidgetItem(tree), Obj(0), Node(node)
{
	QString str;
	unsigned int i;

	RAttrList View(20);
	if(!Node->GetParent()) //theApp->MustDisplayFull())
		View=node->GetAttr();
	else
	{
		View.Diff(node->GetParent()->GetAttr(),node->GetAttr());
	}
	for(i=0;i<View.GetNbAttr();i++)
	{
		if(i>0)
			str+=",";
		str+=ToQString(problem->GetAttrLabel(View[i]));
	}
	setText(0,str);
	setIcon(0,QPixmap(KIconLoader::global()->loadIcon("folder",KIconLoader::Small)));
}



//-----------------------------------------------------------------------------
//
// class QTreeInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
QTreeInfos::QTreeInfos(QWidget* parent)
	: QWidget(parent), Ui(new Ui_QTreeInfos()), Chromo(0)
{
	static_cast<Ui_QTreeInfos*>(Ui)->setupUi(this);
	static_cast<Ui_QTreeInfos*>(Ui)->Tree->setRootIsDecorated(true);
	static_cast<Ui_QTreeInfos*>(Ui)->Tree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(static_cast<Ui_QTreeInfos*>(Ui)->Tree,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(contextMenu(const QPoint&)));
}


//-----------------------------------------------------------------------------
void QTreeInfos::constObjs(RCursor<RObjH> objs,QTreeWidgetItem* item)
{
	for(objs.Start();!objs.End();objs.Next())
		new MyItem(Problem,item,objs());
}


//-----------------------------------------------------------------------------
void QTreeInfos::constNode(QTreeWidgetItem* p,QTreeWidgetItem*& cur,GNodeInfos* n)
{
	QTreeWidgetItem* item(0);

	if((!theApp->MustDisplayTerminals())&&(!n->GetNbNodes()))
	{
		item=p;
	}
	else
	{
		QTreeWidgetItem* item2=0;

		item=cur;
		if(p)
			item=new MyItem(Problem,p,item,n);
		else
			item=new MyItem(Problem,static_cast<Ui_QTreeInfos*>(Ui)->Tree,n);

		// Continue the tree with the subnodes.
		RNodeCursor<GChromoH,GNodeInfos> Cur(n);
		for(Cur.Start();!Cur.End();Cur.Next())
			constNode(item,item2,Cur());
	}

	// Continue the tree with the subobjects.
	if(theApp->MustDisplayObjects())
		constObjs(n->GetObjs(),item);
}


//-----------------------------------------------------------------------------
void QTreeInfos::setProblem(R::iRProblemH<R::RObjH>* problem)
{
	Problem=problem;
}


//-----------------------------------------------------------------------------
void QTreeInfos::setNodes(GChromoH* chromo)
{
	Chromo=chromo;
	repaint();
}


//------------------------------------------------------------------------------
void QTreeInfos::contextMenu(const QPoint& pos)
{
	MyItem* item(dynamic_cast<MyItem*>(static_cast<Ui_QTreeInfos*>(Ui)->Tree->itemAt(pos)));
	if(!item)
		return;

	RAttrList attr(40);
	QMenu Menu;

	if(item->Node)
	{
		Menu.addAction("Node "+QString::number(item->Node->GetId()));
		attr=item->Node->GetAttr();
	}
	else if(item->Obj)
	{
		Menu.addAction(ToQString(item->Obj->GetName())+" ("+QString::number(item->Obj->GetId())+")");
		attr=item->Obj->GetAttr();
	}

	Menu.addSeparator();

	// Put the attributes
	for(size_t i=0;i<attr.GetNbAttr();i++)
		Menu.addAction("  "+ToQString(Problem->GetAttrLabel(attr[i]))+" ("+QString::number(attr[i])+")");

	Menu.exec(mapToGlobal(pos));
}


//------------------------------------------------------------------------------
void QTreeInfos::repaint(void)
{
	if((!Chromo)||(!Problem))
		return;

	QTreeWidgetItem* cur=0;
	static_cast<Ui_QTreeInfos*>(Ui)->Tree->clear();
	RNodeCursor<GChromoH,GNodeInfos> Cur(*Chromo);
	for(Cur.Start();!Cur.End();Cur.Next())
		constNode(0,cur,Cur());
}


//-----------------------------------------------------------------------------
QTreeInfos::~QTreeInfos(void)
{
}
