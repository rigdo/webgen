/*
 * NvidiaPage.cpp
 *
 *  Created on: 13.05.2018
 *      Author: bond
 */

#include "NvidiaPage.h"

#if 0
void NvidiaPage::nvPciList()
{
	nvmlReturn_t result = nvmlInit();
	if (result != NVML_SUCCESS){
		printf("Failed to initialize NVML: %s\n", nvmlErrorString(result));
		return;
	}
	unsigned int device_count;
	result = nvmlDeviceGetCount(&device_count);
	if (result != NVML_SUCCESS){
		printf("Failed to query device count: %s\n", nvmlErrorString(result));
		return;
	}
	for (unsigned i = 0; i < device_count; i++){
		nvmlDevice_t device;
		result = nvmlDeviceGetHandleByIndex(i, &device);
		if (result != NVML_SUCCESS ){
			printf("Failed to get handle for device %i: %s\n", i, nvmlErrorString(result));
			return;
		}
		char name[64];
		result = nvmlDeviceGetName(device, name, sizeof(name)/sizeof(name[0]));
		if (result != NVML_SUCCESS ){
			 printf("Failed to get name of device %i: %s\n", i, nvmlErrorString(result));
			 return;
		}
		nvmlPciInfo_t pci;
		result = nvmlDeviceGetPciInfo(device, &pci);
		if (result != NVML_SUCCESS ){
			printf("Failed to get pci info for device %i: %s\n", i, nvmlErrorString(result));
			return;
		}
		printf("%d. %s [%s]\n", i, name, pci.busId);
		nvmlComputeMode_t compute_mode;
		result = nvmlDeviceGetComputeMode(device, &compute_mode);

	}
		
}
#endif

NvidiaPage::NvidiaPage(SettingsDir *sd)
{
//	Wtext *idx_text_w = new Wtext();
//	Wtext *pciid_text_w = new Wtext();
//	WLineEdit *clock_lineedit = WLineEdit();
//	clock_lineedit->setTextSize(6);
//	WLineEdit *mem_lineedit = WLineEdit();
//	mem_lineedit->setTextSize(6);
//	WLineEdit *fan_lineedit = WLineEdit();
//	fan_lineedit->setTextSize(6);
//	WLineEdit *plim_lineedit = WLineEdit();
//	plim_lineedit->setTextSize(6);
	
	table = new Wt::WTable();
	table->setHeaderCount(1);
	table->setWidth(WLength("100%"));

	datacolumn->addWidget( table );
	
	//create sock to nvoc service
}

NvidiaPage::~NvidiaPage()
{
#if 0
	nvmlReturn_t result = nvmlShutdown();
	if (NVML_SUCCESS != result)
		printf("Failed to shutdown NVML: %s\n", nvmlErrorString(result));
#endif
}


//std::string wpaAt(const std::map<std::string, std::string> &st,
//		std::string name, std::string def_val)
//{
//	std::map<std::string, std::string>::const_iterator ptr = st.find(name);
//	if (ptr == st.end())
//		return def_val;
//	return ptr->second;
//}

#if 0
//cvs from nvoc received
void NvidiaPage::tableReceived(std::vector<std::string> csv_lines)
{
	table->clear();
	table->elementAt(0, 0)->addWidget(new WText("pciid"));
	table->elementAt(0, 1)->addWidget(new WText("name"));
	table->elementAt(0, 2)->addWidget(new WText("coreclk"));
	table->elementAt(0, 3)->addWidget(new WText("memclk"));
	table->elementAt(0, 4)->addWidget(new WText("fan"));
	table->elementAt(0, 5)->addWidget(new WText("plim"));
	
	// csv_lines[0] is header
	std::map<std::string, std::string> csv_hdr = lineToMap(csv_lines[0]);
	
	for(unsigned i=1; i < csv_lines.size(); i++){
		std::map<std::string, std::string> f = lineToMap2(csv_hdr, csv_lines[i]);
		
		std::string guid = fieldByName(f, "guid", "");
		idx_to_guid[i] = guid;
		table->elementAt(i, 0)->addWidget(new WText(fieldByName(f, "pciid", "")));
		table->elementAt(i, 1)->addWidget(new WText(fieldByName(f, "name", "")));
		table->elementAt(i, 2)->addWidget(new WLineEdit(fieldByName(f, "coreclk", "")));
		table->elementAt(i, 3)->addWidget(new WLineEdit(fieldByName(f, "memclk", "")));
		table->elementAt(i, 4)->addWidget(new WLineEdit(fieldByName(f, "fan", "")));
		table->elementAt(i, 5)->addWidget(new WLineEdit(fieldByName(f, "plim", "")));
		WButton *apply_button = new WButton(tr("Apply"));
		table->elementAt(i, 6)->addWidget(apply_button);
		WButton *save_button = new WButton(tr("Save"));
		table->elementAt(i, 7)->addWidget(save_button);
		
		save_button->clicked().connect( this, &NvidiaPage::saveDevParams );
		save_button->clicked().connect( this, &NvidiaPage::applyDevParams );
	}
}

void NvidiaPage::saveDevParams(std::string guid, int idx)
{
	// get idx in settings by guid
	int idx = idx_to_guid[guid];
	
	Settings s = sd->byService("nvoc0");
	
	s.save( "NVML_GUID",    idx, guid );
	s.save( "XNVCTL_CLOCK", idx, getLineEditVal(table, idx, 2, 0) );
	s.save( "XNVCTL_MEM",   idx, getLineEditVal(table, idx, 3, 0) );
	s.save( "XNVCTL_FAN",   idx, getLineEditVal(table, idx, 4, 0) );
	s.save( "NVML_PLIMIT",  idx, getLineEditVal(table, idx, 5, 0) );
	
	
//	s.saveInt( "AUTOSTART", 1);
}

void NvidiaPage::applyDevParams(std::string guid, int idx)
{
	// get idx in settings by guid
	int idx = idx_to_guid[guid];

//	send("SETOC GUID=%s PLIMIT=%s CLOCK=%s MEM=%s FAN=%s\n");
	send("SETOC GUID=%s CLOCK=%s\n", getLineEditVal(table, idx, 2, 0));
	send("SETOC GUID=%s MEM=%s\n", getLineEditVal(table, idx, 3, 0));
	send("SETOC GUID=%s FAN=%s\n", getLineEditVal(table, idx, 4, 0));
	send("SETOC GUID=%s PLIMIT=%s\n", getLineEditVal(table, idx, 5, 0));
}
#endif
