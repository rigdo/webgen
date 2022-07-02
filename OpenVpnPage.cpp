/*
 * OpenVpn.cpp
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */
#include "OpenVpnPage.h"
#include <Wt/WComboBox.h>
#include <Wt/WTextArea.h>
#include <Wt/WLengthValidator.h>
#include <Wt/WAnchor.h>
#include <Wt/WFileResource.h>
#include <Wt/WLink.h>

#include <sys/stat.h>
#include <sys/types.h>

//==============================================================================
//===================== OpenVpnPage ======================================
//==============================================================================
OpenVpnPage::OpenVpnPage(SettingsDir *sd, int idx):
		BasePage()
{
	this->sd = sd;
	this->idx = idx;
	pagetitle_text->setText(tr("OpenVPN{1}").arg(idx + 1));

	WString servicename = WString("openvpn{1}").arg(idx);
	Settings s = sd->byService("openvpn", idx);

//	statickey_textarea = new WTextArea();
//	statickey_textarea->setColumns(40);
//	statickey_textarea->setRows(22);
//	WLengthValidator *validator = new WLengthValidator( 0, 4095 );
//	statickey_textarea->setValidator( validator );


	{
		servicecontrol = datacolumn->addWidget(std::make_unique<ServiceControlWidget>(servicename.toUTF8()));
		{
			WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Autostart")));
			label->setStyleClass("shortlabel");
			autostart_checkbox = c->addWidget(std::make_unique<WCheckBox>());
		}
		{
			configmode_conteiner = datacolumn->addWidget(std::make_unique<WContainerWidget>());
			configmode_conteiner->setStyleClass("setting");
			WText *label = configmode_conteiner->addWidget(std::make_unique<WText>(tr("Config via")));
			label->setStyleClass("label");
			configmode_combobox = configmode_conteiner->addWidget(std::make_unique<WComboBox>());
			configmode_combobox->addItem(tr("Config File"));
			configmode_combobox->addItem(tr("Web"));
		}

		mainsettings_groupbox = datacolumn->addWidget(std::make_unique<WGroupBox>());
		mainsettings_groupbox->setTitle(tr("Main Settings"));
		{
			WContainerWidget *c = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Work Mode")));
			label->setStyleClass("label");
			workmode_combobox = c->addWidget(std::make_unique<WComboBox>());
			workmode_combobox->addItem(tr("Client"));
			workmode_combobox->addItem(tr("Server"));
			workmode_combobox->addItem(tr("P2P Client"));
			workmode_combobox->addItem(tr("P2P Server"));

		}
		{
			connectaddr_container = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			connectaddr_container->setStyleClass("setting");
			WText *label = connectaddr_container->addWidget(std::make_unique<WText>(tr("Connect Addr")));
			label->setStyleClass("label");
			connectaddr_lineedit = connectaddr_container->addWidget(std::make_unique<WLineEdit>());
			connectaddr_lineedit->setStyleClass("iplineedit");
		}
		{
			connectport_container = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			connectport_container->setStyleClass("setting");
			WText *label = connectport_container->addWidget(std::make_unique<WText>(tr("Connect Port")));
			label->setStyleClass("label");
			connectport_lineedit = connectport_container->addWidget(std::make_unique<WLineEdit>());
			connectport_lineedit->setStyleClass("portlineedit");
		}
		{
			listenport_container = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			listenport_container->setStyleClass("setting");
			WText *label = listenport_container->addWidget(std::make_unique<WText>(tr("Listening Port")));
			label->setStyleClass("label");
			listenport_lineedit = listenport_container->addWidget(std::make_unique<WLineEdit>());
		}
		{
			WContainerWidget *c = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Protocol")));
			label->setStyleClass("label");
			proto_combobox = c->addWidget(std::make_unique<WComboBox>());
			proto_combobox->addItem("udp");
			proto_combobox->addItem("tcp");
		}
		{
			WContainerWidget *c = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Tunnel device")));
			label->setStyleClass("label");
			devtype_combobox = c->addWidget(std::make_unique<WComboBox>());
			devtype_combobox->addItem("tun");
			devtype_combobox->addItem("tap");
		}
		{
			WContainerWidget *c = mainsettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Cipher")));
			label->setStyleClass("label");
			cipher_combobox = c->addWidget(std::make_unique<WComboBox>());
			cipher_combobox->addItem("BF-CBC");
			cipher_combobox->addItem("AES-128-CBC");
			cipher_combobox->addItem("DES-EDE3-CBC");
		}

		p2psettings_groupbox = datacolumn->addWidget(std::make_unique<WGroupBox>());
		p2psettings_groupbox->setTitle(tr("Point-to-Point settings"));
		{
			WContainerWidget *c = p2psettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Local Ip in tunnel")));
			label->setStyleClass("label");
			ifconfiglocalip_lineedit = c->addWidget(std::make_unique<WLineEdit>());
			ifconfiglocalip_lineedit->setStyleClass("iplineedit");
		}
		{
			WContainerWidget *c = p2psettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("Remote Ip in tunnel")));
			label->setStyleClass("label");
			ifconfigremoteip_lineedit = c->addWidget(std::make_unique<WLineEdit>());
			ifconfigremoteip_lineedit->setStyleClass("iplineedit");
		}
		{
			WGroupBox *c = p2psettings_groupbox->addWidget(std::make_unique<WGroupBox>());
			c->setTitle(tr("Encryption key"));
			statickey_fileupload = c->addWidget(std::make_unique<UploadedFileWidget>(s.additionalSettingsDirName("guip2pconfig").toUTF8(), "static.key"));
			generatestatickey_button = c->addWidget(std::make_unique<WPushButton>(tr("Generate")));
		}


		clientsettings_groupbox = datacolumn->addWidget(std::make_unique<WGroupBox>());
		clientsettings_groupbox->setTitle(tr("Client settings"));
		{
			cacrt_fileupload = clientsettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(s.additionalSettingsDirName("guiclientconfig").toUTF8(), "ca.crt",
																									   "ca(ca.crt)"));
		}
		{
			takey_fileupload = clientsettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
					s.additionalSettingsDirName("guiclientconfig").toUTF8(), "ta.key",
					"tls-auth(ta.key)"));
		}
		{
			WGroupBox *gb = clientsettings_groupbox->addWidget(std::make_unique<WGroupBox>());
			gb->setTitle(tr("Auth by"));
			authby_combobox = gb->addWidget(std::make_unique<WComboBox>());
			authby_combobox->addItem("Key files");
			authby_combobox->addItem("Password");
			authbykey_container = gb->addWidget(std::make_unique<WContainerWidget>());
			clientcrt_fileupload = authbykey_container->addWidget(std::make_unique<UploadedFileWidget>(
					s.additionalSettingsDirName("guiclientconfig").toUTF8(),
					"client.crt", "cert(client.crt)"));
			clientkey_fileupload = authbykey_container->addWidget(std::make_unique<UploadedFileWidget>(
					s.additionalSettingsDirName("guiclientconfig").toUTF8(),
					"client.key", "key(client.key)"));

			authbypass_container = gb->addWidget(std::make_unique<WContainerWidget>());
			{
				WContainerWidget *c = authbypass_container->addWidget(std::make_unique<WContainerWidget>());
				c->setStyleClass("setting");
				WText *label = c->addWidget(std::make_unique<WText>(tr("User")));
				label->setStyleClass("label");
				authuser_lineedit = c->addWidget(std::make_unique<WLineEdit>());
			}
			{
				WContainerWidget *c = authbypass_container->addWidget(std::make_unique<WContainerWidget>());
				c->setStyleClass("setting");
				WText *label = c->addWidget(std::make_unique<WText>(tr("Password")));
				label->setStyleClass("label");
				authpass_lineedit = c->addWidget(std::make_unique<WLineEdit>());
			}
		}
		{
			WGroupBox *gc = clientsettings_groupbox->addWidget(std::make_unique<WGroupBox>());
			gc->setTitle(tr("Additionl options"));
			additionaloptsclient_textarea = gc->addWidget(std::make_unique<WTextArea>());
			additionaloptsclient_textarea->setColumns(55);
			additionaloptsclient_textarea->setRows(2);
			additionaloptsclient_textarea->setValidator(std::make_shared<WLengthValidator>(0, 4095));
		}


		serversettings_groupbox = datacolumn->addWidget(std::make_unique<WGroupBox>());
		serversettings_groupbox->setTitle(tr("Server settings"));
		{
			WContainerWidget *c = serversettings_groupbox->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("VPN subnet")));
			label->setStyleClass("label");
			servifconfig_lineedit = c->addWidget(std::make_unique<WLineEdit>());
			servifconfig_lineedit->setTextSize(24);
		}

		servcacrt_fileupload = serversettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
				s.additionalSettingsDirName("guiservconfig").toUTF8(), "ca.crt",
				"ca(ca.crt)"));
		servcrt_fileupload = serversettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
				s.additionalSettingsDirName("guiservconfig").toUTF8(), "server.crt",
				"cert(server.crt)"));
		servkey_fileupload = serversettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
				s.additionalSettingsDirName("guiservconfig").toUTF8(), "server.key",
				"key(server.key)"));
		servtakey_fileupload = serversettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
				s.additionalSettingsDirName("guiservconfig").toUTF8(), "ta.key",
				"tls-auth(ta.key)"));
		dhkey_fileupload = serversettings_groupbox->addWidget(std::make_unique<UploadedFileWidget>(
				s.additionalSettingsDirName("guiservconfig").toUTF8(), "dh1024.pem",
				"dh(dh1024.pem)"));

		{
			WGroupBox *gc = serversettings_groupbox->addWidget(std::make_unique<WGroupBox>());
			gc->setTitle(tr("Additionl options"));
			additionaloptsserver_textarea = gc->addWidget(std::make_unique<WTextArea>());
			additionaloptsserver_textarea->setColumns(55);
			additionaloptsserver_textarea->setRows(2);
			additionaloptsserver_textarea->setValidator(std::make_shared<WLengthValidator>(0, 4095));

		}

		files_container = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		{
			WGroupBox *gc = files_container->addWidget(std::make_unique<WGroupBox>());
			gc->setTitle(tr("Main config File"));
			config_fileupload = gc->addWidget(std::make_unique<UploadedFileWidget>(s.additionalSettingsDirName("usrconfig").toUTF8(), "openvpn.conf"));
		}
		ftable = files_container->addWidget(std::make_unique<UploadedFilesTable>(s.additionalSettingsDirName("usrconfig").toUTF8()));
		ftable->setTitle(tr("Additional Files"));
	}
