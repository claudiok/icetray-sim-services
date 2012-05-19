/**
 * copyright  (C) 2004
 * the IceCube collaboration
 *  $Id: I3GeoShifter.h 2601 2005-02-08 20:05:42Z olivas $
 *
 * @file I3GeoShifter.h
 * @version $Revision: 1.3 $
 * @date $Date: 2005-02-08 21:05:42 +0100 (Tue, 08 Feb 2005) $
 * @author olivas
 */

#ifndef I3GEOSHIFTER_H
#define I3GEOSHIFTER_H

/**
 * @brief Utilities for parsing the input parameters for I3GeometrySelector.
 */

#include "dataclasses/geometry/I3Geometry.h"
#include "dataclasses/physics/I3MCTree.h"

namespace I3GeoShifter{

  /**
   * Calculates the center of the detector in X-Y.
   * Returns a std::pair<double,double> where first
   * is x and second is y.
   * You can specify the detector by passing the corresponding
   * I3OMGeo::OMType.  The default is I3OMGeo::IceCube, but other
   * options include I3OMGeo::IceTop and I3OMGeo::AMANDA.
   * Currently you can only specify one and only one detector.
   */
  std::pair<double,double> 
    DetectorCenter(I3FramePtr frame, 
		    I3OMGeo::OMType omType = I3OMGeo::IceCube);

  /**
   * Shifts every particle position in the tree by the given value.
   * The 'first' element in the pair represents the shift in x.
   * The 'second' element in the pair represents the shift in y.
   * So for : 
   *   std::pair<doubl,double> shift(10.*I3Units::m, 20.*I3Units::m)
   *   I3GeoShifter::ShiftTree(shift,mctree);
   * The new particles will have positions:
   *   x_prime = x_0 + shift.first
   *   y_prime = y_0 + shift.second
   */
  void ShiftTree(const std::pair<double,double>&, I3MCTreePtr);


  /**
   * Provided for convenience for generators.  This shifts
   * all of the particles in the mctree to the center of the
   * 'standard' geometry in the frame.  You can specify the 
   * subdetector by passing the omType.  The default is I3OMGeo::IceCube, 
   * but other options include I3OMGeo::IceTop and I3OMGeo::AMANDA.
   * Currently you can only specify one and only one detector.
   */
  void ShiftTree(I3FramePtr frame, I3MCTreePtr, 
		 I3OMGeo::OMType omType = I3OMGeo::IceCube);
					   
}

#endif
