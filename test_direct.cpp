#include "src/AudioSequencer.h"
#include <iostream>

int main() {
    std::cout << "Testing direct playSequenceOnce() call..." << std::endl;
    
    AudioSequencer sequencer;
    
    // Test direct call (should now work)
    sequencer.playSequenceOnce("scale");
    
    std::cout << "Direct call test completed!" << std::endl;
    return 0;
}