//	autostart_checkbox->checked().connect( this,
//			&OpenVpnPage::enableChanged );
//	autostart_checkbox->unChecked().connect( this,
//			&OpenVpnPage::enableChanged );
	save_button->clicked().connect(this, &OpenVpnPage::saveParams);
	configmode_combobox->activated().connect(this,
			&OpenVpnPage::configModeChanged);
	workmode_combobox->activated().connect(this, &OpenVpnPage::workModeChanged);
	authby_combobox->activated().connect(this, &OpenVpnPage::authByChanged);
	generatestatickey_button->clicked().connect(this,
			&OpenVpnPage::genStaticKey);
	loadParams();
}

void OpenVpnPage::genStaticKey()
{
	system("/usr/sbin/openvpn --genkey --secret /tmp/static.key");
	Settings s = sd->byService("openvpn", idx);
	mkdir(statickey_fileupload->workingdir.c_str(), 0777);
	copy_file("/tmp/static.key", statickey_fileupload->filepath.c_str());
	statickey_fileupload->updateView();
}

void OpenVpnPage::enableChanged()
{
//	if( !servicecontrol->autostart_checkbox->isChecked() ){
//		configmode_conteiner->hide();
//		mainsettings_groupbox->hide();
//		p2psettings_groupbox->hide();
//		clientsettings_groupbox->hide();
//		serversettings_groupbox->hide();
//		files_container->hide();
//		help_text->setText( "");
//		return;
//	}
	configmode_conteiner->show();
	configModeChanged(configmode_combobox->currentIndex());
}

