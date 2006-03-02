#ifndef I3MCTIMEGENERATOR_H
#define I3MCTIMEGENERATOR_H

/**
 *
 *@file
 *@brief Definition of I3MCTimeGenerator - A hit writing module
 *
 * Version $Id: I3MCTimeGenerator.h 16564 2006-03-01 03:02:50Z olivas $
 * $Revision: 2.0 $
 * $Author: Gustav Wikstrom$
 * $Date: 2006-02-28 22:02:50 -0500 (Tue, 28 Feb 2006) $
 *
 * (c) 2005 IceCube Collaboration
 */

#include "icetray/I3Module.h"
#include "icetray/I3Tray.h"

class I3Context;
class I3Frame;
class I3EventHeader;

/**
 * I3MCTimeGenerator:  
 * 
 * Sets the time of the simulated event
 */

class I3MCTimeGenerator : public I3Module{
 public:

  SET_LOGGER("I3MCTimeGenerator");

  I3MCTimeGenerator(const I3Context& ctx);
  virtual ~I3MCTimeGenerator();
  void Configure();
  void Physics(I3FramePtr);

 private:
  
  I3MCTimeGenerator();
  I3MCTimeGenerator(const I3MCTimeGenerator&);
  I3MCTimeGenerator& operator=(const I3MCTimeGenerator&);

  ///Parameter to set time extension of frame
  int64_t timePad_;             

  ///Start time of run period
  int startRunYear_;
  int64_t startDAQTime_;

  ///End time of run period
  int endRunYear_;
  int64_t endDAQTime_;

};

#endif
