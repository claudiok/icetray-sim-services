class SanityChecker :
    """
    Superclass for all sanity checkers and its primary function
    is to manage 'Counters'.
    """
    def __init__(self):
        """
        Creates an empty list of 'Counters'. This is public,
        so users should feel free to append counters directly to it.

        Members :
        registry - List of counters.
        _setup_test - A dictionary of functions that must be run
        before a test to prepare data for that test.
        """
        self.registry = list()

    def check( self ):
        """
        Runs all of the registered counters 'check' methods and
        returns the logical and.
        """
        all_is_well = True
        for c in self.registry :
            all_is_well = all_is_well and c.check()
        return all_is_well

    def fail( self ):
        """
        When there's a failure in one of the counters,
        print its failure message.
        """
        for c in self.registry :
            if not c.check() :
                print("FAIL : %s" % c.failure_msg)

    def pprint(self) :
        """
        Pretty print.  Take this class's str and strip off stuff we're
        not interested in, like the leading 'icecube.sim_services' (that
        much should be obvious) and the trailing address of the instance.
        """
        lidx = str(self).find('icecube.sim_services') + len('')
        ridx = str(self).find(' instance at')
        return str(self)[lidx:ridx]
