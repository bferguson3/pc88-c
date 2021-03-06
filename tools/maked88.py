# maked88.py 

# by @retrodevdiscord, CC0
# usage:
# $ python3 MakeD88.py <diskname> [<filename> <C><H><R>]
#  Creates a blank 2D PC88 disk with name <diskname>, or
#  optionally adds <filename> to the disk at location CHR




import sys 
import os 
import math 

class d88sector():
    def __init__(self):
        self.c = 0
        self.h = 0
        self.r = 0
        self.n = 0
        self.sectors = 0
        self.density = 0
        self.deleted = 0
        self.fdc = 0
        self.bytesize = 0
        self.bytes = []
        pass 
    def __str__(self):
        o = 'C H R: ' + str(self.c) + ' '\
                  + str(self.h) + ' '\
                  + str(self.r) + ' '\
                  + '\n'
        o += 'Size byte (128 << n): ' + str(self.n) + '\t'
        o += 'Sectors this track: ' + str(self.sectors) + '\n'
        o += 'Density: ' + str(self.density) + '\t' 
        o += 'Delete flag: ' + str(self.deleted) + '\n' 
        o += 'FDC status: ' + str(self.deleted) + '\t'
        o += 'Total size (b): ' + str(self.bytesize) + '\n'
        return o


class d88track():
    def __init__(self):
        self.sectors = [] 
        self.sectorcount = 0
    def __str__(self):
        o = ''
        i = 0
        while i < self.sectorcount:
            o += str(i+1) + ':' + str(self.sectors[i].r) + ' '
            i += 1
        return o
    
