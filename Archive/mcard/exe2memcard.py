#
# Packs an .exe into a memory card image to execute in unirom
#
# Use python 3.7 or 3.9 or something
#
# Big love to the psdev wiki for some table info on this one
# https://www.psdevwiki.com/ps3/PS1_Savedata
#
#

import os
import sys
import time
import struct
import pathlib
import codecs

from io import BytesIO

if sys.version_info[0] < 3:
    print( "Error: This script requires Python 3.x" )
    sys.exit( 1 )


#
# Handle input params
#


# NOTE: remember to usethe inspector or something if you
#       bundle me with pyinstaller - "__file__" will not work.
localDir = os.path.dirname( os.path.abspath( __file__ ) )


#
# Parse args
#
# Could eventually switch to arg parser, but there's just 2 for now
#

def IsDebugging():
    getTrace = getattr(sys, "gettrace", None)
    if not getTrace:
        return False
    return getTrace()

numArgs = len(sys.argv)

if numArgs != 3:

    # if we're in the debugger, default to the demo stuff, for the sake of sanity


    if IsDebugging():
        print( "Debugger override..." )
        sourceFile = "psx_demo.exe"
        destFile = "psx_demo.mcd"
    else:
        print( " " )
        print( "Error: expected exactly 2 params!" )
        print( "       (rememeber to \"quote\" paths with spaces in them)" )
        print( " " )
        print( "py exe2memcard.py <exe> <memcard>" )
        print( " " )
        sys.exit(1)

else:
    sourceFile = sys.argv[1]
    destFile = sys.argv[2]

print( "Source file: {}".format( sourceFile ) )
print( "Dest file  : {}".format( destFile ) )

#
# Some consts n stuff
#

# up to 32 bits
EXE_LAUNCHER_VERSION = 1

FRAME_SIZE = 128  # 0x80
NUM_BLOCKS = 16   # 0x10
BLOCK_SIZE = 8192 # 0x2000

# 16 x 8k blocks
cardSize = BLOCK_SIZE * NUM_BLOCKS

# we can use 14 full blocks (without icon data n shit like that)
# then most of the first available block
freeBytesInBlock = [ 
    0,                          # table of contents, we can't put anything here
    8192 - 0x80 - 0x80,         # first block we can use, minus the header, minus the icon
    8192,                       # block 1
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192,                       # ..
    8192                        # block 15
]

maxFileSize = 0
for blockID in range(len(freeBytesInBlock)):
    maxFileSize += freeBytesInBlock[blockID]


#
# Icon data. Little floppy disk. Looks allright, tbh.
#
# Drawn in psxgameedit 1.60 if you're wondering
#

clutData = [0x58, 0x10, 0xFF, 0x7F, 0xEF, 0x6D, 0x6B, 0x69, 0x0F, 0x3C, 0x64, 0x49, 0xFF, 0x43, 0x7B, 0x03, 0xCF, 0x45, 0x74, 0x56, 0xD7, 0x5E, 0x67, 0x08, 0x88, 0x10, 0x05, 0x00, 
            0xBD, 0x77, 0x00, 0x00
            ]

iconData = [0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x4F, 0x44, 0xA8, 0xAA, 0x88, 0x8A, 0xF4, 0xFF, 0x4F, 0x33, 0xA8, 0xAA, 0x88, 0x8A, 0x43, 0xFF, 0x4F, 0x33, 0xA8, 0xAA, 
            0x88, 0x8A, 0x33, 0xF4, 0x4F, 0x33, 0xA8, 0xAA, 0xAA, 0x8A, 0x33, 0xF4, 0x4F, 0x33, 0x88, 0x88, 0x88, 0x88, 0x33, 0xF4, 0x4F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF4, 
            0x4F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF4, 0x4F, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xF4, 0x4F, 0x33, 0x77, 0x77, 0x77, 0x77, 0x33, 0xF4, 0x4F, 0x73, 0x66, 0x66, 
            0x66, 0x66, 0x37, 0xF4, 0x4F, 0x73, 0x66, 0x6A, 0x6A, 0x69, 0x37, 0xF4, 0x4F, 0x73, 0xA6, 0xA6, 0xA6, 0x66, 0x37, 0xF4, 0x4F, 0x73, 0x66, 0x66, 0x66, 0x66, 0x37, 0xF4, 
            0x4F, 0x74, 0x77, 0x77, 0x77, 0x77, 0x47, 0xF4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            ]


