
# We dont want to use built in rules. Those are ineffective and we
# can never know all of them, thus those can be unexpected as well.

.SUFFIXES:

MAKEFLAGS += --no-builtin-rules --no-builtin-variables
EMPTY=
COMMA=,
SPACE=$(EMPTY) $(EMPTY)
SHELL=bash

tools/generator: \
		tools/generator.cpp
	g++ $(shell PKG_CONFIG_PATH=/opt/extras.ubuntu.com/csjp/lib/pkgconfig \
		pkg-config --cflags --libs libcsjp0.3) \
		-std=c++11 -g -rdynamic \
		tools/generator.cpp \
		$(shell PKG_CONFIG_PATH=/opt/extras.ubuntu.com/csjp/lib/pkgconfig \
			pkg-config --libs libcsjp0.3) \
		-o tools/generator

$(DIST_DIR)/debian/copyright: enduser-license.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh enduser-license.in > $@.in
	cat $@.in | groff -T utf8 > $@

$(DIST_DIR)/debian/rules: debian/rules.native.in debian/rules.cross.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	test "x$(PACKAGING)" = "xdebian" && { \
		./generator.$(PACKAGING).sh debian/rules.native.in > $@ ; \
	} || { \
		./generator.$(PACKAGING).sh debian/rules.cross.in > $@ ; \
	}
	chmod u+x $@

$(DIST_DIR)/debian/compat:
	@test -d $(dir $@) || mkdir -p $(dir $@)
	echo "5" > $(DIST_DIR)/debian/compat

$(DIST_DIR)/debian/source/format:
	@test -d $(dir $@) || mkdir -p $(dir $@)
	echo "1.0" > $@

$(DIST_DIR)/debian/%: debian/%.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh debian/$*.in > $@

$(DIST_DIR)/debian/$(PKGNAME).install.in: debian/pkg.install.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh debian/pkg.install.in > $@

$(DIST_DIR)/debian/$(PKGNAME)-dbg.install.in: debian/dbg.install.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh debian/dbg.install.in > $@

debian: \
		$(DIST_DIR)/debian/copyright \
		$(DIST_DIR)/debian/changelog \
		$(DIST_DIR)/debian/control \
		$(DIST_DIR)/debian/rules \
		$(DIST_DIR)/debian/compat \
		$(DIST_DIR)/debian/source/format \
		$(DIST_DIR)/debian/$(PKGNAME).install.in \
		$(DIST_DIR)/debian/$(PKGNAME)-dbg.install.in \
		$(DIST_DIR)/doxyfile.in \
		$(DIST_DIR)/butler.desktop.in \
		$(DIST_DIR)/butler.man.in
	@true

$(DIST_DIR)/nsis/$(PKGNAME).nsi.in: nsis/install.nsi.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh nsis/install.nsi.in > $@

$(DIST_DIR)/nsis/license.txt.in: enduser-license.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh enduser-license.in > $@

windows: \
		$(DIST_DIR)/nsis/$(PKGNAME).nsi.in \
		$(DIST_DIR)/nsis/license.txt.in
	@true

$(DIST_DIR)/android/%.xml: android/%.xml.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh android/$*.xml.in > $@

$(DIST_DIR)/android/debug.keystore: android/debug.keystore
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/src/org/%: $(QT_HOME)/5.4/$(ANDROID_ARMV)/src/android/java/src/org/%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/Qt%: $(QT_HOME)/5.4/$(ANDROID_ARMV)/jar/Qt%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libQt%: $(QT_HOME)/5.4/$(ANDROID_ARMV)/lib/libQt%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libgnustl_shared.so: \
		$(ANDROID_NDK_HOME)/sources/cxx-stl/gnu-libstdc++/4.6/libs/$(ARM_CPU)/libgnustl_shared.so
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_android_libqtforandroid.so: \
		$(QT_HOME)/5.4/$(ANDROID_ARMV)/plugins/platforms/android/libqtforandroid.so
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_generic_%: \
		$(QT_HOME)/5.4/$(ANDROID_ARMV)/plugins/generic/%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_%: \
		$(QT_HOME)/5.4/$(ANDROID_ARMV)/plugins/imageformats/%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_%: \
		$(QT_HOME)/5.4/$(ANDROID_ARMV)/plugins/platforms/%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

$(DIST_DIR)/android/libs/$(ARM_CPU)/libqsqlite.so: \
		$(QT_HOME)/5.4/$(ANDROID_ARMV)/plugins/sqldrivers/libqsqlite.so
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@


