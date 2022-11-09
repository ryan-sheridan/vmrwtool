# vmrwtool
A command-line tool that uses 
```kern_return_t vm_read_overwrite()``` and soon ```kern_return_t vm_write()``` functions from the ```mach_vm``` api to read and write data to a processes memory by identifying it by pid.
## Usage
```
./macvmrwtool
usage:
    -w --write      [addr] [bytes] [pid]     write to an address in memory to pid
    -r --read       [addr] [bytes] [pid]     read an address in memory from pid
    -h --help       print this help.
```
```
sudo ./macvmrwtool --read 0x19f42db06 0x124 131
[ >>> ] 131 -> b03 [0]
[ >>> ] found proc slide: 0x170
19f42dc76  43 f9 03 11 40 f9 7f  08 1f d6 7f 23 03 d5 f4 |C...@......#...|
19f42dc88  fd 7b 01 a9 fd 43 00  91 73 d0 3b d5 60 12 40 |.{...C..s.;.`.@|
19f42dc9a  00 35 20 ff ff 97 e8  03 00 2a 68 0a 00 f9 fd |.5.......*h....|
19f42dcac  f4 4f c2 a8 ff 0f 5f  d6 7f 23 03 d5 fc 6f ba |.O...._..#...o.|
19f42dcbe  01 a9 f8 5f 02 a9 f6  57 03 a9 f4 4f 04 a9 fd |..._...W...O...|
19f42dcd0  fd 43 01 91 f3 03 06  aa f4 03 05 aa f5 03 04 |.C.............|
19f42dce2  03 aa f9 03 02 aa f8  03 01 aa f7 03 00 aa 08 |...............|
19f42dcf4  3a 00 08 0a e1 03 1a  aa 14 ff ff 97 a0 04 00 |:..............|
19f42dd06  30 37 fb 00 80 52 1b  00 a2 72 1f 00 1b 6b 41 |07...R...r...kA|
19f42dd18  e0 03 17 aa e1 03 1a  aa e2 03 19 aa e3 03 16 |...............|
19f42dd2a  15 aa e5 03 14 aa e6  03 13 aa 06 ff ff 97 f6 |...............|
19f42dd3c  d8 02 50 37 b9 00 88  52 19 00 a2 72 1f 00 19 |..P7...R...r...|
19f42dd4e  00 54 28 88 88 12 08  03 08 0a 69 00 80 52 3f |.T(.......i..R?|
19f42dd60  08 01 98 1a 29 88 80  12 18 01 09 0a e0 03 17 |....)..........|
19f42dd72  18 aa 02 00 80 52 e3  03 16 aa e4 03 15 aa e5 |.....R.........|
19f42dd84  e6 03 13 aa f1 fe ff  97 1f 00 19 6b e0 fe ff |...........k...|
19f42dd96  45 a9 f4 4f 44 a9 f6  57 43 a9 f8 5f 42 a9 fa |E..OD..WC.._B..|
19f42dda8  fc 6f c6 a8 ff 0f 5f  d6 7f 23 03 d5 f4 4f be |.o...._..#...O.|
```
