/**
 * Definition of I3MCRawDOMStatusFiller class
 *
 * copyright  (C) 2004
 * the IceCube collaboration
 * $Id: I3MCRawDOMStatusFiller.h 6917 2005-04-26 02:56:37Z tmccauley $
 * 
 * @file I3MCRawDOMStatusFiller.h
 * @version $Revision: 1.14 $
 * @date $Date: 2005-04-25 22:56:37 -0400 (Mon , 25 Apr 2005) $
 * @author pretz
 * @author tmccauley
 */

#ifndef I3MCRAWDOMSTATUSSERVICE_H
#define I3MCRAWDOMSTATUSSERVICE_H

#include "phys-services/I3DetectorStatusService.h"
#include "phys-services/I3GeometryService.h"
#include "dataclasses/physics/I3Trigger.h"
#include "dataclasses/status/I3TriggerStatus.h"

/**
 *Default vaules for Detector Status
 */
const int NBINS_FADC = 50;

class I3Geometry;
I3_POINTER_TYPEDEFS(I3Geometry);
/**
 *
 * @brief This module allow you to fill the RawDOMStatus data 
 *	contained on the DetectorStatus stream by hand. 
 *	The information gets cloned for all of the IceCube
 * 	DOMs contained in the geometry.
 */
class I3MCRawDOMStatusService : public I3DetectorStatusService
{
public:

  I3MCRawDOMStatusService(I3GeometryServicePtr g) :
    nBinsFADC_(NBINS_FADC)
    {
      geo_service_ = g;
    }

  virtual I3DetectorStatusConstPtr GetDetectorStatus(I3Time time);
  void InsertTriggerStatus(I3Trigger trig, I3TriggerStatus trigstatus);

  virtual ~I3MCRawDOMStatusService(){};

  SET_LOGGER("I3MCRawDOMStatusService");

  void SetNBinsFADC(int n){ nBinsFADC_ = n; };

 private:
  I3MCRawDOMStatusService();
  I3GeometryServicePtr geo_service_;

  shared_ptr<I3DetectorStatus> status_;

  /**
   *Configuration parameters for detector status
   */

  /**
   *Number of bins to ude for the FADC
   */
  int nBinsFADC_;
};

#endif
