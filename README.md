# SPL
## environment
- Windows 10
- [GCC](https://sourceforge.net/projects/mingw-w64/)
- [Flex & Bison](https://sourceforge.net/projects/winflexbison/files/winflexbison3-latest.zip/download)
	- 一定要使用bison 3.0以上的版本
	- 下载解压后将win_flex和win_bison更名为flex和bison，并添加到系统变量中
- [MASM32](http://www.masm32.com/)
	- 当SPL源代码编译成MASM格式的asm文件后，我们需要在Windows上配置MASM环境，进一步将asm文件汇编为可执行程序
	- 首先，先到MASM32官网下载安装包，并安装至本机中，最好安装到C:/
	- 将C:/masm32/bin添加到系统环境变量中，就配置好了MASM汇编环境
	- 为了检验配置的正确性，我们还需要运行以下汇编代码，将其保存为test.asm

```x86asm
.686  

; 定义内存结构和函数调用形式
.model flat, stdcall

; 定义堆栈段大小
.stack 4096  

; C函数库在msvcrt链接库中
; WINAPI在kernel32链接库中
include C:\masm32\include\msvcrt.inc
include C:\masm32\include\kernel32.inc
includelib C:\masm32\lib\msvcrt.lib  
includelib C:\masm32\lib\kernel32.lib

; 数据段
.data
szMsg		db "This is a demo for MASM.", 0ah, 0
prompt1		db "Please input a number: ", 0ah, 0
format		db "%d", 0
number		dd 0
prompt2		db "Number = %d", 0ah, 0
prompt3		db "pause"

; 代码段
; addr = offset
.code
main proc

    ; printf
    invoke  crt_printf, addr szMsg
    invoke  crt_printf, addr prompt1

    ; scanf
    invoke  crt_scanf, addr format, addr number
    invoke  crt_printf, addr prompt2, number
    invoke  crt_system, addr prompt3

    ; 程序终点，可用WinAPI调用或ret
    invoke  ExitProcess, 0

main endp
end main
```
- - 然后，运行以下两条命令行，即可将asm文件汇编成exe可执行文件

```bash
ml /c /coff /Cp test.asm
link /subsystem:console test.obj
```

## git convention
```
<path_to_file>: [ADD/CHANGE/UPDATE/REMOVE] <message>
```

```shell
git commit -m"src/main.cpp: ADD new class"
```