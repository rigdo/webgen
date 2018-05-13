/*
 * XmrigNvidia.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "XmrigNvidia.h"

//==============================================================================
//===================== XmrigNvidia ===========================================
//==============================================================================
XmrigNvidia::XmrigNvidia(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("xmrig-nvidia"));
	help_text->setText( tr("xmrig-nvidia_help") );

	autostart_checkbox = new WCheckBox();
	pool_lineedit = new WLineEdit();
	pool_lineedit->setTextSize(49);
	pool_lineedit->setStyleClass("settingvalue");
	user_lineedit = new WLineEdit();
	user_lineedit->setTextSize(100);
	user_lineedit->setStyleClass("settingvalue");
	pass_lineedit = new WLineEdit();
	pass_lineedit->setTextSize(49);
	pass_lineedit->setStyleClass("settingvalue");
	cuda_launch_lineedit = new WLineEdit();
	cuda_launch_lineedit->setTextSize(49);
	cuda_launch_lineedit->setStyleClass("settingvalue");

	algo_combobox = new WComboBox();
	algo_combobox->addItem( "cryptonight" );
	algo_combobox->addItem( "cryptonight-heavy" );
	algo_combobox->addItem( "cryptonight-lite" );
	algo_combobox->setStyleClass("xmrig_tty");
	
	nicehash_combobox = new WComboBox();
	nicehash_combobox->addItem( "auto" );
	nicehash_combobox->addItem( "on" );
	nicehash_combobox->addItem( "off" );
	nicehash_combobox->setStyleClass("xmrig_tty");

	console_combobox = new WComboBox();
	console_combobox->addItem( "" );
	console_combobox->addItem( "1" );
	console_combobox->addItem( "2" );
	console_combobox->addItem( "3" );	
	console_combobox->setStyleClass("xmrig_tty");
	
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
		WText *label = new WText(tr("xmrig-nvidia_bind_tty"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( console_combobox );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig-nvidia_pool"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( pool_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig-nvidia_user"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( user_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig-nvidia_pass"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( pass_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig_algo"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( algo_combobox );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig_nicehash_mode"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( nicehash_combobox );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("xmrig-nvidia_cuda-launch"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( cuda_launch_lineedit );
		datacolumn->addWidget( c );
		}
	}
	save_button->clicked().connect( this, &XmrigNvidia::saveParams );
	loadParams();
}

void XmrigNvidia::loadParams()
{
	Settings s = sd->byService("xmrig-nvidia0");
	
	pool_lineedit->setText( s.value("XMRIGN_POOL", "xmr-eu.dwarfpool.com:8080") );
	user_lineedit->setText( s.value("XMRIGN_USER", "45v1SBynnRMKxz3faNASnR2GiEc1TDbFgcZym94VuaJkCMSRsw66VxGRXHtDSHw84eYrW4HtPHcxCc3sP29SNDcnGB5PvXh") );
	pass_lineedit->setText( s.value("XMRIGN_PASS", "") );
	cuda_launch_lineedit->setText( s.value("XMRIGN_CUDA_LAUNCH", "") );

	WString algo_str = s.value("XMRIGN_ALGO", "");
	int algo_idx = algo_combobox->findText(algo_str);
	if(algo_idx >= 0 )
		algo_combobox->setCurrentIndex( algo_idx );
	else
		algo_combobox->setCurrentIndex( 0 );
	
	WString nicehash_str = s.value("XMRIGN_NICEHASH_MODE", "auto");
	int nicehash_idx = nicehash_combobox->findText(nicehash_str);
	if(nicehash_idx >= 0 )
		nicehash_combobox->setCurrentIndex( nicehash_idx );
	else
		nicehash_combobox->setCurrentIndex( 0 );
	
	WString v_str = s.value("XMRIGN_BIND_TTY", "");
	int idx = console_combobox->findText(v_str);
	if(idx >= 0 )
		console_combobox->setCurrentIndex( idx );
	else
		console_combobox->setCurrentIndex( 0 );
	autostart_checkbox->setChecked( s.valueInt("AUTOSTART", 0) );
}

void XmrigNvidia::saveParams()
{
	Settings s = sd->byService("xmrig-nvidia0");
	s.save( "XMRIGN_POOL", pool_lineedit->text() );
	s.save( "XMRIGN_USER", user_lineedit->text() );
	s.save( "XMRIGN_PASS", pass_lineedit->text() );
	s.save( "XMRIGN_CUDA_LAUNCH", cuda_launch_lineedit->text() );
	s.save( "XMRIGN_ALGO", algo_combobox->currentText() );
	s.save( "XMRIG_NICEHASH_MODE", nicehash_combobox->currentText() );
	s.save( "XMRIGN_BIND_TTY", console_combobox->currentText() );
	s.saveInt( "AUTOSTART", autostart_checkbox->isChecked());
}
