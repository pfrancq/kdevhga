/*

	KDevHGAView.cpp

	Generic Window  - Implementation.

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


// include files for Qt
#include <qprinter.h>
#include <qpainter.h>
#include <qdir.h>
// include files for KDE

// application specific includes
#include "kdevhga.h"
#include "kdevhgaview.h"
#include "kdevhgadoc.h"

KDevHGAView::KDevHGAView(KDevHGADoc* pDoc, QWidget *parent, const char* name, int wflags)
 : QWidget(parent, name, wflags)
{
    doc=pDoc;
}

KDevHGAView::~KDevHGAView()
{
}

KDevHGADoc *KDevHGAView::getDocument() const
{
	return doc;
}

void KDevHGAView::update(KDevHGAView* pSender){
	if(pSender != this)
		repaint();
}

void KDevHGAView::print(QPrinter *pPrinter)
{
  if (pPrinter->setup(this))
  {
		QPainter p;
		p.begin(pPrinter);
		
		///////////////////////////////
		// TODO: add your printing code here
		///////////////////////////////
		
		p.end();
  }
}

void KDevHGAView::closeEvent(QCloseEvent* /*e*/)
{

// DO NOT CALL QWidget::closeEvent(e) here !!
// This will accept the closing by QCloseEvent::accept() by default.
// The installed eventFilter() in KDevHGAApp takes care for closing the widget
// or ignoring the close event
		
}
