.586P
.MODEL FLAT, STDCALL
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/CAY-2024ASMLIB.lib
includelib ../Debug/CAY-2024LIB.lib

ExitProcess PROTO : DWORD
copy_string PROTO
extrn WriteInt : proc
extrn WriteLineInt : proc
extrn WriteStr : proc
extrn WriteLineStr : proc
extrn WriteBool : proc
extrn WriteLineBool : proc
extrn dateCAY : proc
extrn randCAY : proc
extrn isEvenCAY : proc
extrn isEmptyCAY : proc
extrn lenCAY : proc
extrn compareCAY : proc

.STACK 4096

.CONST
L0 DWORD 1
L1 DWORD 0
L2 DWORD 1
L3 DWORD 10
L4 DWORD 5
L5 DWORD 100
L6 BYTE " ������ 100!", 0
L7 BYTE " �� ������ 100!", 0
L8 DWORD 11
L9 BYTE "Hello!", 0
L10 BYTE "Hello, World!", 0
L11 DWORD 100
L12 DWORD 2
L13 DWORD 1

.DATA
result_Power DWORD 0
base_main DWORD 0
exponent_main DWORD 0
result_main DWORD 0
currentDate_main BYTE 14 DUP(0)
randomValue_main DWORD 0
isEvenValue_main DWORD 0
isEmptyValue_main DWORD 0
textLength_main DWORD 0
value_main DWORD 0

.CODE

Power_global PROC, exponent_Power : DWORD, base_Power : DWORD
	; �������� ��������
	mov eax, L0
	mov result_Power, eax

	; ������ ����� while
	mov ecx, exponent_Power
start_while0:
	cmp ecx, L1
	jle end_while0
	push result_Power
	push base_Power
	; �������� �� ����� ��� ��������
	pop ebx
	pop eax

	; ���������
	imul eax, ebx
	push eax

	mov result_Power, eax
	push exponent_Power
	push L2
	; �������� �� ����� ��� ��������
	pop ebx
	pop eax

	; ���������
	sub eax, ebx
	push eax

	mov exponent_Power, eax
	mov ecx, exponent_Power
	jmp start_while0

; ����� ����� while
end_while0:
	mov eax, result_Power
	ret
Power_global ENDP

main PROC
START :
	; �������� ��������
	mov eax, L3
	mov base_main, eax

	; �������� ��������
	mov eax, L4
	mov exponent_main, eax

	push base_main
	push exponent_main
	; ����� �������
	call Power_global
	mov result_main, eax
	xor eax, eax

	; ������ ����� �������
	mov eax, result_main
	cmp eax, L5
	jg main0
	jmp else_main0

; ���� if
main0:
	; ����� ��������
	push result_main
	call WriteInt
	; ����� ��������
	push offset L6
	call WriteLineStr
	jmp end_main0

; ���� else
else_main0:
	; ����� ��������
	push result_main
	call WriteInt
	; ����� ��������
	push offset L7
	call WriteLineStr

; ����� ����� if / else
end_main0:
	; ����� �������
	call dateCAY
	push eax
	push offset currentDate_main
	call copy_string

	; ����� �������
	call randCAY
	mov randomValue_main, eax

	push L8
	; ����� �������
	call isEvenCAY
	mov isEvenValue_main, eax

	push offset L9
	; ����� �������
	call isEmptyCAY
	mov isEmptyValue_main, eax

	push offset L10
	; ����� �������
	call lenCAY
	mov textLength_main, eax

	push randomValue_main
	push L11
	; �������� �� ����� ��� ��������
	pop ebx
	pop eax

	; ���������
	sub eax, ebx
	push eax

	push L12
	push L13
	; �������� �� ����� ��� ��������
	pop ebx
	pop eax

	; ���������
	sub eax, ebx
	push eax

	; �������� �� ����� ��� ��������
	pop ebx
	pop eax

	; ���������
	imul eax, ebx
	push eax

	mov value_main, eax
	push 0
	call ExitProcess
main ENDP

end main