MESON = meson

BUILD_DIR = build
DOCS_DIR = docs
SUBPROJECTS_DIR = subprojects
DOCS_DIR = docs

DESTDIR = ~/install

BIN_NAME = awaken

default: all

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(DOCS_DIR)
	@/usr/bin/find $(SUBPROJECTS_DIR) -mindepth 1 -maxdepth 1 -type d -exec $(RM) -r {} \;
.PHONY: clean

$(DOCS_DIR):
	doxygen Doxyfile

$(BUILD_DIR):
	$(MESON) setup $(BUILD_DIR)

release:
	$(MESON) $(BUILD_DIR) --buildtype=release
.PHONY: release

$(BUILD_DIR)/$(BIN_NAME): $(BUILD_DIR)
	$(MESON) compile -C $(BUILD_DIR) $(BIN_NAME)
.PHONY: $(BUILD_DIR)/$(BIN_NAME)

all: $(BUILD_DIR)/$(BIN_NAME)

run: $(BUILD_DIR)/$(BIN_NAME)
	$(BUILD_DIR)/$(BIN_NAME)
.PHONY: run

install:
	cd $(BUILD_DIR) && DESTDIR=$(DESTDIR) meson install
.PHONY: install

list-assertions:
	pmset -g assertions
.PHONY: list-assertions

log-assertions:
	pmset -g assertionslog
.PHONY: log-assertions

spm-clean:
	$(RM) -r ".build"
	$(RM) -r ".swiftpm"
.PHONY: spm-clean

spm-build:
	swift build
.PHONY: spm-build

spm-run:
	swift run awaken
.PHONY: spm-run
