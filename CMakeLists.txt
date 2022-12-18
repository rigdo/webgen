cmake_minimum_required(VERSION 3.5)
project(webgen)

add_executable(localgui
        main2.cpp
        LocalGui.cpp
        LocalGui.h
        SummaryPage.cpp
        SummaryPage.h
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
        xmrig_algos.h
        xmrig_algos.cpp
        VtPage.h
        VtPage.cpp
        PasswordPage.h
        PasswordPage.cpp
        RebootPage.h
        RebootPage.cpp
        ServiceControlWidget.h
        ServiceControlWidget.cpp
        )

target_link_libraries(localgui
        PRIVATE
        wthttp
        wt
        crypt
        )

find_package(NVML)
if (NVML_FOUND)
    message("localgui: with nvml support")
    target_compile_definitions(localgui PUBLIC USE_NVML=1)
    target_sources( localgui PRIVATE
            ${CMAKE_SOURCE_DIR}/NvidiaPage.h
            ${CMAKE_SOURCE_DIR}/NvidiaPage.cpp)
    #nvml.h
    target_include_directories(localgui PUBLIC ${CMAKE_SOURCE_DIR}/)
    #nvidia-ml
    target_link_libraries(localgui PRIVATE NVML::NVML)
endif ()

find_package(ROCM_SMI)
if (ROCM_SMI_FOUND)
    message("localgui: with rocm_smi support")
    target_compile_definitions(localgui PUBLIC USE_ROCM_SMI=1)
    target_sources( localgui PRIVATE
            ${CMAKE_SOURCE_DIR}/AmdPage.h
            ${CMAKE_SOURCE_DIR}/AmdPage.cpp)
    # rocm_smi.h
    target_include_directories(localgui PUBLIC ${CMAKE_SOURCE_DIR}/)
    # rocm_smi64
    target_link_libraries(localgui PRIVATE ROCM_SMI::ROCM_SMI)
endif ()

add_executable(sitegui
        main.cpp
        SiteGui.cpp
        SiteGui.h
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
        xmrig_algos.h
        xmrig_algos.cpp
        ZipGenPage.h
        ZipGenPage.cpp
        PasswordPage.h
        PasswordPage.cpp
        ServiceControlWidget.h
        ServiceControlWidget.cpp
        )

target_link_libraries(sitegui
        PRIVATE
        wthttp
        wt
        crypt
        )
