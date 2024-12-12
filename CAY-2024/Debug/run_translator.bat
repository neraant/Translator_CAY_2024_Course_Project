@TITLE CAY-2024
@echo off
chcp 1251>nul
cls

:: Проверка наличия входного файла
if NOT EXIST in.txt (
    echo Ошибка: отсутствует файл in.txt
    pause
    exit /b
)

:: Выполнение программы с фиксированными именами файлов
call CAY-2024.exe -in:in.txt -log:log.txt -out:../CAY-2024ASM/CAY_res.asm

:: Создание файла компиляции
echo @echo off > Compiler.bat
echo ml /NOLOGO /c ../CAY-2024ASM/CAY_res.asm >> Compiler.bat
echo link /NOLOGO /out:CAY-2024ASM.exe CAY_res.obj CAY-2024LIB.lib CAY-2024ASMLIB.lib /SUBSYSTEM:CONSOLE >> Compiler.bat
echo >> Compiler.bat
echo call CAY-2024ASM.exe >> Compiler.bat

:: Запуск компиляции через VS DevCmd
%comspec% /c ""C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -no_logo & Compiler.bat"

pause
exit
