//////////////////
/// Pc-88 HELLO WORLD
/// (c)2021 Ben Ferguson
/////////////////

#include "pc88-c.h"

#include "img_b.h"
#include "img_r.h"
#include "img_g.h"

// Consts
String strdat = "H e l l o   W o r l d \x00";

PlanarBitmap layeredImage = { 
    img_r, img_g, img_b, 248/8, 100
};

    #define KB_NUM0 0x00
    #define KB_NUM1 0x10
    #define KB_NUM2 0x20
    #define KB_NUM3 0x30
    #define KB_NUM4 0x40
    #define KB_NUM5 0x50
    #define KB_NUM6 0x60
    #define KB_NUM7 0x70

/// Keep me here!
void main()
{   
    //SetIOReg(0x53, 1);  // Hide text 
    // turn on 80 col + color mode 
    //vu8* tgt = (vu8*)(SCREEN_TXT_BASE + 80);
    // 25 line, color, gfc on, n88, 64kb, 200line
    //SetIOReg(0x31, 0b00111011);
    //SetIOReg(0x32, 0b10011001); // default state
    //SetIOReg(0x35, (u8)bit(7)); // GVRAM access on 
    SCREEN_POINTER = (vu8*)SCREEN_TXT_BASE;
    IRQ_OFF
    /* Pal test for layered planar gfx 
    SetIOReg(PAL_REG0, CLR_RED);
    SetIOReg(PAL_REG1, CLR_BLACK);
    SetIOReg(PAL_REG2, CLR_FUSCHIA);
    SetIOReg(PAL_REG3, CLR_GREEN);
    SetIOReg(PAL_REG4, CLR_CYAN);
    SetIOReg(PAL_REG5, CLR_CYAN);
    SetIOReg(PAL_REG6, CLR_CYAN);
    SetIOReg(PAL_REG7, CLR_CYAN);
    */
    //PlanarBitmap* pb = &layeredImage;    
    //DrawPlanarBitmap(pb, 20, 10);
    //SetPixel(360, 180, CLR_BLUE);
    //SetPixel(361, 181, CLR_CYAN);
    //SetPixel(362, 182, CLR_GREEN);
    //SETBANK_MAINRAM()
    
    SetIOReg(EXPANDED_GVRAM_CTRL, (u8)0b10000000); // GVRAM on, comp data off on VRAM ld (???)
    SetIOReg(ALU_MODE_CTRL, (u8)0b11001001);    // ALU on - must be performed after r32 GVRAM enable ^
    SetIOReg(EXPANDED_ALU_CTRL, (u8)0b00000011);     // OR bits on all planes - noop plane 2
    // Set the upper bit and the lower bit to DISABLE writing to that plane.
    // Set only the lower bit to enable OVERWRITE (BIT SET)
    //  In other words, for normal writing, SET the UPPER BIT to HIDE writing to a color plane.
    // e.g. GV2 = 00, GV1 = 01, GV0 = 01 means RESET / SET / SET, or GRB[011], which creates fuschia.
    //  Green plane is set to 0 if it is not. Red and blue plane are always set.
    //This will overwrite any written pixel with PINK color. 
    vu8* vp = (vu8*)0xc100;
    *vp = 0xff;
    SetIOReg(EXPANDED_GVRAM_CTRL, (u8)0); // gvram off 
    
    IRQ_ON

    while(1)
    {
        Wait_VBLANK();
        if(GetKeyDown(KB_RETURN)) print("HI!\x00");
        if(GetKeyDown(KB_Q)) print("BYTE!\x00");
    }
}

bool GetKeyDown(u8 SCANCODE)
{
    u8 r = (SCANCODE & 0xf0) >> 4;
    u8 b = SCANCODE & 0x0f;
    u8 i = ReadIOReg(r);
    if(! (i & bit(b)) ) return true;
    return false;
}

void Wait_VBLANK() __naked 
{
    /* Taken from PC-88 info page:
     http://mydocuments.g2.xrea.com/html/p8/vraminfo.html */

    __asm
    WaitVBlank:
    in	    a,(0x40)        ; SYS_MODE_SENSE
    and     #0x20           ; VBLANK_SIGNAL
    jr      nz,WaitVBlank   ; bit 5 set is vblank interval 
    .wait:
    in      a,(0x40)
    and     #0x20
    jr      z,.wait         ; wait until the signal is off again to be safe!
    ret
    __endasm;
}

void SetCursorPos(u8 x, u8 y)
{
    SCREEN_POINTER = (vu8*)(SCREEN_TXT_BASE + (120 * y) + x);
}

void SetCursorPos40(u8 x, u8 y)
{
    SCREEN_POINTER = (vu8*)(SCREEN_TXT_BASE + (120 * y) + (x * 2));
}

void ClearAttributeRam()
{
    vu8* addr = (vu8*)(SCREEN_ATTR_BASE);
    u16 p = 0xe880;
    for(u8 y = 0; y < 24; y++){
        for(u8 x = 0; x < 20; x++){
            *addr = p;
            addr += 2;   
        }
        addr += 80;
    }
}

void SetTextAttribute(u8 x, u8 y, u8 attr)
{
    vu8* addr = (vu8*)(SCREEN_ATTR_BASE + (y * 120));
    while(*addr != 0x80)
        addr += 2;

    *addr = x; 
    addr++;
    *addr = attr;
}

void putchr(u8 c)
{
    *SCREEN_POINTER = c;
    SCREEN_POINTER++;
}

void putchr40(u8 c)
{
    *SCREEN_POINTER = c;
    SCREEN_POINTER +=2 ;
}

void print(String str)
{
    u8 i = 0;
    while(str[i] != '\x00')
        putchr(str[i++]);
}

void print40(String str)
{
    u8 i = 0;
    while(str[i] != '\x00')
        putchr40(str[i++]);
}


u8 ReadIOReg(u8 r) __naked
{   // Template copied from generated SDCC code and "optimized"
    r;
    __asm 
        ld	iy, #2
        add	iy, sp
        ld a, 0 (iy)
        ld c, a 
        in a, (c)
        ld 0 (iy), a 
        ld	l, 0 (iy)
        ret 
    __endasm;
}

void SetIOReg(u8 r, u8 v) __naked 
{   
    r; v; 
    __asm
        push ix 
        ld ix,#0
        add	ix,sp
    //; 4 (ix) and 5 (ix) contain r and v 
        ld c, 4 (ix)
        ld a, 5 (ix) 
        out (c), a 
        ld l, a
        pop	ix
        ret
    __endasm;
}


void DrawPlaneBMP(const u8* img, u8 plane, u16 x, u16 y, u8 w, u8 h)
{
    if(plane == PLANE_RED)
        SETBANK_RED()
    else if(plane == PLANE_BLUE)
        SETBANK_BLUE()
    else 
        SETBANK_GREEN()
    vu8* p = (vu8*)(0xc000) + (y * 80) + (u8)(x/8);
    const u8* v = img;
    for(u8 y = 0; y < h; y++){
        for(u8 x = 0; x < w; x++){
            *p = *v;
            p++;
            v++;
        }
        p += (80 - w);
    } 
}


void SetPixel(u16 x, u8 y, u8 c)
{
    u8 po = 7 - ((x & 0xff) % 8);
    vu8* p = (vu8*)(0xc000) + (y * 80) + (u8)(x / 8);
        
    if(c & 0b001){
        SETBANK_BLUE()
        *p |= bit(po);
    }
    if(c & 0b010){
        SETBANK_RED()
        *p |= bit(po);
    }
    if(c & 0b100){
        SETBANK_GREEN()
        *p |= bit(po);
    }

}