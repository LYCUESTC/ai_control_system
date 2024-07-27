QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network core-private
QT += websockets-private concurrent

CONFIG += c++11 utf8

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += src/httpserver \
               src/sslserver \
               src/3rdparty/http-parser \
               draw/
SOURCES += \
    add_task_dialog.cpp \
    draw/DrawROI.cpp \
    draw/GraphicsView.cpp \
    draw/GraphicsViewTool.cpp \
    draw_image_roi_dialog.cpp \
#    draw_roi_dialog.cpp \
#    drawroiwidget.cpp \
    httpclient/httpbody.cpp \
    httpclient/httpclient.cpp \
    httpclient/httprequest.cpp \
    httpclient/httpresponse.cpp \
    itemdelegate.cpp \
    main.cpp \
    showform.cpp \
    ai_ctl_widget.cpp \
    src/httpserver/qabstracthttpserver.cpp        src/httpserver/qhttpserverresponder.cpp \
    src/httpserver/qhttpserver.cpp                src/httpserver/qhttpserverresponse.cpp \
    src/httpserver/qhttpserverfutureresponse.cpp  src/httpserver/qhttpserverrouter.cpp \
    src/httpserver/qhttpserverliterals.cpp        src/httpserver/qhttpserverrouterrule.cpp \
    src/httpserver/qhttpserverrequest.cpp \
    src/sslserver/qsslserver.cpp \
    src/3rdparty/http-parser/http_parser.c

HEADERS += \
    add_task_dialog.h \
    draw/DrawROI.h \
    draw/GraphicsView.h \
    draw/GraphicsViewTool.h \
    draw_image_roi_dialog.h \
#    draw_roi_dialog.h \
#    drawroiwidget.h \
    httpclient/httpbody.hpp \
    httpclient/httpclient.hpp \
    httpclient/httprequest.hpp \
    httpclient/httpresponse.hpp \
    httplib.h \
    itemdelegate.h \
    listItemdata.h \
    showform.h \
    ai_ctl_widget.h \
    src/httpserver/qabstracthttpserver.h        src/httpserver/qhttpserverresponse.h \
    src/httpserver/qabstracthttpserver_p.h      src/httpserver/qhttpserverresponse_p.h \
    src/httpserver/qhttpserver.h                src/httpserver/qhttpserverrouter.h \
    src/httpserver/qhttpserver_p.h              src/httpserver/qhttpserverrouter_p.h \
    src/httpserver/qhttpserverfutureresponse.h  src/httpserver/qhttpserverrouterrule.h \
    src/httpserver/qhttpserverliterals_p.h      src/httpserver/qhttpserverrouterrule_p.h \
    src/httpserver/qhttpserverrequest.h         src/httpserver/qhttpserverrouterviewtraits.h \
    src/httpserver/qhttpserverrequest_p.h       src/httpserver/qhttpserverviewtraits.h \
    src/httpserver/qhttpserverresponder.h       src/httpserver/qhttpserverviewtraits_impl.h \
    src/httpserver/qhttpserverresponder_p.h     src/httpserver/qthttpserverglobal.h \
    src/sslserver/qsslserver.h  src/sslserver/qsslserver_p.h  src/sslserver/qtsslserverglobal.h \
    src/3rdparty/http-parser/http_parser.h

FORMS += \
    add_task_dialog.ui \
    draw_image_roi_dialog.ui \
    showform.ui \
    ai_ctl_widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
