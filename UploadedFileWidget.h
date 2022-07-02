/*
 * UploadedFileWidget.h
 *
 *  Created on: 20.08.2012
 *      Author: bond
 */

#ifndef UPLOADEDFILEWIDGET_H_
#define UPLOADEDFILEWIDGET_H_

#include <Wt/WContainerWidget.h>
#include <Wt/WFileUpload.h>
#include <Wt/WPushButton.h>
#include <Wt/WFileResource.h>
#include <Wt/WAnchor.h>
#include <Wt/WGroupBox.h>
#include <Wt/WTable.h>

using namespace Wt;

void copy_file(const char *input, const char *output);
//==============================================================================
//===================== UploadedFileWidget =====================================
//==============================================================================
class UploadedFileWidget: public WContainerWidget
{
public:
	UploadedFileWidget(std::string workingdir, std::string filename,
			WString comment = "");

	virtual ~UploadedFileWidget()
	{}

	void updateView();

private:
	void fileUploaded();
	void fileTooLarge(int64_t size);
	void delFile();
public:
	WString comment;
	std::string filepath;
	std::string workingdir;
	std::string filename;
private:

	WContainerWidget *fileuploadcontainer;
	WFileUpload *fileupload;
	WPushButton *uploadbutton;

	WText *ftext;
	std::shared_ptr<WFileResource> fresource;
	WAnchor *fanchor;
	WPushButton *delbutton;
};

//==============================================================================
//===================== UploadedFilesTable ======================================
//==============================================================================
class UploadedFilesTable: public WGroupBox
{
public:
	UploadedFilesTable(std::string workingdir);

	virtual ~UploadedFilesTable()
	{}

	void setDir(std::string dir);
	void addFile(WString name, std::string filepath);

private:
	void loadFromDir(std::string dir);
	void fileUploaded();
	void fileTooLarge(int64_t size);
	void delFileByName(WString name);
	void delFileByidx(int idx);
private:
	std::string workingdir;
	WTable *table;
	WFileUpload *fileupload;
	WPushButton *uploadbutton;

	struct filewidgets
	{
		std::shared_ptr<WFileResource> fresource;
		WAnchor *fanchor;
		WPushButton *delbutton;
	};
	std::vector<struct filewidgets> files;
};


#endif /* UPLOADEDFILEWIDGET_H_ */
