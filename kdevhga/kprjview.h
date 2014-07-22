/*

	KPrjView.h

	Project Window - Header.

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
#ifndef KPrjView_H
#define KPrjView_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <ruri.h>
#include <robjh.h>
#include <irproblemh.h>


//-----------------------------------------------------------------------------
// include files for KDE/Qt
#include <QtGui/QMdiSubWindow>


//---------------------------------------------------------------------------
// include files for current application
#include <ui_kprjview.h>
#include <kprjview.h>


//-----------------------------------------------------------------------------
/**
* The KPrjView provides a representation of a project window.
* @author Pascal Francq
*/
class KPrjView : public QMdiSubWindow, public Ui_KPrjView, public R::iRProblemH<R::RObjH>
{
	Q_OBJECT

	class Word;

	/**
	 * URI of the file.
	 */
	R::RURI URI;

	/**
	* The list of the views currently connected to the document.
	*/
	QList<QMdiSubWindow*> Views;

	/**
	* The objects to handle.
	*/
	R::RContainer<R::RObjH,true,false> Objs;

	/**
	* Container of words.
	*/
	R::RContainer<Word,true,true> Words;

public:

	/**
	 * Construct a project view.
	* @param uri             URI of the project.
	*/
	KPrjView(const QString& uri);

	/**
	 * Get the URI of the project.
    * @return
    */
	R::RURI GetURI(void) const {return(URI);}

	/**
	 * Get the objects for which a hierarchy must be found.
    * @return a cursor over the objects.
    */
	virtual R::RCursor<R::RObjH> GetObjs(void) const;

	/**
	 * Get a label for a given attribute.
    * @param id             Identifier of the attribute.
    * @return a label.
    */
	virtual RString GetAttrLabel(size_t id) const;

	/**
	 * Add a child window.
    */
	void add(QMdiSubWindow* child);

	/**
	 * Remove a child window.
    */
	void remove(QMdiSubWindow* child);

private:

	/**
	 * Load the project.
    */
	void loadPrj(void);

	/**
	* Construct the project tree.
	*/
	void createPrj(void);

	/**
	 * Accept close element.
    * @param event
    */
	virtual void closeEvent(QCloseEvent* event);

public:

	/**
	* Destruct the project view.
	*/
	~KPrjView(void);
};


//-----------------------------------------------------------------------------
#endif
