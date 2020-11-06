/* 
 *  Simple script to drive the arduino nano's built in LED with a sinusoidally changing PWM signal.
 */

#define pi 3.1415926535897932384626433832795 //Define Pi

float triangle(float t) {
    //Function to generate a triangular wave, oscillating between 0 and 1 with a period of 2 pi.
    t = fmod(t+2*pi, 2*pi);
    if (t < pi)
        return t/pi;
    else
        return (1 - (t-pi)/pi);
}

float squareWave(float t) {
    //Generate a square wave oscillating between 0 and 1 with a period of 2 pi
    t = fmod(t+2*pi, 2*pi);
    if (t < pi)
        return 0;
    else
        return 1;
}



float waveGen(float t, int period, float phaseShift, int waveSelect) {
    //A simple wave generating function that calculates a given wave's amplitude at time t with a given period. 
    //The return is bounded between 0 and 1 for use in PWM systems.
    switch(waveSelect) {
        case 0: //Arbitrary wave for experiments
            return squareWave(t * 2 * pi / period + phaseShift);
            
        case 1: //Triangular wave is chosen, using previously defined function
            return triangle(t*2*pi / period + phaseShift);
            
        case 2: //Sin wave is chosen, shifted up one and halved
            return (sin(t*2*pi / period + phaseShift) + 1) / 2;
       
        case 3: //Sin^2 wave is chosen, period is doubled and phaseshift is halved to account for the duplication in sin^2
            return pow(sin(t*2*pi / (period*2) + phaseShift/2), 2);
        
        case 4: //Sin^3 wave is chosen, shifted up one and halved
            return (pow(sin(t*2*pi / period + phaseShift), 3) + 1) / 2;

        case 5: //sin^60 is chosen, modified as case 3
            return pow(sin(t*2*pi / (period*2) + phaseShift/2), 60);
    }
}



int main() {
    init();

    int numberOfPins = 6;
    int pinsToUse[numberOfPins] = {3, 5, 6, 9, 10, 11};
    // {9, 10, 11, 3, 5, 6};
    // {9, 10, 11, 3, 5, 6};
    
    int wavePeriod = 2500;        //Define period for the sinusoid to refresh at (in ms)
    unsigned long long int t;
    float pwmIntensity[numberOfPins];
    float phaseShift[numberOfPins];
    int dutyCycleValue[numberOfPins];

    
    for (int i = 0; i < numberOfPins; i++) {
        phaseShift[i] = -i* (2*pi/numberOfPins);
    }

    while (1) {
        t = millis();
        for (int i = 0; i < numberOfPins; i++) 
        {
            pwmIntensity[i] = waveGen(t, wavePeriod, phaseShift[i], 2);
            dutyCycleValue[i] = floor(pwmIntensity[i] * 255);
            analogWrite(pinsToUse[i], dutyCycleValue[i]);
        }
    }
}
