from math import sqrt

def _norm_with_errors( h ):
    norm = sum( h.bin_values )
    if norm > 0 :
        bv = [ float(b)/float(norm) for b in h.bin_values ]
        bs = [ sqrt(b)/float(norm) for b in h.bin_values ]
        return (bv,bs)
    return (None, None)

class Histogram :    
    def __init__( self, rhist = None, rchisq_tolerance = 10,\
                  settings = None ):
    
        self.failure_msg  = "FAIL"
        
        if settings :
            self.xmin = settings["xmin"]
            self.xmax = settings["xmax"]
            self.binwidth = settings["binwidth"]

        if rhist :
            if settings and \
                   settings["xmin"] != rhist.xmin and \
                   settings["xmax"] != rhist.xmax and \
                   settings["binwidth"] != rhist.xbinwidth :
                print "WARNING : overriding xmin, xmax, and binwidth to match the reference histogram"
                
            self.xmin = rhist.xmin
            self.xmax = rhist.xmax
            self.binwidth = rhist.binwidth            

        self.overflow = 0
        self.underflow = 0
        
        self.nbins = int( (self.xmax - self.xmin)/self.binwidth )
        
        self.leading_edges = [ self.xmin + i * self.binwidth for i in range( self.nbins ) ]        
        self.bin_values = [ 0 for i in range( self.nbins ) ]
        
        self.rchisq_tolerance= rchisq_tolerance

        self.reference_hist = rhist

    def fill( self, v ):
        if v >= self.xmin and v < self.xmax :
            index = int( (v - self.xmin)/(self.xmin - self.xmax) * self.nbins )
            self.bin_values[ index ] += 1
        else:
            if v >= self.xmax : self.overflow += 1
            if v < self.xmin : self.underflow += 1

    def is_within_tolerance(self):
        # if there's no reference histogram
        # then i suppore you're within tolerance
        if not self.reference_hist : return True

        # only makes sense to compare normalized distributions
        rhist_values, rhist_errors = _norm_with_errors( self.reference_hist )
        hist_values, hist_errors = _norm_with_errors( self )

        if rhist_values and hist_values :
            chisq = sum([ sqrt(2.) * ( (b - bref)**2/(s**2 + sref**2) )  for b,s,bref,sref in \
                          zip( hist_values, hist_errors, rhist_values, rhist_errors )  \
                        if s > 0 or sref > 0 ] )
            ndof = len( [ 1 for b in self.bin_values if b > 0 ] ) - 1
            if ndof > 0 :
                chisq /= ndof

            return chisq < self.rchisq_tolerance
        return True

    def __str__( self ):
        ppstr = "xmin = %s" % str(self.xmin)
        ppstr += "xmax = %s" % str(self.xmax)
        ppstr += "binwidth = %s" % str(self.binwidth)
        ppstr += "overflow = %s" % str(self.overflow)
        ppstr += "underflow = %s" % str(self.underflow)
        ppstr += "nbins = %s" % str(self.nbins)
        ppstr += "leading_edges = %s" % str(self.leading_edges)
        ppstr += "bin_values = %s" % str(self.bin_values)
        ppstr += "rchisq_tolerance = %s" % str(self.rchisq_tolerance)
        return ppstr

    def __getstate__( self ):

        state = { "xmin" : self.xmin , \
              "xmax" : self.xmax , \
              "binwidth" : self.binwidth , \
              "overflow" : self.overflow , \
              "underflow" : self.underflow , \
              "nbins" : self.nbins , \
              "leading_edges" : self.leading_edges , \
              "bin_values" : self.bin_values, \
              "rchisq_tolerance" : self.rchisq_tolerance}

        return state
