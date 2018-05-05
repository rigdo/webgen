/*
 * OpenVpn.h
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */

#ifndef OPENVPN_H_
#define OPENVPN_H_

#include "BasePage.h"
#include "Settings.h"
#include "UploadedFileWidget.h"
#include <Wt/WCheckBox>
#include <Wt/WGroupBox>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WFileUpload>
#include <Wt/WTable>

using namespace Wt;

//==============================================================================
//===================== OpenVpnPage ======================================
//==============================================================================
class OpenVpnPage : public BasePage
{
public:
	OpenVpnPage(SettingsDir *sd, int idx );
	virtual ~OpenVpnPage(){}
	
private:
	void loadParams();
	void saveParams();
	void genStaticKey();
	void enableChanged( );
	void configModeChanged( int newval );
	void workModeChanged( int );
	void authByChanged( int newval );
	
private:
	SettingsDir *sd;
	int idx;
//	WCheckBox *enable_checkbox;
	ServiceControlWidget *servicecontrol;
	WContainerWidget *configmode_conteiner;
	WComboBox *configmode_combobox;
	
	WGroupBox *mainsettings_groupbox;

	WComboBox *workmode_combobox;
	WComboBox *proto_combobox;
	WComboBox *devtype_combobox;
	WComboBox *cipher_combobox;
	WContainerWidget *connectaddr_container;
	WContainerWidget *connectport_container;
	WContainerWidget *listenport_container;
	WLineEdit *connectaddr_lineedit;
	WLineEdit *connectport_lineedit;
	WLineEdit *listenport_lineedit;
	
	WGroupBox *p2psettings_groupbox;
	WLineEdit *ifconfiglocalip_lineedit;
	WLineEdit *ifconfigremoteip_lineedit;
//	WTextArea *statickey_textarea;
	UploadedFileWidget *statickey_fileupload;
	WPushButton *generatestatickey_button;
	
	WGroupBox *clientsettings_groupbox;
	UploadedFileWidget *cacrt_fileupload;
	WComboBox *authby_combobox;
	WContainerWidget *authbykey_container;
	WContainerWidget *authbypass_container;
	UploadedFileWidget *clientcrt_fileupload;
	UploadedFileWidget *clientkey_fileupload;
	UploadedFileWidget *takey_fileupload;
	WLineEdit *authuser_lineedit;
	WLineEdit *authpass_lineedit;
	WTextArea *additionaloptsclient_textarea;
	
	WGroupBox *serversettings_groupbox;
	WLineEdit *servifconfig_lineedit;
	UploadedFileWidget *servcacrt_fileupload;
	UploadedFileWidget *servcrt_fileupload;
	UploadedFileWidget *servkey_fileupload;
	UploadedFileWidget *servtakey_fileupload;
	UploadedFileWidget *dhkey_fileupload;
	WTextArea *additionaloptsserver_textarea;

	
	WContainerWidget *files_container;
	UploadedFileWidget *config_fileupload;
	UploadedFilesTable *ftable;
};

#endif /* OPENVPN_H_ */
