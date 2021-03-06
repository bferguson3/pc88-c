/* R0, R1 (b0-3) : 12 bit period for Ch. A
*  R2, R3 (b0-3) : 12 bit period for Ch. B
*  R4, R5 (b0-3) : 12 bit period for Ch. C
*  R6            : 5 bit noise period
*  R7            : Output ctl: ~BANNNTTT
*        BA: Input enable on IO port A/B
*       NNN: Noise enable on Ch. CBA
*       TTT:  Tone enable on Ch. CBA
*  R8,R9,RA      : b0-3 amplitude (if b4=0)
*                  b4 enable/disable envelope
*  RB,RC         : 16-bit envelope period
*  RD            : 4 bit envelope shape
*  RE/RF         : I/O Ports A/B
*/
#define OPN_REG 0x44
#define OPN_DAT 0x45
//[7] = 0b00111110
// derivative:
// TONE = CLOCK / (16 * PERIOD)
// PERIOD = 256*COARSE + FINE
// relational:
// PERIOD = CLOCK / (TONE * 16)
// COARSE + (FINE / 256) = TONE / 256
// https://pages.mtu.edu/~suits/notefreqs.html

// Z80 actual speed:
#define CPU4MHZ 3993600
// 440 Tunings:
#define C2_440  65.41
#define C2S_440 69.30
#define D2_440 73.42
#define D2S_440 77.78
#define E2_440 82.41
#define F2_440 87.31
#define F2S_440 92.50
#define G2_440 98.00
#define G2S_440 103.83
#define A2_440 110.00
#define A2S_440 116.54
#define B2_440 123.47

#define C3_440 130.81
#define C3S_440 138.59
#define D3_440 146.83
#define D3S_440 155.56
#define E3_440 164.81
#define F3_440 174.61
#define F3S_440 185.00
#define G3_440 196.00
#define G3S_440 207.65
#define A3_440 220.00
#define A3S_440 233.08
#define B3_440 246.94

#define C4_440  261.63
#define C4S_440 277.18	
#define D4_440  293.66
#define D4S_440 311.13
#define E4_440  329.63
#define F4_440  349.23
#define F4S_440 369.99
#define G4_440  392
#define G4S_440 415.30
#define A4_440  440
#define A4S_440 466.16
#define B4_440  493.88

#define C5_440  523.25
#define C5S_440 554.37	
#define D5_440  587.33
#define D5S_440 622.25
#define E5_440  659.25
#define F5_440  698.46
#define F5S_440 739.99
#define G5_440  783.99
#define G5S_440 830.61
#define A5_440  880.00
#define A5S_440 932.33
#define B5_440  987.77

#define C6_440  1046.50
#define C6S_440 1108.73
#define D6_440 1174.66
#define D6S_440 1244.51
#define E6_440 1318.51
#define F6_440 1396.91
#define F6S_440 1479.98
#define G6_440 1567.98
#define G6S_440 1661.22
#define A6_440 1760.00	
#define A6S_440 1864.66
#define B6_440 1975.53

// Bit flags
#define CHA_TONEH 1
#define CHA_TONEL 0
#define CHB_TONEH 3
#define CHB_TONEL 2
#define CHC_TONEH 5
#define CHC_TONEL 4
#define SSG_MIXER 7
#define SSG_NOISETONE 6
#define CHA_AMP 8
#define CHB_AMP 9
#define CHC_AMP 10
#define SSG_ENVPDH 0xb
#define SSG_ENVPDL 0xc
#define SSG_ENVTYPE 0xd
#define SSG_IOA 0xe
#define SSG_IOB 0xf
#define CHA_NOISE_OFF 0b1000
#define CHB_NOISE_OFF 0b10000
#define CHC_NOISE_OFF 0b100000
#define CHA_TONE 0b1
#define CHB_TONE_OFF 0b10
#define CHC_TONE_OFF 0b100

// SSG Period inputs:
#define SSG_C2 (u16)(CPU4MHZ / (C2_440 * 32))
#define SSG_C2S (u16)(CPU4MHZ / (C2S_440 * 32))
#define SSG_D2 (u16)(CPU4MHZ / (D2_440 * 32))
#define SSG_D2S (u16)(CPU4MHZ / (D2S_440 * 32))
#define SSG_E2 (u16)(CPU4MHZ / (E2_440 * 32))
#define SSG_F2 (u16)(CPU4MHZ / (F2_440 * 32))
#define SSG_F2S (u16)(CPU4MHZ / (F2S_440 * 32))
#define SSG_G2 (u16)(CPU4MHZ / (G2_440 * 32))
#define SSG_G2S (u16)(CPU4MHZ / (G2S_440 * 32))
#define SSG_A2 (u16)(CPU4MHZ / (A2_440 * 32))
#define SSG_A2S (u16)(CPU4MHZ / (A2S_440 * 32))
#define SSG_B2 (u16)(CPU4MHZ / (B2_440 * 32))

#define SSG_C3 (u16)(CPU4MHZ / (C3_440 * 32))
#define SSG_C3S (u16)(CPU4MHZ / (C3S_440 * 32))
#define SSG_D3 (u16)(CPU4MHZ / (D3_440 * 32))
#define SSG_D3S (u16)(CPU4MHZ / (D3S_440 * 32))
#define SSG_E3 (u16)(CPU4MHZ / (E3_440 * 32))
#define SSG_F3 (u16)(CPU4MHZ / (F3_440 * 32))
#define SSG_F3S (u16)(CPU4MHZ / (F3S_440 * 32))
#define SSG_G3 (u16)(CPU4MHZ / (G3_440 * 32))
#define SSG_G3S (u16)(CPU4MHZ / (G3S_440 * 32))
#define SSG_A3 (u16)(CPU4MHZ / (A3_440 * 32))
#define SSG_A3S (u16)(CPU4MHZ / (A3S_440 * 32))
#define SSG_B3 (u16)(CPU4MHZ / (B3_440 * 32))

