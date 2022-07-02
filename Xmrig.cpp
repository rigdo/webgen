/*
 * Xmrig.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "Xmrig.h"
#include "xmrig_algos.h"

//==============================================================================
//===================== Xmrig ===========================================
//==============================================================================

Xmrig::Xmrig(SettingsDir *sd, std::string gpu_vendor, std::string servicename):
		BaseServicePage(servicename)
{
	this->sd = sd;
	this->gpu_vendor = gpu_vendor;
	pagetitle_text->setText(tr("xmrig-" + gpu_vendor + "_title"));
	help_text->setText(tr("xmrig-" + gpu_vendor + "_help"));

	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("autostart")));
		label->setStyleClass("label");
		autostart_checkbox = c->addWidget(std::make_unique<WCheckBox>());
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_bind_tty")));
		label->setStyleClass("label");
		console_combobox = buildConsoleComboBox(c);
		console_combobox->setStyleClass("xmrig_tty");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_pool")));
		label->setStyleClass("label");
		pool_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		pool_lineedit->setTextSize(49);
		pool_lineedit->setStyleClass("settingvalue");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_user")));
		label->setStyleClass("label");
		user_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		user_lineedit->setTextSize(100);
		user_lineedit->setStyleClass("settingvalue");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_pass")));
		label->setStyleClass("label");
		pass_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		pass_lineedit->setTextSize(49);
		pass_lineedit->setStyleClass("settingvalue");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_algo")));
		label->setStyleClass("label");
		algo_combobox = c->addWidget(std::make_unique<WComboBox>());
		for (int i = 0; i < 25; i++) {
			algo_combobox->addItem(xmrig_algos[i]);
		}
		algo_combobox->setStyleClass("xmrig_tty");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_nicehash_mode")));
		label->setStyleClass("label");
		nicehash_combobox = c->addWidget(std::make_unique<WComboBox>());
		nicehash_combobox->addItem("auto");
		nicehash_combobox->addItem("on");
		nicehash_combobox->addItem("off");
		nicehash_combobox->setStyleClass("xmrig_tty");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig_donate")));
		label->setStyleClass("label");
		donate_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		donate_lineedit->setTextSize(3);
		donate_lineedit->setStyleClass("settingvalue");
	}
//	if (gpu_vendor == "amd") {
//		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
//		c->setStyleClass("setting");
//		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig-amd_opencl-launch")));
//		label->setStyleClass("label");
//		opencl_launch_lineedit = c->addWidget(std::make_unique<WLineEdit>());
//		opencl_launch_lineedit->setTextSize(49);
//		opencl_launch_lineedit->setStyleClass("settingvalue");
//	}
//	if (gpu_vendor == "nvidia") {
//		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
//		c->setStyleClass("setting");
//		WText *label = c->addWidget(std::make_unique<WText>(tr("xmrig-nvidia_cuda-launch")));
//		label->setStyleClass("label");
//		cuda_launch_lineedit = c->addWidget(std::make_unique<WLineEdit>());
//		cuda_launch_lineedit->setTextSize(49);
//		cuda_launch_lineedit->setStyleClass("settingvalue");
//	}

	save_button->clicked().connect(this, &Xmrig::saveParams);
	loadParams();
}

void Xmrig::loadParams()
{
	Settings s = sd->byService(servicename);

	pool_lineedit->setText(s.value("XMRIG_POOL", "xmr-eu.dwarfpool.com:8080"));
	user_lineedit->setText(s.value("XMRIG_USER",
			"45v1SBynnRMKxz3faNASnR2GiEc1TDbFgcZym94VuaJkCMSRsw66VxGRXHtDSHw84eYrW4HtPHcxCc3sP29SNDcnGB5PvXh"));
	pass_lineedit->setText(s.value("XMRIG_PASS", ""));

	donate_lineedit->setText(s.value("XMRIG_DONATE", "5"));

	opencl_launch_lineedit->setText(s.value("XMRIG_OPENCL_LAUNCH", ""));
	cuda_launch_lineedit->setText(s.value("XMRIG_CUDA_LAUNCH", ""));

	WString algo_str = s.value("XMRIG_ALGO", "");
	int algo_idx = algo_combobox->findText(algo_str);
	if (algo_idx >= 0)
		algo_combobox->setCurrentIndex(algo_idx);
	else
		algo_combobox->setCurrentIndex(0);

	WString nicehash_str = s.value("XMRIG_NICEHASH_MODE", "auto");
	int nicehash_idx = nicehash_combobox->findText(nicehash_str);
	if (nicehash_idx >= 0)
		nicehash_combobox->setCurrentIndex(nicehash_idx);
	else
		nicehash_combobox->setCurrentIndex(0);

	WString tty_str = s.value("XMRIG_BIND_TTY", "");
	int tty_idx = console_combobox->findText(tty_str);
	if (tty_idx >= 0)
		console_combobox->setCurrentIndex(tty_idx);
	else
		console_combobox->setCurrentIndex(0);

	autostart_checkbox->setChecked(s.valueInt("AUTOSTART", 0));
}

void Xmrig::saveParams()
{
	Settings s = sd->byService(servicename);
	s.save("XMRIG_POOL", pool_lineedit->text());
	s.save("XMRIG_USER", user_lineedit->text());
	s.save("XMRIG_PASS", pass_lineedit->text());
	s.save("XMRIG_DONATE", donate_lineedit->text());
	s.save("XMRIG_OPENCL_LAUNCH", opencl_launch_lineedit->text());
	s.save("XMRIG_CUDA_LAUNCH", cuda_launch_lineedit->text());
	s.save("XMRIG_ALGO", algo_combobox->currentText());
	s.save("XMRIG_NICEHASH_MODE", nicehash_combobox->currentText());
	s.save("XMRIG_BIND_TTY", console_combobox->currentText());
	s.saveInt("AUTOSTART", autostart_checkbox->isChecked());
}
