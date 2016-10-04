#include <sim-services/I3CrossSection.h>
#include <fstream>

namespace{
	bool lowerEnergy(const std::pair<double,double>& r1, double en){
		return(r1.first<en);
	}

	double particleMass(I3Particle::ParticleType type){
		I3Particle p(I3Particle::Null,type);
		if(!p.HasMass()){
			log_debug_stream("Treating particle of type " << p.GetTypeString()
			                 << " with unknown mass as massless");
			return(0);
		}
		return(p.GetMass());
	}
	
	///Compute the allowed range of y values for an interaction of a massless particle 
	///interacting with a nucleon with a given Feynman x
	///\param E the energy of the incoming massless particle
	///\param m the mass of the lepton produced in the interaction
	///\param x the Feynman x of the interaction
	///\return a pair containing the minimum and maximum possible Feyman y for the interaction
	std::pair<double,double> allowedY(const double E, const double m, const double x){
		const double M = 9.39E-1;
		double A=2*M*E*x + M*M*x*x;
		double B=(M*m*m*x)/E + m*m - 2*M*E*x;
		double C=(m*m*m*m)/(4*E);
		double d=sqrt(B*B-4*A*C);
		double y1=(B-d)/(2*A);
		double y2=(B+d)/(2*A);
		//TODO: check preceding math for dropped minus sign
		return(std::make_pair(-y2,-y1));
	}
}

I3CrossSection::finalStateRecord 
I3CrossSection::sampleFinalState(double energy, 
                               I3Particle::ParticleType scatteredType, 
                               boost::shared_ptr<I3RandomService> random) const{
	double m=particleMass(scatteredType);
	double yMax=1-m/energy;
	double logYMax=log10(yMax);

	bool accept;
	std::vector<double> x(3), xp(3);
	std::vector<int> cx(3), cxp(3);
	double px, pxp, propx, propxp;

	x[0] = xp[0] = log10(energy);

	//check preconditions
	if(x[0]<crossSection.extents[0][0]
	   || x[0]>crossSection.extents[0][1])
	log_fatal_stream("Interaction energy out of cross section table range: ["
	                 << pow(10.,crossSection.extents[0][0]) << " GeV,"
	                 << pow(10.,crossSection.extents[0][1]) << " GeV]");
	
	//sample an intial point
	do{
		//TODO: set limits more carefully on log x and y
		//	valid x range is always [0,1]?
		//	cross secton may not be avilable over same range at all energies?
		x[1]=random->Uniform(-5,0);
		std::pair<double,double> yrange=allowedY(energy,m,pow(10.,x[1]));
		//TODO: remove hard coded -5, needed to avoid pathological behavior of test spline
		yrange.first=std::max(log10(yrange.first),-5./*crossSection.extents[2][0]*/);
		yrange.second=std::min(log10(yrange.second),crossSection.extents[2][1]);
		//log_info_stream("  lyrange: [" <<yrange.first << ',' << yrange.second << "]");
		x[2]=random->Uniform(yrange.first,yrange.second);
		//log_info_stream("  ly=" << x[2]);

		accept=true;
		//demand that the sampled point be within the table extents
		if(x[1]<crossSection.extents[1][0]
		   || x[1]>crossSection.extents[1][1])
			accept=false;
		if(x[2]<crossSection.extents[2][0]
		   || x[2]>crossSection.extents[2][1])
			accept=false;

		if(accept)
			accept=!tablesearchcenters(&crossSection,x.data(),cx.data());
	} while(!accept);

	propx=1; //TODO: better proposal distribution?
	double measure=pow(10.,x[1]+x[2]);
	px=measure*pow(10.,ndsplineeval(&crossSection,x.data(),cx.data(),0));

	const size_t burnin=200;
	for(size_t j=0; j<=burnin; j++){
		//TODO: same sampling issues as above
		xp[1]=random->Uniform(-5,0);
		std::pair<double,double> yrange=allowedY(energy,m,pow(10.,xp[1]));
		//TODO: remove hard coded -5, needed to avoid pathological behavior of test spline
		yrange.first=std::max(log10(yrange.first),-5./*crossSection.extents[2][0]*/);
		yrange.second=std::min(log10(yrange.second),crossSection.extents[2][1]);
		//log_info_stream("  lyrange: [" <<yrange.first << ',' << yrange.second << "]");
		xp[2]=random->Uniform(yrange.first,yrange.second);
		//log_info_stream("  ly=" << x[2]);

		accept=true;
		//demand that the sampled point be within the table extents
		if(x[1]<crossSection.extents[1][0]
		   || x[1]>crossSection.extents[1][1])
			accept=false;
		if(x[2]<crossSection.extents[2][0]
		   || x[2]>crossSection.extents[2][1])
			accept=false;

		if(accept)
			accept=!tablesearchcenters(&crossSection,xp.data(),cxp.data());
		if(!accept)
			continue;
		propxp=1;
		double measure=pow(10.,xp[1]+xp[2]);
		double eval=ndsplineeval(&crossSection,xp.data(),cxp.data(),0);
		if(std::isnan(eval))
			continue;
		pxp=measure*pow(10.,eval);

		double odds=(pxp/px)*(propx/propxp);
		accept=((odds>1.) || random->Uniform(0,1)<odds);

		if(accept){
			x=xp;
			propx=propxp;
			px=pxp;
		}
	}
	//record whatever we sampled
	//log_info_stream(" lx=" << x[1] << " ly=" << x[2]);
	return(finalStateRecord(pow(10.,x[1]),pow(10.,x[2])));
}

double I3CrossSection::evaluateTotalCrossSection(double energy) const{
	std::vector<std::pair<double,double> >::const_iterator it=
	std::lower_bound(total_crossSection.begin(),total_crossSection.end(),energy,lowerEnergy);
	if(it==total_crossSection.begin() || it==total_crossSection.end())
		log_fatal_stream("Energy out of range of available total cross section: "
		                 << energy/I3Units::GeV << " GeV");
	double c1=(it-1)->second;
	double c2=it->second;
	double x=(energy-(it-1)->first)/(it->first-(it-1)->first);
	return((1-x)*c1 + x*c2);
}

void I3CrossSection::load(std::string dd_crossSectionFile, std::string total_crossSectionFile){
	int status=readsplinefitstable(dd_crossSectionFile.c_str(),&crossSection);
	if(status!=0)
		log_fatal_stream("Failed to read cross section data from spline fits file '"
		                 << dd_crossSectionFile << "': error code " << status);
	if(crossSection.ndim!=3)
		log_fatal_stream("cross section spline has " << crossSection.ndim
		                 << " dimensions, should have 3 (log10(E), log10(x), log10(y))");
	
	std::ifstream totalFile(total_crossSectionFile.c_str());
	if(!totalFile)
		log_fatal_stream("Failed to open total cross section data file '"
		                 << total_crossSectionFile << '\'');
	double e, c;
	while(totalFile >> e >> c)
		total_crossSection.push_back(std::make_pair(e,c));
	if(!totalFile.eof() && totalFile.fail())
		log_fatal_stream("Input failure from total cross section data file '"
		                 << total_crossSectionFile << '\'');
}