#define SSG_C4 (u16)(CPU4MHZ / (C4_440 * 32))
#define SSG_C4S (u16)(CPU4MHZ / (C4S_440 * 32))
#define SSG_D4 (u16)(CPU4MHZ / (D4_440 * 32))
#define SSG_D4S (u16)(CPU4MHZ / (D4S_440 * 32))
#define SSG_E4 (u16)(CPU4MHZ / (E4_440 * 32))
#define SSG_F4 (u16)(CPU4MHZ / (F4_440 * 32))
#define SSG_F4S (u16)(CPU4MHZ / (F4S_440 * 32))
#define SSG_G4 (u16)(CPU4MHZ / (G4_440 * 32))
#define SSG_G4S (u16)(CPU4MHZ / (G4S_440 * 32))
#define SSG_A4 (u16)(CPU4MHZ / (A4_440 * 32))
#define SSG_A4S (u16)(CPU4MHZ / (A4S_440 * 32))
#define SSG_B4 (u16)(CPU4MHZ / (B4_440 * 32))

#define SSG_C5 (u16)(CPU4MHZ / (C5_440 * 32))
#define SSG_C5S (u16)(CPU4MHZ / (C5S_440 * 32))
#define SSG_D5 (u16)(CPU4MHZ / (D5_440 * 32))
#define SSG_D5S (u16)(CPU4MHZ / (D5S_440 * 32))
#define SSG_E5 (u16)(CPU4MHZ / (E5_440 * 32))
#define SSG_F5 (u16)(CPU4MHZ / (F5_440 * 32))
#define SSG_F5S (u16)(CPU4MHZ / (F5S_440 * 32))
#define SSG_G5 (u16)(CPU4MHZ / (G5_440 * 32))
#define SSG_G5S (u16)(CPU4MHZ / (G5S_440 * 32))
#define SSG_A5 (u16)(CPU4MHZ / (A5_440 * 32))
#define SSG_A5S (u16)(CPU4MHZ / (A5S_440 * 32))
#define SSG_B5 (u16)(CPU4MHZ / (B5_440 * 32))

#define SSG_C6 (u16)(CPU4MHZ / (C6_440 * 32))
#define SSG_C6S (u16)(CPU4MHZ / (C6S_440 * 32))
#define SSG_D6 (u16)(CPU4MHZ / (D6_440 * 32))
#define SSG_D6S (u16)(CPU4MHZ / (D6S_440 * 32))
#define SSG_E6 (u16)(CPU4MHZ / (E6_440 * 32))
#define SSG_F6 (u16)(CPU4MHZ / (F6_440 * 32))
#define SSG_F6S (u16)(CPU4MHZ / (F6S_440 * 32))
#define SSG_G6 (u16)(CPU4MHZ / (G6_440 * 32))
#define SSG_G6S (u16)(CPU4MHZ / (G6S_440 * 32))
#define SSG_A6 (u16)(CPU4MHZ / (A6_440 * 32))
#define SSG_A6S (u16)(CPU4MHZ / (A6S_440 * 32))
#define SSG_B6 (u16)(CPU4MHZ / (B6_440 * 32))


static const u16 octavetwo[12] = {
    SSG_C2, SSG_C2S,
    SSG_D2, SSG_D2S, 
    SSG_E2,
    SSG_F2, SSG_F2S,
    SSG_G2, SSG_G2S,
    SSG_A2, SSG_A2S,
    SSG_B2
};
static const u16 octavethree[12] = {
    SSG_C3, SSG_C3S,
    SSG_D3, SSG_D3S, 
    SSG_E3,
    SSG_F3, SSG_F3S,
    SSG_G3, SSG_G3S,
    SSG_A3, SSG_A3S,
    SSG_B3
};
static const u16 octavefour[12] = {
    SSG_C4, SSG_C4S,
    SSG_D4, SSG_D4S,
    SSG_E4,
    SSG_F4, SSG_F4S,
    SSG_G4, SSG_G4S,
    SSG_A4, SSG_A4S,
    SSG_B4
};
static const u16 octavefive[12] = {
    SSG_C5, SSG_C5S,
    SSG_D5, SSG_D5S, 
    SSG_E5,
    SSG_F5, SSG_F5S,
    SSG_G5, SSG_G5S,
    SSG_A5, SSG_A5S,
    SSG_B5
};
static const u16 octavesix[12] = {
    SSG_C6, SSG_C6S,
    SSG_D6, SSG_D6S, 
    SSG_E6,
    SSG_F6, SSG_F6S,
    SSG_G6, SSG_G6S,
    SSG_A6, SSG_A6S,
    SSG_B6
};


/***** Usage from examples/psg : ********
// Play C4 on Ch A, vol 15, no envelope, mute all other
SetIOReg(OPN_REG, CHA_TONEL);
SetIOReg(OPN_DAT, SSG_C4 & 0xff);
SetIOReg(OPN_REG, CHA_TONEH);
SetIOReg(OPN_DAT, (SSG_C4 & 0xf00) >> 8);
//0b00111110:
SetIOReg(OPN_REG, SSG_MIXER);
SetIOReg(OPN_DAT, ~(CHA_TONE | 0xc0));
SetIOReg(OPN_REG, CHA_AMP);
SetIOReg(OPN_DAT, 15);
**************************************/
// max is 4095 or 4095 = 3993600 / 16X
//65520X = 3993600 or B1, but C2 is better starting
