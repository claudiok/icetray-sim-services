#ifndef I3DBTESTMODULE_H
#define I3DBTESTMODULE_H
/**
 * class: I3DBTestModule
 *
 * Version $Id: I3DBTestModule.h,v 1.11 2005/03/03 00:43:41 olivas Exp $
 *
 * date:  2 July 2004
 *
 * (c) 2004 IceCube Collaboration
 */

#include "icetray/I3Module.h"
#include "dataclasses/I3Position.h"
#include "phys-services/I3RandomService.h"

/** 
 *@class I3DBTestModule I3DBTestModule.h 
 */

class I3DBTestModule : public I3Module
{
 public:

  I3DBTestModule(const I3Context& ctx);
  ~I3DBTestModule();
  
  void Configure();
  void DAQ(I3FramePtr frame);

 private:
  I3DBTestModule();
  I3DBTestModule(const I3DBTestModule&);
  I3DBTestModule& operator=(const I3DBTestModule&);

  SET_LOGGER("I3DBTestModule");

};

#endif //I3DBHISTOGRAM_H
