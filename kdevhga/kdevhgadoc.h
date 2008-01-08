/*

	KDevHGADoc.h

	Document representing a HGA problem - Header.

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
#ifndef KDevHGADoc_H
#define KDevHGADoc_H


//-----------------------------------------------------------------------------
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif 


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <robjs.h>
#include <robjh.h>
using namespace R;


//-----------------------------------------------------------------------------
// include files for Qt
#include <qobject.h>
#include <qstring.h>
#include <qlistview.h>


//-----------------------------------------------------------------------------
// include files for KDE
#include <kurl.h>


//-----------------------------------------------------------------------------
// forward declaration of the KDevHGA classes
class KDevHGAView;
class KHGAGAView;
class KHGAPrjView;
class KHGAHeuristicView;
class QTreeInfos;


//-----------------------------------------------------------------------------
/**
* The class Word represents a word and an id.
*/
class Word
{
public:
	RString W;
	unsigned int Id;

	Word(const RString& w) : W(w), Id(0xFFFFFFFF) {}
	int Compare(const Word* w) const {return(W.Compare(w->W));}
	int Compare(const Word& w) const {return(W.Compare(w.W));}
	int Compare(const RString& str) const {return(W.Compare(str));}
	int Compare(const unsigned int id) const {return(Id-id);}
};


//-----------------------------------------------------------------------------
/**
* KDevHGADoc provides a document object for a document-view model.
*
* The KDevHGADoc class provides a document object that can be used in
* conjunction with the classes KDevHGAApp and KDevHGAView to create a
* document-view model for MDI (Multiple Document Interface) KDE 2 applications
* based on KApplication and KTMainWindow as main classes and QWorkspace as MDI
* manager widget.
*
* Thereby, the document object is created by the KDevHGAApp instance (and kept
* in a document list) and contains the document structure with the according
* methods for manipulating the document data by KDevHGAView objects. Also,
* KDevHGADoc contains the methods for serialization of the document data from
* and to files.
* @author Pascal Francq
* @short HGA Document.
*/
class KDevHGADoc : public QObject
{
	Q_OBJECT

	/**
	* The modified flag of the current document.
	*/
	bool modified;

	/**
	* The URL of the document representing the problem.
	*/
	KURL doc_url;

	/**
	* The list of the views currently connected to the document.
	*/
	QList<KDevHGAView> *pViewList;

	/**
	* The objects to handle.
	*/
	RObjs<RObjH>* Objs;

	/**
	* Container of words.
	*/
	RContainer<Word,true,true> Words;

public:

	/**
	* Constructor for the fileclass of the application.
	*/
	KDevHGADoc(void);

	/**
	 */
	RString GetWord(size_t id) const;
	
	/**
	* Adds a view to the document which represents the document contents.
	* Usually this is your main view.
	*/
	void addView(KDevHGAView* view);

	/**
	* Removes a view from the list of currently connected views.
	*/
	void removeView(KDevHGAView* view);

	/**
	* Gets called if a view is removed or added.
	*/
	void changedViewList(void);

	/**
	* Returns the first view instance.
	*/
	KDevHGAView* firstView(void) {return pViewList->first();}

	/**
	* Returns true, if the requested view is the last view of the document.
	*/
	bool isLastView(void);

	/**
	* This method gets called when the user is about to close a frame window.
	* It checks, if more than one view is connected to the document (then the
	* frame can be closed), if pFrame is the last view and the document is
	* modified, the user gets asked if he wants to save the document.
	*/
	bool canCloseFrame(KDevHGAView* pFrame);

	/**
	* Sets the modified flag for the document after a modifying action on the
	* view connected to the document.
	*/
	void setModified(bool _m=true){ modified=_m; };

	/**
	* Returns if the document is modified or not. Use this to determine if your
	* document needs saving by the user on closing.
	*/
	bool isModified(void) {return modified;}

	/**
	* Deletes the document's contents.
	*/
	void deleteContents(void);

	/**
	* Initializes the document generally.
	*/
	bool newDocument(void);

	/**
	* Closes the acutal document.
	*/
	void closeDocument(void);

	/**
	* Loads the document by filename and format and emits the updateViews()
	* signal.
	*/
	bool openDocument(const KURL& url=0,const char* format=0);

	/**
	* Saves the document under filename and format.
	*/
	bool saveDocument(const KURL& url,const char* format=0);

	/**
	* Returns the KURL of the document.
	*/
	const KURL& URL(void) const;

	/**
	* Sets the URL of the document.
	*/
	void setURL(const KURL& url);

public slots:

	/** Calls repaint() on all views connected to the document object and is
	* called by the view by which the document has been changed. As this view
	* normally repaints itself, it is excluded from the paintEvent.
	*/
	void updateAllViews(KDevHGAView* sender);

public:

	/**
	* Destructor for the fileclass of the application.
	*/
	~KDevHGADoc(void);

	// friend classes
	friend class KDevHGAView;
	friend class KHGAGAView;
	friend class KHGAHeuristicView;
	friend class KHGAPrjView;
	friend class QTreeInfos;
};


//-----------------------------------------------------------------------------
#endif // KDevHGADoc_H
