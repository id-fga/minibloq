CODEBLOCKS=codeblocks

MKDIR=mkdir
INSTALL=install
RM=rm
CP=cp
CHOWN=chown
WHOAMI=whoami
HOME=/home/$(shell $(WHOAMI))
UPDATEPACK_INSTALLER=./Resources/instalar_updatepacks.sh

CBP_FILE=Minibloq.Linux.v0.84.0.cbp
MINIBLOQ_VERSION=v0.84.0
#El updatepack1 tiene algunos nombres incorrectos en el XML
#Ademas el updatepack2 se ve mejor
UPDATEPACK_FILE=../updatePacks/up2.classicIcons.zip
DEFAULT_CONFIG_FILE=./Resources/miniBloq.xml
DESKTOP_FILE=./Resources/miniBloq.desktop
ICON_FILE=./Resources/Minibloq.png
LANGUAGE=es

BLOCKS_DIR=Bin/Minibloq/blocks
CORES_DIR=Bin/Minibloq/cores
DRIVERS_DIR=Bin/Minibloq/drivers
GUI_DIR=Bin/Minibloq/GUI
HARD_DIR=Bin/Minibloq/hard
#LANG_DIR=Bin/Minibloq/lang
LIBS_DIR=Bin/Minibloq/libs
TOOLS_DIR=Bin/Minibloq/tools
#WORK_DIR=Bin/Minibloq/work
EXAMPLES_DIR=Bin/Minibloq/work/_examples/
BUILD_DIR=Bin/Minibloq/mbq/$(MINIBLOQ_VERSION)

LOCALE_DIR=Bin/Minibloq/GUI/Texts
LANG_FILE=$(LOCALE_DIR)/$(LANGUAGE).mo
LOCALE_DEST_DIR=/usr/share/locale/$(LANGUAGE)/LC_MESSAGES/
DESKTOP_DEST_DIR=/usr/share/applications/
ICON_DEST_DIR=/usr/share/icons/minibloq/


ROOT_DIR=/opt/minibloq
DEST_DIR=$(ROOT_DIR)/$(MINIBLOQ_VERSION)


all:
	$(CODEBLOCKS) --build $(CBP_FILE)

install:
	$(MKDIR) -p $(DEST_DIR)
	$(MKDIR) -p $(DEST_DIR)/bin/mbq/
	$(MKDIR) -p $(ICON_DEST_DIR)
	$(CP) -rf $(BLOCKS_DIR) $(DEST_DIR)
	$(CP) -rf $(CORES_DIR) $(DEST_DIR)
	$(CP) -rf $(DRIVERS_DIR) $(DEST_DIR)
	$(CP) -rf $(GUI_DIR) $(DEST_DIR)
	$(CP) -rf $(HARD_DIR) $(DEST_DIR)
	$(CP) -rf $(LIBS_DIR) $(DEST_DIR)
	$(CP) -rf $(TOOLS_DIR) $(DEST_DIR)
	$(INSTALL) $(BUILD_DIR)/Minibloq $(DEST_DIR)/bin/mbq/
	$(UPDATEPACK_INSTALLER) $(UPDATEPACK_FILE)
	$(INSTALL) $(LANG_FILE) $(LOCALE_DEST_DIR)/Minibloq.mo
	$(INSTALL) $(DESKTOP_FILE) $(DESKTOP_DEST_DIR)
	$(INSTALL) $(ICON_FILE) $(ICON_DEST_DIR)

config:
	$(MKDIR) -p $(HOME)/.config/miniBloq/
	$(INSTALL) $(DEFAULT_CONFIG_FILE) $(HOME)/.config/miniBloq/

examples:
	$(MKDIR) -p $(HOME)/miniBloq_ejemplos
	$(CP) -rf $(EXAMPLES_DIR)/* $(HOME)/miniBloq_ejemplos/

uninstall:
	$(RM) -rf $(ROOT_DIR)
	$(RM) $(LOCALE_DEST_DIR)/Minibloq.mo
	$(RM) -rf $(DESKTOP_DEST_DIR)/miniBloq.desktop
	$(RM) -rf $(ICON_DEST_DIR)

uninstall_examples:
	$(RM) -rf $(HOME)/miniBloq_ejemplos/

clean:
	$(CODEBLOCKS) --clean $(CBP_FILE)
