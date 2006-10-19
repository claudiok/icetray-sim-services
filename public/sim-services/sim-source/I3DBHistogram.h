#ifndef I3DBHISTOGRAM_H
#define I3DBHISTOGRAM_H
/**
 * class: I3DBHistogram
 *
 * Version $Id: I3DBHistogram.h,v 1.11 2005/03/03 00:43:41 olivas Exp $
 *
 * date:  2 July 2004
 *
 * (c) 2004 IceCube Collaboration
 */

#include "icetray/I3Module.h"
#include "dataclasses/I3Position.h"
#include "phys-services/I3RandomService.h"

/** 
 *@class I3DBHistogram I3DBHistogram.h 
 */

class I3DBHistogram : public I3Module
{
 public:

  I3DBHistogram(const I3Context& ctx);
  ~I3DBHistogram();
  
  void Configure();
  void Physics(I3FramePtr frame);

 private:
  I3DBHistogram();
  I3DBHistogram(const I3DBHistogram&);
  I3DBHistogram& operator=(const I3DBHistogram&);

  SET_LOGGER("I3DBHistogram");

};

#endif //I3DBHISTOGRAM_H
