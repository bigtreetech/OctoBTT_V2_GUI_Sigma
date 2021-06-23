QT += quick gui network concurrent core websockets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../\
                ../headers/\
                ../cpp/

HEADERS += \
        headers/messageParser.h \
        headers/userloginmodule.h\
        headers/accesscontrol.h\
        headers/disposethreadwithapi.h\
#        headers/viewprovider.h \
        headers/websocketmodule.h\
        headers/functionmodule.h\
        headers/connectiondetection.h\
#        headers/cameramodule.h\
        headers/usbfs.h\
        headers/terminal.h\
        headers/machineprofile.h\
        headers/wlanconfig.h

SOURCES += \
        cpp/messageParser.cpp \
#        cpp/viewprovider.cpp \
        main.cpp\
        cpp/userloginmodule.cpp\
        cpp/accesscontrol.cpp\
        cpp/disposethreadwithapi.cpp\
        cpp/websocketmodule.cpp\
        cpp/functionmodule.cpp\
        cpp/connectiondetection.cpp\
#        cpp/cameramodule.cpp\
        cpp/usbfs.cpp\
        cpp/terminal.cpp\
        cpp/machineprofile.cpp\
        cpp/wlanconfig.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
