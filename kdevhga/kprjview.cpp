/*

	KPrjView.cpp

	Project Window - Implementation.

	Copyright 1998-2014 by Pascal Francq (pascal@francq.info).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rqt.h>
#include <rxmlstruct.h>
#include <rxmlfile.h>
#include <rnodecursor.h>
#include <rnumcursor.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QMessageBox>
#include <QCloseEvent>
#include <QMdiArea>


//-----------------------------------------------------------------------------
// include files for current application
#include <kprjview.h>



//-----------------------------------------------------------------------------
//
// class KPrjView::Word
//
//-----------------------------------------------------------------------------
class KPrjView::Word
{
public:
	RString W;
	size_t Id;

	Word(const RString& w) : W(w), Id(cNoRef) {}
	int Compare(const Word* w) const {return(W.Compare(w->W));}
	int Compare(const Word& w) const {return(W.Compare(w.W));}
	int Compare(const RString& str) const {return(W.Compare(str));}
	int Compare(const unsigned int id) const {return(Id-id);}
};



//-----------------------------------------------------------------------------
//
// class KPrjView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KPrjView::KPrjView(const QString& uri)
	 : QMdiSubWindow(), Ui_KPrjView(), URI(FromQString(uri)), Objs(0), Words(1000)
{
	QWidget* ptr=new QWidget();
	setupUi(ptr);
	setWidget(ptr);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(uri);
	loadPrj();
	createPrj();
}


//-----------------------------------------------------------------------------
R::RCursor<R::RObjH> KPrjView::GetObjs(void) const
{
	return(R::RCursor<R::RObjH>(Objs));
}


//-----------------------------------------------------------------------------
RString KPrjView::GetAttrLabel(size_t id) const
{
	return(Words[id]->W);
}


//-----------------------------------------------------------------------------
void KPrjView::add(QMdiSubWindow* child)
{
	Views.insert(Views.size(),child);
}


//-----------------------------------------------------------------------------
void KPrjView::remove(QMdiSubWindow* child)
{
	Views.removeOne(child);
}


//-----------------------------------------------------------------------------
void KPrjView::loadPrj(void)
{
	RXMLStruct s;
	RXMLFile f(URI,&s);
	f.Open(RIO::Read);

	// Load Objects
	RXMLTag* tag(s.GetTag("Objects"));
	if(tag)
	{
		// Read number objects info
		Objs.VerifyTab(tag->GetNbNodes());

		// Read each objects
		size_t i;
		RObjH* obj;
		RNodeCursor<RXMLStruct,RXMLTag> Cur(tag);
		for(i=0,Cur.Start();!Cur.End();i++,Cur.Next())
		{
			if(Cur()->GetName()=="Object")
			{
				//RXMLTagCursor cWords();
				Objs.InsertPtr(obj=new RObjH(i,Cur()->GetAttrValue("Id"),Cur()->GetNbNodes()));
				RNodeCursor<RXMLStruct,RXMLTag> sub(Cur());
				for(sub.Start();!sub.End();sub.Next())
				{
					if(sub()->GetName()=="Include")
					{
						Word* w(Words.GetInsertPtr<RString>(sub()->GetAttrValue("Attribute")));
						if(w->Id==cNoRef)
							w->Id=Words.GetNb()-1;
						obj->AddAttribute(w->Id);
					}
					else
						mThrowRException("Invalid file");
				}
			}
			else
				mThrowRException("Invalid file");
		}
	}
	else
		mThrowRException("Invalid file");
}


//-----------------------------------------------------------------------------
void KPrjView::createPrj(void)
{
	QTreeWidgetItem* item(new QTreeWidgetItem(Elements,QStringList()<<"Objects ("+QString::number(Objs.GetNb())+")"));

	RCursor<RObjH> Obj(Objs);
	for(Obj.Start();!Obj.End();Obj.Next())
	{
		QTreeWidgetItem* item2(new QTreeWidgetItem(item,QStringList()<<ToQString(Obj()->GetName())+" ("+QString::number(Obj()->GetId())+")"));

		RNumCursor<size_t> Attr(Obj()->GetAttr());
		for(Attr.Start();!Attr.End();Attr.Next())
			new QTreeWidgetItem(item2,QStringList()<<"Attribute \""+ToQString(Words[Attr()]->W)+"\" ("+QString::number(Attr())+")");
	}
}


//-----------------------------------------------------------------------------
void KPrjView::closeEvent(QCloseEvent* event)
{
	QMessageBox msgBox;
	msgBox.setText("Are you sure you want to close this project?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	if(msgBox.exec()==QMessageBox::Yes)
	{
		while(!Views.isEmpty())
	     mdiArea()->removeSubWindow(Views.takeFirst());
		event->accept();
	}
	else
		event->ignore();
}


//-----------------------------------------------------------------------------
KPrjView::~KPrjView(void)
{
}
