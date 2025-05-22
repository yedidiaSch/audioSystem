#ifndef NOTES_H
#define NOTES_H

#include <cmath>

// Frequency of the standard note A4 (440Hz)
const float A4_FREQUENCY = 440.0f;

// Function to calculate the frequency of a given note based on A4 as the reference
inline float getNoteFrequency(int semitoneOffset)  
{
    return A4_FREQUENCY * std::pow(2.0f, semitoneOffset / 12.0f);
}

// Define the note values using their semitone offsets relative to A4
// Notes defined as constants for compile-time evaluation
const int NOTE_C0 = -57;
const int NOTE_C_SHARP0 = -56;
const int NOTE_D0 = -55;
const int NOTE_D_SHARP0 = -54;
const int NOTE_E0 = -53;
const int NOTE_F0 = -52;
const int NOTE_F_SHARP0 = -51;
const int NOTE_G0 = -50;
const int NOTE_G_SHARP0 = -49;
const int NOTE_A0 = -48;
const int NOTE_A_SHARP0 = -47;
const int NOTE_B0 = -46;

const int NOTE_C1 = -45;
const int NOTE_C_SHARP1 = -44;
const int NOTE_D1 = -43;
const int NOTE_D_SHARP1 = -42;
const int NOTE_E1 = -41;
const int NOTE_F1 = -40;
const int NOTE_F_SHARP1 = -39;
const int NOTE_G1 = -38;
const int NOTE_G_SHARP1 = -37;
const int NOTE_A1 = -36;
const int NOTE_A_SHARP1 = -35;
const int NOTE_B1 = -34;

const int NOTE_C2 = -33;
const int NOTE_C_SHARP2 = -32;
const int NOTE_D2 = -31;
const int NOTE_D_SHARP2 = -30;
const int NOTE_E2 = -29;
const int NOTE_F2 = -28;
const int NOTE_F_SHARP2 = -27;
const int NOTE_G2 = -26;
const int NOTE_G_SHARP2 = -25;
const int NOTE_A2 = -24;
const int NOTE_A_SHARP2 = -23;
const int NOTE_B2 = -22;

const int NOTE_C3 = -21;
const int NOTE_C_SHARP3 = -20;
const int NOTE_D3 = -19;
const int NOTE_D_SHARP3 = -18;
const int NOTE_E3 = -17;
const int NOTE_F3 = -16;
const int NOTE_F_SHARP3 = -15;
const int NOTE_G3 = -14;
const int NOTE_G_SHARP3 = -13;
const int NOTE_A3 = -12;
const int NOTE_A_SHARP3 = -11;
const int NOTE_B3 = -10;

const int NOTE_C4 = -9;      // Middle C
const int NOTE_C_SHARP4 = -8;
const int NOTE_D4 = -7;
const int NOTE_D_SHARP4 = -6;
const int NOTE_E4 = -5;
const int NOTE_F4 = -4;
const int NOTE_F_SHARP4 = -3;
const int NOTE_G4 = -2;
const int NOTE_G_SHARP4 = -1;
const int NOTE_A4 = 0;       // Reference A4 (440Hz)
const int NOTE_A_SHARP4 = 1;
const int NOTE_B4 = 2;

const int NOTE_C5 = 3;
const int NOTE_C_SHARP5 = 4;
const int NOTE_D5 = 5;
const int NOTE_D_SHARP5 = 6;
const int NOTE_E5 = 7;
const int NOTE_F5 = 8;
const int NOTE_F_SHARP5 = 9;
const int NOTE_G5 = 10;
const int NOTE_G_SHARP5 = 11;
const int NOTE_A5 = 12;
const int NOTE_A_SHARP5 = 13;
const int NOTE_B5 = 14;

const int NOTE_C6 = 15;
const int NOTE_C_SHARP6 = 16;
const int NOTE_D6 = 17;
const int NOTE_D_SHARP6 = 18;
const int NOTE_E6 = 19;
const int NOTE_F6 = 20;
const int NOTE_F_SHARP6 = 21;
const int NOTE_G6 = 22;
const int NOTE_G_SHARP6 = 23;
const int NOTE_A6 = 24;
const int NOTE_A_SHARP6 = 25;
const int NOTE_B6 = 26;

const int NOTE_C7 = 27;
const int NOTE_C_SHARP7 = 28;
const int NOTE_D7 = 29;
const int NOTE_D_SHARP7 = 30;
const int NOTE_E7 = 31;
const int NOTE_F7 = 32;
const int NOTE_F_SHARP7 = 33;
const int NOTE_G7 = 34;
const int NOTE_G_SHARP7 = 35;
const int NOTE_A7 = 36;
const int NOTE_A_SHARP7 = 37;
const int NOTE_B7 = 38;

