/*
 * NvidiaPage.cpp
 *
 *  Created on: 13.05.2018
 *      Author: bond
 */

#include "NvidiaPage.h"
#include <Wt/WLineEdit.h>

#include "nvml.h"

std::string toStr(int v)
{
	char buf[80];
	sprintf(buf, "%d", v);
	return std::string(buf);
}

std::string toStr(double v)
{
	char buf[80];
	sprintf(buf, "%.1lf", v);
	return std::string(buf);
}

#if 1

std::vector<std::map<std::string, std::string> > nvPciList()
{
	std::vector<std::map<std::string, std::string> > info_all;

	nvmlReturn_t result = nvmlInit();
	if (result != NVML_SUCCESS) {
		printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));
		return info_all;
	}
	unsigned int device_count;
	result = nvmlDeviceGetCount(&device_count);
	if (result != NVML_SUCCESS) {
		printf("Failed to query device count: %s\n", nvmlErrorString(result));
		return info_all;
	}
	for (unsigned i = 0; i < device_count; i++) {
		std::map<std::string, std::string> info;
		info["idx"] = toStr((int) i);
		nvmlDevice_t device;
		result = nvmlDeviceGetHandleByIndex(i, &device);
		if (result != NVML_SUCCESS) {
			printf("Failed to get handle for device %i: %s\n", i,
					nvmlErrorString(result));
			info_all.push_back(info);
			continue;
		}
		char name[64];
		result =
				nvmlDeviceGetName(device, name, sizeof(name) / sizeof(name[0]));
		if (result != NVML_SUCCESS) {
			printf("Failed to get name of device %i: %s\n", i,
					nvmlErrorString(result));
//			 continue;
		}
		info["name"] = name;

		nvmlPciInfo_t pci;
		result = nvmlDeviceGetPciInfo(device, &pci);
		if (result != NVML_SUCCESS) {
			printf("Failed to get pci info for device %i: %s\n", i,
					nvmlErrorString(result));
//			continue;
		}
		info["pciid"] = pci.busId;

		char uuid[80];
		result = nvmlDeviceGetUUID(device, uuid, 80);
		info["uuid"] = uuid;

		unsigned int plim;
		result = nvmlDeviceGetPowerManagementLimit(device, &plim);
		info["plim"] = toStr(((double) plim) / 1000.0);

//		printf("%d. %s [%s], %s\n", i, name, pci.busId, uuid);

		info_all.push_back(info);
	}
	return info_all;
}

#endif

NvidiaPage::NvidiaPage(SettingsDir *sd)
{
	this->sd = sd;

	table = new Wt::WTable();
	table->setHeaderCount(1);
	table->setWidth(WLength("100%"));

	datacolumn->addWidget(table);

	//create sock to nvoc service
	std::vector<std::map<std::string, std::string> > gpu_info = nvPciList();
	tableReceived(gpu_info);
}

NvidiaPage::~NvidiaPage()
{
#if 0
	nvmlReturn_t result = nvmlShutdown();
	if (NVML_SUCCESS != result)
		printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));
#endif
}

class EditApplySave: public WContainerWidget
{
public:
	EditApplySave()
	{
		line_edit = new WLineEdit();
		line_edit->setTextSize(6);
		addWidget(line_edit);
		apply_button = new WPushButton(tr("apply"));
		addWidget(apply_button);
		save_button = new WPushButton(tr("save"));
		addWidget(save_button);
	}

public:
	WLineEdit *line_edit;
	WPushButton *apply_button;
	WPushButton *save_button;
};

std::string
fieldByName(const std::map<std::string, std::string> &st, std::string name,
		std::string def_val)
{
	std::map<std::string, std::string>::const_iterator ptr = st.find(name);
	if (ptr == st.end())
		return def_val;
	return ptr->second;
}


#if 1

