#ifndef ADSR_ENVELOPE_H
#define ADSR_ENVELOPE_H

class ADSREnvelope {
public:
    ADSREnvelope(float attackTime, float decayTime, float sustainLevel, float releaseTime);
    float process(bool noteOn, float sampleRate);
    void reset();
    
private:
    enum class Stage { Attack, Decay, Sustain, Release, Idle };
    Stage currentStage;
    
    float attackTime, decayTime, sustainLevel, releaseTime;
    float currentLevel;
    float currentSample;
};

#endif // ADSR_ENVELOPE_H