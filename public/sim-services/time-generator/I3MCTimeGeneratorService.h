#ifndef I3MCTIMEGENERATORSERVICE_H
#define I3MCTIMEGENERATORSERVICE_H

/**
 *
 *@file
 *@brief Definition of I3MCTimeGeneratorService - A hit writing module
 *
 * Version $Id: I3MCTimeGeneratorService.h 16564 2006-03-01 03:02:50Z olivas $
 * $Revision: 2.0 $
 * $Author: Gustav Wikstrom$
 * $Date: 2006-02-28 22:02:50 -0500 (Tue, 28 Feb 2006) $
 *
 * (c) 2005 IceCube Collaboration
 */

#include "phys-services/I3EventService.h"
#include "icetray/I3Tray.h"

class I3Context;
class I3Frame;
class I3EventHeader;

/**
 * I3MCTimeGeneratorService:  
 * 
 * Sets the time of the simulated event
 */

class I3MCTimeGeneratorService : public I3EventService
{
 public:

  SET_LOGGER("I3MCTimeGeneratorService");

  I3MCTimeGeneratorService(int, int64_t, unsigned);
  virtual bool MoreEvents();
  virtual I3Time PopEvent(I3Frame&);
  virtual ~I3MCTimeGeneratorService() { };
 private:
  
  ///Start time of run period
  int year_;
  int64_t daqTime_;
  int mjd_;
  unsigned runNumber_;
};

#endif