#
# Formatting
#

def CreateFormattedMemoryCard( inBytes ):
    # type: (bytes) -> bytes
    # create a memory card image

    for blockID in range(16):

        headerPos = blockID * 128

        if blockID == 0:
            inBytes[0] = 0x4D  # MC
            inBytes[1] = 0x43
            inBytes[127] = 0x0E  # checksum

            inBytes[0x1F80] = 0x4D  # MC
            inBytes[0x1F81] = 0x43
            inBytes[0x1FFF] = 0x0E  # checksum

        else:

            inBytes[headerPos + 0]   = 0xA0  # nothing here
            inBytes[headerPos + 8]   = 0xFF  # link
            inBytes[headerPos + 9]   = 0xFF  # link
            inBytes[headerPos + 127] = 0xA0  # checksum


#
# ASCII -> ShiftJIS
#
# Based on ASC2SJIS.C in the psyq libs
# The builtin encodings/codecs (including cp932) 
# a pain to work with, so here's Sony's own approach
#
def ToShiftJis( asciiByte ):
    # type: (int) -> int
    # convert an ascii byte to a shiftjis byte pair
    
    asciiTable = [
        [0x824f, 0x30],	# 0-9
        [0x8260, 0x41],	# A-Z
        [0x8281, 0x61],	# a-z
    ]

    # ASCII code to Shift-JIS code transfer table (kigou)
    asciiKTable = [
        0x8140,		#  
        0x8149,		# !
        0x8168,		# "
        0x8194,		# #
        0x8190,		# $
        0x8193,		# %
        0x8195,		# &
        0x8166,		# '
        0x8169,		# (
        0x816a,		# )
        0x8196,		# *
        0x817b,		# +
        0x8143,		# ,
        0x817c,		# -
        0x8144,		# .
        0x815e,		# /
        0x8146,		# :
        0x8147,		# ;
        0x8171,		# <
        0x8181,		# =
        0x8172,		# >
        0x8148,		# ?
        0x8197,		# @
        0x816d,		# [
        0x818f,		# \
        0x816e,		# ]
        0x814f,		# ^
        0x8151,		# _
        0x8165,		# `
        0x816f,		# {
        0x8162,		# |
        0x8170,		# }
        0x8150 		# ~
    ]

    returnVal = 0
    table1 = 0
    table2 = 0

    if ((asciiByte >= 0x20) and (asciiByte <= 0x2f)):
        table2 = 1
    elif ((asciiByte >= 0x30) and (asciiByte <= 0x39)):
        table1 = 0
    elif ((asciiByte >= 0x3a) and (asciiByte <= 0x40)):
        table2 = 11
    elif ((asciiByte >= 0x41) and (asciiByte <= 0x5a)):
        table1 = 1
    elif ((asciiByte >= 0x5b) and (asciiByte <= 0x60)):
        table2 = 37
    elif ((asciiByte >= 0x61) and (asciiByte <= 0x7a)):
        table1 = 2
    elif ((asciiByte >= 0x7b) and (asciiByte <= 0x7e)):
        table2 = 63
    else:
        return returnVal
    
    if table2:
        returnVal = asciiKTable[asciiByte - 0x20 - (table2-1)]
    else:
       returnVal = asciiTable[table1][0] + asciiByte - asciiTable[table1][1]; 

    return returnVal


outBytes = []
for i in range(cardSize):
    outBytes.append( 0x00 )


if os.path.exists( destFile ):
    try:
        os.rename( destFile, destFile + ".bak" + str(time.time()) )
    except Exception as e:
        print( "Error: Could not rename existing file: " + destFile )
        sys.exit( 1 )

