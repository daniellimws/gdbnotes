---
categories:
- Basics
date: "2020-05-07"
slug: basics/stepping
tags:
- basics
title: Stepping
weight: 3
---

{{< code language="c" title="basic.c" id="1" expand="Show" collapse="Hide" isCollapsed="true" >}}
// gcc -g basic.c -o basic -no-pie
#include <stdio.h>

int main()
{
   int a = 10;

   printf("%d\n", a);
   return 0;
}
{{< /code >}}

To step through instructions in the debugger, we need to set a breakpoint, then run the program.

```sh
(gdb) break *main
Breakpoint 1 at 0x4004e7: file basic.c, line 5.
```
```sh
(gdb) r
Starting program: /tmp/basic

Breakpoint 1, main () at basic.c:5
5	{
```

Run `disas` to know where we currently are in the program. (This only works if the binary is not stripped.)

```sh
(gdb) disas
Dump of assembler code for function main:
=> 0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

## si [n] (step instruction)
This command is to step a single instruction in GDB.

```sh
(gdb) disas
Dump of assembler code for function main:
=> 0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) si
0x00000000004004e8	5	{
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
=> 0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

To step through multiple instructions, call `s` with the number of steps as an argument. E.g. `si 7`.

```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
=> 0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) si 7
0x0000000000400507	8	   printf("%d\n", a);
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
=> 0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

If `si` is called at a `call` instruction, the debugger will step into the function.

```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
=> 0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) si
0x00000000004003f0 in printf@plt ()
```
```sh
(gdb) disas
Dump of assembler code for function printf@plt:
=> 0x00000000004003f0 <+0>:	jmp    QWORD PTR [rip+0x200c22]        # 0x601018
   0x00000000004003f6 <+6>:	push   0x0
   0x00000000004003fb <+11>:	jmp    0x4003e0
End of assembler dump.
```

As seen above, the program is now inside `printf`.

## ni [n] (next instruction)
To avoid stepping into functions, use `ni` instead.

```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
=> 0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) ni
10
9	   return 0;
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
=> 0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

Unlike `si`, `ni` moves immediately to the next instruction, without stepping into the `printf` function.

Similarly, call `ni n` to take n steps at once.

```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
=> 0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) ni 2
0x0000000000400512	10	}
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
=> 0x0000000000400512 <+43>:	ret
End of assembler dump.
```

## s/n (step/next)
`step` and `next` (or their aliases `s` and `n`) behaves similarly to `si` and `ni`. However, instead of executing machine instructions, they execute lines in the source code.

```sh
(gdb) disas
Dump of assembler code for function main:
=> 0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) n
6	   int a = 10;
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
=> 0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

## until/advance *\<address>

In general, these two commands behave similarly - continue running until the specified address is reached. According to the [documentation](https://sourceware.org/gdb/onlinedocs/gdb/Continuing-and-Stepping.html):


> until location
>
> Continue running your program until either the specified location is reached, or the current stack frame returns. The specified location is actually reached only if it is in the current frame. This implies that until can be used to skip over recursive function invocations.

> advance location
>
> Continue running the program up to the given location. Execution will also stop upon exit from the current stack frame. This command is similar to `until`, but `advance` will not skip over recursive function calls, and the target location doesn’t have to be in the same frame as the current one.

tldr: `until` only works if the location is in the current function frame, `advance` doesn't require the location to be in the current frame.

```sh
(gdb) disas
Dump of assembler code for function main:
=> 0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) advance *0x400507
0x0000000000400507 in main () at basic.c:8
8	   printf("%d\n", a);
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
=> 0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

> **Note:** After calling this command, all breakpoints will be disabled. Remember to turn them back on.

## finish
This command is to step until the end of the function, and return from the function.

```sh
(gdb) disas
Dump of assembler code for function printf@plt:
=> 0x00000000004003f0 <+0>:	jmp    QWORD PTR [rip+0x200c22]        # 0x601018
   0x00000000004003f6 <+6>:	push   0x0
   0x00000000004003fb <+11>:	jmp    0x4003e0
End of assembler dump.
```
```sh
(gdb) finish
Run till exit from #0  0x00000000004003f0 in printf@plt ()
10
main () at basic.c:9
9	   return 0;
```
```sh
(gdb) disas
Dump of assembler code for function main:
   0x00000000004004e7 <+0>:	push   rbp
   0x00000000004004e8 <+1>:	mov    rbp,rsp
   0x00000000004004eb <+4>:	sub    rsp,0x10
   0x00000000004004ef <+8>:	mov    DWORD PTR [rbp-0x4],0xa
   0x00000000004004f6 <+15>:	mov    eax,DWORD PTR [rbp-0x4]
   0x00000000004004f9 <+18>:	mov    esi,eax
   0x00000000004004fb <+20>:	lea    rdi,[rip+0xa2]        # 0x4005a4
   0x0000000000400502 <+27>:	mov    eax,0x0
   0x0000000000400507 <+32>:	call   0x4003f0 <printf@plt>
=> 0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```

## bt (backtrace)
This command is to show the call stack of the current function.

```sh
(gdb) bt
#0  0x00000000004003f0 in printf@plt ()
#1  0x000000000040050c in main () at basic.c:8
```

The output above states that the program is currently inside `printf`, called from `main`.