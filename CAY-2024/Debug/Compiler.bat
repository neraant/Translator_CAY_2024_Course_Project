@echo off 
ml /NOLOGO /c ../CAY-2024ASM/CAY_res.asm 
link /NOLOGO /out:CAY-2024ASM.exe CAY_res.obj CAY-2024LIB.lib CAY-2024ASMLIB.lib /SUBSYSTEM:CONSOLE 
����� ������ ������ �� ����� (ECHO) ��������.
call CAY-2024ASM.exe 
