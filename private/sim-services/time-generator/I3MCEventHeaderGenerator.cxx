/**
 *
 * $Id$
 *
 * Puts Event headers in empty frames for use in simulation
 *
 */

#include <icetray/I3Frame.h>
#include <icetray/I3Tray.h>
#include <icetray/I3Module.h>
#include <dataclasses/physics/I3EventHeader.h>
#include <dataclasses/I3Time.h>
#include <dataclasses/I3Bool.h>

class I3MCEventHeaderGenerator : public I3Module
{
 public:

  I3MCEventHeaderGenerator(const I3Context&);
  void Configure();
  void DAQ(I3FramePtr fr);	

  SET_LOGGER("I3MCEventHeaderGenerator");

 private:

  //Start time of run period
  int year_;
  int64_t daqTime_;
  std::string eventServiceName_;
  int mjd_;
  int mjd_s_;
  double mjd_ns_;
  unsigned runNumber_;
  unsigned eventID_;

  /**
   * If true this service will increment the eventID after adding
   * one to the frame.  This is useful for looking at untriggered events
   * where you still want unique event IDs.
   */
  bool incEventID_;

};

I3_MODULE(I3MCEventHeaderGenerator);

const int DEFAULT_YEAR = 2006;
const int64_t DEFAULT_DAQTIME = 0;

const int DEFAULT_MJD_SECONDS(0);
const double DEFAULT_MJD_NANOSECONDS(0.);

I3MCEventHeaderGenerator::I3MCEventHeaderGenerator(const I3Context& ctx) : I3Module(ctx),
  year_(DEFAULT_YEAR),
  daqTime_(DEFAULT_DAQTIME),
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
	AddParameter("RunNumber", "Run Number", runNumber_);
	AddParameter("EventID", "Event ID", eventID_);
	AddParameter("IncrementEventID", "Increment Event ID (default =  false)", incEventID_);

	AddOutBox("OutBox");
}

void I3MCEventHeaderGenerator::Configure()
{
	GetParameter("Year", year_);
	GetParameter("DAQTime", daqTime_);
	GetParameter("MJD",mjd_);
	GetParameter("MJDSeconds",mjd_s_);
	GetParameter("MJDNanoSeconds",mjd_ns_);
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

void I3MCEventHeaderGenerator::DAQ(I3FramePtr fr) 
{
      I3Time evtTime(year_, daqTime_);

      I3EventHeaderPtr eventHeader_(new I3EventHeader);
      eventHeader_->SetStartTime(evtTime);
      eventHeader_->SetRunID(runNumber_);
      eventHeader_->SetSubRunID(0);
      eventHeader_->SetEventID(eventID_);
      fr->Put<I3EventHeader>(eventHeader_);

      if(incEventID_) eventID_++;

      I3BoolPtr incID(new I3Bool(incEventID_));
      fr->Put("MCTimeIncEventID",incID);

      PushFrame(fr);

}