void OpenVpnPage::configModeChanged(int newval)
{
	switch (newval) {
		default:
		case 0: //File
			mainsettings_groupbox->hide();
			p2psettings_groupbox->hide();
			clientsettings_groupbox->hide();
			serversettings_groupbox->hide();
			files_container->show();
			help_text->setText(tr("OpenVpn_configbyfilehelp"));
			break;
		case 1: //Web
			files_container->hide();
			mainsettings_groupbox->show();
			workModeChanged(workmode_combobox->currentIndex());
			break;
	}
}

void OpenVpnPage::workModeChanged(int newval)
{
	switch (newval) {
		case 0://Client 
			p2psettings_groupbox->hide();
			clientsettings_groupbox->show();
			serversettings_groupbox->hide();
			connectaddr_container->show();
			connectport_container->show();
			listenport_container->hide();
			authByChanged(authby_combobox->currentIndex());
			break;
		case 1: //Server
			p2psettings_groupbox->hide();
			clientsettings_groupbox->hide();
			serversettings_groupbox->show();
			connectaddr_container->hide();
			connectport_container->hide();
			listenport_container->show();
			help_text->setText(WString(tr("OpenVpn_server_help")).arg(
					"tls-auth(ta.key)").arg(tr("Additionl options")));
			break;
		case 2: //P2P Client
			p2psettings_groupbox->show();
			clientsettings_groupbox->hide();
			serversettings_groupbox->hide();
			connectaddr_container->show();
			connectport_container->show();
			listenport_container->hide();
			help_text->setText(
					WString(tr("OpenVpn_p2p_help")).arg(tr("Encryption key")));
			break;
		case 3: //P2P Server
			p2psettings_groupbox->show();
			clientsettings_groupbox->hide();
			serversettings_groupbox->hide();
			connectaddr_container->hide();
			connectport_container->hide();
			listenport_container->show();
			help_text->setText(
					WString(tr("OpenVpn_p2p_help")).arg(tr("Encryption key")));
			break;
		default:
			break;
	}
}

