class SanityChecker :
    def __init__(self):
        self.registry = list()

    # returns True if all is well
    def check( self ):

        all_is_well = True
        for c in self.registry :
            all_is_well = all_is_well and not c.fail()
        return all_is_well

    # what to do if there's a failure
    def fail( self ) :
        for c in self.registry :
            if c.fail() : print("FAIL : ",c.failure_msg)                

    # setup the tests
    def setup_test( self, frame ) :
        pass

