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
#include "dataclasses/physics/I3EventHeader.h"
#include "icetray/I3Frame.h"
#include "icetray/I3Tray.h"
#include "dataclasses/I3Bool.h"

I3MCTimeGeneratorService::I3MCTimeGeneratorService(int year, 
						   int64_t daqTime,
						   unsigned runNumber,
						   unsigned eventID):
  year_(year),
  daqTime_(daqTime),
  runNumber_(runNumber),
  eventID_(eventID),
  incEventID_(false)
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
  I3Time evtTime(year_, daqTime_);

  // Someone has specified a run number
  if(runNumber_ > 0 ) 
    {
      //Lets make a frame header that can be used for testing
      I3EventHeaderPtr eventHeader_(new I3EventHeader);
      eventHeader_->SetStartTime(evtTime);
      eventHeader_->SetRunID(runNumber_);
      eventHeader_->SetSubRunID(0);
      eventHeader_->SetEventID(eventID_);
      //Lets put it in the frame
      frame.Put<I3EventHeader>(eventHeader_);

      if(incEventID_) eventID_++;

      //Need to tell the conglomerator to back off
      //and not bother incrementing the eventID
      I3BoolPtr incID(new I3Bool(incEventID_));
      frame.Put("MCTimeIncEventID",incID);
    }

  return evtTime;
} 
