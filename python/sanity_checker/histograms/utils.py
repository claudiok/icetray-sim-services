import numpy

def bins(nbins, min, max) :
    db = float(max-min)/float(nbins)
    return numpy.arange(min,max,db)
