/*
 * Ethminer.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "Ethminer.h"
#include <Wt/WRegExpValidator>
#include <Wt/WBreak>

//==============================================================================
//===================== Ethminer ===========================================
//==============================================================================
Ethminer::Ethminer(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("ethminer"));
	help_text->setText( tr("ethminer_help") );

	autostart_checkbox = new WCheckBox();
	pool_lineedit = new WLineEdit();
	pool_lineedit->setTextSize(49);
	pool_lineedit->setStyleClass("settingvalue");
	user_lineedit = new WLineEdit();
	user_lineedit->setTextSize(49);
	user_lineedit->setStyleClass("settingvalue");
	console_combobox = new WComboBox();
	console_combobox->addItem( "" );
	console_combobox->addItem( "1" );
	console_combobox->addItem( "2" );
	console_combobox->addItem( "3" );	
	console_combobox->setStyleClass("xmrig_tty");

	
	save_button = new WPushButton(tr("Save"));
	{
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("autostart"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( autostart_checkbox );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("ethminer_pool"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( pool_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("ethminer_user"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( user_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig_bind_tty"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( console_combobox );
		datacolumn->addWidget( c );
		}
		
		datacolumn->addWidget( new WBreak() );
		
		WContainerWidget *buttonscontainer = new WContainerWidget();
		buttonscontainer->setStyleClass("buttonscontainer");
		buttonscontainer->addWidget( save_button );
		datacolumn->addWidget( buttonscontainer );
	}
	save_button->clicked().connect( this, &Ethminer::saveParams );
	loadParams();
}

void Ethminer::loadParams()
{
	Settings s = sd->byService("ethminer0");
	
	pool_lineedit->setText( s.value("ETHMINER_POOL", "eth-eu.dwarfpool.com:80") );
	user_lineedit->setText( s.value("ETHMINER_WALLET", "0x00747e59ee64851ecb100d18f3d51878463c0f94") );
//	servicecontrol->autostart_checkbox->setChecked( s.valueInt("AUTOSTART", 0) );
	
	WString v_str = s.value("ETHMINER_BIND_TTY", "");
	int idx = console_combobox->findText(v_str);
	if(idx >= 0 )
		console_combobox->setCurrentIndex( idx );
	else
		console_combobox->setCurrentIndex( 0 );
	autostart_checkbox->setChecked( s.valueInt("AUTOSTART", 0) );
}

void Ethminer::saveParams()
{
	Settings s = sd->byService("ethminer0");
	s.save( "ETHMINER_POOL", pool_lineedit->text() );
	s.save( "ETHMINER_WALLET", user_lineedit->text() );
	s.save( "ETHMINER_BIND_TTY", console_combobox->currentText() );
	s.saveInt( "AUTOSTART", autostart_checkbox->isChecked());
}
