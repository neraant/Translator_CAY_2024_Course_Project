.586
.model flat, stdcall
includelib kernel32.lib
includelib ../Debug/CAY-2024LIB.lib
ExitProcess PROTO : DWORD
extrn WriteInt : proc
extrn WriteLineInt : proc
extrn WriteStr : proc
extrn WriteLineStr : proc
extrn WriteBool : proc
extrn WriteLineBool : proc
extrn strcpy : proc

.const

.code
copy_string proc
    push ebp
    mov ebp, esp
    push esi
    push edi

    mov esi, [ebp + 12]  ; Исходная строка
    mov edi, [ebp + 8]   ; Строка назначения

copy_loop:
    mov al, [esi]        ; Загружаем текущий символ
    mov [edi], al        ; Копируем символ
    
    test al, al          ; Проверка на нулевой байт
    jz copy_done         ; Если нулевой - завершаем
    
    inc esi              ; Переход к следующему символу
    inc edi
    jmp copy_loop

copy_done:
    pop edi
    pop esi
    mov esp, ebp
    pop ebp
    ret 8       
copy_string endp

end
