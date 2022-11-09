TARGET := iphone:clang:latest:7.0

include $(THEOS)/makefiles/common.mk

ARCHS = armv7 arm64

TOOL_NAME = vmrwtool

vmrwtool_FILES = include/kern_help.c main.c
vmrwtool_CFLAGS = -fobjc-arc
vmrwtool_CODESIGN_FLAGS = -Sentitlements.plist
vmrwtool_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk

client:
	@echo "making for mac, doesnt work the same as ios but is good for test runs."
	@echo ""

	gcc include/kern_help.c main.c -o macvmrwtool
