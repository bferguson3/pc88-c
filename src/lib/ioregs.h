//ioregs.h

#define PD_CTL_REG      0x10         // W - Printer Out OR Calendar Lock (uPD1990AC)
// If PD1990AC - bit0 - bit2 : Command output
//               bit3 : Data output
#define USART_DATA_REG  0x20              // R/W (pd8251C)
#define USART_CTL_REG   0x21              // R/W
/* SYSTEM CONTROL REGISTER A //
**READ: Dipswitch status
Bit	7	6	5	    4	    3	    2	    1	    0
           SW1-S5	SW1-S4	SW1-S3	SW1-S2	SW1-S1	SW4-S1

SW1-S5	DEL code response: 0-OK 1-Ignore
SW1-S4	S parameter: 0-ON 1-OFF
SW1-S3	Text lines: 0-25L 1-20L
SW1-S2	Text columns: 0-80C 1-40C
SW1-S1	Boot mode: 0-Terminal 1-BASIC
SW4-S1	BASIC mode: 0-NBASIC 1-N88-BASIC
**WRITE: System Control Port
Bit	7	6	54	3	    2	1	    0
            BS	MTON	CDS	COLOR	40

BS	USART Channel control:
    00: CMT 600baud     10: RS-232C
    01: CMT 1200baud    11: RS-232C
MTON Motor Control: 0-OFF 1-Motor ON
CDS	Carrier control: 0-Space(1200hz) 1-Mark(2400hz)
COLOR Text screen color mode: 0-Color 1-B&W
40 40 Col mode: 0-40c 1-80c
//*/
#define SYS_CTL_REGA    0x30
/* SYSTEM CONTROL REGISTER B //
** WRITE: System Control Port
Bit	    7	6	5	      4	      3	    2	    1	    0
            25LINE	HCOLOR	GRAPH	RMODE	MMODE	200LINE
25LINE Hi-res 25 row mode: 0-All other modes 1-ON
HCOLOR Graphic color mode: 0-B&W 1-Color
GRAPH Graphic control: 0-Graphic screens off 1-ON
RMODE ROM mode: 0-N88-BASIC 1-N-BASIC
MMODE RAM mode: 0-ROM/RAM 1-64K RAM
200LINE	Hi-res CRT mode: 0-640×400 1-640x200
  Recommended values: 0b000110x1
                      0b00010011 
** READ: Dipswitch status
Bit	7	    6	    5	    4	    3	    2	    1	    0
    SW4-S2	SW3-S0	SW2-S6	SW2-S5	SW2-S4	SW2-S3	SW2-S2	SW2-S1

SW4-S2 V1/V2 Mode: 0-V2 1-V1
SW3-S0 Hispeed mode: 0-Off (V1S) 1-On (V1H, V2)
SW2-S6 Transfer direction: 0-Half-duplex 1-Full-duplex
SW2-S5 X parameter: 0-ON 1-OFF
SW2-S4 Stop bit length: 0-2 bits 1-1 bit
SW2-S3 Data bit length: 0-8 bits 1-7 bits
SW2-S2 Parity bit: 0-Even 1-Odd
SW2-S1 Parity check: 0-Party 1-No parity
//*/
#define SYS_CTL_REGB    0x31
#define LINES25 (1<<5)
#define HCOLOR (1<<4)
#define GRAPH (1<<3)
#define RMODE (1<<2)
#define MMODE (1<<1)
#define LINE200 (1)
/* MODE SET REGISTER
[R/W] These registers are not decoded on systems
    before the PC-8801mkII. On models after SR,
    double-check this register on boot.
Bit	7	    6	    5	    4	    32	10
    SINTM	GVAM	PMODE	TMODE	AVC	EROMSL

SINTM Sound IRQ: 0-Enable 1-Disable
GVAM Graphics VRAM access mode: 0-Independant 1-Expanded
PMODE Palette mode: 0-Digital 8 color 1-Analog 512 color
TMODE FastRAM access mode (V1H/V2 only):
  0: F000h～FFFFh FastRAM
  1: F000h～FFFFh Main RAM
AVC	Screen output mode: 
  00-TV/Video out   10-Computer out
  01-(off)          11-Option mode
EROMSL Select expaned ROM bank (0-3) 
INDEPENDANT mode is the same as V1 mode. GVRAM is written separately.
EXPANDED mode (SR+ only) lets you access all planes simultaneously.
*NOTE: When VRAM is selected, overall processing speed is decreased.
  Remember to page in Main RAM!
Default values: xxxx1001
*/
#define ALU_MODE_CTRL 0x32
/* Write only
Bit	7	6	    5	    4	    3	2	    1	    0
        ALU21	ALU11	ALU01	    ALU20	ALU10	ALU00
        GV2H    GV1H    GV0H        GV2L    GV1L    GV0L
GV0 - ALU mode for GVRAM bank 0
GV1 - ALU mode for GVRAM bank 1
GV2 - ALU mode for GVRAM bank 2
 Bits arranged as H:L
00: Bit reset
01: Bit set
10: Bit invert
11: No op */
#define EXPANDED_ALU_CTRL 0x34 
/* Expanded Access Mode GVRAM Control   W only
bit  7      6   5 4     3   2   1   0
     GAM    -   GDM     -   GC  RC  BC

GAM - GVRAM Access Mode
 0 = Main RAM, 1 = GVRAM
GDM - Graphic data multiplexer control
 00 = Write all planes (check port $34)
 01 = Write last VRAM read data to all 3 planes
 10 = Transfer R plane to B
 11 = Transfer B plane to R

* If GDM = 00, you can perform logical operations as explained
using register $34. It appears this mode does not use a buffer.
(There are ways to utilize this to advantage combined with GDM=01 
but I am unclear)

* If GDM = 01, when a single byte is read from VRAM, all 3 planar
data parts are read into a buffer. When writing, the stored buffer
data is written to all 3 planes simultaneously. You can use this to
copy within VRAM without extraneous calculations (ie port $34),
which can be used to create scrolling.

* GC/RC/BC = Bit comp data. Bits written to these fields act as a
color plane filter to VRAM reads. E.g. if bits2-0 are 011 (purple),
the pixel data read from VRAM will be 1 if purple, or 0 if not.

* When ALU EXPANDED access mode is on, reading data from VRAM is
not stored in a register. What is loaded in the register is
'comparison data'; VRAM itself is stored in a hidden buffer.

ld a,%10010000  ; gvram access on, 3 plane buffer r/w mode
out ($35),a 
ld a,($c000)	; this is bit comp data, but the
ld ($c0002),a   ; operation commands a vram 'copy'.*/
#define EXPANDED_GVRAM_CTRL 0x35
// Bit 6 - Joystick port output
// Octave4 (A-440)
// Value in HL (number of loop cycles to wait)
// C 315
// D 281
// E 250 
// F 235
// G 209
// A 186
// B 165
// MkII only
#define SYS_MODE_SENSE 0x40   // Write also for beepin'
#define STROBE_PORT 0x40      // Read

