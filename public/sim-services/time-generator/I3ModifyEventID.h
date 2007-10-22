#ifndef I3MODIFYEVENTID_H
#define I3MODIFYEVENTID_H
/**
 * class: I3ModifyEventID
 *
 * Version $Id: I3ModifyEventID.h,v 1.11 2005/03/03 00:43:41 olivas Exp $
 *
 * date:  2 July 2004
 *
 * (c) 2004 IceCube Collaboration
 */

#include "icetray/I3Module.h"
#include "dataclasses/I3Position.h"
#include "phys-services/I3RandomService.h"

/** 
 *@class I3ModifyEventID I3ModifyEventID.h 
 */

class I3ModifyEventID : public I3Module
{
 public:

  I3ModifyEventID(const I3Context& ctx);
  ~I3ModifyEventID();
  
  void Configure();
  void Physics(I3FramePtr frame);

 private:
  I3ModifyEventID();
  I3ModifyEventID(const I3ModifyEventID&);
  I3ModifyEventID& operator=(const I3ModifyEventID&);

  unsigned startEID_;

  SET_LOGGER("I3ModifyEventID");

};

#endif //I3MODIFYEVENTID_H
