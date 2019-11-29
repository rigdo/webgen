/*
 * VtPage.cpp
 *
 *  Created on: 10.01.2017
 *      Author: bond
 */
#include "VtPage.h"
#include <string.h>
#include <errno.h>
#include <Wt/WApplication>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

VtState::VtState()
{
	idx = 0;
	vt_rows = 0;
	vt_cols = 0;
	vt_fd = -1;
}

VtState::~VtState()
{
	if(vt_fd >= 0)
		close(vt_fd);
}

bool VtState::init(int idx)
{
	this->idx = idx;
	char fname[256];
	sprintf(fname,"/dev/vcsa%d",idx);
	int fd = open(fname,O_RDONLY);
	if( fd < 0){
		printf("cant open %s: %s\n", fname, strerror(errno));
		return false;
	}
	uint8_t buf[2];
	read(fd,buf,2);
	vt_rows = buf[0];
	vt_cols = buf[1];
	close(fd);
	printf("VtState %d: rows: %d, cols: %d\n", idx, vt_rows, vt_cols);

	sprintf(fname,"/dev/vcsu%d",idx);
	vt_fd = open(fname,O_RDONLY);
	if(vt_fd < 0){
		printf("cant open %s: %s\n", fname, strerror(errno));
		return false;
	}
	
	return true;
}

WString VtState::grabScreen()
{
	lseek(vt_fd, 0, SEEK_SET);
	int bufsize = vt_rows*vt_cols*4;
	uint32_t *buf = new uint32_t[bufsize];
	int ret = read(vt_fd, buf, bufsize);
	if(ret != bufsize ){
		delete [] buf;
		printf("VtState: wan't read %d, ret: %d\n",bufsize, ret);
		return "";
	}
	//add '\n' on end of each line (fold)
	int bufsize2 = vt_rows*(vt_cols+1)*4;
	uint32_t *buf2 = new uint32_t[bufsize2+1];
	for(int r =0; r<vt_rows; r++){
		memcpy(&buf2[r*(vt_cols+1)], &buf[r*vt_cols], vt_cols*4);
		buf2[r*(vt_cols+1)+vt_cols] = (uint32_t)'\n';
	}
	buf2[vt_rows*vt_cols] = 0;
	WString s((const wchar_t*)buf2);
	//std::string s1((const char*)buf2, bufsize2);
	//WString s = WString::fromUTF8(s1);
	delete [] buf2;
	delete [] buf;
	return s;
}

//==============================================================================
//===================== VtPage ===========================================
//==============================================================================
VtPage::VtPage(VtState *vt) : WContainerWidget()
{
	this->vt = vt;
	WContainerWidget *label_div = new WContainerWidget();
	label_div->setStyleClass("pagetitle");
	WText *pagetitle_text = new WText(WString("vt{1}").arg(vt->idx));
	label_div->addWidget( pagetitle_text );
	addWidget( label_div);
	log_textarea = new WTextArea;
	log_textarea->setColumns(vt->cols()+1);
	log_textarea->setRows(vt->rows());
	log_textarea->setInline( false );
	addWidget( log_textarea );
	refresh_button = new WPushButton("Refresh");
	addWidget( refresh_button );
	
	readLog();
	refresh_button->clicked().connect( this, &VtPage::readLog );
}

void VtPage::readLog()
{
	WString s = vt->grabScreen();
	log_textarea->setText(s);
	
}