void OpenVpnPage::authByChanged(int newval)
{
	if (newval == 0) { // by key
		authbykey_container->show();
		authbypass_container->hide();
		help_text->setText(WString(tr("OpenVpn_client_bykey_help")).arg(
				"tls-auth(ta.key)").arg(tr("Additionl options")));
	} else { //by password
		authbykey_container->hide();
		authbypass_container->show();
		help_text->setText(WString(tr("OpenVpn_client_bypass_help")).arg(
				"tls-auth(ta.key)").arg(tr("Additionl options")));
	}
}

// http://openvpn.net/index.php/open-source/documentation/miscellaneous/78-stat
void OpenVpnPage::loadParams()
{
	Settings s = sd->byService("openvpn", idx);

	configmode_combobox->setCurrentIndex(s.valueInt("OPENVPN_CONFIGMODE", 0));

	int proto_idx =
			proto_combobox->findText(s.value("OPENVPN_PROTOCOL", "udp"));
	if (proto_idx < 0)
		proto_idx = 0;
	proto_combobox->setCurrentIndex(proto_idx);

	int devtype_idx =
			devtype_combobox->findText(s.value("OPENVPN_DEVTYPE", "tun"));
	if (devtype_idx < 0)
		devtype_idx = 0;
	devtype_combobox->setCurrentIndex(devtype_idx);

	int cipher_idx =
			cipher_combobox->findText(s.value("OPENVPN_CIPHER", "BF-CBC"));
	if (cipher_idx < 0)
		cipher_idx = 0;
	cipher_combobox->setCurrentIndex(cipher_idx);

	workmode_combobox->setCurrentIndex(s.valueInt("OPENVPN_WORKMODE", 0));
	connectaddr_lineedit->setText(s.value("OPENVPN_CONNECTADDR", "127.0.0.1"));
	connectport_lineedit->setText(
			WString("{1}").arg(s.valueInt("OPENVPN_CONNECTPORT", 1194)));
	listenport_lineedit->setText(
			WString("{1}").arg(s.valueInt("OPENVPN_LISTENPORT", 1194 + idx)));

	authby_combobox->setCurrentIndex(s.valueInt("OPENVPN_AUTHBY", 0));
	additionaloptsclient_textarea->setText(
			s.value("OPENVPN_CLIENTADDITIONALOPTS", ""));
	authuser_lineedit->setText(s.value("OPENVPN_CLIENTAUTHNAME", ""));
	authpass_lineedit->setText(s.value("OPENVPN_CLIENTAUTHPASS", ""));

	additionaloptsserver_textarea->setText(
			s.value("OPENVPN_SERVADDITIONALOPTS", ""));
	servifconfig_lineedit->setText(
			s.value("OPENVPN_SERVIFCONFIG", "10.8.0.0 255.255.255.0"));

	ifconfiglocalip_lineedit->setText(
			s.value("OPENVPN_IFCONFIGLOCALIP", "10.8.0.1"));
	ifconfigremoteip_lineedit->setText(
			s.value("OPENVPN_IFCONFIGREMOTEIP", "10.8.0.2"));
//	WString fname = s.value("OPENVPN_STATICKEY", s.fileName("static.key") );
//	std::string sk = Settings::loadFile( fname.toUTF8().c_str(), 4096 );
//	statickey_textarea->setText( WString::fromUTF8(sk) );

	autostart_checkbox->setChecked(s.valueInt("AUTOSTART", 0));
	enableChanged();
}

