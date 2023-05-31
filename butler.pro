#-------------------------------------------------
#
# Project created by QtCreator 2023-05-28T13:04:41
#
#-------------------------------------------------

CONFIG += static
CONFIG += staticlib
CONFIG += debug

QT     += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = butler
TEMPLATE = app

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += sqlite3
}

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui


INCLUDEPATH += libcsjp/devel
INCLUDEPATH += libcsjp/core
INCLUDEPATH += libcsjp/container
INCLUDEPATH += libcsjp/system
INCLUDEPATH += libcsjp/human
INCLUDEPATH += src
INCLUDEPATH += src/qt

QMAKE_CXXFLAGS+=-Wno-expansion-to-defined
QMAKE_CXXFLAGS+=-Wno-deprecated-copy
QMAKE_CXXFLAGS+=-Wno-expansion-to-defined
QMAKE_CXXFLAGS+=-Wno-implicit-fallthrough
QMAKE_CXXFLAGS+=-Wno-unused-function

QMAKE_CXXFLAGS_DEBUG+=-Og -g3 -ggdb3

SOURCES_test += \
    libcsjp/devel/test/exception.cpp \
    libcsjp/container/test/array.cpp \
    libcsjp/container/test/bintree.cpp \
    libcsjp/container/test/container.cpp \
    libcsjp/container/test/container_speed.cpp \
    libcsjp/container/test/index.cpp \
    libcsjp/container/test/json.cpp \
    libcsjp/container/test/pod_array.cpp \
    libcsjp/container/test/ref_array.cpp \
    libcsjp/container/test/sorter_container.cpp \
    libcsjp/container/test/table.cpp \
    libcsjp/core/test/file.cpp \
    libcsjp/core/test/mutex.cpp \
    libcsjp/core/test/socket.cpp \
    libcsjp/core/test/str.cpp \
    libcsjp/core/test/string.cpp \
    libcsjp/human/test/text.cpp \
    libcsjp/human/test/unichar.cpp \
    libcsjp/system/test/client.cpp \
    libcsjp/system/test/daemon.cpp \
    libcsjp/system/test/epoll.cpp \
    libcsjp/system/test/http.cpp \
    libcsjp/system/test/listener.cpp \
    libcsjp/system/test/nc.cpp \
    libcsjp/system/test/server.cpp \
    libcsjp/system/test/signal.cpp \
    libcsjp/system/test/socket.cpp \
    libcsjp/system/test/websocket.cpp

SOURCES_nc += \
    libcsjp/tools/nc.cpp

SOURCES_unused += \
    libcsjp/human/csjp_text.h \
    libcsjp/human/csjp_unichar.h \
    libcsjp/human/csjp_text.cpp \
    libcsjp/human/csjp_unichar.cpp \

HEADERS  += \
    src/qt/butler_abstract_table_model.h \
    libcsjp/container/csjp_array.h \
    libcsjp/container/csjp_bintree.h \
    libcsjp/container/csjp_container.h \
    libcsjp/container/csjp_ipc_block.h \
    libcsjp/container/csjp_json.h \
    libcsjp/container/csjp_owner_container.h \
    libcsjp/container/csjp_ownerbintree.h \
    libcsjp/container/csjp_pod_array.h \
    libcsjp/container/csjp_ref_array.h \
    libcsjp/container/csjp_reference_container.h \
    libcsjp/container/csjp_sorter_container.h \
    libcsjp/container/csjp_sorter_owner_container.h \
    libcsjp/container/csjp_sorter_reference_container.h \
    libcsjp/container/csjp_value_bintree.h \
    libcsjp/container/csjp_value_container.h \
    libcsjp/core/csjp_astr.h \
    libcsjp/core/csjp_file.h \
    libcsjp/core/csjp_mutex.h \
    libcsjp/core/csjp_stopper.h \
    libcsjp/core/csjp_str.h \
    libcsjp/core/csjp_string.h \
    libcsjp/devel/csjp_carray.h \
    libcsjp/devel/csjp_ctypes.h \
    libcsjp/devel/csjp_defines.h \
    libcsjp/devel/csjp_exception.h \
    libcsjp/devel/csjp_logger.h \
    libcsjp/devel/csjp_object.h \
    libcsjp/devel/csjp_skeleton.h \
    libcsjp/devel/csjp_test.h \
    libcsjp/devel/csjp_utility.h \
    libcsjp/devel/csjp_utility_android.h \
    libcsjp/devel/csjp_utility_mxe.h \
    libcsjp/system/csjp_client.h \
    libcsjp/system/csjp_daemon.h \
    libcsjp/system/csjp_dup_socket.h \
    libcsjp/system/csjp_epoll.h \
    libcsjp/system/csjp_epoll_control.h \
    libcsjp/system/csjp_http.h \
    libcsjp/system/csjp_listener.h \
    libcsjp/system/csjp_server.h \
    libcsjp/system/csjp_signal.h \
    libcsjp/system/csjp_socket.h \
    libcsjp/system/csjp_websocket.h \
    src/qt/butler_application.h \
    src/qt/butler_config.h \
    src/qt/butler_customview.h \
    src/qt/butler_databases.h \
    src/qt/butler_databasesview.h \
    src/qt/butler_datamodel.h \
    src/qt/butler_editaccountview.h \
    src/qt/butler_editbrandview.h \
    src/qt/butler_editcompanyview.h \
    src/qt/butler_editdbdescview.h \
    src/qt/butler_editinventoryview.h \
    src/qt/butler_edititemview.h \
    src/qt/butler_editpartnerview.h \
    src/qt/butler_editpaymentview.h \
    src/qt/butler_edittagview.h \
    src/qt/butler_editwareview.h \
    src/qt/butler_infoview.h \
    src/qt/butler_kineticscroller.h \
    src/qt/butler_listviews.h \
    src/qt/butler_mainview.h \
    src/qt/butler_pannview.h \
    src/qt/butler_paymentqueryoptionsview.h \
    src/qt/butler_paymentstatsview.h \
    src/qt/butler_paymentview.h \
    src/qt/butler_queryoptionsview.h \
    src/qt/butler_statsview.h \
    src/qt/butler_tagwidget.h \
    src/qt/butler_texts.h \
    src/qt/butler_widgets.h \
    src/butler_conversions.h \
    src/butler_dataclasses.h \
    src/butler_datetime.h \
    src/butler_macros.h \
    src/butler_sql_connection.h \
    src/butler_text.h \
    src/butler_types.h \
    src/config.h

