
class Checker :
    def __init__(self):
        self.checkers = list()
        self.generate_references = False

    # returns True if all is well
    def check( self ):

        all_is_well = True
        for c in self.checkers :
            all_is_well = all_is_well and c.is_within_tolerance()
        return all_is_well

    def register( self, c ):
        self.checkers.append( c )

    # what to do if there's a failure
    def fail( self ) :
        for c in self.checkers :
            if not c.is_within_tolerance() :
                print "FAIL : ",c.failure_msg

    def configure( self ):
        pass
 
    def finish( self ):
        pass

            

