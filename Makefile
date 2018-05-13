TARGET=webgui 

arch=rigdo
#arch=x86_stretch
#arch=x86_stretch_local

ifeq ($(arch),x86_stretch)
CXX=g++
STRIP=/bin/true
CPPFLAGS=-g
LDFLAGS=-pthread -lwthttp -lwt -Wl,-Bstatic  \
	-lboost_thread -lboost_system -lboost_filesystem -lboost_regex \
        -lboost_signals -lboost_date_time \
        -Wl,-Bdynamic -lssl -lcrypto -ldl -lz -lcrypt
#-lboost_program_options
else
ifeq ($(arch),x86_stretch_local)
WTDESTDIR=/home/bond/projects/2f_rigdo/witty/wt-3.3.9/build/destdir
CXX=g++
STRIP=/bin/true
CPPFLAGS=-g -I $(WTDESTDIR)/usr/local/include
LDFLAGS=-L$(WTDESTDIR)/usr/local/lib -pthread -lwthttp -lwt -Wl,-Bstatic  \
	-lboost_thread -lboost_system -lboost_filesystem -lboost_regex \
        -lboost_signals -lboost_date_time -lboost_program_options -lboost_random\
        -Wl,-Bdynamic -ldl -lz -lcrypt
else
BR=/home/bond/projects/ad_rigdo/buildroot-2017.11
CXX=$(BR)/output/host/usr/bin/x86_64-linux-g++ --sysroot $(BR)/output/staging 
STRIP=$(BR)/output/host/usr/bin/x86_64-linux-strip
#STRIP=/bin/true
LDFLAGS=-pthread -lwthttp -lwt \
	-lboost_thread -lboost_system -lboost_filesystem -lboost_regex \
	-lboost_signals -lboost_program_options -lboost_date_time -lboost_random \
	-lcrypt
endif
endif

OBJS=main.o Settings.o SiteGui.o BasePage.o \
	EthernetPage.o OpenVpnPage.o UploadedFileWidget.o \
	WiFiPage.o Xmrig.o Ethminer.o XmrigNvidia.o XmrigAmd.o \
	ZipGenPage.o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $@

main.o: main.cpp SiteGui.h Settings.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
	
Settings.o: Settings.cpp Settings.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

SiteGui.o: SiteGui.cpp SiteGui.h Settings.h EthernetPage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

BasePage.o: BasePage.cpp BasePage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

EthernetPage.o: EthernetPage.cpp EthernetPage.h BasePage.h Settings.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

OpenVpnPage.o: OpenVpnPage.cpp OpenVpnPage.h BasePage.h Settings.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

UploadedFileWidget.o: UploadedFileWidget.cpp UploadedFileWidget.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
	
WiFiPage.o: WiFiPage.cpp WiFiPage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Xmrig.o: Xmrig.cpp Xmrig.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
	
XmrigNvidia.o: XmrigNvidia.cpp XmrigNvidia.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

XmrigAmd.o: XmrigAmd.cpp XmrigAmd.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

Ethminer.o: Ethminer.cpp Ethminer.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

ZipGenPage.o: ZipGenPage.cpp ZipGenPage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

##########################################

OBJS2=main2.o Settings.o LocalGui.o BasePage.o \
	EthernetPage.o OpenVpnPage.o UploadedFileWidget.o \
	WiFiPage.o Xmrig.o Ethminer.o XmrigNvidia.o XmrigAmd.o \
	SummaryPage.o

localgui: $(OBJS2)
	$(CXX) -o $@ $(OBJS2) $(LDFLAGS)
	#$(STRIP) $@

main2.o: main2.cpp SiteGui.h Settings.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

LocalGui.o: LocalGui.cpp LocalGui.h Settings.h EthernetPage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

SummaryPage.o: SummaryPage.cpp SummaryPage.h Settings.h EthernetPage.h
	$(CXX) $(CPPFLAGS) -c $< -o $@


clean:
	rm -rf $(TARGET) $(OBJS) localgui $(OBJS2)
