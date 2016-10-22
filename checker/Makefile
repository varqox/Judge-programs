include Makefile.config

.PHONY: all
all: build-info
	@$(MAKE) -f Makefile2
ifeq ($(MAKELEVEL), 0)
	@echo "\033[32mBuild finished\033[0m"
endif

.PHONY: build-info
build-info:
ifeq ($(MAKELEVEL), 0)
	@echo "DEBUG: $(DEBUG)"
	@echo "CC -> $(CC)"
	@echo "CXX -> $(CXX)"
endif


.PHONY: release
release: EXTRA_LD_FLAGS += -static -s
release:
	@$(RM) check
	@$(MAKE) all

.PHONY: clean
clean:
	@$(MAKE) -f Makefile2 clean

.PHONY: help
help:
	@echo "Nothing is here yet..."
