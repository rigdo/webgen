/*
 * Ethminer.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "Ethminer.h"

//==============================================================================
//===================== Ethminer ===========================================
//==============================================================================
Ethminer::Ethminer(SettingsDir *sd, std::string gpu_vendor, std::string servicename):
		BaseServicePage(servicename)
{
	this->sd = sd;
	this->gpu_vendor = gpu_vendor;

	pagetitle_text->setText(tr("ethminer_" + gpu_vendor + "_title"));

	help_text->setText(tr("ethminer_help_" + gpu_vendor));

	autostart_checkbox = new WCheckBox();
	pool_lineedit = new WLineEdit();
	pool_lineedit->setTextSize(49);
	pool_lineedit->setStyleClass("settingvalue");
	user_lineedit = new WLineEdit();
	user_lineedit->setTextSize(49);
	user_lineedit->setStyleClass("settingvalue");

	worker_lineedit = new WLineEdit();
	worker_lineedit->setTextSize(49);
	worker_lineedit->setStyleClass("settingvalue");

	scheme_combobox = new WComboBox();
	scheme_combobox->addItem("stratum+tcp");
	scheme_combobox->addItem("http");

	console_combobox = buildConsoleComboBox();
	console_combobox->setStyleClass("xmrig_tty");

	opencl_mode_combobox = 0;
	if (gpu_vendor == "amd"){
		opencl_mode_combobox = new WComboBox();
		opencl_mode_combobox->addItem("amdgpu-pro");
		opencl_mode_combobox->addItem("rocm");
	}

	{
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("autostart"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(autostart_checkbox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig_bind_tty"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(console_combobox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethminer_scheme"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(scheme_combobox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethminer_pool"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(pool_lineedit);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethminer_user"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(user_lineedit);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethminer_worker"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(worker_lineedit);
			datacolumn->addWidget(c);
		}
		if (opencl_mode_combobox){
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("amd_opencl_mode"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(opencl_mode_combobox);
			datacolumn->addWidget(c);
		}

	}
	save_button->clicked().connect(this, &Ethminer::saveParams);
	loadParams();
}

void Ethminer::loadParams()
{
	Settings s = sd->byService(servicename);

	pool_lineedit->setText(s.value("ETHMINER_POOL", "eu1.ethermine.org:4444"));
	user_lineedit->setText(s.value("ETHMINER_WALLET",
			"0x00747e59ee64851ecb100d18f3d51878463c0f94"));
	worker_lineedit->setText(s.value("ETHMINER_WORKER", "donate"));

	WString scheme_str = s.value("ETHMINER_SCHEME", "stratum+tcp");
	int scheme_idx = scheme_combobox->findText(scheme_str);
	if (scheme_idx >= 0)
		scheme_combobox->setCurrentIndex(scheme_idx);
	else
		scheme_combobox->setCurrentIndex(0);

	if (opencl_mode_combobox){
		WString opencl_mode_str = s.value("ETHMINER_OPENCL_MODE", "amdgpu-pro");
		int opencl_mode_idx = opencl_mode_combobox->findText(opencl_mode_str);
		if (opencl_mode_idx >= 0)
			opencl_mode_combobox->setCurrentIndex(opencl_mode_idx);
		else
			opencl_mode_combobox->setCurrentIndex(0);
	}

	WString v_str = s.value("ETHMINER_BIND_TTY", "");
	int idx = console_combobox->findText(v_str);
	if (idx >= 0)
		console_combobox->setCurrentIndex(idx);
	else
		console_combobox->setCurrentIndex(0);
	autostart_checkbox->setChecked(s.valueInt("AUTOSTART", 0));
}

void Ethminer::saveParams()
{
	Settings s = sd->byService(servicename);
	s.save("ETHMINER_POOL", pool_lineedit->text());
	s.save("ETHMINER_WALLET", user_lineedit->text());
	s.save("ETHMINER_WORKER", worker_lineedit->text());
	s.save("ETHMINER_SCHEME", scheme_combobox->currentText());
	s.save("ETHMINER_BIND_TTY", console_combobox->currentText());
	if(opencl_mode_combobox){
		s.save("ETHMINER_OPENCL_MODE", opencl_mode_combobox->currentText());
	}
	s.saveInt("AUTOSTART", autostart_checkbox->isChecked());
}
