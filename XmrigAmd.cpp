/*
 * XmrigAmd.cpp
 */

#include "XmrigAmd.h"
#include "xmrig_algos.h"

//==============================================================================
//===================== XmrigAmd ===========================================
//==============================================================================
XmrigAmd::XmrigAmd(SettingsDir *sd):
		BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("xmrig-amd"));
	help_text->setText(tr("xmrig-amd_help"));

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
	opencl_launch_lineedit = new WLineEdit();
	opencl_launch_lineedit->setTextSize(49);
	opencl_launch_lineedit->setStyleClass("settingvalue");

	algo_combobox = new WComboBox();
	for (int i = 0; i < 25; i++) {
		algo_combobox->addItem(xmrig_algos[i]);
	}
	algo_combobox->setStyleClass("xmrig_tty");

	nicehash_combobox = new WComboBox();
	nicehash_combobox->addItem("auto");
	nicehash_combobox->addItem("on");
	nicehash_combobox->addItem("off");
	nicehash_combobox->setStyleClass("xmrig_tty");

	console_combobox = buildConsoleComboBox();
	console_combobox->setStyleClass("xmrig_tty");
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
			WText *label = new WText(tr("xmrig-amd_bind_tty"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(console_combobox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig-amd_pool"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(pool_lineedit);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig-amd_user"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(user_lineedit);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig-amd_pass"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(pass_lineedit);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig_algo"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(algo_combobox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig_nicehash_mode"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(nicehash_combobox);
			datacolumn->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("xmrig-amd_opencl-launch"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(opencl_launch_lineedit);
			datacolumn->addWidget(c);
		}
	}
	save_button->clicked().connect(this, &XmrigAmd::saveParams);
	loadParams();
}

void XmrigAmd::loadParams()
{
	Settings s = sd->byService("xmrig-amd0");

	pool_lineedit->setText(s.value("XMRIGA_POOL", "xmr-eu.dwarfpool.com:8080"));
	user_lineedit->setText(s.value("XMRIGA_USER",
			"45v1SBynnRMKxz3faNASnR2GiEc1TDbFgcZym94VuaJkCMSRsw66VxGRXHtDSHw84eYrW4HtPHcxCc3sP29SNDcnGB5PvXh"));
	pass_lineedit->setText(s.value("XMRIGA_PASS", ""));
	opencl_launch_lineedit->setText(s.value("XMRIGA_CUDA_LAUNCH", ""));

	WString algo_str = s.value("XMRIGA_ALGO", "");
	int algo_idx = algo_combobox->findText(algo_str);
	if (algo_idx >= 0)
		algo_combobox->setCurrentIndex(algo_idx);
	else
		algo_combobox->setCurrentIndex(0);

	WString nicehash_str = s.value("XMRIGA_NICEHASH_MODE", "auto");
	int nicehash_idx = nicehash_combobox->findText(nicehash_str);
	if (nicehash_idx >= 0)
		nicehash_combobox->setCurrentIndex(nicehash_idx);
	else
		nicehash_combobox->setCurrentIndex(0);

	WString v_str = s.value("XMRIGA_BIND_TTY", "");
	int idx = console_combobox->findText(v_str);
	if (idx >= 0)
		console_combobox->setCurrentIndex(idx);
	else
		console_combobox->setCurrentIndex(0);
	autostart_checkbox->setChecked(s.valueInt("AUTOSTART", 0));
}

void XmrigAmd::saveParams()
{
	Settings s = sd->byService("xmrig-amd0");
	s.save("XMRIGA_POOL", pool_lineedit->text());
	s.save("XMRIGA_USER", user_lineedit->text());
	s.save("XMRIGA_PASS", pass_lineedit->text());
	s.save("XMRIGA_CUDA_LAUNCH", opencl_launch_lineedit->text());
	s.save("XMRIGA_ALGO", algo_combobox->currentText());
	s.save("XMRIGA_NICEHASH_MODE", nicehash_combobox->currentText());
	s.save("XMRIGA_BIND_TTY", console_combobox->currentText());
	s.saveInt("AUTOSTART", autostart_checkbox->isChecked());
}
