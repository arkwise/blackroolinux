TARGET = pspboot
ifdef FW150
OBJS = main.o
else
OBJS = main.o kmodlib.o
endif

INSTALL_DIR := /share/psp
#PSP_FW_VERSION = 371
INCDIR = 
CFLAGS = -O2 -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

ifdef FW150
CFLAGS += -DFW150
endif

LIBS += -lz
LIBDIR =
LDFLAGS =

ifdef FW150
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = uClinux on PSP 0.22 (for FW150)
else
EXTRA_TARGETS = EBOOT.PBP ./kmodlib/kmodlib.prx
PSP_EBOOT_TITLE = uClinux on PSP 0.22
endif
PSP_EBOOT_ICON = icon.png
PSP_EBOOT_PIC1 =
PSP_EBOOT_SND0 =

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

# Extra rules
./kmodlib/kmodlib.prx:
	$(MAKE) -C ./kmodlib

kmodlib.o: kmodlib.S

kmodlib.S: ./kmodlib/kmodlib.exp
	psp-build-exports -s $<

clean: kmodlib_clean

.PHONY: kmodlib_clean
kmodlib_clean:
	$(MAKE) -C ./kmodlib clean
	rm -f kmodlib.S

.PNONY: install
install: $(EXTRA_TARGETS)
	cp ./EBOOT.PBP $(INSTALL_DIR)/EBOOT.PBP
ifndef FW150
	cp ./kmodlib/kmodlib.prx $(INSTALL_DIR)/kmodlib.prx
endif
	cp ./pspboot.conf $(INSTALL_DIR)/./pspboot.conf
	@echo "*** Done ***"
