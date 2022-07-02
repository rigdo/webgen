/*
 * AmdPage.cpp
 *
 *  Created on: 20.02.2020
 *      Author: bond
 */

#include "AmdPage.h"
#include <Wt/WLineEdit.h>
#include "rocm_smi.h"
#include <inttypes.h>

std::string toStr(int v);
std::string toStr(double v);

char g_err_str[80];
const char *rsmi_error_string(rsmi_status_t err)
{
	sprintf(g_err_str, "%d", err);
	return g_err_str;
}

std::vector<std::map<std::string, std::string> > amdPciList()
{
	std::vector<std::map<std::string, std::string> > info_all;

	rsmi_status_t result = rsmi_init(0);
	if (result != RSMI_STATUS_SUCCESS) {
		printf("Failed to initialize rsmi: %s\n", rsmi_error_string(result));
		return info_all;
	}
	uint32_t device_count;
	result = rsmi_num_monitor_devices(&device_count);
	if (result != RSMI_STATUS_SUCCESS) {
		printf("Failed to query device count: %s\n", rsmi_error_string(result));
		return info_all;
	}
	for (uint32_t device = 0; device < device_count; device++) {
		std::map<std::string, std::string> info;
		info["idx"] = toStr((int) device);

		char name[64];
		///rsmi_dev_brand_get ?
		result = rsmi_dev_name_get(device, name, sizeof(name) / sizeof(name[0]));
//		result = rsmi_dev_serial_number_get(device, name, sizeof(name) / sizeof(name[0]));
		if (result != RSMI_STATUS_SUCCESS) {
			printf("Failed to get name of device %i: %s\n", device,
					rsmi_error_string(result));
//			 continue;
		}
		info["name"] = name;

		uint64_t bdfid;
		//BDFID = ((DOMAIN & 0xffffffff) << 32) | ((BUS & 0xff) << 8) |((DEVICE & 0x1f) <<3 ) | (FUNCTION & 0x7)
		result = rsmi_dev_pci_id_get(device, &bdfid);
		if (result != RSMI_STATUS_SUCCESS) {
			printf("Failed to get pci info for device %i: %s\n", device,
					rsmi_error_string(result));
//			continue;
		}
		char pciid_buf[64];
//		Name  Field
//		Domain [64:32]
//		Reserved [31:16]
//		Bus [15: 8]
//		Device [ 7: 3]
//		Function [ 2: 0]
		unsigned pci_domain = (bdfid >> 32)& 0xffffffff;
		unsigned pci_bus = (bdfid >> 8) & 0xff;
		unsigned pci_device = (bdfid >> 3) & 0x1f;
		unsigned pci_function = (bdfid >> 0) & 0x07;
		sprintf(pciid_buf, "%04x:%02x:%02x.%x", pci_domain, pci_bus, pci_device,
				pci_function);
		info["pciid"] = pciid_buf;


		uint64_t uuid;
		result = rsmi_dev_unique_id_get(device, &uuid);
		if (result != RSMI_STATUS_SUCCESS) {
			printf("Failed to get unique_id for device %i: %s, fallback to pciid\n", device,
					rsmi_error_string(result));
			info["uuid"] = info["pciid"];
		}
		else{
			char uuid_buf[80];
			sprintf(uuid_buf, "%" PRIu64 "\n", uuid);
			info["uuid"] = uuid_buf;
		}

		uint64_t plim = 0; // microwatts
		result = rsmi_dev_power_cap_get(device, 0, &plim);
		if (result != RSMI_STATUS_SUCCESS) {
			printf("Failed to get power_cap for device %i: %s\n", device,
					rsmi_error_string(result));
//			continue;
		}
		info["plim"] = toStr(((double) plim) / 1000000.0);

		info_all.push_back(info);
	}
	return info_all;
}

AmdPage::AmdPage(SettingsDir *sd)
{
	this->sd = sd;

	table = datacolumn->addWidget(std::make_unique<WTable>());
	table->setHeaderCount(1);
	table->setWidth(WLength("100%"));

	//create sock to nvoc service
	std::vector<std::map<std::string, std::string> > gpu_info = amdPciList();
	this->gpu_info = gpu_info;
	tableReceived(gpu_info);
}

AmdPage::~AmdPage()
{
	rsmi_status_t result = rsmi_shut_down();
	if (result != RSMI_STATUS_SUCCESS)
		printf("Failed to shutdown rsmi: %s\n", rsmi_error_string(result));

}

class EditApplySave2: public WContainerWidget
{
public:
	EditApplySave2()
	{
		line_edit = addWidget(std::make_unique<WLineEdit>());
		line_edit->setTextSize(6);
		apply_button = addWidget(std::make_unique<WPushButton>(tr("apply")));
		save_button = addWidget(std::make_unique<WPushButton>(tr("save")));
	}

public:
	WLineEdit *line_edit;
	WPushButton *apply_button;
	WPushButton *save_button;
};

std::string
fieldByName(const std::map<std::string, std::string> &st, std::string name,
		std::string def_val);