try:
    f = open( sourceFile, "rb" )

    f.seek( 0x10 )
    jumpAddr = int.from_bytes( f.read( 4 ), "little" )
    
    f.seek( 0x18 )
    writeAddr = int.from_bytes( f.read( 4 ), "little" )
    
    f.seek( 0x1C )
    fileSize = int.from_bytes( f.read( 4 ), "little" )

    f.seek( 0x30 )
    stackAddr = int.from_bytes( f.read( 4 ), "little" )

    # we don't need to include the header sector from the binary
    sizeNoHeader = fileSize - 0x800

    print( "Jump addr        : 0x%08x" % jumpAddr )
    print( "Write addr       : 0x%08x" % writeAddr )
    print( "Size             : 0x%08x" % fileSize )
    print( "Size (no header) : 0x%08x" % sizeNoHeader )
    print( "StackAddr        : 0x%08x" % stackAddr )

    if jumpAddr > 0x0801B0000:
        print( "Warning: This .exe starts after 0x801B0000" )
        print( "You might not be able to launch it from UniROM" )
        input( "Press a fing to continue anyway" )
    
    if fileSize > maxFileSize:
        print( "Error: File is too big: Max={} (0x{})  Actual={} (0x{})".format(maxFileSize, hex(maxFileSize),fileSize, hex(fileSize)) )
        sys.exit( 1 )

    #
    # Get the framework in place
    #        

    CreateFormattedMemoryCard( outBytes )


    # Quick run through to determine how many blocks we'll need
    # Easier to do it now since some blocks may have more free space
    blocksRequired = 0
    bytesLeft = sizeNoHeader
    while bytesLeft > 0:
        if ( bytesLeft > 0 ):
            blocksRequired += 1
            bytesLeft -= freeBytesInBlock[blocksRequired]

    print( "Blocks required: {}".format(blocksRequired) )

    # Start filling stuff in
    fileReadPos = 0x800
    
    for ix in range( 1, 1+blocksRequired, 1 ):

        #
        # Are we on the first block, last b lock, etc
        #

        # zero indexed, but offset by one, so it all evens out again
        isFirstBlock = ( ix == 1 )
        isLastBlock = ( ix == blocksRequired )
        isMidBlock = ( not isFirstBlock and not isLastBlock )

        #
        # Copy actual block data into place
        #

        thisBlockSize = freeBytesInBlock[ix]
        fileWritePos = BLOCK_SIZE * ix

        # make a little room for the header in the first block
        # or any other blocks if you change the table
        fileWritePos += (BLOCK_SIZE - thisBlockSize)

        f.seek( fileReadPos )
        bytes = f.read( thisBlockSize )

        # We might ask for like 128 bytes but there's only 60 left in the file...
        # so use len(bytes) rather than thisBlockSize
        outBytes[fileWritePos : fileWritePos + len(bytes)] = bytes
                
        #
        # Copy the header stuff in to place
        #
        headerPos = ix * FRAME_SIZE
        blockPos = ix * BLOCK_SIZE
        
        # block type:

        
        if isFirstBlock:
            blockType = 0x51
            linkSize = 0x2000 * blocksRequired # 00 20 00 00 -> 00 E0 01 00
            if NUM_BLOCKS > 1:
                # Id of the next block, but we're offset by 1, so use the current index
                linkId = ix
            else:
                linkId = 0xFFFF
            
        elif isLastBlock:
            blockType = 0x53
            linkSize = 0
            linkId = 0xFFFF

        else:
            blockType = 0x52
            linkSize =  0
            # Id of the next block, but we're offset by 1, so use the current index
            linkId = ix
        
        # 51-53
        outBytes[headerPos + 0] = blockType

        # 00 20 00 00 -> 00 E0 01 00
        sizeBytes = struct.pack( "<I", linkSize )
        outBytes[headerPos + 4 : headerPos + 4 + len(sizeBytes) ] = sizeBytes
        
        # next block link  1 - 15
        # easy one: we're not doing anything fancy
        idBytes = struct.pack( "<H", linkId )
        outBytes[headerPos + 8 : headerPos + 8 + len(idBytes)] = idBytes

        # Put the name in the header slot of the first block only

        if ix == 1:

            # BASLUTS-01234, etc
            # thankfully ascii will do the job here \o/
            # 8 fname chars + .exe here or so
            stem = pathlib.Path( sourceFile ).name[:12]
            outBytes[headerPos + 10 : headerPos + 10 + len(stem)] = stem.encode( "ascii" )
            
            # possible to add more here, but it's not user-facing anyway
            # EXEC identifier
            #eString = "UEXE".encode( "ascii" )
            #outBytes[headerPos + 22: headerPos + 22 + len(eString)] = eString
        
        # } header for first block

        #
        # The actual blocks have a small header too
        # (magic bytes, icon, optional checksums, etc)
        # We need to fill that in for the first of our multibyte blocks
        #

        if ix == 1:


            outBytes[blockPos + 0] = 0x53  # "SC"
            outBytes[blockPos + 1] = 0x43

            outBytes[blockPos + 2] = 0x11  # 1-frame (static) icon

            outBytes[blockPos + 3] = blocksRequired
            
            # some fuckaround bytes at 0xX044 (0x2044, 0x4044, etc)
            # we can use these for:
            # jumpAddr, writeAddr, size
            jumpAddrBytes = struct.pack( "<I", jumpAddr )
            outBytes[blockPos + 0x44 : blockPos + 0x44 + len(jumpAddrBytes)] = jumpAddrBytes
            writeAddrBytes = struct.pack( "<I", writeAddr )
            outBytes[blockPos + 0x48 : blockPos + 0x48 + len(writeAddrBytes)] = writeAddrBytes
            sizeBytes = struct.pack( "<I", sizeNoHeader )
            outBytes[blockPos + 0x4C : blockPos + 0x4C + len(sizeBytes)] = sizeBytes
            
            # and then at 0xX058
            # we can use these for:
            # stackPos, version
            stackPosBytes = struct.pack( "<I", stackAddr )
            outBytes[blockPos + 0x58 : blockPos + 0x58 + len(stackPosBytes)] = stackPosBytes
            versionBytes = struct.pack( "<I", EXE_LAUNCHER_VERSION )
            outBytes[blockPos + 0x5C : blockPos + 0x5C + len(versionBytes)] = versionBytes


            # Shift-jisify the filename
            # 20 chars (40 shift-jis bytes)
            longStem = stem = pathlib.Path( sourceFile ).name[:20]

            bArray = bytearray()
            bArray.extend( map(ord,longStem) )

            for jx in range(len(bArray)):
                asciiByte = bArray[jx]
                jis = ToShiftJis( asciiByte )
                idBytes = struct.pack( ">H", jis )
                writePos= blockPos + 4 + (jx *2) 
                outBytes[ writePos: writePos + len(idBytes) ] = idBytes

            # icon clut + pixel data
            outBytes[ blockPos + 0x60 : blockPos + 0x60 + len(clutData) ] = clutData
            outBytes[ blockPos + 0x80 : blockPos + 0x80 + len(iconData) ] = iconData
        
        # } data for first block

        # increment
        fileReadPos += thisBlockSize
        print( "Wrote block {}".format(ix) )


    #
    # Finally, add in the per-sector checksums
    #

    # 15 normal frames, 20 relocation frames

    for frameId in range( 1, (15+20), 1 ):
        framePos = frameId * FRAME_SIZE

        checkVal = 0x00

        for i in range( FRAME_SIZE -1 ):
            checkVal ^= outBytes[framePos + i]

        #print( "frame {} checksum at {} being set to {}".format(frameId, hex(framePos + 127), hex(checkVal) ) )
        outBytes[framePos + 127] = checkVal

    outIO = bytearray(outBytes)    
    
    if len(outIO) != cardSize:
        print( "Programmer error, probably wrong slice length" )
        expected = cardSize
        got = len(outIO)
        delta = expected - got
        print( "Expected: {} Got {} Delta {}".format(expected, got, delta) )
        print( "Find someone about this" )
        sys.exit(1)

    f = open( destFile, "wb" )
    f.write( outIO )

except Exception as e:
    print( "Exception! e={}".format(e) )
    sys.exit( 1 )

print( "Donesies, bye" )
print( "You may upload via e.g. \"nops /fast /mcup 0 filename.mcd COMx\"" )












