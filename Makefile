#
# $Id: Makefile 4187 2004-10-15 20:42:04Z troy $
#
# This is a generic makefile, and should most probably not be changed.  
#

ifndef I3_WORK
$(error please set I3_WORK to your icecube work directory.)
endif
ifeq ($(shell test -d $(I3_WORK) || echo -n NO),NO)
  $(error I3_WORK is set ($(I3_WORK)), but the directory does not exist)
endif

ifndef I3_TOOLS
$(error please set I3_TOOLS to your icecube tools directory.)
endif
ifeq ($(shell test -d $(I3_TOOLS) || echo -n NO),NO)
  $(error I3_TOOLS is set ($(I3_TOOLS)), but the directory does not exist)
endif

include $(I3_WORK)/offline-mk/config.mk

include $(PROJECT_CONFIG_FILENAME)

include $(I3_WORK)/offline-mk/defs.mk
include $(I3_WORK)/offline-mk/targets.mk




