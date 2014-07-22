/*

	KHGAView.h

	HGA Window - Header.

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
#ifndef KHGAView_H
#define KHGAView_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <robject.h>
using namespace R;


//-----------------------------------------------------------------------------
#include <ghga.h>
using namespace GALILEI;


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_khgaview.h>
#include <kprjview.h>


//-----------------------------------------------------------------------------
/**
* The KHGAGAView class provides a representation of a result of a GA that is
* running.
* @author Pascal Francq
*/
class KHGAView : public QMdiSubWindow, public Ui_KHGAView, public RObject
{
	Q_OBJECT

	/**
	 * Project
	*/
	KPrjView* Project;

	/**
	* Identifier of the current showed solution.
	*/
	size_t CurId;

	/**
	* The GA that will be used.
	*/
	GInstH* Instance;

	/**
	* Number of generation already executed.
	*/
	size_t Gen;

	/**
	 * Number of windows.
	 */
	static size_t WinNb;

	/*
	 * Is the GA Running ?
	*/
	bool Running;

public:

	/**
	* Constructor for the view.
	*/
	KHGAView(KPrjView* project);

	virtual RCString GetClassName(void) const {return("KHGAView");}

	/**
	* Run the GA.
	*/
	void RunGA(void);

	/**
	* Pause the GA.
	*/
	void PauseGA(void);

	/**
	 * See if the GA is running.
    * @return true if yes.
    */
	bool IsRunning(void) const {return(Running);}

	/**
	 * See if the GA is end.
    * @return true if yes.
    */
	bool End(void) const;

protected:

	/**
	 * catch a generation notification.
	 */
	void Generation(const R::RNotification& notification);

	/**
	 * catch a best notification.
	 */
	void BestChromo(const R::RNotification& notification);

	/**
	 * catch a interact notification.
	 */
	void Interact(const R::RNotification& notification);

	/**
	* Key release event method. The implementation is needed to change the
	* chromosome to show when the user uses the keys "Page Up" and "Page Down".
	* With "Ctrl-G", a dialog box appears to select directly the identifier.
	*/
	virtual void keyReleaseEvent(QKeyEvent* e);

public:

	/**
	* Destruct the view.
	*/
	~KHGAView(void);
};


//-----------------------------------------------------------------------------
#endif
