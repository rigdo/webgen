cmake_minimum_required(VERSION 3.5)
project (localgui)
add_executable(${PROJECT_NAME} 
	main2.cpp
	LocalGui.cpp
	LocalGui.h
	SummaryPage.cpp
	SummaryPage.h
	NvidiaPage.cpp
	NvidiaPage.h
	Settings.h
	Settings.cpp
	BasePage.h
	BasePage.cpp
	EthernetPage.h
	EthernetPage.cpp
	OpenVpnPage.h
	OpenVpnPage.cpp
	UploadedFileWidget.h
	UploadedFileWidget.cpp
	WiFiPage.h
	WiFiPage.cpp
	Xmrig.h
	Xmrig.cpp
	Ethminer.h
	Ethminer.cpp
	XmrigNvidia.h
	XmrigNvidia.cpp
	XmrigAmd.h
	XmrigAmd.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE 
	.
)

target_link_libraries( ${PROJECT_NAME}
	PRIVATE
	nvidia-ml
	wthttp
	wt
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})


