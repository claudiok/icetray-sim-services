class Test :
    """
    Simple data aggregate to store the test, its setup, and the
    module it's testing.
    Arguments :
      o name - String name of the test.
      o module - The sanity checker module or histogram that's being tested.
      o test - A function that takes the module as an argument and returns a bool.
         (True = pass, False = fail)
      o frame_setup - A function that take a frame as an argument and loads it with
         whatever's needed to perform the test.
    """
    def __init__(self, name, module, test, frame_setup):
        self.name = name
        self.module = module
        self.test = test
        self.frame_setup = frame_setup
        
class TestFailure(Exception) :
    """
    Custom exception to raise when tests fail.
    """
    def __init__(self, message) :
        super(TestFailure, self).__init__(message) 
           
class _TestRegistry :
    """
    A simple container class that manages a list of tests
    """
    def __init__(self) :
        """
        Creates an empty list to store the tests.
        """
        self.registry = list()

    def register_test(self, test) :
        """
        Appends the 'test' to the registry.
        """
        self.registry.append(test)

test_registry = _TestRegistry()

        
    
