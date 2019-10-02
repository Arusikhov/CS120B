/usr/csshare/pkgs/cs120b-avrtools/createProject.sh
git init
git add .
git commit -m "Initializing repository"
git remote add origin https://github.com/Arusikhov/CS120B.git
git remote -v
git push -u origin master
ls
cd Lab2-introToAVR
ls
cd
ls
avr-gcc -mmcu=atmega1284 -Wall -o build/objects/main.elf source/main.c
cd Lab2-introToAVR
ls
cd source/
ls
nano main.c
gedit main.c 
cd
avr-gcc -mmcu=atmega1284 -Wall -o build/objects/main.elf source/main.c
cd Lab2-introToAVR/
avr-gcc -mmcu=atmega1284 -Wall -o build/objects/main.elf source/main.c
cd source
gedit main.c 
cd
cd Lab2-introToAVR/
ls
avr-gcc -mmcu=atmega1284 -Wall -o build/objects/main.elf source/main.c
ls
ls build/objects
cd header
ls
gedit simAVRHeader.h 
cd
cd Lab2-introToAVR/
avr-gcc -mmcu=atmega1284 -I/usr/csshare/pkgs/simavr/ -Iheader/ -D_SIMULATE_ -Wall -Wl,--undefined=_mmcu,--section-start=.mmcu=910000 -o build/objects/main.elf source/main.c
simavr -v -v -v -m atmega1284 -f 8000000 build/objects/main.elf
gtkwave build/results/Lab2-introToAVR_trace.vcd 
m build/results/Lab2_introToAVR_trace.vcd
rm build/results/Lab2_introToAVR_trace.vcd
cd
ls
cd Lab2-introToAVR/
rm build/results/Lab2_introToAVR_trace.vcd
cd build/results/
ls
rm Lab2-introToAVR_trace.vcd 
cd
git commit -m "Update Lab2 02/02/20"
git add .
git add
git add .'

git add --all
git commit -a
