
source: \
	$(DIST_DIR)/src/datamodel/butler_item.cpp \
	$(DIST_DIR)/src/datamodel/butler_item_set.cpp \
	$(DIST_DIR)/src/datamodel/butler_query.cpp \
	$(DIST_DIR)/src/datamodel/butler_query_set.cpp \
	$(DIST_DIR)/src/datamodel/butler_shop.cpp \
	$(DIST_DIR)/src/datamodel/butler_shop_set.cpp \
	$(DIST_DIR)/src/datamodel/butler_tag.cpp \
	$(DIST_DIR)/src/datamodel/butler_tag_set.cpp \
	$(DIST_DIR)/src/datamodel/butler_ware.cpp \
	$(DIST_DIR)/src/datamodel/butler_ware_set.cpp \
	$(DIST_DIR)/src/datamodel/butler_item.h \
	$(DIST_DIR)/src/datamodel/butler_item_set.h \
	$(DIST_DIR)/src/datamodel/butler_query.h \
	$(DIST_DIR)/src/datamodel/butler_query_set.h \
	$(DIST_DIR)/src/datamodel/butler_shop.h \
	$(DIST_DIR)/src/datamodel/butler_shop_set.h \
	$(DIST_DIR)/src/datamodel/butler_tag.h \
	$(DIST_DIR)/src/datamodel/butler_tag_set.h \
	$(DIST_DIR)/src/datamodel/butler_ware.h \
	$(DIST_DIR)/src/datamodel/butler_ware_set.h \
	$(DIST_DIR)/src/datamodel/butler_macros.h \
	$(DIST_DIR)/src/datamodel/test/item.cpp \
	$(DIST_DIR)/src/datamodel/test/item_set.cpp \
	$(DIST_DIR)/src/datamodel/test/query.cpp \
	$(DIST_DIR)/src/datamodel/test/query_set.cpp \
	$(DIST_DIR)/src/datamodel/test/shop.cpp \
	$(DIST_DIR)/src/datamodel/test/shop_set.cpp \
	$(DIST_DIR)/src/datamodel/test/tag.cpp \
	$(DIST_DIR)/src/datamodel/test/tag_set.cpp \
	$(DIST_DIR)/src/datamodel/test/ware.cpp \
	$(DIST_DIR)/src/datamodel/test/ware_set.cpp \
	$(DIST_DIR)/src/sql/butler_database_descriptor.cpp \
	$(DIST_DIR)/src/sql/butler_sql_connection.cpp \
	$(DIST_DIR)/src/sql/butler_sqlquery.cpp \
	$(DIST_DIR)/src/sql/butler_db.cpp \
	$(DIST_DIR)/src/sql/butler_itembought_table.cpp \
	$(DIST_DIR)/src/sql/butler_item_table.cpp \
	$(DIST_DIR)/src/sql/butler_item_db.cpp \
	$(DIST_DIR)/src/sql/butler_queries_table.cpp \
	$(DIST_DIR)/src/sql/butler_queries_db.cpp \
	$(DIST_DIR)/src/sql/butler_queryshops_table.cpp \
	$(DIST_DIR)/src/sql/butler_querytags_table.cpp \
	$(DIST_DIR)/src/sql/butler_querywares_table.cpp \
	$(DIST_DIR)/src/sql/butler_shop_table.cpp \
	$(DIST_DIR)/src/sql/butler_shop_db.cpp \
	$(DIST_DIR)/src/sql/butler_tag_table.cpp \
	$(DIST_DIR)/src/sql/butler_tag_db.cpp \
	$(DIST_DIR)/src/sql/butler_warecategories_table.cpp \
	$(DIST_DIR)/src/sql/butler_ware_table.cpp \
	$(DIST_DIR)/src/sql/butler_ware_db.cpp \
	$(DIST_DIR)/src/sql/butler_waretags_table.cpp \
	$(DIST_DIR)/src/sql/butler_database_descriptor.h \
	$(DIST_DIR)/src/sql/butler_db.h \
	$(DIST_DIR)/src/sql/butler_sql_connection.h \
	$(DIST_DIR)/src/sql/butler_sqlquery.h \
	$(DIST_DIR)/src/sql/butler_itembought_table.h \
	$(DIST_DIR)/src/sql/butler_item_table.h \
	$(DIST_DIR)/src/sql/butler_item_db.h \
	$(DIST_DIR)/src/sql/butler_queries_table.h \
	$(DIST_DIR)/src/sql/butler_queries_db.h \
	$(DIST_DIR)/src/sql/butler_queryshops_table.h \
	$(DIST_DIR)/src/sql/butler_querytags_table.h \
	$(DIST_DIR)/src/sql/butler_querywares_table.h \
	$(DIST_DIR)/src/sql/butler_shop_table.h \
	$(DIST_DIR)/src/sql/butler_shop_db.h \
	$(DIST_DIR)/src/sql/butler_tag_table.h \
	$(DIST_DIR)/src/sql/butler_tag_db.h \
	$(DIST_DIR)/src/sql/butler_warecategories_table.h \
	$(DIST_DIR)/src/sql/butler_ware_table.h \
	$(DIST_DIR)/src/sql/butler_ware_db.h \
	$(DIST_DIR)/src/sql/butler_waretags_table.h \
	$(DIST_DIR)/src/sql/test/item.cpp \
	$(DIST_DIR)/src/sql/test/query.cpp \
	$(DIST_DIR)/src/sql/test/shop.cpp \
	$(DIST_DIR)/src/sql/test/tag.cpp \
	$(DIST_DIR)/src/sql/test/ware.cpp \
	$(DIST_DIR)/src/qt/butler_accountingview.cpp \
	$(DIST_DIR)/src/qt/butler_application.cpp \
	$(DIST_DIR)/src/qt/butler_buyitemview.cpp \
	$(DIST_DIR)/src/qt/butler_config.cpp \
	$(DIST_DIR)/src/qt/butler_custommodel.cpp \
	$(DIST_DIR)/src/qt/butler_customview.cpp \
	$(DIST_DIR)/src/qt/butler_edititemview.cpp \
	$(DIST_DIR)/src/qt/butler_editshopview.cpp \
	$(DIST_DIR)/src/qt/butler_edittagview.cpp \
	$(DIST_DIR)/src/qt/butler_editwareview.cpp \
	$(DIST_DIR)/src/qt/butler_itemsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_databases.cpp \
	$(DIST_DIR)/src/qt/butler_mainview.cpp \
	$(DIST_DIR)/src/qt/butler_infoview.cpp \
	$(DIST_DIR)/src/qt/butler_newitemview.cpp \
	$(DIST_DIR)/src/qt/butler_newshopview.cpp \
	$(DIST_DIR)/src/qt/butler_newtagview.cpp \
	$(DIST_DIR)/src/qt/butler_newwareview.cpp \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.cpp \
	$(DIST_DIR)/src/qt/butler_shoppingmodel.cpp \
	$(DIST_DIR)/src/qt/butler_shoppingview.cpp \
	$(DIST_DIR)/src/qt/butler_shopsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_shopsview.cpp \
	$(DIST_DIR)/src/qt/butler_stockmodel.cpp \
	$(DIST_DIR)/src/qt/butler_stockview.cpp \
	$(DIST_DIR)/src/qt/butler_tagfilterview.cpp \
	$(DIST_DIR)/src/qt/butler_tagsmodel.cpp \
	$(DIST_DIR)/src/qt/butler_tagsview.cpp \
	$(DIST_DIR)/src/qt/butler_tagwidget.cpp \
	$(DIST_DIR)/src/qt/butler_waresmodel.cpp \
	$(DIST_DIR)/src/qt/butler_waresview.cpp \
	$(DIST_DIR)/src/qt/butler_queriesmodel.cpp \
	$(DIST_DIR)/src/qt/butler_accountingview.h \
	$(DIST_DIR)/src/qt/butler_application.h \
	$(DIST_DIR)/src/qt/butler_buyitemview.h \
	$(DIST_DIR)/src/qt/butler_config.h \
	$(DIST_DIR)/src/qt/butler_custommodel.h \
	$(DIST_DIR)/src/qt/butler_customview.h \
	$(DIST_DIR)/src/qt/butler_edititemview.h \
	$(DIST_DIR)/src/qt/butler_editshopview.h \
	$(DIST_DIR)/src/qt/butler_edittagview.h \
	$(DIST_DIR)/src/qt/butler_editwareview.h \
	$(DIST_DIR)/src/qt/butler_itemsmodel.h \
	$(DIST_DIR)/src/qt/butler_databases.h \
	$(DIST_DIR)/src/qt/butler_mainview.h \
	$(DIST_DIR)/src/qt/butler_infoview.h \
	$(DIST_DIR)/src/qt/butler_newitemview.h \
	$(DIST_DIR)/src/qt/butler_newshopview.h \
	$(DIST_DIR)/src/qt/butler_newtagview.h \
	$(DIST_DIR)/src/qt/butler_newwareview.h \
	$(DIST_DIR)/src/qt/butler_queryoptionsview.h \
	$(DIST_DIR)/src/qt/butler_shoppingmodel.h \
	$(DIST_DIR)/src/qt/butler_shoppingview.h \
	$(DIST_DIR)/src/qt/butler_shopsmodel.h \
	$(DIST_DIR)/src/qt/butler_shopsview.h \
	$(DIST_DIR)/src/qt/butler_stockmodel.h \
	$(DIST_DIR)/src/qt/butler_stockview.h \
	$(DIST_DIR)/src/qt/butler_tagfilterview.h \
	$(DIST_DIR)/src/qt/butler_tagsmodel.h \
	$(DIST_DIR)/src/qt/butler_tagsview.h \
	$(DIST_DIR)/src/qt/butler_tagwidget.h \
	$(DIST_DIR)/src/qt/butler_waresmodel.h \
	$(DIST_DIR)/src/qt/butler_waresview.h \
	$(DIST_DIR)/src/qt/butler_queriesmodel.h \
	$(DIST_DIR)/src/qt/main.cpp \
	$(DIST_DIR)/share/icons/copyright \
	$(DIST_DIR)/share/icons/accounting.png \
	$(DIST_DIR)/share/icons/add.png \
	$(DIST_DIR)/share/icons/buy.png \
	$(DIST_DIR)/share/icons/custom.png \
	$(DIST_DIR)/share/icons/delete.png \
	$(DIST_DIR)/share/icons/edit.png \
	$(DIST_DIR)/share/icons/query.png \
	$(DIST_DIR)/share/icons/shop.png \
	$(DIST_DIR)/share/icons/shopping.png \
	$(DIST_DIR)/share/icons/stock.png \
	$(DIST_DIR)/share/icons/tag.png \
	$(DIST_DIR)/share/icons/trash.png \
	$(DIST_DIR)/share/icons/ware.png \
	$(DIST_DIR)/share/icons/info.png \
	$(DIST_DIR)/share/icons/butler.png

$(DIST_DIR)/%: %
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp -pd $< $@

