/*
 * UploadedFileWidget.cpp
 *
 *  Created on: 20.08.2012
 *      Author: bond
 */
#include "UploadedFileWidget.h"
#include <Wt/WText>

#include <iostream>
#include <fstream>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>


void copy_file( const char *input, const char *output )
{
	std::ifstream f1 (input, std::fstream::binary);
	std::ofstream f2 (output, std::fstream::trunc|std::fstream::binary);
	f2 << f1.rdbuf ();
}

//==============================================================================
//===================== UploadedFileWidget =====================================
//==============================================================================
UploadedFileWidget::UploadedFileWidget( std::string workingdir, 
		std::string filename, WString comment ) : WContainerWidget()
{
	this->comment = comment;
	this->workingdir = workingdir;
	this->filename = filename;
	this->filepath = workingdir + "/" + filename;
	fileuploadcontainer = new WContainerWidget();
	fileupload = 0;
	uploadbutton = new WPushButton(tr("Upload"));
	
	ftext = new WText( comment );
	ftext->setStyleClass("labelcomment");
	fresource = new WFileResource( filepath );
	fanchor = new WAnchor( WLink( fresource ), filename );
	delbutton = new WPushButton(tr("Delete"));
	delbutton->clicked().connect( boost::bind( &UploadedFileWidget::delFile, this ) );

	fileuploadcontainer->setInline( true );
	addWidget( ftext );
	addWidget( fileuploadcontainer );
	addWidget( uploadbutton );
	addWidget( fanchor );
	addWidget( delbutton );
	updateView();
}

void UploadedFileWidget::updateView()
{
	struct stat statbuf;
	if( stat(filepath.c_str(), &statbuf ) == 0 ){ //has file
		delete fileupload;
		fileupload = 0;
		uploadbutton->hide();
		ftext->hide();
		fanchor->show();
		delbutton->show();
	}
	else{
		if( fileupload )
			delete fileupload;
		fileupload = new WFileUpload();
		fileupload->setFileTextSize(15);
		uploadbutton->clicked().connect(fileupload, &WFileUpload::upload);
		fileupload->uploaded().connect(this, &UploadedFileWidget::fileUploaded);
		fileupload->fileTooLarge().connect(this, &UploadedFileWidget::fileTooLarge);
		fileuploadcontainer->addWidget( fileupload );
		uploadbutton->show();
		if( comment != "" )
			ftext->show();
		fanchor->hide();
		delbutton->hide();
	}
}

void UploadedFileWidget::fileUploaded()
{
	printf("config fileUploaded: %s, client name: %s\n", 
			fileupload->spoolFileName().c_str(), 
			fileupload->clientFileName().toUTF8().c_str() );
	mkdir( workingdir.c_str(),  0777 );
	copy_file( fileupload->spoolFileName().c_str(), filepath.c_str() );
	
	updateView();
}

void UploadedFileWidget::fileTooLarge( int64_t size )
{
	printf("config fileTooLarge: tryed : %d \n", size );
//	text->setText(tr("Too big fw file"));
}

void UploadedFileWidget::delFile( )
{
	unlink( filepath.c_str() );	
	updateView();
}

//==============================================================================
//===================== UploadedFilesTable ======================================
//==============================================================================
UploadedFilesTable::UploadedFilesTable(  std::string workingdir ) : WGroupBox()
{
	this->workingdir = workingdir;
	table = new WTable();
	fileupload = new WFileUpload();
//	fileupload->setFileTextSize(10);
	uploadbutton = new WPushButton(tr("Upload"));
	addWidget( table );
	addWidget( fileupload );
	addWidget( uploadbutton );
	
	uploadbutton->clicked().connect(fileupload, &WFileUpload::upload);
	fileupload->uploaded().connect(this, &UploadedFilesTable::fileUploaded);
	fileupload->fileTooLarge().connect(this, &UploadedFilesTable::fileTooLarge);

	loadFromDir( workingdir );
}

void UploadedFilesTable::setDir( std::string dir )
{
	workingdir = dir;
	table->clear();
	files.clear();
	loadFromDir( workingdir );
}

void UploadedFilesTable::loadFromDir( std::string dir )
{
	struct dirent **namelist;
	int n;
	n = scandir(dir.c_str(), &namelist, 0, alphasort);
	if (n < 0){
//        perror("scandir");
        return;
	}
	while( n-- ){
//		printf("%s\n", namelist[n]->d_name);
		std::string basename = namelist[n]->d_name;
		if( (basename == ".") || (basename == "..") || (basename == "openvpn.conf"))
			continue;
		free(namelist[n]);
		addFile( basename, dir + "/" + basename  );
	}
	free(namelist);
}

void UploadedFilesTable::addFile( WString name, std::string filepath )
{
	for( int i =0; i < files.size(); i++ ){
		if( files[i].fresource->fileName() == filepath ){
			// alreaday has link on this file
			return;
		}
	}
	int row = table->rowCount();
	struct filewidgets fw;
	fw.fresource = new WFileResource( filepath );
	fw.fanchor = new WAnchor( WLink( fw.fresource ), name );
	fw.delbutton = new WPushButton(tr("Delete"));
	files.push_back( fw );
	table->elementAt( row, 0 )->addWidget( fw.fanchor );
	table->elementAt( row, 1 )->addWidget( fw.delbutton );
	
	fw.delbutton->clicked().connect( boost::bind( &UploadedFilesTable::delFileByName, this, name) );
}

void UploadedFilesTable::delFileByName( WString name )
{
	int idx = -1;
	for( int i =0; i < files.size(); i++ ){
		if( files[i].fanchor->text() == name ){
			idx = i;
			break;
		}
	}
	if( idx == -1 )
		return;
	delFileByidx( idx );
}

void UploadedFilesTable::delFileByidx( int idx )
{
	unlink( files[idx].fresource->fileName().c_str() );
	files.erase( files.begin() + idx );
	table->deleteRow( idx );
}


void UploadedFilesTable::fileUploaded()
{
	printf("fileUploaded: %s, client name: %s\n", 
			fileupload->spoolFileName().c_str(), 
			fileupload->clientFileName().toUTF8().c_str() );
	//ToDo -> check fileupload->clientFileName() on [a-zA-Z.]+ -> no spaces and /
	std::string savepath = workingdir + "/" + fileupload->clientFileName().toUTF8();
	mkdir( workingdir.c_str(),  0777 );
	copy_file( fileupload->spoolFileName().c_str(), savepath.c_str() );
//	if( rename( fileupload->spoolFileName().c_str(), savepath.c_str() ) != 0 ){
//		printf("can't move %s to %s : %s\n", fileupload->spoolFileName().c_str(), 
//				savepath.c_str(), strerror(errno) );
//		return;
//	}
	addFile( fileupload->clientFileName(), savepath );
}

void UploadedFilesTable::fileTooLarge( int64_t size )
{
	printf("fileTooLarge: tryed : %d \n", size );
//	text->setText(tr("Too big fw file"));
}


