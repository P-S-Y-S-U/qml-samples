QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../cmake-prefix/lib/release/ -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../cmake-prefix/lib/debug/ -lopencv_core -lopencv_imgcodecs -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../cmake-prefix/lib/ -lopencv_core -lopencv_imgcodecs -lopencv_imgproc

INCLUDEPATH += $$PWD/../../cmake-prefix/include/opencv4
DEPENDPATH += $$PWD/../../cmake-prefix/include/

INCLUDEPATH += $$PWD/../../cmake-prefix/include
DEPENDPATH += $$PWD/../../cmake-prefix/include
