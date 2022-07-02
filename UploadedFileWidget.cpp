/*
 * UploadedFileWidget.cpp
 *
 *  Created on: 20.08.2012
 *      Author: bond
 */
#include "UploadedFileWidget.h"
#include <Wt/WText.h>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>


void copy_file(const char *input, const char *output)
{
	std::ifstream f1(input, std::fstream::binary);
	std::ofstream f2(output, std::fstream::trunc | std::fstream::binary);
	f2 << f1.rdbuf();
}

//==============================================================================
//===================== UploadedFileWidget =====================================
//==============================================================================
UploadedFileWidget::UploadedFileWidget(std::string workingdir,
		std::string filename, WString comment):
		WContainerWidget()
{
	this->comment = comment;
	this->workingdir = workingdir;
	this->filename = filename;
	this->filepath = workingdir + "/" + filename;
	fileupload = nullptr;


	ftext = addWidget(std::make_unique<WText>(comment));
	ftext->setStyleClass("labelcomment");
	fileuploadcontainer = addWidget(std::make_unique<WContainerWidget>());
	fileuploadcontainer->setInline(true);
	uploadbutton = addWidget(std::make_unique<WPushButton>(tr("Upload")));
	fresource = std::make_shared<WFileResource>(filepath);
	fanchor = addWidget(std::make_unique<WAnchor>(WLink(fresource), filename));
	delbutton = addWidget(std::make_unique<WPushButton>(tr("Delete")));
	delbutton->clicked().connect(
			std::bind(&UploadedFileWidget::delFile, this));
	updateView();
}

void UploadedFileWidget::updateView()
{
	struct stat statbuf;
	if (stat(filepath.c_str(), &statbuf) == 0) { //has file
		if (fileupload) {
			fileuploadcontainer->removeChild(fileupload);
			delete fileupload;
			fileupload = 0;
		}
		uploadbutton->hide();
		ftext->hide();
		fanchor->show();
		delbutton->show();
	} else {
		if (fileupload) {
			fileuploadcontainer->removeChild(fileupload);
//			delete fileupload;
		}
		uploadbutton->clicked().connect(fileupload, &WFileUpload::upload);
		fileupload->uploaded().connect(this, &UploadedFileWidget::fileUploaded);
		fileupload->fileTooLarge().connect(this,
				&UploadedFileWidget::fileTooLarge);
		fileupload = fileuploadcontainer->addWidget(std::make_unique<WFileUpload>());
		fileupload->setFileTextSize(15);
		uploadbutton->show();
		if (comment != "")
			ftext->show();
		fanchor->hide();
		delbutton->hide();
	}
}

void UploadedFileWidget::fileUploaded()
{
	printf("config fileUploaded: %s, client name: %s\n",
			fileupload->spoolFileName().c_str(),
			fileupload->clientFileName().toUTF8().c_str());
	mkdir(workingdir.c_str(), 0777);
	copy_file(fileupload->spoolFileName().c_str(), filepath.c_str());

	updateView();
}

void UploadedFileWidget::fileTooLarge(int64_t size)
{
	printf("config fileTooLarge: tryed : %d \n", size);
//	text->setText(tr("Too big fw file"));
}

void UploadedFileWidget::delFile()
{
	unlink(filepath.c_str());
	updateView();
}

//==============================================================================
//===================== UploadedFilesTable ======================================
//==============================================================================
UploadedFilesTable::UploadedFilesTable(std::string workingdir):
		WGroupBox()
{
	this->workingdir = workingdir;

	table = addWidget(std::make_unique<WTable>());
	fileupload = addWidget(std::make_unique<WFileUpload>());
	fileupload->uploaded().connect(this, &UploadedFilesTable::fileUploaded);
	fileupload->fileTooLarge().connect(this, &UploadedFilesTable::fileTooLarge);
	uploadbutton = addWidget(std::make_unique<WPushButton>(tr("Upload")));
	uploadbutton->clicked().connect(fileupload, &WFileUpload::upload);

	loadFromDir(workingdir);
}

void UploadedFilesTable::setDir(std::string dir)
{
	workingdir = dir;
	table->clear();
	files.clear();
	loadFromDir(workingdir);
}

void UploadedFilesTable::loadFromDir(std::string dir)
{
	struct dirent **namelist;
	int n;
	n = scandir(dir.c_str(), &namelist, 0, alphasort);
	if (n < 0) {
//        perror("scandir");
		return;
	}
	while (n--) {
//		printf("%s\n", namelist[n]->d_name);
		std::string basename = namelist[n]->d_name;
		if ((basename == ".") || (basename == "..")
				|| (basename == "openvpn.conf"))
			continue;
		free(namelist[n]);
		addFile(basename, dir + "/" + basename);
	}
	free(namelist);
}

void UploadedFilesTable::addFile(WString name, std::string filepath)
{
	for (int i = 0; i < files.size(); i++) {
		if (files[i].fresource->fileName() == filepath) {
			// alreaday has link on this file
			return;
		}
	}
	int row = table->rowCount();
	struct filewidgets fw;
	fw.fresource = std::make_shared<WFileResource>(filepath);
	fw.fanchor = table->elementAt(row, 0)->addWidget(std::make_unique<WAnchor>(WLink(fw.fresource), name));
	fw.delbutton = table->elementAt(row, 1)->addWidget(std::make_unique<WPushButton>(tr("Delete")));
	fw.delbutton->clicked().connect(
			std::bind(&UploadedFilesTable::delFileByName, this, name));
	files.push_back(fw);
}

void UploadedFilesTable::delFileByName(WString name)
{
	int idx = -1;
	for (int i = 0; i < files.size(); i++) {
		if (files[i].fanchor->text() == name) {
			idx = i;
			break;
		}
	}
	if (idx == -1)
		return;
	delFileByidx(idx);
}

void UploadedFilesTable::delFileByidx(int idx)
{
	// ToDo
#if 0
	unlink(files[idx].fresource->fileName().c_str());
	files.erase(files.begin() + idx);
	table->deleteRow(idx);
#endif
}

void UploadedFilesTable::fileUploaded()
{
	printf("fileUploaded: %s, client name: %s\n",
			fileupload->spoolFileName().c_str(),
			fileupload->clientFileName().toUTF8().c_str());
	//ToDo -> check fileupload->clientFileName() on [a-zA-Z.]+ -> no spaces and /
	std::string
			savepath = workingdir + "/" + fileupload->clientFileName().toUTF8();
	mkdir(workingdir.c_str(), 0777);
	copy_file(fileupload->spoolFileName().c_str(), savepath.c_str());
//	if( rename( fileupload->spoolFileName().c_str(), savepath.c_str() ) != 0 ){
//		printf("can't move %s to %s : %s\n", fileupload->spoolFileName().c_str(), 
//				savepath.c_str(), strerror(errno) );
//		return;
//	}
	addFile(fileupload->clientFileName(), savepath);
}

void UploadedFilesTable::fileTooLarge(int64_t size)
{
	printf("fileTooLarge: tryed : %d \n", size);
//	text->setText(tr("Too big fw file"));
}


