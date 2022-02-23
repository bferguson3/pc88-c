#include <pc88-c.h>

void VGMPlayer() __naked; 
void Vblank() __critical __interrupt;
void LoadVGM(const u8* sn);
inline void SetVBLIRQ();

static u8* songPointer;
static bool SONGPLAYING = false;

//playing:
#include "testssg.h"
// not playing
const unsigned char song[] = {\
// Vgm_ 1.60 header
	'V', 'g', 'm', ' ', 0x0, 0x0, 0x0, 0x0, 0x60, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
// 60
	0x0, 0x0, 0x0, 0x0, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
// vgm data offset +4c bytes
	0x0, 0x0, 0x0, 0x0, 0x4c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
// z80 pc88 zpeed: 3993600 : total samples = ?
	0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x3c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
// example from ym2612 manual
	0x55, 0x27, 0x0, 0x55, 0x28, 0x0, 
	0x55, 0x30, 0x71, 0x55, 0x34, 0xd, 0x55, 0x38, 0x33, 0x55, 0x3c, 0x1,
	0x55, 0x40, 0x23, 0x55, 0x44, 0x2d, 0x55, 0x48, 0x26, 0x55, 0x4c, 0x0, 
	0x55, 0x50, 0x5f, 0x55, 0x54, 0x99, 0x55, 0x58, 0x5f, 0x55, 0x5c, 0x94,
	0x55, 0x60, 0x5, 0x55, 0x64, 0x5, 0x55, 0x68, 0x5, 0x55, 0x6c, 0x7, 
	0x55, 0x70, 0x2, 0x55, 0x74, 0x2, 0x55, 0x78, 0x2, 0x55, 0x7c, 0x2, 
	0x55, 0x80, 0x11, 0x55, 0x84, 0x11, 0x55, 0x88, 0x11, 0x55, 0x8c, 0xa6, 
	0x55, 0x90, 0x0, 0x55, 0x94, 0x0, 0x55, 0x98, 0x0, 0x55, 0x9c, 0x0, 
	0x55, 0xb0, 0x32, 
	0x55, 0xb4, 0xc0,
	0x55, 0x28, 0x0,
	0x55, 0xa4, 0x22,
	0x55, 0xa0, 0x69,
	0x55, 0x28, 0xf0, 
// 1 second wait
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
	0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62, 0x62,
// off 
	0x55, 0x28, 0x0, 0x66 
};


inline void SetVBLIRQ()
{
    SetIOReg(IRQ_LEVEL_SET, 2);      // Set IRQ high for VBL
    SetIOReg(IRQ_MASK, 0b10);        // Reset mask for VBL
}

void Vblank() __critical __interrupt
{
    IRQ_OFF         // Disable interrupts during VBL
    SetVBLIRQ();    // Reset interrupt level and mask
    
	// Do our Vblank code:
    if(SONGPLAYING)
    {
	VGMPlayer();
    }

    IRQ_ON
}

u8* songStart;

void main()
{
	IRQ_OFF 
	__asm 
		ld hl, #_Vblank
		ld (CRTC_IRQ),hl
	__endasm;
	SetVBLIRQ();        // And enable the VBL interrupt!
	IRQ_ON 

	LoadVGM((const u8*)&outsong);
	//songPointer = (const u8*)&song + 0x80;
	//SONGPLAYING = true;

	print("Now playing: Shantae - Burning Town (Jake Kaufman)");
	SetCursorPos(0,1);
	print("Converted from Deflemask for Pi");
	SetCursorPos(0,2);
	print("Using a 3-channel SN76489 to YM2203 script");

	while(1)
	{}
}

void LoadVGM(const u8* sn) 
{
	songPointer = sn + 0x40; // 1.50 FIXME
	songStart = songPointer;
	SONGPLAYING = true;
}

void VGMPlayer() __naked
{
	// songPointer contains byte 0 of VGM data (if 1.60)
	__asm 
		push hl 
		push bc 
		push de 
		push af 

		ld hl,(_songPointer)
		ld a,(hl) 
		inc hl 
		ld (_songPointer),hl 

		jp PlayLoop

	endPlay:
		pop af 
		pop de 
		pop bc 
		pop hl 

		ret
		
	PlayLoop: 
		cp #0x55
		jp z,_WRITEOPN
		cp #0x61
		jp z,_SAMPLEWAIT 
		cp #0x62 
		jp z,_ENDFRAME
		cp #0x63
		jp z,_ENDFRAME 
		cp #0x66
		jp z,_ENDSONG
		cp #0x67 
		jp z,_ERROR ; no data blocks
		cp #0x80		; 70-7f
		jp c,_QWAIT

		jp endPlay 
	
	_GETNEXTBYTE:
		ld hl,(_songPointer) 
		ld a,(hl)
		inc hl 
		ld (_songPointer),hl
		ret

	_WRITEOPN:
		; Get register
		ld c,#0x44
		call _GETNEXTBYTE
		out (c), a 
		;nop 
		;nop 
		;nop
		;nop 
		; Get byte 
		ld c,#0x45
		call _GETNEXTBYTE
		out (c), a 		

		; Loop 
		call _GETNEXTBYTE
		jp PlayLoop

	_SAMPLEWAIT:
		jp endPlay 
	
	_ENDFRAME:
		jp endPlay 

	_ENDSONG: 
		;xor a 
		;ld (_SONGPLAYING),a
		ld hl,(_songStart) 
		ld (_songPointer),hl 
		jp endPlay 

	_ERROR:
		jp _ERROR 
	
	_QWAIT: 
		jp endPlay 
	
	__endasm;
}
