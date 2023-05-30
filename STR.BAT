@ECHO OFF
DEL *.ISO
DEL *.IMG
DEL *.TOC
CLS



echo ░██████╗████████╗██████╗░  ██████╗░██╗░░░░░░█████╗░██╗░░░██╗███████╗██████╗░
echo ██╔════╝╚══██╔══╝██╔══██╗  ██╔══██╗██║░░░░░██╔══██╗╚██╗░██╔╝██╔════╝██╔══██╗
echo ╚█████╗░░░░██║░░░██████╔╝  ██████╔╝██║░░░░░███████║░╚████╔╝░█████╗░░██████╔╝
echo ░╚═══██╗░░░██║░░░██╔══██╗  ██╔═══╝░██║░░░░░██╔══██║░░╚██╔╝░░██╔══╝░░██╔══██╗
echo ██████╔╝░░░██║░░░██║░░██║  ██║░░░░░███████╗██║░░██║░░░██║░░░███████╗██║░░██║
echo ╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝  ╚═╝░░░░░╚══════╝╚═╝░░╚═╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝

echo.
echo.
echo Welcome to the STR PLayer Video Producer
echo.
echo 1: First Convert your video file to a RAW AVI file using ffmpeg
echo.
echo 2: Rip Audio from your Original Video file and save it to a wav file using ffmpeg
echo.
echo 3: Add your raw file to MT32 and convert it to the STR format and use this
echo option to configure a CTI file
echo.
echo 4: Finally we will Convert the IMG file into A Officially Authenticated FAKE PSX Disc.
echo.

IF "%command%"=="1" (
    SET /P infile=Enter the name of the Media file: 
    SET /P outfile=Enter the name of the OUTPUT file: 
  ffmpeg -i "%infile%" -vcodec rawvideo -s 320x240 -r 30 -an "%outfile%"
) ELSE IF "%command%"=="2" (
    SET /P infile=Enter the name of the Media file: 
    SET /P outfile=Enter the name of the OUTPUT WAV file:   
  ffmpeg -i "%infile%" -acodec pcm_s16le -vn "%outfile%"
) ELSE IF "%command%"=="3" (
SET /P config=Enter the name of the CTI file: 
echo !file_content! > %config%.cti

set /p new_str_file=Enter new STR file name:
setlocal enabledelayedexpansion
set "file_content="
for /f "tokens=*" %%a in (%config%.cti) do (
    set "line=%%a"
    if "!line!"=="File BADAPPLE.STR" (
        set "line=File !new_str_file!"
    )
    set "file_content=!file_content!%%a"
)



SET /P config=Enter the name of the CTI file: 
echo !file_content! > %config%.cti
SET /P filename=Enter the name of the IMG file: 

ECHO Please Wait!
ECHO Building %filename%...
BUILDCD -l -i%filename%.IMG %config%%.CTI
REM PAUSE
ECHO Converting %filename%.IMG to %filename%.ISO...
STRIPISO S 2352 %filename%.IMG %filename%.ISO
ECHO.
ECHO The %filename% CD-ROM Image was Created Successfully!
PSXLICENSE /eu /i %filename%.ISO

) ELSE (
  ECHO Error: there are only two choices and you still couldn't get that right.
  EXIT /B 1
)

REM Check if the output file exists
IF EXIST "%outfile%" (
  ECHO Output file '%outfile%' created successfully.
) ELSE (
  ECHO Error: Output file '%outfile%' not found.
)
