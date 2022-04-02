#include <pc88-c.h>

#include "song_b.h"
#include "sprites.h"

#include "graphics.h"
#include "gfx/deck.h"
#include "gfx/roboboar.h"

// TRAP EXAMPLES
// string descriptor 
// what stats it targets, tgt val
// how much damage it will do 

const char desctest[256] = \
"You rummage about for a bit, but find nothing useful.\n\
Just then, a wild rampaging RoboBoar crashes through the brush! \n\
Will you 1) try to pull your gun and shoot it, or 2) abscond while\n\
you are still able?";

typedef struct encounter { 
    const char* desc;
    u8 stats[2];    // 0 1 or 2 to identify stat
    u8 difficulty;  // 3-18
    u8 damage;      // always on fail 
} Encounter;

// e.g. 

// You encounter a raging pit bull!
//  Do you:
//  1. Try to shoot it 
//  2. Run away 
// Difficulty: 8

// All stat checks are 2d6. 

// Result: 4
// Failed!
//  Use effort? (Remaining: n)
// 1 Yes   2 No (Fail)

// NG! You take damage: 2 

// OK! You got away safely. 



const u8 map1[] = { \
    2,2,2,1,1,1,2,1,
    2,2,1,2,1,2,2,2,
    1,2,1,1,1,1,1,1,
    2,2,1,2,1,2,1,2,
    1,2,1,1,2,1,1,1,
    1,2,1,1,1,1,1,1,
    1,2,2,1,2,2,1,1,
    2,1,1,2,2,2,2,2
};



u8 revealedMap[] = { \
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    1,1,0,0,0,0,0,0,
    1,1,1,0,0,0,0,0,
    1,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};


inline void SetIRQs()
{
    SetIOReg(IRQ_LEVEL_SET, 2);
    SetIOReg(IRQ_MASK, 2);
}

extern bool playingSong;
extern struct Song currentSong;
extern signed int ticker;
extern u16 RANDOMSEED;

u32 idleCount;
u8 ctr;

struct tileinfo { 
    XYPos pos;
    u8 tileNo;
};

struct tileinfo loadedTiles[8];
XYPos zeropos;
XYPos player_hex_pos;
u8* tilePtrs[8];
char* res;
s16 randnum;
bool frameDone;
bool playerMoved;
XYPos player_pos;
XYPos targetHex;
//struct xypos zeropos;
u8 lastKey;
u8 inputMode;
Encounter test;

#define EXPLORING 1
#define WAITING 0
#define ENC_SELECT 2
#define GUN 0
#define SPEED 1
#define BOOK 2

// F DEFS
void DrawHexTile(u8 x, u8 y, u8 index);
void DrawCleanGrid();
s16 roll(u8 numDie, u8 sides, s8 mod);
void GameInit();
void DrawFullUI();
void DrawAreaAroundPlayer();
void DrawInputUI();
void BufferInput();
inline void MovementKeys();
void ConfigIntro();
void CPUWAIT(u16 n);
void RunIntro();

#include "intro.h"


enum Inputs { 
    UpRight=9, Right=6, DownRight=3, UpLeft=7, Left=4, DownLeft=1, Confirm=5, Cancel=0
};
    