# http://developer.android.com/guide/topics/manifest/manifest-intro.html
# $(DIST_DIR)/android/build-apk.sh.in
android: \
		$(DIST_DIR)/android/AndroidManifest.xml \
		$(DIST_DIR)/android/res/values/strings.xml \
		$(DIST_DIR)/android/build.xml \
		$(DIST_DIR)/android/res/values/libs.xml \
		$(DIST_DIR)/android/src/org/qtproject/qt5/android/bindings/QtActivity.java \
		$(DIST_DIR)/android/src/org/qtproject/qt5/android/bindings/QtApplication.java \
		$(DIST_DIR)/android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
		$(DIST_DIR)/android/src/org/kde/necessitas/ministro/IMinistro.aidl \
		$(DIST_DIR)/android/project.properties \
		$(DIST_DIR)/android/local.properties \
		$(DIST_DIR)/android/libs/QtAndroidAccessibility-bundled.jar \
		$(DIST_DIR)/android/libs/QtAndroid-bundled.jar \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libqsqlite.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libgnustl_shared.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libQt5Core.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libQt5Gui.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libQt5Widgets.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libQt5Sql.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_android_libqtforandroid.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_generic_libqevdevkeyboardplugin.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_generic_libqevdevmouseplugin.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_generic_libqevdevtabletplugin.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_generic_libqevdevtouchplugin.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqdds.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqgif.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqicns.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqico.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqjp2.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqjpeg.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqmng.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqtga.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqtiff.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_imageformats_libqwbmp.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_libqeglfs.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_libqminimalegl.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_libqminimal.so \
		$(DIST_DIR)/android/libs/$(ARM_CPU)/libplugins_platforms_libqoffscreen.so
	@true

$(DIST_DIR)/android/project.properties:
	@test -d $(dir $@) || mkdir -p $(dir $@)
	echo "target=$(ANDROID_SDK_API)" > $@
#	echo "target=android-8" > $@

$(DIST_DIR)/android/local.properties:
	@test -d $(dir $@) || mkdir -p $(dir $@)
	echo "sdk.dir=$(ANDROID_SDK_HOME)" > $@

$(DIST_DIR)/configure: configure.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh configure.in > $@
	chmod u+x $(DIST_DIR)/configure

$(DIST_DIR)/%-license: %-license.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh $*-license.in > $@

$(DIST_DIR)/share/translations/%.qm: share/translations/%.ts
	@test -d $(dir $@) || mkdir -p $(dir $@)
	lrelease share/translations/$*.ts -qm $@

$(DIST_DIR)/share/css/application.css.in: share/css/$(PACKAGING).css.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh share/css/$(PACKAGING).css.in > $@

$(DIST_DIR)/Makefile.in: Makefile.in Makefile.$(PACKAGING).in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	@echo "all: build" > $@
	./generator.$(PACKAGING).sh Makefile.$(PACKAGING).in >> $@
	./generator.$(PACKAGING).sh Makefile.in >> $@

$(DIST_DIR)/%.in: %.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh $*.in > $@

$(DIST_DIR)/src/butler_sql_connection.h: \
		tpl/dataclass_members.h \
		tpl/dataclass_nonmembers.h \
		tpl/dataclass_set.h \
		src/butler_sql_connection.h \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/butler_sql_connection.h > $@

$(DIST_DIR)/src/butler_dataclasses.h: \
		tpl/dbclass.h \
		tpl/dataclass_members.h \
		tpl/dataclass_nonmembers.h \
		tpl/dataclass_set.h \
		src/butler_dataclasses.h \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/butler_dataclasses.h > $@

$(DIST_DIR)/src/butler_dataclasses.cpp: \
		tpl/dbclass.cpp \
		src/butler_dataclasses.cpp \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/butler_dataclasses.cpp > $@

$(DIST_DIR)/src/butler_dbclasses.h: \
		tpl/dbclass.h \
		src/butler_dbclasses.h \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/butler_dbclasses.h > $@

$(DIST_DIR)/src/butler_dbclasses.cpp: \
		tpl/dbclass.cpp \
		src/butler_dbclasses.cpp \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/butler_dbclasses.cpp > $@

$(DIST_DIR)/src/qt/butler_datamodel.h: \
		tpl/datamodel.h \
		tpl/dbdatamodel.h \
		tpl/dbdatamodel_spec.h \
		src/qt/butler_datamodel.h \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/qt/butler_datamodel.h > $@

