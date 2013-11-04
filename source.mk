
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

$(DIST_DIR)/android/src/org/kde/necessitas/%: android/src/org/kde/necessitas/%
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $+ $@

# http://developer.android.com/guide/topics/manifest/manifest-intro.html
# $(DIST_DIR)/android/build-apk.sh.in
android: \
		$(DIST_DIR)/android/AndroidManifest.xml \
		$(DIST_DIR)/android/res/values/strings.xml \
		$(DIST_DIR)/android/build.xml \
		$(DIST_DIR)/android/res/values/libs.xml \
		$(DIST_DIR)/android/src/org/kde/necessitas/origo/QtActivity.java \
		$(DIST_DIR)/android/src/org/kde/necessitas/origo/QtApplication.java \
		$(DIST_DIR)/android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
		$(DIST_DIR)/android/src/org/kde/necessitas/ministro/IMinistro.aidl \
		$(DIST_DIR)/android/project.properties \
		$(DIST_DIR)/android/local.properties
	@true

$(DIST_DIR)/android/project.properties:
	@test -d $(dir $@) || mkdir -p $(dir $@)
	echo "target=android-8" > $@
#	echo "target=$(ANDROID_SDK_API)" > $@

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
	@echo "all: build\n" > $@
	./generator.$(PACKAGING).sh Makefile.$(PACKAGING).in >> $@
	./generator.$(PACKAGING).sh Makefile.in >> $@

$(DIST_DIR)/%.in: %.in
	@test -d $(dir $@) || mkdir -p $(dir $@)
	./generator.$(PACKAGING).sh $*.in > $@

$(DIST_DIR)/%: %
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp -pd $< $@

