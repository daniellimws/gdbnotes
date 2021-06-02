---
categories:
- Basics
date: "2020-05-07"
slug: basics/usage
tags:
- basics
title: Usage
weight: 001
---

{{< code language="c" title="basic.c" id="1" expand="Show" collapse="Hide" isCollapsed="false" >}}
// gcc -g basic.c -o basic -no-pie
#include <stdio.h>

int main()
{
   int a = 10;

   printf("%d\n", a);
   return 0;
}
{{< /code >}}

> In these examples, compile the code above with the `-g` flag to enable debugging information, and `-no-pie` to disable compiling as a position-independent executable (which makes debugging less straightforward).


## start gdb
Start using GDB on a binary with `gdb -q <program>`. The `-q` option tells GDB to start in quiet mode. Try running GDB without it.

```sh
❯ gdb -q ./basic
Reading symbols from ./basic...done.
(gdb)
```

## info functions
Normally when debugging or reverse engineering, the list of functions is already known, either from the source code or from a disassembler like IDA, Ghidra or Rizin/R2.
Nonetheless, this is a command worth knowing.

`info functions` lists the functions in the binary.

```sh
(gdb) info functions
All defined functions:

File basic.c:
int main();

Non-debugging symbols:
0x00000000004003c8  _init
0x00000000004003f0  printf@plt
0x0000000000400400  _start
0x0000000000400430  _dl_relocate_static_pie
0x0000000000400440  deregister_tm_clones
0x0000000000400470  register_tm_clones
0x00000000004004b0  __do_global_dtors_aux
0x00000000004004e0  frame_dummy
0x0000000000400520  __libc_csu_init
0x0000000000400590  __libc_csu_fini
0x0000000000400594  _fini
```


## list \<function>
If a binary is compiled with the `-g` flag, the `list` command can be used to view the source of the file. For example:

```sh
(gdb) list main
1	// gcc -g basic.c -o basic -no-pie
2	#include <stdio.h>
3
4	int main()
5	{
6	   int a = 10;
7
8	   printf("%d\n", a);
9	   return 0;
10	}
```

## disas \<function>
The `disas` command can be used to view the disassembly of the program.

```sh
(gdb) disas main
Dump of assembler code for function main:
   0x000000000000064a <+0>:	push   %rbp
   0x000000000000064b <+1>:	mov    %rsp,%rbp
   0x000000000000064e <+4>:	sub    $0x10,%rsp
   0x0000000000000652 <+8>:	movl   $0xa,-0x4(%rbp)
   0x0000000000000659 <+15>:	mov    -0x4(%rbp),%eax
   0x000000000000065c <+18>:	mov    %eax,%esi
   0x000000000000065e <+20>:	lea    0x9f(%rip),%rdi        # 0x704
   0x0000000000000665 <+27>:	mov    $0x0,%eax
   0x000000000000066a <+32>:	callq  0x520 <printf@plt>
   0x000000000000066f <+37>:	mov    $0x0,%eax
   0x0000000000000674 <+42>:	leaveq
   0x0000000000000675 <+43>:	retq
End of assembler dump.
```

### set disassembly-flavor intel
By default, GDB displays the disassembly output in the AT&T syntax. To use the more familiar-looking Intel syntax, call `set disassembly-flavor intel`.

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
   0x000000000040050c <+37>:	mov    eax,0x0
   0x0000000000400511 <+42>:	leave
   0x0000000000400512 <+43>:	ret
End of assembler dump.
```