$(DIST_DIR)/src/qt/butler_datamodel.cpp: \
		tpl/datamodel.cpp \
		tpl/dbdatamodel.cpp \
		tpl/dbdatamodel_spec.cpp \
		src/qt/butler_datamodel.cpp \
		tools/generator
	@test -d $(dir $@) || mkdir -p $(dir $@)
	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/qt/butler_datamodel.cpp > $@

#$(DIST_DIR)/src/%.h: \
#		src/%.h \
#		tools/generator
#	@test -d $(dir $@) || mkdir -p $(dir $@)
#	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/$*.h > $@

#$(DIST_DIR)/src/%.cpp: \
#		src/%.cpp \
#		tools/generator
#	@test -d $(dir $@) || mkdir -p $(dir $@)
#	tools/generator -t ./tpl/ -d src/butler_declarations.h < src/$*.cpp > $@

$(DIST_DIR)/%: %
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp -pd $< $@

source: \
	$(DIST_DIR)/config \
	$(DIST_DIR)/configure \
	$(DIST_DIR)/Makefile.in \
	$(DIST_DIR)/source-license \
	$(DIST_DIR)/enduser-license \
	$(DIST_DIR)/share/translations/en.qm \
	$(DIST_DIR)/share/translations/hu.qm \
	$(DIST_DIR)/share/css/application.css.in \
	\
	$(DIST_DIR)/src/butler_text.h \
	$(DIST_DIR)/src/butler_datetime.h \
	$(DIST_DIR)/src/qt/butler_abstract_table_model.h \
	$(DIST_DIR)/src/butler_macros.h \
	$(DIST_DIR)/src/butler_types.h \
	$(DIST_DIR)/src/butler_conversions.h \
	$(DIST_DIR)/src/butler_dataclasses.h \
	$(DIST_DIR)/src/butler_dataclasses.cpp \
	$(DIST_DIR)/src/butler_dbclasses.h \
	$(DIST_DIR)/src/butler_dbclasses.cpp \
	\
	$(DIST_DIR)/src/qt/butler_datamodel.h \
	$(DIST_DIR)/src/qt/butler_datamodel.cpp \
	\
	$(DIST_DIR)/src/config.h.in \
	$(DIST_DIR)/src/qt/butler_config.h \
	$(DIST_DIR)/src/qt/butler_config.cpp \
	\
	$(DIST_DIR)/src/qt/butler_texts.h \
	$(DIST_DIR)/src/qt/butler_texts.cpp \
	\
	$(DIST_DIR)/src/qt/butler_widgets.h \
	$(DIST_DIR)/src/qt/butler_widgets.cpp \
	\
	$(DIST_DIR)/src/qt/butler_pannview.h \
	$(DIST_DIR)/src/qt/butler_pannview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_kineticscroller.cpp \
	$(DIST_DIR)/src/qt/butler_kineticscroller.h \
	\
	$(DIST_DIR)/src/butler_sql_connection.h \
	$(DIST_DIR)/src/butler_sql_connection.cpp \
	\
	$(DIST_DIR)/src/qt/butler_tagsmodel.h \
	$(DIST_DIR)/src/qt/butler_tagsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_tagsview.h \
	$(DIST_DIR)/src/qt/butler_tagsview.cpp \
	$(DIST_DIR)/src/qt/butler_edittagview.h \
	$(DIST_DIR)/src/qt/butler_edittagview.cpp \
	$(DIST_DIR)/src/qt/butler_tagwidget.h \
	$(DIST_DIR)/src/qt/butler_tagwidget.cpp \
	\
	$(DIST_DIR)/src/qt/butler_waresmodel.h \
	$(DIST_DIR)/src/qt/butler_waresmodel.cpp \
	$(DIST_DIR)/src/qt/butler_waretypesmodel.h \
	$(DIST_DIR)/src/qt/butler_waretypesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_waresview.h \
	$(DIST_DIR)/src/qt/butler_waresview.cpp \
	$(DIST_DIR)/src/qt/butler_editwareview.h \
	$(DIST_DIR)/src/qt/butler_editwareview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_companiesmodel.h \
	$(DIST_DIR)/src/qt/butler_companiesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_companyview.h \
	$(DIST_DIR)/src/qt/butler_companyview.cpp \
	$(DIST_DIR)/src/qt/butler_editcompanyview.h \
	$(DIST_DIR)/src/qt/butler_editcompanyview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_brandsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_brandsmodel.h \
	$(DIST_DIR)/src/qt/butler_brandsview.h \
	$(DIST_DIR)/src/qt/butler_brandsview.cpp \
	$(DIST_DIR)/src/qt/butler_editbrandview.h \
	$(DIST_DIR)/src/qt/butler_editbrandview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_inventoriesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_inventoriesmodel.h \
	$(DIST_DIR)/src/qt/butler_inventoriesview.h \
	$(DIST_DIR)/src/qt/butler_inventoriesview.cpp \
	$(DIST_DIR)/src/qt/butler_editinventoryview.h \
	$(DIST_DIR)/src/qt/butler_editinventoryview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_accountsmodel.h \
	$(DIST_DIR)/src/qt/butler_accountsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_accountsview.h \
	$(DIST_DIR)/src/qt/butler_accountsview.cpp \
	$(DIST_DIR)/src/qt/butler_editaccountview.h \
	$(DIST_DIR)/src/qt/butler_editaccountview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_partnersmodel.h \
	$(DIST_DIR)/src/qt/butler_partnersmodel.cpp \
	$(DIST_DIR)/src/qt/butler_partnersview.h \
	$(DIST_DIR)/src/qt/butler_partnersview.cpp \
	$(DIST_DIR)/src/qt/butler_editpartnerview.h \
	$(DIST_DIR)/src/qt/butler_editpartnerview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_queriesmodel.h \
	$(DIST_DIR)/src/qt/butler_queriesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.h \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_itemsmodel.h \
	$(DIST_DIR)/src/qt/butler_itemsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_edititemview.h \
	$(DIST_DIR)/src/qt/butler_edititemview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_customview.h \
	$(DIST_DIR)/src/qt/butler_customview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_databases.h \
	$(DIST_DIR)/src/qt/butler_databases.cpp \
	$(DIST_DIR)/src/qt/butler_databasesmodel.h \
	$(DIST_DIR)/src/qt/butler_databasesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_databasesview.h \
	$(DIST_DIR)/src/qt/butler_databasesview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_editdbdescview.h \
	$(DIST_DIR)/src/qt/butler_editdbdescview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_statsview.h \
	$(DIST_DIR)/src/qt/butler_statsview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_infoview.h \
	$(DIST_DIR)/src/qt/butler_infoview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_mainview.h \
	$(DIST_DIR)/src/qt/butler_mainview.cpp \
	\
	$(DIST_DIR)/src/qt/butler_application.h \
	$(DIST_DIR)/src/qt/butler_application.cpp \
	\
	$(DIST_DIR)/src/qt/main.cpp \
	\
	$(DIST_DIR)/share/icons/accounting.png \
	$(DIST_DIR)/share/icons/analitics.png \
	$(DIST_DIR)/share/icons/add.png \
	$(DIST_DIR)/share/icons/buy.png \
	$(DIST_DIR)/share/icons/delete.png \
	$(DIST_DIR)/share/icons/edit.png \
	$(DIST_DIR)/share/icons/query.png \
	$(DIST_DIR)/share/icons/shopping.png \
	$(DIST_DIR)/share/icons/tag.png \
	$(DIST_DIR)/share/icons/brand.png \
	$(DIST_DIR)/share/icons/inventory.png \
	$(DIST_DIR)/share/icons/trash.png \
	$(DIST_DIR)/share/icons/cancel.png \
	$(DIST_DIR)/share/icons/ok.png \
	$(DIST_DIR)/share/icons/ware.png \
	$(DIST_DIR)/share/icons/info.png \
	$(DIST_DIR)/share/icons/warning.png \
	$(DIST_DIR)/share/icons/list.png \
	$(DIST_DIR)/share/icons/left.png \
	$(DIST_DIR)/share/icons/right.png \
	$(DIST_DIR)/share/icons/tick.png \
	$(DIST_DIR)/share/icons/partner.png \
	$(DIST_DIR)/share/icons/account.png \
	$(DIST_DIR)/share/icons/company.png \
	$(DIST_DIR)/share/icons/refresh.png \
	$(DIST_DIR)/share/icons/statistics.png \
	$(DIST_DIR)/share/icons/comboarrowdown.png \
	$(DIST_DIR)/share/icons/comboarrowup.png \
	$(DIST_DIR)/share/icons/calendar.png \
	$(DIST_DIR)/share/icons/databases.png \
	$(DIST_DIR)/share/icons/use.png \
	$(DIST_DIR)/share/icons/butler.png \
	\
	$(DIST_DIR)/share/icons/copyright
	@true

