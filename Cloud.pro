QT       += core gui sql network
QT += core gui widgets axcontainer
QT += multimedia multimediawidgets
QT += pdf
QT+=pdfwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addsharedfiledialog.cpp \
    customlistwidget.cpp \
    downloadprogresstablewidget.cpp \
    globaldata.cpp \
    handleexceldialog.cpp \
    handleimagedialog.cpp \
    handlepdfworddialog.cpp \
    handletxtdialog.cpp \
    handlevideodialog.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    sharefiledialog.cpp \
    sharelistdialog.cpp
    # 添加 Poppler 头文件路径
    INCLUDEPATH += C:/ProgramData/chocolatey/lib/poppler/tools/Library/include/poppler/cpp

    # 添加 Poppler 库文件路径
    LIBS += C:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/poppler.lib
    LIBS += C:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/poppler-cpp.lib
    LIBS += C:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/poppler-glib.lib
    DEPENDPATH+=C:/ProgramData/chocolatey/lib/poppler
HEADERS += \
    addsharedfiledialog.h \
    customlistwidget.h \
    downloadprogresstablewidget.h \
    globaldata.h \
    handleexceldialog.h \
    handleimagedialog.h \
    handlepdfworddialog.h \
    handletxtdialog.h \
    handlevideodialog.h \
    logindialog.h \
    mainwindow.h \
    sharefiledialog.h \
    sharelistdialog.h

FORMS += \
    addsharedfiledialog.ui \
    handleexceldialog.ui \
    handleimagedialog.ui \
    handlepdfworddialog.ui \
    handletxtdialog.ui \
    handlevideodialog.ui \
    logindialog.ui \
    mainwindow.ui \
    sharefiledialog.ui \
    sharelistdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FileIcon.qrc

win32:CONFIG(release, debug|release): LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler
else:unix: LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler

INCLUDEPATH += C:/ProgramData/chocolatey/lib/poppler/tools/Library/include
DEPENDPATH += C:/ProgramData/chocolatey/lib/poppler/tools/Library/include



win32:CONFIG(release, debug|release): LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler-cpp
else:unix: LIBS += -LC:/ProgramData/chocolatey/lib/poppler/tools/Library/lib/ -lpoppler-cpp

INCLUDEPATH += C:/ProgramData/chocolatey/lib/poppler/tools/Library/include
DEPENDPATH += C:/ProgramData/chocolatey/lib/poppler/tools/Library/include