bool AmdPage::device_by_uuid(std::string uuid, uint32_t *device)
{
	for (unsigned i = 0; i < gpu_info.size(); i++) {
		std::map<std::string, std::string> f = gpu_info[i];
		std::string uuid = fieldByName(f, "uuid", "");
		if( uuid != fieldByName(f, "uuid", ""))
			continue;
		*device = atoi(fieldByName(f, "idx", "").c_str());
		return true;
	}
	return false;
}

void AmdPage::tableReceived(
		const std::vector<std::map<std::string, std::string> > &gpu_info)
{
	table->clear();
	table->elementAt(0, 0)->addWidget(std::make_unique<WText>("pciid"));
	table->elementAt(0, 1)->addWidget(std::make_unique<WText>("name"));
	table->elementAt(0, 2)->addWidget(std::make_unique<WText>("coreclk"));
	table->elementAt(0, 3)->addWidget(std::make_unique<WText>("memclk"));
	table->elementAt(0, 4)->addWidget(std::make_unique<WText>("fan"));
	table->elementAt(0, 5)->addWidget(std::make_unique<WText>("plim"));

	for (unsigned i = 0; i < gpu_info.size(); i++) {
		std::map<std::string, std::string> f = gpu_info[i];;

		std::string uuid = fieldByName(f, "uuid", "");

		table->elementAt(i + 1, 0)->addWidget(
				std::make_unique<WText>(fieldByName(f, "pciid", "")));
		table->elementAt(i + 1, 1)->addWidget(
				std::make_unique<WText>(fieldByName(f, "name", "")));
		EditApplySave2 *w_coreclk = table->elementAt(i + 1, 2)->addWidget(std::make_unique<EditApplySave2>());
		w_coreclk->line_edit->setText(fieldByName(f, "coreclk", ""));
		EditApplySave2 *w_memclk = table->elementAt(i + 1, 3)->addWidget(std::make_unique<EditApplySave2>());
		w_memclk->line_edit->setText(fieldByName(f, "memclk", ""));
		EditApplySave2 *w_fan = table->elementAt(i + 1, 4)->addWidget(std::make_unique<EditApplySave2>());
		w_fan->line_edit->setText(fieldByName(f, "fan", ""));
		EditApplySave2 *w_plim = table->elementAt(i + 1, 5)->addWidget(std::make_unique<EditApplySave2>());
		w_plim->line_edit->setText(fieldByName(f, "plim", ""));

		w_coreclk->save_button->clicked().connect(
				std::bind(&AmdPage::saveCoreClock, this, uuid,
						w_coreclk->line_edit));
		w_memclk->save_button->clicked().connect(
				std::bind(&AmdPage::saveMemClock, this, uuid,
						w_memclk->line_edit));
		w_fan->save_button->clicked().connect(
				std::bind(&AmdPage::saveFan, this, uuid,
						w_fan->line_edit));
		w_plim->save_button->clicked().connect(
				std::bind(&AmdPage::savePlim, this, uuid,
						w_plim->line_edit));

		w_coreclk->apply_button->clicked().connect(
				std::bind(&AmdPage::applyCoreClock, this, uuid,
						w_coreclk->line_edit));
		w_memclk->apply_button->clicked().connect(
				std::bind(&AmdPage::applyMemClock, this, uuid,
						w_memclk->line_edit));
		w_fan->apply_button->clicked().connect(
				std::bind(&AmdPage::applyFan, this, uuid,
						w_fan->line_edit));
		w_plim->apply_button->clicked().connect(
				std::bind(&AmdPage::applyPlim, this, uuid,
						w_plim->line_edit));
	}
}

std::string removeDash(std::string str);

void AmdPage::saveCoreClock(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("amdoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("CORE_CLOCK_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void AmdPage::saveMemClock(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("amdoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("XNVCTL_MEM_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void AmdPage::saveFan(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("amdoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("XNVCTL_FAN_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void AmdPage::savePlim(std::string uuid, WLineEdit *line_edit)
{
	Settings s = sd->byService("amdoc0");
	std::string uuid_t = removeDash(uuid);
	s.save("NVML_PLIMIT_" + uuid_t, line_edit->text());
	s.save("AUTOSTART", "1");
}

void AmdPage::applyCoreClock(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s CLOCK=%s\n", line_edit->text());
}

void AmdPage::applyMemClock(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s MEM=%s\n", line_edit->text());
}

void AmdPage::applyFan(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC UUID=%s FAN=%s\n", line_edit->text());
}

void AmdPage::applyPlim(std::string uuid, WLineEdit *line_edit)
{
//	send("SETOC uuid=%s PLIMIT=%s\n", line_edit->text());

	uint32_t device;
	bool ok = device_by_uuid(uuid, &device);
	if (!ok) {
		printf("Failed to get dev by uuid %s\n", uuid.c_str());
		return;
	}
	uint64_t limit = atof(line_edit->text().toUTF8().c_str()) * 1000000.0;
	rsmi_status_t ret = rsmi_dev_power_cap_set(device, 0, limit);
	if (ret != RSMI_STATUS_SUCCESS) {
		printf("Failed to set plim: %s\n", rsmi_error_string(ret));
		return;
	}
	printf("applyPlim %d ok\n", limit);
}