void OpenVpnPage::saveParams()
{
	Settings s = sd->byService("openvpn", idx);
	s.saveInt("AUTOSTART", autostart_checkbox->isChecked());
	s.saveInt("OPENVPN_CONFIGMODE", configmode_combobox->currentIndex());

	s.save("OPENVPN_PROTOCOL", proto_combobox->currentText());
	s.save("OPENVPN_DEVTYPE", devtype_combobox->currentText());
	s.save("OPENVPN_CIPHER", cipher_combobox->currentText());
	s.saveInt("OPENVPN_WORKMODE", workmode_combobox->currentIndex());
	s.save("OPENVPN_CONNECTADDR", connectaddr_lineedit->text());
	s.save("OPENVPN_CONNECTPORT", connectport_lineedit->text());
	s.save("OPENVPN_LISTENPORT", listenport_lineedit->text());

	s.saveInt("OPENVPN_AUTHBY", authby_combobox->currentIndex());
	s.save("OPENVPN_CLIENTADDITIONALOPTS",
			additionaloptsclient_textarea->text());
	s.save("OPENVPN_CLIENTAUTHNAME", authuser_lineedit->text());
	s.save("OPENVPN_CLIENTAUTHPASS", authpass_lineedit->text());

	s.save("OPENVPN_SERVADDITIONALOPTS", additionaloptsserver_textarea->text());
	s.save("OPENVPN_SERVIFCONFIG", servifconfig_lineedit->text());

	s.save("OPENVPN_IFCONFIGLOCALIP", ifconfiglocalip_lineedit->text());
	s.save("OPENVPN_IFCONFIGREMOTEIP", ifconfigremoteip_lineedit->text());

//	std::string data = statickey_textarea->text().toUTF8();
//	if( data.length() > 4096 ){
//		return;
//	}
//	WString fname = s.value("OPENVPN_STATICKEY", s.fileName( "static.key" ) );
//	Settings::saveFile( fname.toUTF8().c_str(), data );
}
