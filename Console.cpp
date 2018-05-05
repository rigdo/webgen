/*
 * Console.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "Console.h"
#include <Wt/WRegExpValidator>

//==============================================================================
//===================== Console ===========================================
//==============================================================================
Console::Console(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("console"));
	help_text->setText( tr("console_help") );
	
	for(unsigned i=0; i< 6; i++){
		owner_combobox[i] = new WComboBox();
		owner_combobox[i]->addItem( tr("owner_none") );
		owner_combobox[i]->addItem( tr("owner_getty") );
		owner_combobox[i]->addItem( tr("owner_xmrig") );
		owner_combobox[i]->addItem( tr("owner_ethminer") );
	}
	
	save_button = new WPushButton(tr("Save"));
	{
		for(unsigned i=0; i< 6; i++){
			WContainerWidget *configmode_conteiner = new WContainerWidget();
			configmode_conteiner->setStyleClass("setting");
			WString l = WString("console{1}").arg(i);
			WText *label = new WText(l);
			label->setStyleClass("label");
			configmode_conteiner->addWidget( label );
			configmode_conteiner->addWidget( owner_combobox[i] );
			datacolumn->addWidget( configmode_conteiner );
		}
		
		WContainerWidget *buttonscontainer = new WContainerWidget();
		buttonscontainer->setStyleClass("buttonscontainer");
		buttonscontainer->addWidget( save_button );
		datacolumn->addWidget( buttonscontainer );
	}
	save_button->clicked().connect( this, &Console::saveParams );
	loadParams();
}

int getOwner(int v_xmrig, int v_ethm, int v_getty, int v_def)
{
	if(v_xmrig)
		return 2;
	if(v_ethm)
		return 3;
	if(v_getty)
		return 1;
	return 0;
}

void Console::loadParams()
{
#if 0
	Settings s_xmrig("xmrig0");
	int v_xmrig = s_xmrig.valueInt("XMRIG_BIND_TTY", -1);
	Settings s_ethm("ethminer0");
	int v_ethm = s_ethm.valueInt("ETHMINER_BIND_TTY", -1);
	
	for(unsigned i=0; i< 6; i++){
		Settings s("console%{1}", i+1);
		int v_getty;
		if (i < 2)
			v_getty = s.valueInt("AUTOSTART", 1);
		else
			v_getty = s.valueInt("AUTOSTART", 0);
		int v_def = 0;
		if(i == 0)
			v_def = 2;
		if(i == 1)
			v_def = 1;
		int idx = getOwner((v_xmrig == (i+1)), (v_ethm == (i+1)), v_getty, v_def);
		owner_combobox[i]->setCurrentIndex(idx);
	}
#endif
}

void Console::saveParams()
{
//	Settings s("xmrig0");
//	s.save( "XMRIG_POOL", pool_lineedit->text() );
//	s.save( "XMRIG_USER", user_lineedit->text() );
//	s.saveInt( "AUTOSTART", servicecontrol->autostart_checkbox->isChecked());
//
//	Settings s2("wpa_cli0");
//	s2.saveInt( "AUTOSTART", servicecontrol->autostart_checkbox->isChecked());
}
