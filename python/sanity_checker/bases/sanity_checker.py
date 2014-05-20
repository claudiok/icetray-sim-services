class SanityChecker :
    def __init__(self):
        self.registry = list()

    # returns True if all is well
    def check( self ):

        all_is_well = True
        for c in self.registry :
            all_is_well = all_is_well and c.check()
        return all_is_well

    # what to do if there's a failure
    def fail( self ) :
        for c in self.registry :
            if not c.check() : print("FAIL : ",c.failure_msg)                

    # setup the tests
    def setup_test( self, frame ) :
        pass

    def pprint(self) :
        lidx = str(self).find('icecube.sim_services') + len('')
        ridx = str(self).find(' instance at')
        return str(self)[lidx:ridx]
