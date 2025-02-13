#ifndef IEFFECT_H
#define IEFFECT_H



#include <utility> // For std::pair

class IEffect 

{
public:

    virtual ~IEffect() = default;

    // Process a stereo audio sample (left and right channels)
    virtual std::pair<float, float> process(std::pair<float, float> stereoSample) = 0;

    // Optional reset function for stateful effects (e.g., reverb, delay)
    virtual void reset() {}
    
};


#endif /* IEFFECT_H */