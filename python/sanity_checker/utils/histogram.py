from math import sqrt
from copy import copy

def _norm_with_errors( h ):
    norm = sum( h.bin_values )
    if norm > 0 :
        bv = [ float(b)/float(norm) for b in h.bin_values ]
        bs = [ sqrt(b)/float(norm) for b in h.bin_values ]
        return (bv,bs)
    return (None, None)

def calc_rchisq( this_h, ref_h ):
    # only makes sense to compare normalized distributions
    this_h_values, this_h_errors = _norm_with_errors( this_h )
    ref_h_values, ref_h_errors = _norm_with_errors( ref_h )

    if this_h_values and ref_h_values :
        chisq = sum([ sqrt(2.) * ( (b - bref)**2/(s**2 + sref**2) )  for b,s,bref,sref in \
                      zip( this_h_values, this_h_errors, ref_h_values, ref_h_errors )  \
                      if s > 0 or sref > 0 ] )
        ndof = len( [ 1 for b in ref_h.bin_values if b > 0 ] )
        if ndof > 0 :
            return float(chisq)/float(ndof)
    return None

class Histogram :    
    def __init__( self, rhist = None , \
                  settings = None , \
                  min_entries = 1000 , \
                  fail_rchisq_tolerance = 10,\
                  warn_rchisq_tolerance = 2 ):
        if not rhist and not settings :
            print("Either you need to pass settings ( dict() with 'xmin', 'xmax','binwidth' ) "\
                  "or you need to pass a reference histogram to the constructor.")
            raise Exception
        
        if rhist :
            self.xmin = rhist.xmin
            self.xmax = rhist.xmax
            self.binwidth = rhist.binwidth
        else :
            self.xmin = settings["xmin"]
            self.xmax = settings["xmax"]
            self.binwidth = settings["binwidth"]

        self.overflow = 0        
        self.underflow = 0
        
        self.nbins = int( (self.xmax - self.xmin)/self.binwidth )
        
        self.leading_edges = [ self.xmin + i * self.binwidth for i in range( self.nbins ) ]        
        self.bin_values = [ 0 for i in range( self.nbins ) ]

        self.min_entries = min_entries
        self.warn_rchisq_tolerance = warn_rchisq_tolerance
        self.fail_rchisq_tolerance = fail_rchisq_tolerance
        self.reference_hist = rhist

        self.failure_msg  = "Oops \n"
        self.warn_msg = "WARN"

    def reset( self ):
        if self.reference_hist == None :
            rhist = copy( self )
            self.__init__( rhist = rhist )
        
    def fill( self, v, w = 1 ):
        if v >= self.xmin and v < self.xmax :
            index = int( float(v - self.xmin)/float(self.xmax - self.xmin) * self.nbins )
            self.bin_values[ index ] += w
        else:
            if v >= self.xmax : self.overflow += w
            if v < self.xmin : self.underflow += w

    def is_within_tolerance(self):
        # if there's no reference histogram
        # then i suppose you're within tolerance
        if not self.reference_hist : return True

        #if sum( self.bin_values ) < 0.1 * sum( self.reference_hist.bin_values ) : return True
        if sum( self.bin_values ) < self.min_entries : return True

#        if calc_rchisq( self, self.reference_hist ) > self.fail_rchisq_tolerance :
#            print "CHI^2 = ", calc_rchisq( self, self.reference_hist )
#            print str( self )

        return calc_rchisq( self, self.reference_hist ) < self.fail_rchisq_tolerance

    def warn_but_within_tolerance(self):
        # if there's no reference histogram
        # then i suppose you're within tolerance
        if not self.reference_hist : return True

        if sum( self.bin_values ) < self.min_entries : return True

        return calc_rchisq( self, self.reference_hist ) < self.warn_rchisq_tolerance

    def __str__( self ):
        ppstr = "xmin = %s \n" % str(self.xmin)
        ppstr += "xmax = %s \n" % str(self.xmax)
        ppstr += "binwidth = %s \n" % str(self.binwidth)
        ppstr += "overflow = %s \n" % str(self.overflow)
        ppstr += "underflow = %s \n" % str(self.underflow)
        ppstr += "nbins = %s \n" % str(self.nbins)
        ppstr += "leading_edges = %s \n" % str(self.leading_edges)
        ppstr += "bin_values = %s \n" % str(self.bin_values)
        ppstr += "min_entries = %s \n" % str(self.min_entries)
        ppstr += "fail_rchisq_tolerance = %s \n" % str(self.fail_rchisq_tolerance)
        ppstr += "warn_rchisq_tolerance = %s \n" % str(self.warn_rchisq_tolerance)
        if self.reference_hist :
            ppstr += "---REFEREENCE---\n %s \n" % str(self.reference_hist)
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
                  "min_entries" : self.min_entries, \
                  "fail_rchisq_tolerance" : self.fail_rchisq_tolerance, \
                  "warn_rchisq_tolerance" : self.warn_rchisq_tolerance,
                  "reference_hist" : self.reference_hist
                  }

        return state
