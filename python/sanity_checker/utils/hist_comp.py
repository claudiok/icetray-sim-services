from numpy import array
from numpy import shape
from scipy.stats import chisqprob

def _test_hist_consistency(h1, h2):
    if shape(h1.binedges) != shape(h2.binedges) \
       or shape(h1.bincenters) != shape(h2.bincenters) \
       or shape(h1.binwidths) != shape(h2.binwidths) \
       or shape(h1.xerr) != shape(h2.xerr) \
       or shape(h1.binerror) != shape(h2.binerror) \
       or shape(h1.bincontent) != shape(h2.bincontent) :
        print ("binedges : ", bool(shape(h1.binedges) != shape(h2.binedges)))
        print ("bincenters : ", bool(shape(h1.bincenters) != shape(h2.bincenters)))
        print ("binwidths : ", bool(shape(h1.binwidths) != shape(h2.binwidths)))
        print ("xerr : ", bool(shape(h1.xerr) != shape(h2.xerr)))
        print ("binerror : ", bool(shape(h1.binerror) != shape(h2.binerror)))
        print ("bincontent : ", bool(shape(h1.bincontent) != shape(h2.bincontent)))
        return False

    if h1.binedges.all() != h2.binedges.all() \
       or h1.bincenters.all() != h2.bincenters.all() \
       or h1.binwidths.all() != h2.binwidths.all() :
        print ("different binning")
        return False

    if len(h1.bincontent) != len(h2.bincontent) :
        print ("different lengths")
        return False

    return True
        
# Both of the following tests are from F.Porter
# "Testing Consistency of Two Histograms" arXiv : 0804.0380
# There are fancier tests, but these chi2 tests will get us started.
def _test_norm_chisq(h1, h2):
    # calculate the \chi^2 test statistic
    terms = [(u - v)**2/(u + v) \
             for u,v in zip(h1.hist.bincontent, h2.hist.bincontent)\
             if u > 0 and v > 0]
    chisq = sum(terms)
    return chisq, chisqprob(chisq, len(terms))

def _test_shape_chisq(h1, h2):
    # calculate the \chi^2 test statistic
    n1 = sum(h1.hist.bincontent)
    n2 = sum(h2.hist.bincontent)
    n1sq = n1**2
    n2sq = n2**2
    terms = [(u/n1 - v/n2)**2/(u/n1sq + v/n2sq) \
             for u,v in zip(h1.hist.bincontent, h2.hist.bincontent)\
             if u > 0 and v > 0]
    chisq = sum(terms)

    return chisq, chisqprob(chisq, len(terms) - 1)

def compare(hist1, hist2,
            test_norm = True, test_shape = True,
            norm_pval = 1e-3, shape_pval = 1e-3) :

    if not _test_hist_consistency(hist1.hist, hist2.hist) :
        print("histograms are inconsistent.")
        return False

    passed_norm = True
    if test_norm :
        T, pval = _test_norm_chisq(hist1, hist2)
        if pval < norm_pval :
            passed_norm = False

    passed_shape = True
    if test_shape :
        T, pval = _test_shape_chisq(hist1, hist2)
        if pval < shape_pval :
            passed_shape = False

    return (passed_norm, passed_shape)
    
    
