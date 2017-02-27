#ifndef I3CROSSSECTION_H
#define I3CROSSSECTION_H

#include <photospline/bspline.h>
#include <phys-services/I3RandomService.h>
#include <dataclasses/physics/I3Particle.h>

class I3CrossSection{
public:
	///A single final state drawn from a differential cross section
	struct finalStateRecord{
		///Feynman x
		double x;
		///Feynman y
		double y;

		///Constructor
		finalStateRecord(double x, double y):
		x(x),y(y){}
	};

	///Sample a final state
	///\param energy the energy of the incoming neutrino
	///\param scatteredType the type of the outgoing lepton
	///\param random a source of random numbers
	finalStateRecord sampleFinalState(double energy,
	                                  I3Particle::ParticleType scatteredType,
	                                  boost::shared_ptr<I3RandomService> random) const;
	
	///\param energy the neutrino energy in GeV
	///\return the cross section in in squae meters
	double evaluateTotalCrossSection(double energy) const;

	const splinetable& getCrossSection() const{ return(crossSection); }

	I3CrossSection(){
		memset(&crossSection,0,sizeof(splinetable));
		memset(&totalCrossSection,0,sizeof(splinetable));
	}

	///\param dd_crossSectionFile path to the doubly-differential cross section spline to load
	///\param total_crossSectionFile path to the total cross section file to load
	I3CrossSection(std::string dd_crossSectionFile, std::string total_crossSectionFile){
		memset(&crossSection,0,sizeof(splinetable));
		memset(&totalCrossSection,0,sizeof(splinetable));
		load(dd_crossSectionFile, total_crossSectionFile);
	}

	~I3CrossSection(){
		splinetable_free(&crossSection);
		splinetable_free(&totalCrossSection);
	}

	///\param dd_crossSectionFile path to the doubly-differential cross section spline to load
	///\param total_crossSectionFile path to the total cross section file to load
	void load(std::string dd_crossSectionFile, std::string total_crossSectionFile);

private:
	splinetable crossSection;
	splinetable totalCrossSection;
};

#endif //I3CROSSSECTION_H
