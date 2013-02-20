$(DIST_DIR)/%: %
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp -pd $< $@
