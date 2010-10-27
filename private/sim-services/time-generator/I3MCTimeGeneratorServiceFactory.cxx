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
#include <dataclasses/I3Time.h>

using namespace std;

I3_SERVICE_FACTORY(I3MCTimeGeneratorServiceFactory);

const int DEFAULT_YEAR = 2006;
const int64_t DEFAULT_DAQTIME = 0;

const int DEFAULT_MJD_SECONDS(0);
const double DEFAULT_MJD_NANOSECONDS(0.);

I3MCTimeGeneratorServiceFactory::I3MCTimeGeneratorServiceFactory(const I3Context& ctx) : 
  I3ServiceFactory(ctx),
  year_(DEFAULT_YEAR),
  daqTime_(DEFAULT_DAQTIME),
  eventServiceName_(I3DefaultName<I3EventService>::value()),
  mjd_(INT_MIN),
  mjd_s_(DEFAULT_MJD_SECONDS),
  mjd_ns_(DEFAULT_MJD_NANOSECONDS),
  runNumber_(0),
  eventID_(0),
  incEventID_(false)
{
  AddParameter("Year", "Year of the run", year_);
  AddParameter("DAQTime", "DAQTime of the run in 1/10 of ns", daqTime_);
  AddParameter("MJD","Modified Julian Date",mjd_);
  AddParameter("MJDSeconds","Number of seconds after the start of the MJD.",mjd_s_);
  AddParameter("MJDNanoSeconds","Number of nanoseconds after the start of the second given in MJDSeconds.",mjd_ns_);
  AddParameter("InstallEventServiceAs", "Name to install event service under", eventServiceName_);
  AddParameter("RunNumber", "Run Number", runNumber_);
  AddParameter("EventID", "Event ID", eventID_);
  AddParameter("IncrementEventID", "Increment Event ID (default =  false)", incEventID_);
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
  GetParameter("MJD",mjd_);
  GetParameter("MJDSeconds",mjd_s_);
  GetParameter("MJDNanoSeconds",mjd_ns_);
  GetParameter("InstallEventServiceAs", eventServiceName_);
  GetParameter("RunNumber", runNumber_);
  GetParameter("EventID", eventID_);
  GetParameter("IncrementEventID", incEventID_);

  if(mjd_ != INT_MIN && 
     (year_ != DEFAULT_YEAR || daqTime_ != DEFAULT_DAQTIME ))
     log_fatal("Ambiguous settings : Please choose either Mjd or Year and DAQTime.  Not both.");

  if(mjd_ != INT_MIN){
    I3Time t;
    t.SetModJulianTime(mjd_,mjd_s_,mjd_ns_);
    year_ = t.GetUTCYear();
    daqTime_ = t.GetUTCDaqTime();
  }
}

bool I3MCTimeGeneratorServiceFactory::InstallService(I3Context& services)
{
  log_debug("Entering I3MCTimeGeneratorServiceFactory::Physics()");
  if(!eventService_){
    eventService_ = 
      shared_ptr<I3MCTimeGeneratorService>
      (new I3MCTimeGeneratorService(year_,daqTime_, runNumber_, eventID_));
    eventService_->IncrementEventID(incEventID_);
    log_debug("Made new I3MCRawDOMStatusService.");
  }
  return services.Put<I3EventService>(eventService_,eventServiceName_);
} 