SOURCES += \
    libcsjp/container/csjp_json.cpp \
    libcsjp/core/csjp_astr.cpp \
    libcsjp/core/csjp_file.cpp \
    libcsjp/core/csjp_mutex.cpp \
    libcsjp/core/csjp_str.cpp \
    libcsjp/core/csjp_string.cpp \
    libcsjp/devel/csjp_exception.cpp \
    libcsjp/devel/csjp_logger.cpp \
    libcsjp/devel/csjp_utility.cpp \
    libcsjp/devel/csjp_utility_android.cpp \
    libcsjp/devel/csjp_utility_mxe.cpp \
    libcsjp/system/csjp_client.cpp \
    libcsjp/system/csjp_daemon.cpp \
    libcsjp/system/csjp_dup_socket.cpp \
    libcsjp/system/csjp_epoll.cpp \
    libcsjp/system/csjp_epoll_control.cpp \
    libcsjp/system/csjp_http.cpp \
    libcsjp/system/csjp_listener.cpp \
    libcsjp/system/csjp_server.cpp \
    libcsjp/system/csjp_signal.cpp \
    libcsjp/system/csjp_socket.cpp \
    libcsjp/system/csjp_websocket.cpp \
    src/qt/butler_application.cpp \
    src/qt/butler_config.cpp \
    src/qt/butler_customview.cpp \
    src/qt/butler_databases.cpp \
    src/qt/butler_databasesview.cpp \
    src/qt/butler_datamodel.cpp \
    src/qt/butler_editaccountview.cpp \
    src/qt/butler_editbrandview.cpp \
    src/qt/butler_editcompanyview.cpp \
    src/qt/butler_editdbdescview.cpp \
    src/qt/butler_editinventoryview.cpp \
    src/qt/butler_edititemview.cpp \
    src/qt/butler_editpartnerview.cpp \
    src/qt/butler_editpaymentview.cpp \
    src/qt/butler_edittagview.cpp \
    src/qt/butler_editwareview.cpp \
    src/qt/butler_infoview.cpp \
    src/qt/butler_kineticscroller.cpp \
    src/qt/butler_listviews.cpp \
    src/qt/butler_mainview.cpp \
    src/qt/butler_pannview.cpp \
    src/qt/butler_paymentqueryoptionsview.cpp \
    src/qt/butler_paymentstatsview.cpp \
    src/qt/butler_paymentview.cpp \
    src/qt/butler_queryoptionsview.cpp \
    src/qt/butler_statsview.cpp \
    src/qt/butler_tagwidget.cpp \
    src/qt/butler_texts.cpp \
    src/qt/butler_widgets.cpp \
    src/qt/main.cpp \
    src/butler_dataclasses.cpp \
    src/butler_sql_connection.cpp

DESTDIR = build
RELEASEDIR = release

butler.path=$$RELEASEDIR/bin/
butler.files=$$DESTDIR/butler
INSTALLS += butler

image.path = $$RELEASEDIR/share/butler/icons/
image.files = share/icons/*.png
INSTALLS += image

css.path = $$RELEASEDIR/share/butler/css/
css.files = share/css/application.css
INSTALLS += css

desktop.path = $$RELEASEDIR/share/applications/butler.desktop
desktop.files = share/applications/butler.desktop
INSTALLS += desktop

translations.path = $$RELEASEDIR/share/butler/translations/
translations.files = share/translations/en.qm
translations.files += share/translations/hu.qm
INSTALLS += translations

DISTFILES += \
	README.md

