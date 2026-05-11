PYTHON ?= /opt/homebrew/bin/python3
VENV   := .venv
PIP    := $(VENV)/bin/pip
PY     := $(VENV)/bin/python

REPO_ROOT       := $(realpath $(CURDIR)/..)
DATA_DIR        := $(REPO_ROOT)/data
VENDOR_LIB      := $(REPO_ROOT)/vendor-lib/WB32F10x_StdPeriph_Lib_V0.1.10
VENDOR_DOCS     := $(REPO_ROOT)/vendor-docs
CHIBIOS_CONTRIB ?= $(HOME)/Development/Personal/WB32/ChibiOS-Contrib

CHIP ?=

.PHONY: help venv install data validate clean distclean

help:
	@echo "Targets:"
	@echo "  venv       create Python venv at $(VENV)"
	@echo "  install    install package + dependencies into venv"
	@echo "  data       generate YAML for all chips (or CHIP=<name>)"
	@echo "  validate   run cross-source validation, write reports"
	@echo "  clean      remove generated YAML"
	@echo "  distclean  remove venv and generated YAML"
	@echo ""
	@echo "Variables:"
	@echo "  CHIP             single chip name (e.g. WB32FQ95xC)"
	@echo "  CHIBIOS_CONTRIB  path to ChibiOS-Contrib checkout"
	@echo "                   (current: $(CHIBIOS_CONTRIB))"

$(VENV)/bin/python:
	$(PYTHON) -m venv $(VENV)
	$(PIP) install --upgrade pip

venv: $(VENV)/bin/python

install: venv
	$(PIP) install -e .

data: install
	$(PY) -m wb32_data generate \
		--vendor-lib $(VENDOR_LIB) \
		--vendor-docs $(VENDOR_DOCS) \
		--chibios-contrib $(CHIBIOS_CONTRIB) \
		--repo-root $(REPO_ROOT) \
		--out $(DATA_DIR) \
		$(if $(CHIP),--chip $(CHIP),)

validate: install
	$(PY) -m wb32_data validate \
		--vendor-lib $(VENDOR_LIB) \
		--vendor-docs $(VENDOR_DOCS) \
		--chibios-contrib $(CHIBIOS_CONTRIB) \
		--repo-root $(REPO_ROOT) \
		--data $(DATA_DIR) \
		$(if $(CHIP),--chip $(CHIP),)

clean:
	rm -rf $(DATA_DIR)/chips/*.yaml $(DATA_DIR)/validation-reports/*.md

distclean: clean
	rm -rf $(VENV)
