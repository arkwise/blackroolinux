Store playstation executable on a memory card for use with unirom 8.0.K and above

usage:

py psxthing.exe memcardimage.mcd

Notes:
- Use python 3
- Different format to that used by caetla or mclaunch.
- Max size is just under 15 full blocks ( 122624 / 1DF00 bytes )
- send via e.g. nops /fast /mcup 0 yourthing.mcd COM14
- feel free to add icon support, etc
- may collide with uni if the exe is loaded too high (will give a warning)
