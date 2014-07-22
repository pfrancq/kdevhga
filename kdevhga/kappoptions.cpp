/*

	RVLSI Project Library

	KAppOptions.cpp

	Application options - Implementation.

	Copyright 2000-2014 by Pascal Francq (pascal@francq.info).

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



//------------------------------------------------------------------------------
// include files for R
#include <rqt.h>


//------------------------------------------------------------------------------
// includes files for current application
#include <kdevhga.h>
#include <kappoptions.h>


//------------------------------------------------------------------------------
//
// class KAppOptions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
KAppOptions::KAppOptions(QWidget* parent)
	: KDialog(parent), Ui_KAppOptions()
{
	setCaption("Preferences");
	QWidget* widget=new QWidget(this);
	setupUi(widget);
	setMainWidget(widget);
	setButtons(KDialog::Cancel|KDialog::Apply);
	connect(this,SIGNAL(applyClicked()),this,SLOT(accept()));
}


//------------------------------------------------------------------------------
void KAppOptions::exec(KDevHGA* app)
{
	PopSize->setValue(app->GetConfig()->GetUInt("Population Size"));
	MaxGen->setValue(app->GetConfig()->GetUInt("Max Gen"));
	VerifyGA->setChecked(app->GetConfig()->GetBool("Verify GA"));

	Step->setChecked(app->GetConfig()->GetBool("Step"));
	StepGen->setValue(app->GetConfig()->GetUInt("Step Gen"));
	StepGen->setEnabled(app->GetConfig()->GetBool("Step"));

	DisplayFull->setChecked(app->GetConfig()->GetBool("Display Full"));
	DisplayTerminals->setChecked(app->GetConfig()->GetBool("Display Terminals"));
	DisplayObjects->setChecked(app->GetConfig()->GetBool("Display Objects"));

	if(KDialog::exec())
	{
		app->GetConfig()->SetUInt("Population Size",PopSize->value());
		app->GetConfig()->SetUInt("Max Gen",MaxGen->value());
		app->GetConfig()->SetBool("Verify GA",VerifyGA->isChecked());

		app->GetConfig()->SetBool("Step",Step->isChecked());
		app->GetConfig()->SetUInt("Step Gen",StepGen->value());

		app->GetConfig()->SetBool("Display Full",DisplayFull->isChecked());
		app->GetConfig()->SetBool("Display Terminals",DisplayTerminals->isChecked());
		app->GetConfig()->SetBool("Display Objects",DisplayObjects->isChecked());

		app->Apply();
	}
}


//------------------------------------------------------------------------------
void KAppOptions::readOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");
}


//------------------------------------------------------------------------------
void KAppOptions::saveOptions(void)
{
	KConfig Config;
	KConfigGroup General(&Config,"Configure");
}
