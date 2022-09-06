# vmrwtool
an attempt to learn

i dont have a clue what im doing but this code when finished should be able to read and write to kernel memory on my iphone 5,2 on ios 10.3.4, h3lix jailbreak seems to have the hgsp4 patch instead of tfp0 so im using this.

problems i have encountered are the fact that hgsp4 always returns c03 which is strange as all my other experience has lead me to believe task ports dont return the same number every time the function is run. my experience with previous tfp0 has shown me that the task port is different on every run.

the second problem encountered is the kslide, dealing with kaslr is a must as i wont be able to correctly read where i want to in memory with out it, my long term solution that i believe will work is a debug or dev iboot and just disable kaslr on boot, or, patch iboot as the kslide is passed to the kernel in the bootargs struct in x0. so i just change it? i think. or use my power of task ports to find the kslide with task_info().

all said this is day 1 and ill finish this project eventually, hopefully.
