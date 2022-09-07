# vmrwtool
an attempt to learn

i dont have a clue what im doing but this code when finished should be able to read and write to kernel memory on my iphone 5,2 on ios 10.3.4, h3lix jailbreak seems to have the hgsp4 patch instead of tfp0 so im using this.

problems i have encountered are the fact that hgsp4 always returns c03 which is strange as all my other experience has lead me to believe task ports dont return the same number every time the function is run. my experience with previous tfp0 has shown me that the task port is different on every run.

the second problem encountered is the kslide, dealing with kaslr is a must as i wont be able to correctly read where i want to in memory with out it, my long term solution that i believe will work is a debug or dev iboot and just disable kaslr on boot, or, patch iboot as the kslide is passed to the kernel in the bootargs struct in x0. so i just change it? i think. or use my power of task ports to find the kslide with task_info().

all said this is day 1 and ill finish this project eventually, hopefully. wed 7 sept 2022 00:25

#day 2
wed 7 sept 2022 16:51

i have tryed out the program on my 64 bit iphone 7 running ios 13 and it seems to successfully get kernel slide with the function i have implemented today.

32 bit hgsp4:
''' 
[ >>> ] 0 -> 0 [ error code: 5 ]
[ >>> ] trying hgsp4 ...
[ >>> ] 4 -> c03 [0]

[ >>> ] stage 2, doing stuffs with task port
[ >>> ] task port c03 has KASLR, ofcourse, finding slide ...
[ >>> ] found kernel slide: 0x0
'''

64 bit tpf0:
'''
[ >>> ] 0 -> 110f [0]

[ >>> ] stage 2, doing stuffs with task port
[ >>> ] task port 110f has KASLR, ofcourse, finding slide ...
[ >>> ] found kernel slide: 0x218677248
'''

this is a strange bug and i need to figure it out, obviously its going to have kaslr as iboot passes the aslr to the kernel because it cant give itself its own kaslr. but the h3lix jailbreak isnt a iboot exploit so the kaslr should be implemented as usual. to fix on 32 bit, i think im going to look at the task_info() function on the xnu source of the 10.3.4 ios and see whats up, maybe task_info() was only implemented after 10.3.4, who knows...
