/**
 * class: I3MCTimeGeneratorService
 *
 * @file
 * @brief I3MCTimeGeneratorService - A hit writing module
 *
 * Version $Id: I3MCTimeGeneratorService.cxx 16564 2006-03-01 03:02:50Z olivas $
 * $Revision: 2.0 $
 * $Author: Gustav Wikstrom$
 * $Date: 2006-02-28 22:02:50 -0500 (Tue, 28 Feb 2006) $
 *
 * (c) 2005 IceCube Collaboration
 */

#include "sim-services/time-generator/I3MCTimeGeneratorService.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3Frame.h"
#include "icetray/I3Tray.h"

I3MCTimeGeneratorService::I3MCTimeGeneratorService(int year, int64_t daqTime):
  year_(year),
  daqTime_(daqTime)
{
}

/**
 *Configures I3MCTimeGeneratorService
 */
bool I3MCTimeGeneratorService::MoreEvents()
{
  return true;//there are always more events
}

I3Time I3MCTimeGeneratorService::PopEvent(I3Frame& frame)
{
  log_debug("Entering PopEvent()");
  return I3Time(year_, daqTime_);
} 
