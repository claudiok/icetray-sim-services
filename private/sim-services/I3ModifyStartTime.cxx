/**
 * class: I3ModifyEventID.cxx
 * (c) 2004 IceCube Collaboration
 * Version $Id: I3ModifyEventID.cxx,v 1.16 2005/03/17 18:33:43 olivas Exp $
 *
 * Date 08 June 2006
 * @version $Revision: 1.1 $
 * @date $Date: 2005/03/17 18:33:43 $
 * @author Alex Olivas <olivas@icecube.umd.edu>
 *
 */
#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Tray.h"
#include "icetray/I3Module.h"
#include "dataclasses/I3Time.h"
#include "dataclasses/physics/I3EventHeader.h"

/**
 *@class I3ModifyStartTime
 *@brief Modifies the StartTime in the frame's I3EventHeader.
 */
class I3ModifyStartTime : public I3Module {
 public:

  I3ModifyStartTime(const I3Context& ctx);
  ~I3ModifyStartTime();

  void Configure();
  void DAQ(I3FramePtr);

 private:
  I3ModifyStartTime();
  I3ModifyStartTime(const I3ModifyStartTime&);
  I3ModifyStartTime& operator=(const I3ModifyStartTime&);

  /// Start time of run period
  I3Time start_time_;

  SET_LOGGER("I3ModifyStartTime");

};

I3_MODULE(I3ModifyStartTime);

I3ModifyStartTime::I3ModifyStartTime(const I3Context& ctx) :
  I3Module(ctx)
{
  AddParameter("StartTime", "I3Time : New start time of the event", start_time_);
  AddOutBox("OutBox");
}

I3ModifyStartTime::~I3ModifyStartTime(){}

void I3ModifyStartTime::Configure()
{
  GetParameter("StartTime", start_time_);
  if(start_time_ == I3Time()){
    log_fatal("A new start time must be set.");
  }
}

void I3ModifyStartTime::DAQ(I3FramePtr frame)
{
  log_debug("DAQ");

  if (frame->Has(I3DefaultName<I3EventHeader>::value()) ) {
    const I3EventHeader& oldHeader = frame->Get<I3EventHeader>();
    I3EventHeaderPtr event_header(new I3EventHeader(oldHeader));

    event_header->SetStartTime(start_time_);

    frame->Delete("I3EventHeader");
    frame->Put(event_header);
  } else {
    log_debug("No I3EventHeader to modify.");
  }

  PushFrame(frame,"OutBox");
}
