-include $(SRCROOT)/devtools/makefile_base_posix.mak.link

$(SRCROOT)/devtools/makefile_base_posix.mak.link: $(shell which $(CC)) $(SRCROOT)/$(firstword $(MAKEFILE_LIST))
	if [ "$(shell printf "$(shell $(CC) -dumpversion)\n8" | sort -Vr | head -1)" -eq 8 ]; then \
		ln -sf makefile_base_posix.mak.default $@ ;\
	else \
		ln -sf makefile_base_posix.mak.gcc8 $@ ;\
	fi
