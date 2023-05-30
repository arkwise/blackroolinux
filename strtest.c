/*

░██████╗████████╗██████╗░  ██████╗░██╗░░░░░░█████╗░██╗░░░██╗███████╗██████╗░
██╔════╝╚══██╔══╝██╔══██╗  ██╔══██╗██║░░░░░██╔══██╗╚██╗░██╔╝██╔════╝██╔══██╗
╚█████╗░░░░██║░░░██████╔╝  ██████╔╝██║░░░░░███████║░╚████╔╝░█████╗░░██████╔╝
░╚═══██╗░░░██║░░░██╔══██╗  ██╔═══╝░██║░░░░░██╔══██║░░╚██╔╝░░██╔══╝░░██╔══██╗
██████╔╝░░░██║░░░██║░░██║  ██║░░░░░███████╗██║░░██║░░░██║░░░███████╗██║░░██║
╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝  ╚═╝░░░░░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝ 0.3v

#### Updated 7/5/23 ####

The STR PLayer 0.3v is just bit of an update for ease of user for more novice users.

Basically i updated the psyq.ini file located in \psyq\bin and
		append the following into the stdlib line:
		
		libds.lib libpress.lib

so other users can skip having to add that and then the STRBUILD App now creates all the changes needed to create new video files.

make sure you add the correct amount of frames for your video from the MT32 Program as you can only have 9000 frames per-video or
else the video will not play.

strplay.c example by Lameguy64 of Meido-Tek Productions
	
	Compile with: ccpsx -O -Xo$80010000 strtest.c -o strtest.cpe
	
*/

//#define DEBUG		1	// Uncomment so that the library will printf any error messages

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libcd.h>


// Include the STR player library
#include "strplay.c"


// STR file entry (it is a lot easier to deal with STR files this way)
STRFILE StrFile[] = {
	// File name	Resolution		Frame count
	"\\SOUTHPARK.STR;1", 320, 240, 9319
};


main() {

	// Reset and initialize stuff
	ResetCallback();
	CdInit();
	PadInit(0);
	ResetGraph(0);
	SetGraphDebug(0);
	
	// Play the video in loop
	while (1) {
		
		if (PlayStr(320, 240, 0, 0, &StrFile[0]) == 0)	// If player presses Start
			break;	// Exit the loop
		
	}
	
}