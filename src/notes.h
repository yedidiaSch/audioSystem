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
 int C0 = -57;
 int C_SHARP0 = -56;
 int D0 = -55;
 int D_SHARP0 = -54;
 int E0 = -53;
 int F0 = -52;
 int F_SHARP0 = -51;
 int G0 = -50;
 int G_SHARP0 = -49;
 int A0 = -48;
 int A_SHARP0 = -47;
 int B0 = -46;

 int C1 = -45;
 int C_SHARP1 = -44;
 int D1 = -43;
 int D_SHARP1 = -42;
 int E1 = -41;
 int F1 = -40;
 int F_SHARP1 = -39;
 int G1 = -38;
 int G_SHARP1 = -37;
 int A1 = -36;
 int A_SHARP1 = -35;
 int B1 = -34;

 int C2 = -33;
 int C_SHARP2 = -32;
 int D2 = -31;
 int D_SHARP2 = -30;
 int E2 = -29;
 int F2 = -28;
 int F_SHARP2 = -27;
 int G2 = -26;
 int G_SHARP2 = -25;
 int A2 = -24;
 int A_SHARP2 = -23;
 int B2 = -22;

 int C3 = -21;
 int C_SHARP3 = -20;
 int D3 = -19;
 int D_SHARP3 = -18;
 int E3 = -17;
 int F3 = -16;
 int F_SHARP3 = -15;
 int G3 = -14;
 int G_SHARP3 = -13;
 int A3 = -12;
 int A_SHARP3 = -11;
 int B3 = -10;

 int C4 = -9;      // Middle C
 int C_SHARP4 = -8;
 int D4 = -7;
 int D_SHARP4 = -6;
 int E4 = -5;
 int F4 = -4;
 int F_SHARP4 = -3;
 int G4 = -2;
 int G_SHARP4 = -1;
 int A4 = 0;       // Reference A4 (440Hz)
 int A_SHARP4 = 1;
 int B4 = 2;

 int C5 = 3;
 int C_SHARP5 = 4;
 int D5 = 5;
 int D_SHARP5 = 6;
 int E5 = 7;
 int F5 = 8;
 int F_SHARP5 = 9;
 int G5 = 10;
 int G_SHARP5 = 11;
 int A5 = 12;
 int A_SHARP5 = 13;
 int B5 = 14;

 int C6 = 15;
 int C_SHARP6 = 16;
 int D6 = 17;
 int D_SHARP6 = 18;
 int E6 = 19;
 int F6 = 20;
 int F_SHARP6 = 21;
 int G6 = 22;
 int G_SHARP6 = 23;
 int A6 = 24;
 int A_SHARP6 = 25;
 int B6 = 26;

 int C7 = 27;
 int C_SHARP7 = 28;
 int D7 = 29;
 int D_SHARP7 = 30;
 int E7 = 31;
 int F7 = 32;
 int F_SHARP7 = 33;
 int G7 = 34;
 int G_SHARP7 = 35;
 int A7 = 36;
 int A_SHARP7 = 37;
 int B7 = 38;

 int C8 = 39;      // Highest C in 88-key piano
 int C_SHARP8 = 40;
 int D8 = 41;
 int D_SHARP8 = 42;
 int E8 = 43;
 int F8 = 44;
 int F_SHARP8 = 45;
 int G8 = 46;
 int G_SHARP8 = 47;
 int A8 = 48;
 int A_SHARP8 = 49;
 int B8 = 50;


/// Frequencies for all notes from C0 to B8
float C0_FREQ = getNoteFrequency(C0);
float C_SHARP0_FREQ = getNoteFrequency(C_SHARP0);
float D0_FREQ = getNoteFrequency(D0);
float D_SHARP0_FREQ = getNoteFrequency(D_SHARP0);
float E0_FREQ = getNoteFrequency(E0);
float F0_FREQ = getNoteFrequency(F0);
float F_SHARP0_FREQ = getNoteFrequency(F_SHARP0);
float G0_FREQ = getNoteFrequency(G0);
float G_SHARP0_FREQ = getNoteFrequency(G_SHARP0);
float A0_FREQ = getNoteFrequency(A0);
float A_SHARP0_FREQ = getNoteFrequency(A_SHARP0);
float B0_FREQ = getNoteFrequency(B0);

float C1_FREQ = getNoteFrequency(C1);
float C_SHARP1_FREQ = getNoteFrequency(C_SHARP1);
float D1_FREQ = getNoteFrequency(D1);
float D_SHARP1_FREQ = getNoteFrequency(D_SHARP1);
float E1_FREQ = getNoteFrequency(E1);
float F1_FREQ = getNoteFrequency(F1);
float F_SHARP1_FREQ = getNoteFrequency(F_SHARP1);
float G1_FREQ = getNoteFrequency(G1);
float G_SHARP1_FREQ = getNoteFrequency(G_SHARP1);
float A1_FREQ = getNoteFrequency(A1);
float A_SHARP1_FREQ = getNoteFrequency(A_SHARP1);
float B1_FREQ = getNoteFrequency(B1);

