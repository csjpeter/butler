cmake_minimum_required(VERSION 3.16)

project(butler VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libcsjp)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(SQLite3 REQUIRED)


set(TS_FILES butler_en_150.ts)

set(PROJECT_SOURCES
	src/butler_dataclasses.cpp
	src/butler_sql_connection.cpp
	src/qt/main.cpp
	src/qt/butler_application.cpp
	src/qt/butler_config.cpp
	src/qt/butler_customview.cpp
	src/qt/butler_databases.cpp
	src/qt/butler_databasesview.cpp
	src/qt/butler_datamodel.cpp
	src/qt/butler_editaccountview.cpp
	src/qt/butler_editbrandview.cpp
	src/qt/butler_editcompanyview.cpp
	src/qt/butler_editdbdescview.cpp
	src/qt/butler_editinventoryview.cpp
	src/qt/butler_edititemview.cpp
	src/qt/butler_editpartnerview.cpp
	src/qt/butler_editpaymentview.cpp
	src/qt/butler_edittagview.cpp
	src/qt/butler_editwareview.cpp
	src/qt/butler_infoview.cpp
	src/qt/butler_kineticscroller.cpp
	src/qt/butler_listviews.cpp
	src/qt/butler_mainview.cpp
	src/qt/butler_pannview.cpp
	src/qt/butler_paymentqueryoptionsview.cpp
	src/qt/butler_paymentstatsview.cpp
	src/qt/butler_paymentview.cpp
	src/qt/butler_queryoptionsview.cpp
	src/qt/butler_statsview.cpp
	src/qt/butler_tagwidget.cpp
	src/qt/butler_texts.cpp
	src/qt/butler_widgets.cpp
	${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(butler
		MANUAL_FINALIZATION
		${PROJECT_SOURCES}
	)
	# Define target properties for Android with Qt 6 as:
	#	 set_property(TARGET butler APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
	#					${CMAKE_CURRENT_SOURCE_DIR}/android)
	# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

	qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
	if(ANDROID)
		add_library(butler SHARED
			${PROJECT_SOURCES}
		)
		# Define properties for Android with Qt 5 after find_package() calls as:
		#	 set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
	else()
		add_executable(butler
			${PROJECT_SOURCES}
		)
	endif()

	qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(butler
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/qt
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(butler
	PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
	libcsjp
    SQLite::SQLite3)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
	set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.butler)
endif()
set_target_properties(butler PROPERTIES
	${BUNDLE_ID_OPTION}
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS butler
	BUNDLE DESTINATION .
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(butler)
endif()