class d88disk():
    def __init__(self, fn = -1, sz = 0x55eb0):
        if(fn != -1):
            f = open(fn, 'rb')
            self.bytes = f.read()
            f.close()
        else:
            self.bytes = []
            i = 0
            while i < sz:
                self.bytes.append(0)
                i += 1
        
        self.diskname = ''
        self.pointer = 0
        self.writeprotect = 0
        self.mediatype = 0
        self.disksize = len(self.bytes)
        
        self.tracktable = []
        self.tracks = []
        
        if(fn != -1):
            f = open(fn, 'rb')
            self.bytes = f.read() 
            f.close()
            self.GetDiskInfo()
            self.GetTrackInfo()
            self.PopulateSectors()
    ###
    def PopulateSectors(self):
        f = 0
        while f < len(self.tracktable):
            i = self.tracktable[f] 
            tr = d88track() # make new track
            # how  many sectors? read from first sector:
            tr.sectorcount = self.bytes[i+4] | (self.bytes[i+5] << 8) 
            sn = 0
            while sn < tr.sectorcount:
                s = d88sector() # make a sector
                s.c = self.bytes[i] 
                i += 1
                s.h = self.bytes[i] 
                i += 1
                s.r = self.bytes[i] 
                i += 1
                s.n = self.bytes[i] # 128 < s.n = byte approximation
                i += 1
                s.sectors = self.bytes[i] | (self.bytes[i+1] << 8) 
                i += 2
                s.density = self.bytes[i] 
                i += 1
                s.deleted = self.bytes[i] 
                i += 1 
                s.fdc = self.bytes[i] 
                i += 6
                s.bytesize = self.bytes[i] + (self.bytes[i+1] << 8) 
                i += 2
                j = 0
                while j < s.bytesize:
                    s.bytes.append(self.bytes[i])
                    i += 1
                    j += 1
                tr.sectors.append(s)
                sn += 1
            self.tracks.append(tr)
            f += 1
        
    def GetDiskInfo(self):
        i = 0
        while i < 0x10:
            if(self.bytes[i] == 0):
                i = 0x10 
            else:
                self.diskname = self.diskname + chr(self.bytes[i])
            i += 1
        self.writeprotect = self.bytes[0x1a] 
        self.mediatype = self.bytes[0x1b] 
        self.disksize = self.bytes[0x1c] + (self.bytes[0x1d]<<8) + (self.bytes[0x1e]<<16)
    ###
    
    def GetTrackInfo(self):
        i = 0x20
        trofs = -1
        
        while (trofs != 0):
            trofs = self.bytes[i] | (self.bytes[i+1] << 8) | (self.bytes[i+2] << 16)
            self.tracktable.append(trofs)
            i += 4
        self.tracktable.pop() ## remove last element		
    ###
    def copy(self, cop = -1):
        if(cop == -1):
            cop = d88disk(sz=self.disksize)
        i = 0
        while i < 16 and i < (len(self.diskname)):
            cop.bytes[i] = chr(self.diskname[i])
        cop.bytes[0x1a] = self.writeprotect 
        cop.bytes[0x1b] = self.mediatype
        cop.bytes[0x1c] = self.disksize & (0xff) 
        cop.bytes[0x1d] = (self.disksize & 0xff00) >> 8
        cop.bytes[0x1e] = (self.disksize & 0xff0000) >> 16 
        #print('disk size to copy', self.disksize)
        # now copy track table
        i = 0x20
        tn = 0
        while tn < len(self.tracks):
            cop.bytes[i] = self.tracktable[tn] & 0xff 
            cop.bytes[i+1] = (self.tracktable[tn] & 0xff00) >> 8
            cop.bytes[i+2] = (self.tracktable[tn] & 0xff0000) >> 16
            i += 4
            tn += 1
        # skip to 2b0, write each track header, then each track bytes
        tt = 0
        #print(len(self.tracks))
        i = 0x2b0
        while tt < len(self.tracks):
            #print('copying track', tt)
            ss = self.tracks[tt].sectorcount
            #print('sector count: ', ss)
            si = 0
            while si < ss:
                cop.bytes[i] = self.tracks[tt].sectors[si].c
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].h 
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].r
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].n 
                i += 1
                cop.bytes[i] = self.tracks[tt].sectorcount & 0xff
                i += 1
                cop.bytes[i] = (self.tracks[tt].sectorcount & 0xff00) >> 8 
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].density
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].deleted
                i += 1
                cop.bytes[i] = self.tracks[tt].sectors[si].fdc
                i += 1
                i += 5
                cop.bytes[i] = self.tracks[tt].sectors[si].bytesize & 0xff
                i += 1
                cop.bytes[i] = (self.tracks[tt].sectors[si].bytesize & 0xff00) >> 8
                i += 1
                bc = 0
                while bc < self.tracks[tt].sectors[si].bytesize:
                    cop.bytes[i] = self.tracks[tt].sectors[si].bytes[bc]
                    i += 1
                    bc += 1
                si += 1
            tt += 1
            
        cop.GetDiskInfo()
        cop.GetTrackInfo()
        cop.PopulateSectors()
        return cop
    ##
    def Format(self, name=''):
        # write disk header
        i = 0
        #self.diskname = name 
        while i < len(self.diskname):
            self.bytes[i] = ord(self.diskname[i])#self.diskname[i] 
            i += 1
        print(self.disksize)
        self.bytes[0x1a] = 0#self.writeprotect
        self.bytes[0x1b] = 0#self.mediatype
        self.bytes[0x1c] = self.disksize & 0xff
        self.bytes[0x1d] = (self.disksize & 0xff00) >> 8
        self.bytes[0x1e] = (self.disksize & 0xff0000) >> 16
        # track pointers
        i = 0x20
        ofs = 0x2b0
        ct = 0
        gap = 4352 # TODO
        while ct < 80:#i < 0x2b0:
            self.bytes[i] = (ofs & 0xff)
            self.bytes[i+1] = (ofs & 0xff00) >> 8
            self.bytes[i+2] = (ofs & 0xff0000) >> 16
            ofs += (256+16)
            ct += 1
            i += 4
        # sector headers: i = 0x2b0 ...
        i = 0x2b0
        #C, H, R, n, (0010), dd, del, fdc, 0, 0, 0, 0, 0, (0100)
        c = 0
        while c < 0x28:
            print('c',c)
            h = 0
            while h < 2:
                print('h',h)
                r = 1
                while r <= 16:
                    print('r',r)
                    self.bytes[i] = c 
                    self.bytes[i+1] = h 
                    self.bytes[i+2] = r 
                    self.bytes[i+3] = 1 # TODO n
                    self.bytes[i+4] = 0x10 
                    self.bytes[i+0xe] = 0 
                    self.bytes[i+0xf] = 1 #TODO bytes
                    r += 1
                    i += (256+16)
                h += 1
            c += 1

    def WriteBytes(self, fn):
        f = open(fn, 'wb')
        i  =0
        while i < len(self.bytes):
            f.write(bytes([self.bytes[i]]))
            i += 1
        f.close()        
        print(fn, 'written successfully.')

