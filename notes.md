# vmrwtool
an attempt to learn

i dont have a clue what im doing but this code when finished should be able to read and write to kernel memory on my iphone 5,2 on ios 10.3.4, h3lix jailbreak seems to have the hgsp4 patch instead of tfp0 so im using this.

problems i have encountered are the fact that hgsp4 always returns c03 which is strange as all my other experience has lead me to believe task ports dont return the same number every time the function is run. my experience with previous tfp0 has shown me that the task port is different on every run.

the second problem encountered is the kslide, dealing with kaslr is a must as i wont be able to correctly read where i want to in memory with out it, my long term solution that i believe will work is a debug or dev iboot and just disable kaslr on boot, or, patch iboot as the kslide is passed to the kernel in the bootargs struct in x0. so i just change it? i think. or use my power of task ports to find the kslide with task_info().

all said this is day 1 and ill finish this project eventually, hopefully. wed 7 sept 2022 00:25

# day 2
wed 7 sept 2022 16:51

i have tryed out the program on my 64 bit iphone 7 running ios 13 and it seems to successfully get kernel slide with the function i have implemented today.

32 bit hgsp4:
```
[ >>> ] 0 -> 0 [ error code: 5 ]
[ >>> ] trying hgsp4 ...
[ >>> ] 4 -> c03 [0]

[ >>> ] stage 2, doing stuffs with task port
[ >>> ] task port c03 has KASLR, ofcourse, finding slide ...
[ >>> ] found kernel slide: 0x0
```

64 bit tfp0:
```
[ >>> ] 0 -> 110f [0]

[ >>> ] stage 2, doing stuffs with task port
[ >>> ] task port 110f has KASLR, ofcourse, finding slide ...
[ >>> ] found kernel slide: 0x218677248
```

this is a strange bug and i need to figure it out, obviously its going to have kaslr as iboot passes the aslr to the kernel because it cant give itself its own kaslr. but the h3lix jailbreak isnt a iboot exploit so the kaslr should be implemented as usual. to fix on 32 bit, i think im going to look at the task_info() function on the xnu source of the 10.3.4 ios and see whats up, maybe task_info() was only implemented after 10.3.4, who knows...

# day 4 ish 

wed 14 sep 02:14, after an hour or two work, well not really just going down rabbit holes of problems that dont need fixing, today i managed to complete the read_mem() function with the ability to pass args too vmrwtool, it can read kernel memory with the arg -r [addr] it only reads from that address and only reads 8 bytes. its barebones but it works, even with an address straight from ida or hopper.

the write_mem is giving me weird errors, (ipc/send) something like that, ill figure it out another day, im assuming its nothing got to do with any weird permissions or ppl or anything, as from what i know if it was apple ppl (lockdown) it would just panic the device when i tried to write somewhere im not supposed to. the error said something about the task port but ill look back on that another time.

what im doing now: ive currently tried implementing a hex to ascii converter so when i read memory from the kernel it will auto show what ascii is linked to that byte. i have working functions for this, but since i stole the code online and i dont fully know how it works, since its not working im not sure exactly how to fix it, but im sure this wont be the hardest part to fix.

for now ive ditched hgsp4 support, im to lazy. loading the binary onto my 5 is slow af and i dont really care for it right now. progressively my code is getting messier and messier by the day, this is bound to happen since im not the greatest at c (the reason why im practicing) and my coding organization skills are not the best, (along with my real life ones) anyways, it would be nice to sort things out in a few neat header files.

another thing to include that im not bothered to do right now is an install method in my makefile. im writing this so ill be able to tick the box in the future.

anyways heres some snippets of output from today.

```
heisbssjsiw:~ root# vmrwtool -r 0xfffffff007036fb4
[ >>> ] 0 -> e0f [0]

[ >>> ] stage 2, doing stuffs with task port
[ >>> ] task port e0f has KASLR, ofcourse, finding slide ...
[ >>> ] found kernel slide: 0x12270000
0xfffffff0192a6fb4: 64 6f 6e 61 74 65 64 00
```
help function:
```
heisbssjsiw:~ root# vmrwtool -h
usage:
	-w --write [addr]   write to an address in kmemory
	-h --help   print this help.
```
oh shit i forgot to include the -r bit, oh well, for another time.