float C2_FREQ = getNoteFrequency(C2);
float C_SHARP2_FREQ = getNoteFrequency(C_SHARP2);
float D2_FREQ = getNoteFrequency(D2);
float D_SHARP2_FREQ = getNoteFrequency(D_SHARP2);
float E2_FREQ = getNoteFrequency(E2);
float F2_FREQ = getNoteFrequency(F2);
float F_SHARP2_FREQ = getNoteFrequency(F_SHARP2);
float G2_FREQ = getNoteFrequency(G2);
float G_SHARP2_FREQ = getNoteFrequency(G_SHARP2);
float A2_FREQ = getNoteFrequency(A2);
float A_SHARP2_FREQ = getNoteFrequency(A_SHARP2);
float B2_FREQ = getNoteFrequency(B2);

float C3_FREQ = getNoteFrequency(C3);
float C_SHARP3_FREQ = getNoteFrequency(C_SHARP3);
float D3_FREQ = getNoteFrequency(D3);
float D_SHARP3_FREQ = getNoteFrequency(D_SHARP3);
float E3_FREQ = getNoteFrequency(E3);
float F3_FREQ = getNoteFrequency(F3);
float F_SHARP3_FREQ = getNoteFrequency(F_SHARP3);
float G3_FREQ = getNoteFrequency(G3);
float G_SHARP3_FREQ = getNoteFrequency(G_SHARP3);
float A3_FREQ = getNoteFrequency(A3);
float A_SHARP3_FREQ = getNoteFrequency(A_SHARP3);
float B3_FREQ = getNoteFrequency(B3);

float C4_FREQ = getNoteFrequency(C4);  // Middle C
float C_SHARP4_FREQ = getNoteFrequency(C_SHARP4);
float D4_FREQ = getNoteFrequency(D4);
float D_SHARP4_FREQ = getNoteFrequency(D_SHARP4);
float E4_FREQ = getNoteFrequency(E4);
float F4_FREQ = getNoteFrequency(F4);
float F_SHARP4_FREQ = getNoteFrequency(F_SHARP4);
float G4_FREQ = getNoteFrequency(G4);
float G_SHARP4_FREQ = getNoteFrequency(G_SHARP4);
float A4_FREQ = getNoteFrequency(A4);  // 440Hz
float A_SHARP4_FREQ = getNoteFrequency(A_SHARP4);
float B4_FREQ = getNoteFrequency(B4);

float C5_FREQ = getNoteFrequency(C5);
float C_SHARP5_FREQ = getNoteFrequency(C_SHARP5);
float D5_FREQ = getNoteFrequency(D5);
float D_SHARP5_FREQ = getNoteFrequency(D_SHARP5);
float E5_FREQ = getNoteFrequency(E5);
float F5_FREQ = getNoteFrequency(F5);
float F_SHARP5_FREQ = getNoteFrequency(F_SHARP5);
float G5_FREQ = getNoteFrequency(G5);
float G_SHARP5_FREQ = getNoteFrequency(G_SHARP5);
float A5_FREQ = getNoteFrequency(A5);
float A_SHARP5_FREQ = getNoteFrequency(A_SHARP5);
float B5_FREQ = getNoteFrequency(B5);

float C6_FREQ = getNoteFrequency(C6);
float C_SHARP6_FREQ = getNoteFrequency(C_SHARP6);
float D6_FREQ = getNoteFrequency(D6);
float D_SHARP6_FREQ = getNoteFrequency(D_SHARP6);
float E6_FREQ = getNoteFrequency(E6);
float F6_FREQ = getNoteFrequency(F6);
float F_SHARP6_FREQ = getNoteFrequency(F_SHARP6);
float G6_FREQ = getNoteFrequency(G6);
float G_SHARP6_FREQ = getNoteFrequency(G_SHARP6);
float A6_FREQ = getNoteFrequency(A6);
float A_SHARP6_FREQ = getNoteFrequency(A_SHARP6);
float B6_FREQ = getNoteFrequency(B6);

float C7_FREQ = getNoteFrequency(C7);
float C_SHARP7_FREQ = getNoteFrequency(C_SHARP7);
float D7_FREQ = getNoteFrequency(D7);
float D_SHARP7_FREQ = getNoteFrequency(D_SHARP7);
float E7_FREQ = getNoteFrequency(E7);
float F7_FREQ = getNoteFrequency(F7);
float F_SHARP7_FREQ = getNoteFrequency(F_SHARP7);
float G7_FREQ = getNoteFrequency(G7);
float G_SHARP7_FREQ = getNoteFrequency(G_SHARP7);
float A7_FREQ = getNoteFrequency(A7);
float A_SHARP7_FREQ = getNoteFrequency(A_SHARP7);
float B7_FREQ = getNoteFrequency(B7);

float C8_FREQ = getNoteFrequency(C8);
float C_SHARP8_FREQ = getNoteFrequency(C_SHARP8);
float D8_FREQ = getNoteFrequency(D8);
float D_SHARP8_FREQ = getNoteFrequency(D_SHARP8);
float E8_FREQ = getNoteFrequency(E8);
float F8_FREQ = getNoteFrequency(F8);
float F_SHARP8_FREQ = getNoteFrequency(F_SHARP8);
float G8_FREQ = getNoteFrequency(G8);
float G_SHARP8_FREQ = getNoteFrequency(G_SHARP8);
float A8_FREQ = getNoteFrequency(A8);
float A_SHARP8_FREQ = getNoteFrequency(A_SHARP8);
float B8_FREQ = getNoteFrequency(B8);


#endif /* NOTES_H */
