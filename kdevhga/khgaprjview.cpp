/*

  khgaprjview.cpp

  Description - Implementation.

  (c) 2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

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
#include <rstd/rcontainercursor.h>
using namespace RStd;


//-----------------------------------------------------------------------------
// include files for current application
#include "khgaprjview.h"
#include "kdevhgadoc.h"



//-----------------------------------------------------------------------------
//
// class KHGAPrjView
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
KHGAPrjView::KHGAPrjView(KDevHGADoc* pDoc,QWidget *parent,const char *name,int wflags)
	: KDevHGAView(pDoc,parent,name,wflags),prj(0)
{
	prj=new QListView(this,"Project Elements"+doc->URL().path());
	prj->addColumn("Elements");
	prj->setRootIsDecorated(true);
	prj->setSorting(-1);
	createPrj();
}


//-----------------------------------------------------------------------------
void KHGAPrjView::createPrj(void)
{
	char tmp[100];
	QListViewItem *item=0,*item2=0,*item3;
	RContainerCursor<RObjH,unsigned int,true,false> Objs(doc->Objs);
	unsigned int i;
	unsigned int *ptr;

	// Construct Objects
	sprintf(tmp,"Objects (%u)",doc->Objs->NbPtr);
	item = new QListViewItem(prj,item,tmp);
	for(Objs.Start();!Objs.End();Objs.Next())
	{

		// Name of the object
		sprintf(tmp,"%s (%u)",Objs()->GetName()(),Objs()->GetId());
		item2 = new QListViewItem(item,item2,tmp);
		item3=0;

		// Words
		for(i=Objs()->GetAttr()->NbAttr+1,ptr=Objs()->GetAttr()->List;--i;ptr++)
		{
			sprintf(tmp,"Attribute: \"%s\" (%u)",doc->Words.GetPtr<unsigned int>(*ptr,false)->W(),*ptr);
			item3=new QListViewItem(item2,item3,tmp);
		}
		
	}
}


//-----------------------------------------------------------------------------
void KHGAPrjView::resizeEvent(QResizeEvent *)
{
	prj->resize(width(),height());
}


//-----------------------------------------------------------------------------
KHGAPrjView::~KHGAPrjView()
{
}
