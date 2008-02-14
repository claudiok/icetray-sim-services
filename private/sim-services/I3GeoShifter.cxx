/**
 * copyright  (C) 2004
 * the icecube collaboration
 * $Id: I3GeoShifter.cxx 2598 2005-02-04 17:18:56Z olivas $
 *
 * @file I3GeoShifter.cxx
 * @version $Revision: 1.8 $
 * @date $Date: 2005-02-04 18:18:56 +0100 (Fri, 04 Feb 2005) $
 * @author olivas
 *
 * This namespace contains a collection of utilities useful
 * for shifting particles to the detector center.
*/

// headers for this selection
#include "sim-services/I3GeoShifter.h"
#include "icetray/I3Frame.h"
#include <numeric> //for accumulate

using namespace std;

namespace I3GeoShifter{

  std::pair<double,double> 
  DetectorCenter(I3FramePtr frame,
		  I3OMGeo::OMType omType){

    const I3Geometry geo = frame->Get<I3Geometry>();
    vector<double> x;
    vector<double> y;
    I3OMGeoMap::const_iterator  i;
    int current_str(INT_MIN);
    for(i=geo.omgeo.begin(); i!=geo.omgeo.end(); i++){
      if(i->first.GetString() != current_str){
	current_str = i->first.GetString();
	//all strings are weighted equally
	//not based on the number of DOMs
	//one vote per string
	x.push_back(i->second.position.GetX());
	y.push_back(i->second.position.GetY());
      }
    }

    double x_avg(accumulate(x.begin(),x.end(),0.));
    x_avg /= static_cast<double>(x.size());

    double y_avg(accumulate(y.begin(),y.end(),0.));
    y_avg /= static_cast<double>(y.size());

    std::pair<double,double> r(x_avg,y_avg);
    return r;
  }

  void ShiftTree(const std::pair<double,double>& xy, 
		 I3MCTreePtr mctree){
    I3MCTree::iterator t_iter;
    for(t_iter = mctree->begin();
	t_iter != mctree->end(); t_iter++){
      double x(t_iter->GetX() + xy.first);
      double y(t_iter->GetY() + xy.second);
      double z(t_iter->GetZ());
      t_iter->SetPos(x,y,z);
    }
  }

  void ShiftTree(I3FramePtr frame,
		 I3MCTreePtr mctree,
		 I3OMGeo::OMType omType){
    pair<double,double> xy = DetectorCenter(frame, omType);
    ShiftTree(xy,mctree);
  }


}//end namespace I3GeoShifter

