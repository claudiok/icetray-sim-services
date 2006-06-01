/**
 * class: I3MCTimeGeneratorServiceFactory
 *
 * @file
 * @brief I3MCTimeGeneratorServiceFactory - A hit writing module
 *
 * Version $Id: I3MCTimeGeneratorServiceFactory.cxx 16564 2006-03-01 03:02:50Z olivas $
 * $Revision: 2.0 $
 * $Author: Gustav Wikstrom$
 * $Date: 2006-02-28 22:02:50 -0500 (Tue, 28 Feb 2006) $
 *
 * (c) 2005 IceCube Collaboration
 */

#include "sim-services/time-generator/I3MCTimeGeneratorServiceFactory.h"
#include "sim-services/time-generator/I3MCTimeGeneratorService.h"
#include "dataclasses/I3Units.h"
#include "icetray/I3Frame.h"
#include "icetray/I3Tray.h"

using namespace std;

I3MCTimeGeneratorServiceFactory::I3MCTimeGeneratorServiceFactory(const I3Context& ctx) : 
  I3ServiceFactory(ctx),
  year_(2006),
  daqTime_(0)
{
  AddParameter("Year", "Year of the run", year_);
  AddParameter("DAQTime", "DAQTime of the run in 1/10 of ns", daqTime_);
}

/**
 *Destructor!
 */
I3MCTimeGeneratorServiceFactory::~I3MCTimeGeneratorServiceFactory()
{
}

/**
 *Configures I3MCTimeGeneratorServiceFactory
 */
void I3MCTimeGeneratorServiceFactory::Configure()
{
  log_info("Configuring the I3MCTimeGeneratorServiceFactory");
  
  GetParameter("Year", year_);
  GetParameter("DAQTime", daqTime_);
}

bool I3MCTimeGeneratorServiceFactory::InstallService(I3Context& services)
{
  log_debug("Entering I3MCTimeGeneratorServiceFactory::Physics()");
  if(!eventService_){
    eventService_ = 
      shared_ptr<I3MCTimeGeneratorService>
      (new I3MCTimeGeneratorService(year_,daqTime_));
    log_debug("Made new I3MCRawDOMStatusService.");
  }
  return services.Put<I3EventService>(eventService_);
} 