void main()
{
    IRQ_OFF; 
    // clear vars
    RANDOMSEED += rand();
    ctr = 0;
    idleCount = 0;
    u8 i = 0;
    u8 c = 0;
    playingSong = false;
    
    c = ReadIOReg(SYS_CTL_REGB);
    if(c & V1MODE_FLAG)
    {
        SetCursorPos(0, 0);
        print("V 2   M O D E   O N L Y ");
        while(1){};
    }

    ConfigIntro();
    RunIntro();

    SetCursorPos(10,5);
    print("                                      ");

    SetCursorPos(30,16);
    print("Loading...");

    GameInit();

    // Set up currentSong pointers
    LoadSong(&song[0]);
    playingSong = true;
    ticker = 0;

    // Use this entire sequence to change CRT Rows (bugged)
    //volatile static __sfr __at 0x40 CRTPort;
    //i = CRTPort;
    //if(CRTPort & 0b10)
    //    SetMonitor(15, 25);
    //else 
    //    SetMonitor(24, 25);


    // ALU drawing here
    ExpandedGVRAM_On();     
    EnableALU(FASTMEM_ON);
    
    // Grid test:
    // init blank grid 
    DrawCleanGrid();

    DrawFullUI();

#define GRASS 1
#define TREE 2
#define TEMPGVR_SPRITE_0 0xfe80 
#define TEMPGVR_SPRITE_1 0xfee0

// hero hex area ::
    player_hex_pos.x = 1;
    player_hex_pos.y = 4;
    DrawAreaAroundPlayer();

    // Hero:// MASKING TEST 
    // Remember, the sprites are 384 bytes of bitmap data, but they only take up
    //  96 bytes per plane. 
    // 0xfe80, 0xfee0, ff40, ffa0 room for 4
    player_pos.x = 12;
    player_pos.y = 64;
    
    // deck test 
    //DrawImage_V2(52, 162, &deck[0], 8, 38);
    //DrawImage_V2(67, 162, &deck[0], 8, 38);
    
    // copy out mask
    //ExpandedGVRAM_Copy_On();
    //ALUCopyOut(GVRAM_BASE+(player_pos.y*80)+player_pos.x, TEMPGVR_SPRITE_1, 4, 24);
    // draw sprite 
    //ExpandedGVRAM_On();
    //DrawTransparentImage_V2(player_pos.x, player_pos.y, &librarianSprite[0], 32/8, 24);
    
    // encounter area 
    //DrawTransparentImage_V2(54, 152, &librarianSprite[0], 32/8, 24);
    //DrawTransparentImage_V2(69, 152, &roboboar[0], 32/8, 24);
    // return backup
    //ExpandedGVRAM_Copy_On();
    //ALUCopyIn(TEMPGVR_SPRITE_1, GVRAM_BASE+(player_pos.y*80)+player_pos.x, 4, 24);
    
    // ALU off
    //DisableALU(FASTMEM_OFF);
    //ExpandedGVRAM_Off();   

    PrintExploreUI();
    
    inputMode = EXPLORING;
    
    SetCursorPos(30,16);
    print("               ");

    
    // First, Write the address of the Vblank routine to the CRTC IRQ vector @ f302
    SetCRTC_IRQ((void*)&Vblank);
    SetIRQs();          // And enable the VBL interrupt!
    IRQ_ON              // run once!
    while(1)
    { 
        // wait for 1/60 irq to finish
        while(!frameDone)
        {
            idleCount++;
            BufferInput();    
        };
        IRQ_OFF 
        frameDone = false;
            
        if(lastKey != -1)
        {
            if(inputMode == EXPLORING)
            {
                if(lastKey == Confirm)
                {
                    // search this tile - 1 trap, 20 heal random
                    // forest - 1-2 trap
                }
                else if (lastKey == Cancel)
                {
                    // open menu
                }
                else
                { 
                    MovementKeys();
                    // now we have the target hex location 
                    if(targetHex.x >= 0 && targetHex.x < 8)
                        if(targetHex.y >= 0 && targetHex.y < 8) // make sure its on the map
                        {

                        }
                    ;
                    // if not valid tile, dont take a turn.
                }
            }
        }
        
        // Main Loop
        // modify seed 
        lastKey = -1;
        RANDOMSEED += rand();
        randnum = (targetHex.x << 4) | targetHex.y;
        
        // return 
        IRQ_ON 
    }
}

void Vblank() __critical __interrupt
{
    IRQ_OFF; 

    // MUSIC?
    if(playingSong)
        PlaySong();
    else {
        //SetCursorPos(5, 0);
        //print("OVER!");
    }

    if(playerMoved)
    {
        //copy spr to buffer 

        // copy in old bg 

        //get hex of direction moving
        
        // copy out bg buffer 

        // copy in spr 

        playerMoved = false;
    }
    
    // PRINT IDLE COUNTS
    //SetCursorPos(0, 0);
    //u8* d = byToHex((u8)(idleCount >> 8)); 
    //print(d);
    //u8* d2 = byToHex((u8)(idleCount & 0xff));
    //print(d2);
    SetCursorPos(0, 19);
    res = byToHex(randnum & 0xff);
    print(res);
    
    
    idleCount = 0;

    //SETBANK_MAINRAM();
    //
    frameDone = true;
    SetIRQs(); 
    IRQ_ON;
}


s16 roll(u8 numDie, u8 sides, s8 mod)
{
    s16 r = 0;
    for(u8 i = 0; i < numDie; i++)
        r += (u8)(1 + (rand() & (sides-1))); // 0-255
    r += mod;
    return r;
}

void CPUWAIT(u16 n)
{
    for(s32 i = n * 1000; i > 0; i--) { 
        __asm__("nop");
    }
}

#include "rpj4.c"