'''
def WriteBlank2D():
    # WRITE DISK HEADER
    ###
    f = open(sys.argv[1], 'wb')
    # WRITE DISK NAME
    disk_name = "                "
    f.write( bytearray(disk_name, 'ascii') )
    # null
    f.write(bytes([0]))
    i = 0x11
    while i <= 0x19:
        f.write(bytes([0]))
        i += 1
    # write prot flag
    f.write(bytes([0]))
    # 2D
    f.write(bytes([0]))
    # ^ disk size:
    dsize = 348848
    f.write(bytes([dsize & 0xff]))
    f.write(bytes([(dsize & 0xff00)>>8]))
    f.write(bytes([(dsize & 0xff0000)>>16]))
    f.write(bytes([(dsize & 0xff000000)>>24]))
    # print sector locations
    headerloc = 688 
    i = 0
    while i < 80:
        v = headerloc + (0x1100 * i) # b0 35 00 00 
        bn = 0
        while bn < 4:
            o = v & (0xff << (bn * 8))
            o = o >> (bn * 8)
            f.write(bytes([o]))
            bn += 1
        # each head (group of sectors)
        i += 1
    # 0x150 null bytes
    i = 0x160
    while i < 0x2b0:
        f.write(bytes([0]))
        i += 1

    # WRITE SECTOR 
    c = 0
    h = 0
    r = 0
    while c < 0x28:
        h = 0
        while h < 2:
            r = 1
            while r <= 16:
                f.write(bytes([c])) # cylinder - 28 per disk
                f.write(bytes([h])) # head - 2 per disk, (0 and 1)
                f.write(bytes([r])) # record/sector id - 16 per head (0x01 to 0x10!)
                # bytes / 256 in sector (always 1)
                f.write(bytes([1]))
                # num. sectors in disk - always 10 00
                f.write(bytes([0x10]))
                f.write(bytes([0]))
                # double density 00 = true
                f.write(bytes([0]))
                # deleted flag
                f.write(bytes([0]))
                # FDC code (0 = ok)
                f.write(bytes([0]))
                # reserved null bytes
                i = 0
                while i < 5:
                    f.write(bytes([0]))
                    i += 1
                # sector data size (256)
                f.write(bytes([0]))
                f.write(bytes([1]))
                # fill with FF
                i = 0
                while i < 256:
                    f.write(bytes([0xff]))
                    i += 1
                r += 1
            h += 1
        c += 1
    f.close()
'''

def AddFile(filename, cyl, hed, rec):
    inb = open(sys.argv[1], 'rb')
    indat = bytearray(inb.read())
    inb.close() 
    rsize = (256 + 16)
    hsize = rsize * 16
    csize = hsize * 2
    start = (0x2b0 + 16) - rsize 
    start += (rec * rsize) + (hed * hsize) + (cyl * csize)
    # iterate every 256 bytes, then skipping 16... 
    print("Embedding at location " + hex(start))
    tow = open(sys.argv[2], 'rb')
    outdat = bytearray(tow.read())
    tow.close()
    i = 0
    bc = 0
    while i < len(outdat):
        indat[start+bc] = outdat[i]
        i += 1
        bc += 1
        if (i % 256 == 0):
            bc += 16
    if(indat[start+bc] == 0xff):
        indat[start+bc] = 0xc9
    f = open(sys.argv[1], 'wb')
    f.write(indat)
    f.close()


if(len(sys.argv) == 1):
    print('MakeD88.py\n\tUsage: $ python3 maked88.py <diskname> [<filename> <C> <H> <R>]\n\
Where:\n<diskname> is the name of the disk file.\n  (Without further arguments this generates a blank 2D PC-88 disk.)\n\
<filename> File to add to target CHR location\n<C> Target cylinder (0-39)\n<H> Target head (0 or 1)\n<R> Record no (1-16)\n\
e.g. $ python3 maked88.py new.d88 ipl.bin 0 0 1\n Adds ipl.bin to the boot sector, and makes the disk if it does not exist.')
elif(len(sys.argv) > 2):
    if(len(sys.argv) != 6):
        print('Error parsing arguments. Run `python3 maked88.py` to see instructions.')
    else:
        if(os.path.exists(sys.argv[1]) == False):
            print(sys.argv[1] + ' not found. Creating.')
            new = d88disk(sz=348848) 
            new.Format() 
            new.WriteBytes(sys.argv[1])
        print('Adding file ' + sys.argv[2] + ' to ' + sys.argv[1] + '...', end=' ')
        AddFile(sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5]))
else:
    print('Creating blank 2D disk ' + sys.argv[1] + '...')
    new = d88disk(sz=348848) 
    new.Format() 
    new.WriteBytes(sys.argv[1])