const int NOTE_C8 = 39;      // Highest C in 88-key piano
const int NOTE_C_SHARP8 = 40;
const int NOTE_D8 = 41;
const int NOTE_D_SHARP8 = 42;
const int NOTE_E8 = 43;
const int NOTE_F8 = 44;
const int NOTE_F_SHARP8 = 45;
const int NOTE_G8 = 46;
const int NOTE_G_SHARP8 = 47;
const int NOTE_A8 = 48;
const int NOTE_A_SHARP8 = 49;
const int NOTE_B8 = 50;

/// Frequencies for all notes from C0 to B8
const float C0_FREQ = getNoteFrequency(NOTE_C0);
const float C_SHARP0_FREQ = getNoteFrequency(NOTE_C_SHARP0);
const float D0_FREQ = getNoteFrequency(NOTE_D0);
const float D_SHARP0_FREQ = getNoteFrequency(NOTE_D_SHARP0);
const float E0_FREQ = getNoteFrequency(NOTE_E0);
const float F0_FREQ = getNoteFrequency(NOTE_F0);
const float F_SHARP0_FREQ = getNoteFrequency(NOTE_F_SHARP0);
const float G0_FREQ = getNoteFrequency(NOTE_G0);
const float G_SHARP0_FREQ = getNoteFrequency(NOTE_G_SHARP0);
const float A0_FREQ = getNoteFrequency(NOTE_A0);
const float A_SHARP0_FREQ = getNoteFrequency(NOTE_A_SHARP0);
const float B0_FREQ = getNoteFrequency(NOTE_B0);

const float C1_FREQ = getNoteFrequency(NOTE_C1);
const float C_SHARP1_FREQ = getNoteFrequency(NOTE_C_SHARP1);
const float D1_FREQ = getNoteFrequency(NOTE_D1);
const float D_SHARP1_FREQ = getNoteFrequency(NOTE_D_SHARP1);
const float E1_FREQ = getNoteFrequency(NOTE_E1);
const float F1_FREQ = getNoteFrequency(NOTE_F1);
const float F_SHARP1_FREQ = getNoteFrequency(NOTE_F_SHARP1);
const float G1_FREQ = getNoteFrequency(NOTE_G1);
const float G_SHARP1_FREQ = getNoteFrequency(NOTE_G_SHARP1);
const float A1_FREQ = getNoteFrequency(NOTE_A1);
const float A_SHARP1_FREQ = getNoteFrequency(NOTE_A_SHARP1);
const float B1_FREQ = getNoteFrequency(NOTE_B1);

const float C2_FREQ = getNoteFrequency(NOTE_C2);
const float C_SHARP2_FREQ = getNoteFrequency(NOTE_C_SHARP2);
const float D2_FREQ = getNoteFrequency(NOTE_D2);
const float D_SHARP2_FREQ = getNoteFrequency(NOTE_D_SHARP2);
const float E2_FREQ = getNoteFrequency(NOTE_E2);
const float F2_FREQ = getNoteFrequency(NOTE_F2);
const float F_SHARP2_FREQ = getNoteFrequency(NOTE_F_SHARP2);
const float G2_FREQ = getNoteFrequency(NOTE_G2);
const float G_SHARP2_FREQ = getNoteFrequency(NOTE_G_SHARP2);
const float A2_FREQ = getNoteFrequency(NOTE_A2);
const float A_SHARP2_FREQ = getNoteFrequency(NOTE_A_SHARP2);
const float B2_FREQ = getNoteFrequency(NOTE_B2);

const float C3_FREQ = getNoteFrequency(NOTE_C3);
const float C_SHARP3_FREQ = getNoteFrequency(NOTE_C_SHARP3);
const float D3_FREQ = getNoteFrequency(NOTE_D3);
const float D_SHARP3_FREQ = getNoteFrequency(NOTE_D_SHARP3);
const float E3_FREQ = getNoteFrequency(NOTE_E3);
const float F3_FREQ = getNoteFrequency(NOTE_F3);
const float F_SHARP3_FREQ = getNoteFrequency(NOTE_F_SHARP3);
const float G3_FREQ = getNoteFrequency(NOTE_G3);
const float G_SHARP3_FREQ = getNoteFrequency(NOTE_G_SHARP3);
const float A3_FREQ = getNoteFrequency(NOTE_A3);
const float A_SHARP3_FREQ = getNoteFrequency(NOTE_A_SHARP3);
const float B3_FREQ = getNoteFrequency(NOTE_B3);

