---
categories:
- Basics
date: "2020-05-07"
slug: basics/breakpoints
tags:
- basics
title: Breakpoints
weight: 2
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

## break
To inspect the memory contents of the running process, we need to set a breakpoint.

### break *\<function>
To set a breakpoint at the start of `main`, call `break *main`.

```sh
(gdb) break *main
Breakpoint 1 at 0x4004e7: file basic.c, line 5.
```

This command only works when the binary is unstripped. If a binary is stripped, the symbols (function names) are gone, and the following output will be obtained instead.

```sh
(gdb) break main
Function "main" not defined.
```

### break *\<address>
This command is for breaking in the middle of a function, or when there are no symbols. For example, to set a breakpoint after `printf` is called:

```sh
(gdb) set disassembly-flavor intel
(gdb) disas main
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
   0x000000000040050c <+37>:	mov    eax,0x0               # break here
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
```sh
(gdb) break *0x40050c
Breakpoint 2 at 0x40050c: file basic.c, line 9.
```

> **Don't forget the `*` in front of the symbol or address when calling `break`!**

## info break
This command is to list the breakpoints that are set.

```sh
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004004e7 in main at basic.c:5
2       breakpoint     keep y   0x000000000040050c in main at basic.c:9
```

## enable/disable break [num]
This command is to enable/disable/delete a breakpoint.

Notice that in the output below, the `Enb` field of breakpoint 1 is set to `n`.

```sh
(gdb) disable break 1
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep n   0x00000000004004e7 in main at basic.c:5
2       breakpoint     keep y   0x000000000040050c in main at basic.c:9
```

To set the `Enb` field of breakpoint 1 back to `y`:

```sh
(gdb) enable break 1
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004004e7 in main at basic.c:5
2       breakpoint     keep y   0x000000000040050c in main at basic.c:9
```

To disable all breakpoints:

```sh
(gdb) disable break
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep n   0x00000000004004e7 in main at basic.c:5
2       breakpoint     keep n   0x000000000040050c in main at basic.c:9
```

To enable all breakpoints:

```sh
(gdb) enable break
(gdb) info break
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004004e7 in main at basic.c:5
2       breakpoint     keep y   0x000000000040050c in main at basic.c:9
```

## delete break [num]

To delete a breakpoint:

```sh
(gdb) delete break 1
(gdb) info break
Num     Type           Disp Enb Address            What
2       breakpoint     keep y   0x000000000040050c in main at basic.c:9
```

To delete all breakpoints:

```sh
(gdb) delete break
Delete all breakpoints? (y or n) y
(gdb) info break
No breakpoints or watchpoints.
```

## run
This command is to start running the progranm. With breakpoints set, the program will pause at the first breakpoint encountered.

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


## c (continue)
When the program is paused, use this command to continue running until the next breakpoint.

```sh
(gdb) break *0x40050c
Breakpoint 2 at 0x40050c: file basic.c, line 9.
```
```sh
(gdb) c
Continuing.
10

Breakpoint 2, main () at basic.c:9
9	   return 0;
```
