rm main.obj cgalib.obj
wcc -bt=DOS main.c
wcc -bt=DOS cgalib.c
wcl main.obj cgalib.obj
mv main.exe lvm.exe