//cvs from nvoc received
void NvidiaPage::tableReceived(
		const std::vector<std::map<std::string, std::string> > &gpu_info)
{
	table->clear();
	table->elementAt(0, 0)->addWidget(new WText("pciid"));
	table->elementAt(0, 1)->addWidget(new WText("name"));
	table->elementAt(0, 2)->addWidget(new WText("coreclk"));
	table->elementAt(0, 3)->addWidget(new WText("memclk"));
	table->elementAt(0, 4)->addWidget(new WText("fan"));
	table->elementAt(0, 5)->addWidget(new WText("plim"));

	for (unsigned i = 0; i < gpu_info.size(); i++) {
		std::map<std::string, std::string> f = gpu_info[i];;

		std::string uuid = fieldByName(f, "uuid", "");

		table->elementAt(i + 1, 0)->addWidget(
				new WText(fieldByName(f, "pciid", "")));
		table->elementAt(i + 1, 1)->addWidget(
				new WText(fieldByName(f, "name", "")));
		EditApplySave *w_coreclk = new EditApplySave();
		w_coreclk->line_edit->setText(fieldByName(f, "coreclk", ""));
		table->elementAt(i + 1, 2)->addWidget(w_coreclk);
		EditApplySave *w_memclk = new EditApplySave();
		w_memclk->line_edit->setText(fieldByName(f, "memclk", ""));
		table->elementAt(i + 1, 3)->addWidget(w_memclk);
		EditApplySave *w_fan = new EditApplySave();
		w_fan->line_edit->setText(fieldByName(f, "fan", ""));
		table->elementAt(i + 1, 4)->addWidget(w_fan);
		EditApplySave *w_plim = new EditApplySave();
		w_plim->line_edit->setText(fieldByName(f, "plim", ""));
		table->elementAt(i + 1, 5)->addWidget(w_plim);

		w_coreclk->save_button->clicked().connect(
				boost::bind(&NvidiaPage::saveCoreClock, this, uuid,
						w_coreclk->line_edit));
		w_memclk->save_button->clicked().connect(
				boost::bind(&NvidiaPage::saveMemClock, this, uuid,
						w_memclk->line_edit));
		w_fan->save_button->clicked().connect(
				boost::bind(&NvidiaPage::saveFan, this, uuid,
						w_fan->line_edit));
		w_plim->save_button->clicked().connect(
				boost::bind(&NvidiaPage::savePlim, this, uuid,
						w_plim->line_edit));

		w_coreclk->apply_button->clicked().connect(
				boost::bind(&NvidiaPage::applyCoreClock, this, uuid,
						w_coreclk->line_edit));
		w_memclk->apply_button->clicked().connect(
				boost::bind(&NvidiaPage::applyMemClock, this, uuid,
						w_memclk->line_edit));
		w_fan->apply_button->clicked().connect(
				boost::bind(&NvidiaPage::applyFan, this, uuid,
						w_fan->line_edit));
		w_plim->apply_button->clicked().connect(
				boost::bind(&NvidiaPage::applyPlim, this, uuid,
						w_plim->line_edit));
	}
}

std::string removeDash(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '-'), str.end());
	return str;
}

void NvidiaPage::saveCoreClock(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("nvoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("XNVCTL_CLOCK_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void NvidiaPage::saveMemClock(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("nvoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("XNVCTL_MEM_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void NvidiaPage::saveFan(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("nvoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("XNVCTL_FAN_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void NvidiaPage::savePlim(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("nvoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("NVML_PLIMIT_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void NvidiaPage::applyCoreClock(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s CLOCK=%s\n", line_edit->text());
}

void NvidiaPage::applyMemClock(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s MEM=%s\n", line_edit->text());
}

void NvidiaPage::applyFan(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s FAN=%s\n", line_edit->text());
}

void NvidiaPage::applyPlim(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC uuid=%s PLIMIT=%s\n", line_edit->text());

	nvmlDevice_t device;
	nvmlReturn_t ret = nvmlDeviceGetHandleByUUID(uuid.c_str(), &device);
	if (ret != NVML_SUCCESS) {
		printf("Failed to get dev by uuid: %s\n", nvmlErrorString(ret));
		return;
	}
	unsigned int limit = atof(line_edit->text().toUTF8().c_str()) * 1000.0;
	ret = nvmlDeviceSetPowerManagementLimit(device, limit);
	if (ret != NVML_SUCCESS) {
		printf("Failed to set plim: %s\n", nvmlErrorString(ret));
		return;
	}
	printf("applyPlim %d ok\n", limit);
}

#endif
