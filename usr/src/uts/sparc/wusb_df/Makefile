#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# uts/sparc/wusb_df/Makefile
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
#
#	This makefile drives the production of the wusb_df driver kernel module.
#	sparc architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
MODULE		= wusb_df
OBJECTS		= $(WUSB_DF_OBJS:%=$(OBJS_DIR)/%)
LINTS		= $(WUSB_DF_OBJS:%.o=$(LINTS_DIR)/%.ln)
ROOTMODULE	= $(ROOT_DRV_DIR)/$(MODULE)
WARLOCK_OUT	= $(WUSB_DF_OBJS:%.o=%.ll)
WARLOCK_OK	= $(MODULE).ok
WLCMD_DIR	= $(UTSBASE)/common/io/warlock

#
#	Include common rules.
#
include $(UTSBASE)/sparc/Makefile.sparc

# 
# lint pass one enforcement 
# 
CFLAGS += $(CCVERBOSE)

#
# depends on misc/usba
#
LDFLAGS		+= -dy -Nmisc/usba

#
#	Define targets
#
ALL_TARGET	= $(BINARY)
LINT_TARGET	= $(MODULE).lint
INSTALL_TARGET	= $(BINARY) $(ROOTMODULE)


#
# For now, disable these lint checks; maintainers should endeavor
# to investigate and remove these for maximum lint coverage.
# Please do not carry these forward to new Makefiles.
#
LINTTAGS	+= -erroff=E_PTRDIFF_OVERFLOW

CERRWARN	+= -_gcc=-Wno-uninitialized

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)
		$(RM) $(WARLOCK_OUT) $(WARLOCK_OK)

clobber:	$(CLOBBER_DEPS)
		$(RM) $(WARLOCK_OUT) $(WARLOCK_OK)

lint:		$(LINT_DEPS)

modlintlib:	$(MODLINTLIB_DEPS)

clean.lint:	$(CLEAN_LINT_DEPS)

install:	$(INSTALL_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/sparc/Makefile.targ

#
#	Defines for local commands.
#
WARLOCK		= warlock
WLCC		= wlcc
TOUCH		= touch
SCCS		= sccs
TEST		= test

#
#	lock_lint rules
#
USBA_FILES = $(USBA_OBJS:%.o= -l ../usba/%.ll)
UHCI_FILES = $(UHCI_OBJS:%.o=../uhci/%.ll)
OHCI_FILES = $(OHCI_OBJS:%.o=../ohci/%.ll)
EHCI_FILES = $(EHCI_OBJS:%.o=../ehci/%.ll)

warlock: $(WARLOCK_OK) warlock_with_usba

%.wlcmd:
	cd $(WLCMD_DIR); $(TEST) -f $@  || $(SCCS) get $@

$(WARLOCK_OK): $(WARLOCK_OUT) wusb_df.wlcmd warlock_ddi.files
	$(WARLOCK) -c $(WLCMD_DIR)/wusb_df.wlcmd $(WARLOCK_OUT)  \
		-l ../warlock/ddi_dki_impl.ll
	$(TOUCH) $@

%.ll: $(UTSBASE)/common/io/usb/clients/wusb_df/%.c \
		$(UTSBASE)/common/sys/usb/clients/wusb_df/wusb_df.h
	$(WLCC)  $(CPPFLAGS) -DDEBUG -DWUSB_DF_PM -DWUSB_DF_CPR -o $@ $<

warlock_with_usba: wusb_df_with_usba.wlcmd  $(WARLOCK_OUT) usba_files \
	warlock_ddi.files
	$(WARLOCK) -c $(WLCMD_DIR)/wusb_df_with_usba.wlcmd \
	$(USBA_FILES) $(WARLOCK_OUT) \
	-l ../warlock/ddi_dki_impl.ll

usba_files:
	@cd ../usba; pwd; $(MAKE) warlock

uhci_files:
	@cd ../uhci; pwd; $(MAKE) warlock

ohci_files:
	@cd ../ohci; pwd; $(MAKE) warlock

ehci_files:
	@cd ../ehci; pwd; $(MAKE) warlock

warlock_ddi.files:
	@cd ../warlock; pwd; $(MAKE) warlock