#define FM_REG_0 0x44     // <FR/MR - OPN, FH/MH> - OPNA SSG/FM1-3
#define FM_REG_1 0x45     // <FR/MR - OPN, FH/MH> - OPNA SSG/FM1-3
#define FM_REG_2 0x46     // FH/MH - OPNA ADPCM/FM4-6
#define FM_REG_3 0x47     // FH/MH - OPNA ADPCM/FM4-6

#define CRTC_CTLREG_A 0x50     // pd3301AC control ports 
#define CRTC_CTLREG_B 0x51
// W - bits 4-6 only.
// bit  7   6 5 4   3   2   1
//          G R B                   
#define BG_COLOR_REG 0x52     
/* Screen Display Control - Write only
Bit	7	6	5	4	3	    2	    1	    0
                    G2DS	G1DS	G0DS	TEXTDS

G2DS GVRAM2 display: 0-ON 1-OFF
G1DS GVRAM1 display: 0-ON 1-OFF
G0DS GVRAM0 display: 0-ON 1-OFF
TEXTDS Text screen display: 0-ON 1-OFF
* Set bit 3 to 1 when using 640x400 mode.
In color graphics mode, G*DS bits are ignored.
*/
#define SCREEN_DISP_CTL 0x53
#define PAL_REG0 0x54  // bits 0-2 contain palette value 0-7.
#define PAL_REG1 0x55  // bits 0-2 contain palette value 0-7.
#define PAL_REG2 0x56  // bits 0-2 contain palette value 0-7.
#define PAL_REG3 0x57  // bits 0-2 contain palette value 0-7.
#define PAL_REG4 0x58  // bits 0-2 contain palette value 0-7.
#define PAL_REG5 0x59  // bits 0-2 contain palette value 0-7.
#define PAL_REG6 0x5a  // bits 0-2 contain palette value 0-7.
#define PAL_REG7 0x5b  // bits 0-2 contain palette value 0-7.
/* Read: GVRAM bank display status
   bit 0 - GVRAM0 
   bit 1 - GVRAM1
   bit 2 - GVRAM3
1 = Selected
0 = Not selected */ 
#define GVRAM_BANK_REG 0x5c

