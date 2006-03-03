/**
 * class: I3MCTimeGenerator
 *
 * @file
 * @brief I3MCTimeGenerator - A hit writing module
 *
 * Version $Id: I3MCTimeGenerator.cxx 16564 2006-03-01 03:02:50Z olivas $
 * $Revision: 2.0 $
 * $Author: Gustav Wikstrom$
 * $Date: 2006-02-28 22:02:50 -0500 (Tue, 28 Feb 2006) $
 *
 * (c) 2005 IceCube Collaboration
 */

#include "time-generator/I3MCTimeGenerator.h"
#include "dataclasses/I3Units.h"
#include "dataclasses/physics/I3EventHeader.h"
#include "phys-services/I3RandomService.h"
#include "icetray/I3Frame.h"
#include "icetray/I3Tray.h"

using namespace std;

const int64_t MAX_DAQTIME =  315569260000000000LL; //Number of tenths of ns in a year
const int64_t MAX_EVENT_TIME = 10000000; //10^7 tenths of ns (= 1 ms)

I3MCTimeGenerator::I3MCTimeGenerator(const I3Context& ctx) : 
  I3Module(ctx),
  startRunYear_(2006),
  startDAQTime_(0),
  endRunYear_(2006),
  endDAQTime_(0)
{
  AddParameter("StartRunYear", "Year of start of run period", startRunYear_);
  AddParameter("StartDAQTime", "DAQTime of start of run period in 1/10 of ns", startDAQTime_);
  AddParameter("EndRunYear", "Year of end of run period", endRunYear_);
  AddParameter("EndDAQTime", "DAQTime of end of run period in 1/10 of ns", endDAQTime_);

  AddOutBox("OutBox");
}

/**
 *Destructor!
 */
I3MCTimeGenerator::~I3MCTimeGenerator()
{
}

/**
 *Configures I3MCTimeGenerator
 */
void I3MCTimeGenerator::Configure()
{
  log_info("Configuring the I3MCTimeGenerator");
  
  GetParameter("StartRunYear", startRunYear_);
  GetParameter("StartDAQTime", startDAQTime_);
  GetParameter("EndRunYear", endRunYear_);
  GetParameter("EndDAQTime", endDAQTime_);

}

void I3MCTimeGenerator::Physics(I3FramePtr frame)
{
  log_debug("Entering I3MCTimeGenerator::Physics()");

  I3RandomService& random = GetService<I3RandomService>();
  //I3RandomServicePtr random = context_.Get<I3RandomServicePtr>();

  double startRunYear = static_cast<double>(startRunYear_);
  double endRunYear = static_cast<double>(endRunYear_);

  I3EventHeaderPtr eventHeader(new I3EventHeader);

  //Adding 0.5 effectively rounds
  int eventYear = static_cast<int>(random.Uniform(startRunYear,endRunYear) + 0.5); 

  double startDAQTime;
  double endDAQTime;

  if(eventYear == startRunYear_){
    startDAQTime = static_cast<double>(startDAQTime_);
    endDAQTime = static_cast<double>(MAX_DAQTIME);
  }
  else if(eventYear == endRunYear_){
    startDAQTime = 0.;
    endDAQTime = static_cast<double>(endDAQTime_);
  }

  int64_t startTime = static_cast<int64_t>(random.Uniform(startDAQTime,endDAQTime) + 0.5);

  I3Time eventStartTime(eventYear,startTime);
  eventHeader->SetStartTime(eventStartTime);

  I3Time eventEndTime;
  endDAQTime += MAX_EVENT_TIME;
  //have to account for that random New Year's Eve event
  if(endDAQTime>MAX_DAQTIME){
    //HAPPY NEW YEAR!!!!  Should auld acquaintance be forgot...
    int64_t endTime = (static_cast<int64_t>(endDAQTime + 0.5)) - MAX_DAQTIME;
    eventEndTime.SetDaqTime(eventYear+1,endTime);
  }else{
    int64_t endTime = (static_cast<int64_t>(endDAQTime + 0.5)) + MAX_EVENT_TIME;
    eventEndTime.SetDaqTime(eventYear,endTime);
  }

  eventHeader->SetEndTime(eventEndTime);

  frame->Put("EventHeader",eventHeader);

  PushFrame(frame,"OutBox");
  log_debug("Leaving I3MCTimeGenerator");
} 
