/*
 * BasePage.cpp
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */
#include "ServiceControlWidget.h"
#include <Wt/WPanel.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

//==============================================================================
//===================== ServiceControlWidget =========================================
//==============================================================================
ServiceControlWidget::ServiceControlWidget( const std::string &service )
{
	this->service = service;
	setTitle( tr("Service Contol"));

	{
		WContainerWidget *c = this->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("Control")));
		label->setStyleClass("shortlabel");

		WContainerWidget *buttonscontainer = c->addWidget(std::make_unique<WContainerWidget>());
		buttonscontainer->setStyleClass("servicebuttonscontainer");
		start_button = buttonscontainer->addWidget( std::make_unique<WPushButton>(tr("Start")) );
		stop_button = buttonscontainer->addWidget( std::make_unique<WPushButton>(tr("Stop")));
		restart_tbutton = buttonscontainer->addWidget( std::make_unique<WPushButton>(tr("Restart")) );
	}
	{
		WContainerWidget *c = this->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("State")));
		label->setStyleClass("shortlabel");
		startstopstate = c->addWidget( std::make_unique<WText>() );
	}

	start_button->clicked().connect( this, &ServiceControlWidget::start );
	stop_button->clicked().connect( this, &ServiceControlWidget::stop );
	restart_tbutton->clicked().connect( this, &ServiceControlWidget::restart );

	updateView();
}

//run: vsftpd: (pid 533) 52334s; run: log: (pid 530) 52334s
// fail,run,down
std::string ServiceControlWidget::getState()
{
	std::string cmd = "sv status " + service;
	FILE *fp = popen( cmd.c_str(), "r" );
	if( !fp )
		return "fail";
	char buf[4096];
	buf[0] = 0;
	buf[sizeof(buf)-1] = 0;
	fgets( buf, sizeof(buf)-1, fp );
	std::string line(buf);
	size_t pos = line.find(':');
	if( pos == std::string::npos )
		return "fail";
	std::string state = line.substr( 0, pos );
	pclose( fp );
	return state;
}

void ServiceControlWidget::updateView()
{
	std::string state = getState();
	if( state == "fail" ){ //no service
		startstopstate->setText( tr("disabled") );
		start_button->setDisabled( false );
		stop_button->setDisabled( true );
		restart_tbutton->setDisabled( true );
	}else
	if( state == "down" ){
		startstopstate->setText( tr("down") );
		start_button->setDisabled( false );
		stop_button->setDisabled( true );
		restart_tbutton->setDisabled( true );
	}else
	if( state == "run" ){
		startstopstate->setText( tr("run") );
		start_button->setDisabled( true );
		stop_button->setDisabled( false );
		restart_tbutton->setDisabled( false );
	}
}

void ServiceControlWidget::addService()
{
	std::string cmd;
	cmd = "addservice.sh " + service;
	system( cmd.c_str() );
	//give runsvdir time to detect new dir
	int cnt = 0;
	do
	{
		struct stat f{};
		std::string statfile = "/var/service/" + service + "/supervise/stat";
		if( stat( statfile.c_str(), &f) == 0 )
			break;
		sleep(1);
		cnt++;
	}while( cnt < 10 );
//	sleep(6); 
	updateView();
}

void ServiceControlWidget::delService()
{
	std::string cmd;
	cmd = "delservice.sh " + service;
	system( cmd.c_str() );
	updateView();
}

void ServiceControlWidget::start()
{
	addService();
	std::string cmd = "sv start " + service;
	system( cmd.c_str() );
	updateView();
}

void ServiceControlWidget::stop()
{
	delService();
//	std::string cmd = "sv stop " + service;
//	system( cmd.c_str() );
	updateView();
}

void ServiceControlWidget::restart()
{
	std::string cmd = "sv restart " + service;
	system( cmd.c_str() );
	updateView();
}