#define GVRAM_SEL_0 0x5c  // Write any value to select vram0 (Blue plane)
#define GVRAM_SEL_1 0x5d  // Write any value to select vram1 (Red plane)
#define GVRAM_SEL_2 0x5e  // Write any value to select vram2 (Green plane)
#define MAINRAM_SEL 0x5f  // Write any value to select Main RAM

#define DMAC_REG0 0x60 // PD8257C DMA controller register (R/W)
#define DMAC_REG1 0x61 // PD8257C DMA controller register (R/W)
#define DMAC_REG2 0x62 // PD8257C DMA controller register (R/W)
#define DMAC_REG3 0x63 // PD8257C DMA controller register (R/W)
#define DMAC_REG4 0x64 // PD8257C DMA controller register (R/W)
#define DMAC_REG5 0x65 // PD8257C DMA controller register (R/W)
#define DMAC_REG6 0x66 // PD8257C DMA controller register (R/W)
#define DMAC_REG7 0x67 // PD8257C DMA controller register (R/W)
#define DMAC_REG8 0x68 // PD8257C DMA controller register  (R/W)

#define CPU_CLOCK_CTL 0x6e // Bit 7 only. 1=4MHz, 0=8MHz
/* (R/W) Baud rate set register, FH+ only
Bits 0-3 only:
0000	75
0001	150
0010	300
0011	600
0100	1200
0101	2400
0110	4800
0111	9600
1000	19200
1001～1111	OFF */
#define BAUDRATE_REG 0x6f
#define TEXTWIN_OFS_REG 0x70 // R/W: Upper 8 bits of text window address
/* Expansion ROM select
Only one bit can be set at once:
Bit7 Expansion ROM 7 select
Bit6 Expansion ROM 6 select
Bit5 Expansion ROM 5 select
Bit4 Expansion ROM 4 select
Bit3 Expansion ROM 3 select
Bit2 Expansion ROM 2 select
Bit1 Expansion ROM 1 select
Bit0 Expansion ROM 0/4th rom select
Writing 0xff selects Main ROM.
*/
#define ROM_EXPANSION_SEL 0x71
#define TEXTWIN_INC_REG 0x78 // W: Increment TEXTWIN_OFS once

// To do:
// Registers Ax - Sound board II registers
/* Interrupt Level Set Register
 bits 0 - 2: Interrupt level 
 0 (high) - RS232 / Serial
 1 - CRTC / Vertical blank
 2 - 1/600s timer
 3 - INT4
 4 - Sound IRQ
 5 - INT2
 6 - FDINT1
 7 - FDINT2
Set to 1 higher than the IRQ you wish to enable.
e.g. writing 0x03 will enable 0, 1, and 2. */
#define IRQ_LEVEL_SET 0xe4
/* Interrupt Mask Register
Bit 2 - Serial / RS232 interrupt
Bit 1 - Vertical blank from CRTC
Bit 0 - 1/600s timer interrupt
 Setting a bit ALLOWS the interrupt. */
#define IRQ_MASK 0xe6

// Registers Ex - Expanded RAM read/write control registers, including Kanji ROMs
// Registers Fx - Disc subsystem 8255 control regs

/* IRQ Vector Table @ 0xf300~
f300 - Serial
f302 - Vblank
f304 - Sound1
f306 - Sound2 ...
*/