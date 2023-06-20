#!/bin/bash

# Prompt the user for which command to run

# 9000 frames max @ 15 fps 

echo "PLaystation STR Video Encoder 0.1a"
echo "."
echo "PSMCD created by chelson aitcheson
echo ".""
echo "Which ffmpeg command do you want to run?"
echo "1) -vcodec rawvideo -s 320x240 -r 30 -an"
echo "2) -acodec pcm_s16le -vn"
echo "."
echo "."
echo "-r option defines the framerate. 15 fps is always going to run better"
read -p "Enter the number of the command you want to run: " command

# Prompt the user for the input file name
read -p "Enter the input file name: " infile

# Prompt the user for the output file name
read -p "Enter the output file name: " outfile

# Run the selected ffmpeg command with the specified input and output files
if [ "$command" == "1" ]; then
  sudo ./ffmpeg -i "$infile" -vcodec rawvideo -s 320x240 -r 30 -an "$outfile"
elif [ "$command" == "2" ]; then
  sudo ./ffmpeg -i "$infile" -acodec pcm_s16le -vn "$outfile"
else
  echo "Error: there is only two choices and you still couldnt get that right.."
  exit 1
fi

# Check if the output file exists
if [ -f "$outfile" ]; then
  echo "Output file '$outfile' created successfully."
else
  echo "Error: Output file '$outfile' not found."
fi
