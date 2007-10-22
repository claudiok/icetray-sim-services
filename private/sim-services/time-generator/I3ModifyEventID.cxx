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

#include "sim-services/time-generator/I3ModifyEventID.h"
#include "dataclasses/physics/I3EventHeader.h"

#include "icetray/I3TrayHeaders.h"
#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

I3_MODULE(I3ModifyEventID);

I3ModifyEventID::I3ModifyEventID(const I3Context& ctx) : 
  I3Module(ctx),
  startEID_(0)
{ 
  log_debug("Constructor I3ModifyEventID");
  AddParameter("StartEventID","Starting Event ID",startEID_);
  AddOutBox("OutBox");
}

I3ModifyEventID::~I3ModifyEventID(){}

void I3ModifyEventID::Configure()
{
  log_debug("Configuring I3ModifyEventID");
  GetParameter("StartEventID", startEID_);
}

void I3ModifyEventID::Physics(I3FramePtr frame)
{
  log_debug("Physics");

  const I3EventHeader& oldHeader = frame->Get<I3EventHeader>();
  I3EventHeaderPtr ehPtr(new I3EventHeader(oldHeader));
  ehPtr->SetEventID(startEID_++);

  frame->Delete("I3EventHeader");
  frame->Put(ehPtr);
  PushFrame(frame,"OutBox");
}//Physics()
 