source: \
	$(DIST_DIR)/src/butler_text.h \
	$(DIST_DIR)/src/butler_datetime.h \
	$(DIST_DIR)/src/qt/butler_abstract_table_model.h \
	$(DIST_DIR)/src/butler_macros.h \
	$(DIST_DIR)/src/butler_conversions.h \
	$(DIST_DIR)/src/butler_dataclasses.json \
	$(DIST_DIR)/src/butler_dataclasses.h \
	$(DIST_DIR)/src/sql/butler_sql_connection.cpp \
	$(DIST_DIR)/src/sql/butler_item_db.cpp \
	$(DIST_DIR)/src/sql/butler_queries_db.cpp \
	$(DIST_DIR)/src/sql/butler_partner_db.cpp \
	$(DIST_DIR)/src/sql/butler_company_db.cpp \
	$(DIST_DIR)/src/sql/butler_tag_db.cpp \
	$(DIST_DIR)/src/sql/butler_ware_db.cpp \
	$(DIST_DIR)/src/sql/butler_sql_connection.h \
	$(DIST_DIR)/src/sql/butler_item_db.h \
	$(DIST_DIR)/src/sql/butler_queries_db.h \
	$(DIST_DIR)/src/sql/butler_partner_db.h \
	$(DIST_DIR)/src/sql/butler_company_db.h \
	$(DIST_DIR)/src/sql/butler_tag_db.h \
	$(DIST_DIR)/src/sql/butler_ware_db.h \
	$(DIST_DIR)/src/qt/butler_edititemview.cpp \
	$(DIST_DIR)/src/qt/butler_application.cpp \
	$(DIST_DIR)/src/qt/butler_buyitemview.cpp \
	$(DIST_DIR)/src/qt/butler_config.cpp \
	$(DIST_DIR)/src/qt/butler_custommodel.cpp \
	$(DIST_DIR)/src/qt/butler_customview.cpp \
	$(DIST_DIR)/src/qt/butler_databasesview.cpp \
	$(DIST_DIR)/src/qt/butler_editpartnerview.cpp \
	$(DIST_DIR)/src/qt/butler_editcompanyview.cpp \
	$(DIST_DIR)/src/qt/butler_editdbdescview.cpp \
	$(DIST_DIR)/src/qt/butler_edittagview.cpp \
	$(DIST_DIR)/src/qt/butler_editwareview.cpp \
	$(DIST_DIR)/src/qt/butler_itemsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_databases.cpp \
	$(DIST_DIR)/src/qt/butler_mainview.cpp \
	$(DIST_DIR)/src/qt/butler_infoview.cpp \
	$(DIST_DIR)/src/qt/butler_newitemview.cpp \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.cpp \
	$(DIST_DIR)/src/qt/butler_shoppingmodel.cpp \
	$(DIST_DIR)/src/qt/butler_shoppingview.cpp \
	$(DIST_DIR)/src/qt/butler_partnersmodel.cpp \
	$(DIST_DIR)/src/qt/butler_partnersview.cpp \
	$(DIST_DIR)/src/qt/butler_companymodel.cpp \
	$(DIST_DIR)/src/qt/butler_companyview.cpp \
	$(DIST_DIR)/src/qt/butler_tagfilterview.cpp \
	$(DIST_DIR)/src/qt/butler_tagsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_tagsview.cpp \
	$(DIST_DIR)/src/qt/butler_tagwidget.cpp \
	$(DIST_DIR)/src/qt/butler_waresmodel.cpp \
	$(DIST_DIR)/src/qt/butler_warecategoriesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_waresview.cpp \
	$(DIST_DIR)/src/qt/butler_queriesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_databasesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_edititemview.h \
	$(DIST_DIR)/src/qt/butler_application.h \
	$(DIST_DIR)/src/qt/butler_buyitemview.h \
	$(DIST_DIR)/src/qt/butler_config.h \
	$(DIST_DIR)/src/qt/butler_custommodel.h \
	$(DIST_DIR)/src/qt/butler_customview.h \
	$(DIST_DIR)/src/qt/butler_databasesview.h \
	$(DIST_DIR)/src/qt/butler_editpartnerview.h \
	$(DIST_DIR)/src/qt/butler_editcompanyview.h \
	$(DIST_DIR)/src/qt/butler_editdbdescview.h \
	$(DIST_DIR)/src/qt/butler_edittagview.h \
	$(DIST_DIR)/src/qt/butler_editwareview.h \
	$(DIST_DIR)/src/qt/butler_itemsmodel.h \
	$(DIST_DIR)/src/qt/butler_databases.h \
	$(DIST_DIR)/src/qt/butler_mainview.h \
	$(DIST_DIR)/src/qt/butler_infoview.h \
	$(DIST_DIR)/src/qt/butler_newitemview.h \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.h \
	$(DIST_DIR)/src/qt/butler_shoppingmodel.h \
	$(DIST_DIR)/src/qt/butler_shoppingview.h \
	$(DIST_DIR)/src/qt/butler_partnersmodel.h \
	$(DIST_DIR)/src/qt/butler_partnersview.h \
	$(DIST_DIR)/src/qt/butler_companymodel.h \
	$(DIST_DIR)/src/qt/butler_companyview.h \
	$(DIST_DIR)/src/qt/butler_tagfilterview.h \
	$(DIST_DIR)/src/qt/butler_tagsmodel.h \
	$(DIST_DIR)/src/qt/butler_tagsview.h \
	$(DIST_DIR)/src/qt/butler_tagwidget.h \
	$(DIST_DIR)/src/qt/butler_waresmodel.h \
	$(DIST_DIR)/src/qt/butler_warecategoriesmodel.h \
	$(DIST_DIR)/src/qt/butler_waresview.h \
	$(DIST_DIR)/src/qt/butler_queriesmodel.h \
	$(DIST_DIR)/src/qt/butler_databasesmodel.h \
	$(DIST_DIR)/src/qt/butler_kineticscroller.cpp \
	$(DIST_DIR)/src/qt/butler_kineticscroller.h \
	$(DIST_DIR)/src/qt/butler_pannview.h \
	$(DIST_DIR)/src/qt/butler_pannview.cpp \
	$(DIST_DIR)/src/qt/butler_widgets.h \
	$(DIST_DIR)/src/qt/butler_widgets.cpp \
	$(DIST_DIR)/src/qt/butler_statsview.cpp \
	$(DIST_DIR)/src/qt/butler_statsview.h \
	$(DIST_DIR)/src/qt/butler_texts.cpp \
	$(DIST_DIR)/src/qt/butler_texts.h \
	$(DIST_DIR)/src/qt/main.cpp \
	$(DIST_DIR)/share/icons/copyright \
	$(DIST_DIR)/share/icons/accounting.png \
	$(DIST_DIR)/share/icons/analitics.png \
	$(DIST_DIR)/share/icons/add.png \
	$(DIST_DIR)/share/icons/buy.png \
	$(DIST_DIR)/share/icons/delete.png \
	$(DIST_DIR)/share/icons/edit.png \
	$(DIST_DIR)/share/icons/query.png \
	$(DIST_DIR)/share/icons/shopping.png \
	$(DIST_DIR)/share/icons/tag.png \
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
	$(DIST_DIR)/share/icons/company.png \
	$(DIST_DIR)/share/icons/refresh.png \
	$(DIST_DIR)/share/icons/statistics.png \
	$(DIST_DIR)/share/icons/comboarrowdown.png \
	$(DIST_DIR)/share/icons/comboarrowup.png \
	$(DIST_DIR)/share/icons/calendar.png \
	$(DIST_DIR)/share/icons/databases.png \
	$(DIST_DIR)/share/icons/use.png \
	$(DIST_DIR)/share/icons/butler.png \
	$(DIST_DIR)/config \
	$(DIST_DIR)/configure \
	$(DIST_DIR)/Makefile.in \
	$(DIST_DIR)/src/config.h.in \
	$(DIST_DIR)/source-license \
	$(DIST_DIR)/enduser-license \
	$(DIST_DIR)/share/translations/en.qm \
	$(DIST_DIR)/share/translations/hu.qm \
	$(DIST_DIR)/share/css/application.css.in
	@true