const float C4_FREQ = getNoteFrequency(NOTE_C4);  // Middle C
const float C_SHARP4_FREQ = getNoteFrequency(NOTE_C_SHARP4);
const float D4_FREQ = getNoteFrequency(NOTE_D4);
const float D_SHARP4_FREQ = getNoteFrequency(NOTE_D_SHARP4);
const float E4_FREQ = getNoteFrequency(NOTE_E4);
const float F4_FREQ = getNoteFrequency(NOTE_F4);
const float F_SHARP4_FREQ = getNoteFrequency(NOTE_F_SHARP4);
const float G4_FREQ = getNoteFrequency(NOTE_G4);
const float G_SHARP4_FREQ = getNoteFrequency(NOTE_G_SHARP4);
const float A4_FREQ = getNoteFrequency(NOTE_A4);  // 440Hz
const float A_SHARP4_FREQ = getNoteFrequency(NOTE_A_SHARP4);
const float B4_FREQ = getNoteFrequency(NOTE_B4);

const float C5_FREQ = getNoteFrequency(NOTE_C5);
const float C_SHARP5_FREQ = getNoteFrequency(NOTE_C_SHARP5);
const float D5_FREQ = getNoteFrequency(NOTE_D5);
const float D_SHARP5_FREQ = getNoteFrequency(NOTE_D_SHARP5);
const float E5_FREQ = getNoteFrequency(NOTE_E5);
const float F5_FREQ = getNoteFrequency(NOTE_F5);
const float F_SHARP5_FREQ = getNoteFrequency(NOTE_F_SHARP5);
const float G5_FREQ = getNoteFrequency(NOTE_G5);
const float G_SHARP5_FREQ = getNoteFrequency(NOTE_G_SHARP5);
const float A5_FREQ = getNoteFrequency(NOTE_A5);
const float A_SHARP5_FREQ = getNoteFrequency(NOTE_A_SHARP5);
const float B5_FREQ = getNoteFrequency(NOTE_B5);

const float C6_FREQ = getNoteFrequency(NOTE_C6);
const float C_SHARP6_FREQ = getNoteFrequency(NOTE_C_SHARP6);
const float D6_FREQ = getNoteFrequency(NOTE_D6);
const float D_SHARP6_FREQ = getNoteFrequency(NOTE_D_SHARP6);
const float E6_FREQ = getNoteFrequency(NOTE_E6);
const float F6_FREQ = getNoteFrequency(NOTE_F6);
const float F_SHARP6_FREQ = getNoteFrequency(NOTE_F_SHARP6);
const float G6_FREQ = getNoteFrequency(NOTE_G6);
const float G_SHARP6_FREQ = getNoteFrequency(NOTE_G_SHARP6);
const float A6_FREQ = getNoteFrequency(NOTE_A6);
const float A_SHARP6_FREQ = getNoteFrequency(NOTE_A_SHARP6);
const float B6_FREQ = getNoteFrequency(NOTE_B6);

const float C7_FREQ = getNoteFrequency(NOTE_C7);
const float C_SHARP7_FREQ = getNoteFrequency(NOTE_C_SHARP7);
const float D7_FREQ = getNoteFrequency(NOTE_D7);
const float D_SHARP7_FREQ = getNoteFrequency(NOTE_D_SHARP7);
const float E7_FREQ = getNoteFrequency(NOTE_E7);
const float F7_FREQ = getNoteFrequency(NOTE_F7);
const float F_SHARP7_FREQ = getNoteFrequency(NOTE_F_SHARP7);
const float G7_FREQ = getNoteFrequency(NOTE_G7);
const float G_SHARP7_FREQ = getNoteFrequency(NOTE_G_SHARP7);
const float A7_FREQ = getNoteFrequency(NOTE_A7);
const float A_SHARP7_FREQ = getNoteFrequency(NOTE_A_SHARP7);
const float B7_FREQ = getNoteFrequency(NOTE_B7);

const float C8_FREQ = getNoteFrequency(NOTE_C8);
const float C_SHARP8_FREQ = getNoteFrequency(NOTE_C_SHARP8);
const float D8_FREQ = getNoteFrequency(NOTE_D8);
const float D_SHARP8_FREQ = getNoteFrequency(NOTE_D_SHARP8);
const float E8_FREQ = getNoteFrequency(NOTE_E8);
const float F8_FREQ = getNoteFrequency(NOTE_F8);
const float F_SHARP8_FREQ = getNoteFrequency(NOTE_F_SHARP8);
const float G8_FREQ = getNoteFrequency(NOTE_G8);
const float G_SHARP8_FREQ = getNoteFrequency(NOTE_G_SHARP8);
const float A8_FREQ = getNoteFrequency(NOTE_A8);
const float A_SHARP8_FREQ = getNoteFrequency(NOTE_A_SHARP8);
const float B8_FREQ = getNoteFrequency(NOTE_B8);


#endif /* NOTES_H */
