---
categories:
- Basics
date: "2020-05-07"
slug: basics/inspecting
tags:
- basics
title: Inspecting
weight: 004
---


{{< code language="c" title="basic2.c" id="1" expand="Show" collapse="Hide" isCollapsed="false" >}}
// gcc -g basic.c -o basic -no-pie
#include <stdio.h>

typedef struct my_struct {
    char a;
    short b;
    int c;
    double d;
    char e[20];
    long long f;
} my_struct;

int main()
{
    my_struct m = {
        .a = '6',
        .b = 56,
        .c = 1337,
        .d = 3.1415,
        .e = {'h', 'e', 'l', 'l', 'o', ' ', 'f', 'r', 'i', 'e', 'n', 'd', '\0'},
        .f = 0x1337133713371337
    };

    printf("%p\n", &m);
    return 0;
}
{{< /code >}}

In this example, we set a breakpoint at the instruction after the `printf` call, and inspect the memory contents of `m`.

```sh
(gdb) disas main
Dump of assembler code for function main:
...
   0x00000000004005fb <+164>:	lea    rdi,[rip+0xb6]        # 0x4006b8
   0x0000000000400602 <+171>:	mov    eax,0x0
   0x0000000000400607 <+176>:	call   0x400460 <printf@plt>
   0x000000000040060c <+181>:	mov    eax,0x0               # break here
   0x0000000000400611 <+186>:	mov    rdx,QWORD PTR [rbp-0x8]
...
End of assembler dump.
```
```sh
(gdb) break *0x40060c
Breakpoint 1 at 0x40060c: file basic2.c, line 25.
```
```sh
(gdb) r
Starting program: /tmp/basic2
0x7fffffffe3c0

Breakpoint 1, main () at basic2.c:25
25	    return 0;
```

As seen in the program output after running `r`, `m` is stored in `0x7fffffffe3c0`.

## info reg
This command prints the names and values of all registers, except floating-point and vector registers (like `st`, `xmm`, `ymm`).

```sh
(gdb) info registers
rax            0xf	15
rbx            0x0	0
rcx            0x0	0
rdx            0x0	0
rsi            0x602260	6300256
rdi            0x1	1
rbp            0x7fffffffe400	0x7fffffffe400
rsp            0x7fffffffe3c0	0x7fffffffe3c0
r8             0x0	0
r9             0x0	0
r10            0x602010	6299664
r11            0x246	582
r12            0x400470	4195440
r13            0x7fffffffe4e0	140737488348384
r14            0x0	0
r15            0x0	0
rip            0x40060c	0x40060c <main+181>
eflags         0x206	[ PF IF ]
cs             0x33	51
ss             0x2b	43
ds             0x0	0
es             0x0	0
fs             0x0	0
gs             0x0	0
```

To print **every** register, call `info all-reg`. However, it is not very practical to do so.

{{< code language="gdb" title="info-all-reg" id="2" expand="Show" collapse="Hide" isCollapsed="true" >}}
(gdb) info all-reg
rax            0xf	15
rbx            0x0	0
rcx            0x0	0
rdx            0x0	0
rsi            0x602260	6300256
rdi            0x1	1
rbp            0x7fffffffe400	0x7fffffffe400
rsp            0x7fffffffe3c0	0x7fffffffe3c0
r8             0x0	0
r9             0x0	0
r10            0x602010	6299664
r11            0x246	582
r12            0x400470	4195440
r13            0x7fffffffe4e0	140737488348384
r14            0x0	0
r15            0x0	0
rip            0x40060c	0x40060c <main+181>
eflags         0x206	[ PF IF ]
cs             0x33	51
ss             0x2b	43
ds             0x0	0
es             0x0	0
fs             0x0	0
gs             0x0	0
st0            0	(raw 0x00000000000000000000)
st1            0	(raw 0x00000000000000000000)
st2            0	(raw 0x00000000000000000000)
st3            0	(raw 0x00000000000000000000)
st4            0	(raw 0x00000000000000000000)
st5            0	(raw 0x00000000000000000000)
st6            0	(raw 0x00000000000000000000)
st7            0	(raw 0x00000000000000000000)
fctrl          0x37f	895
fstat          0x0	0
ftag           0xffff	65535
fiseg          0x0	0
fioff          0x0	0
foseg          0x0	0
fooff          0x0	0
fop            0x0	0
mxcsr          0x1f80	[ IM DM ZM OM UM PM ]
ymm0           {v8_float = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, v4_double = {0x0, 0x0, 0x0, 0x0}, v32_int8 = {0x25 <repeats 16 times>, 0x0 <repeats 16 times>},
---Type <return> to continue, or q <return> to quit---
{{< /code >}}

## p/[format] (print)
This command can be used to print values in different representations, register contents, or evaluate arithmetic expressions.

### p \<value>
By default, the given value is printed in its decimal form.

```sh
(gdb) p 0x1337
$1 = 4919
```
`p` can be given the following format modifiers, to print the value in different representations:
- `p/t` binary
- `p/o` octal
- `p/d` decimal
- `p/u` unsigned decimal
- `p/x` hex
- `p/c` char
- `p/f` float

```sh
(gdb) p/t 0x41
$2 = 1000001
```
```sh
(gdb) p/o 0x41
$3 = 0101
```
```sh
(gdb) p/d 0x41
$4 = 65
```
```sh
(gdb) p/x 0x41
$5 = 0x41
```
```sh
(gdb) p/c 0x41
$6 = 65 'A'
```


### p $\<reg>
This command prints individual registers. E.g. `p $rax` to print the value of the `rax` register.

```sh
(gdb) p $rax
$7 = 15
```

Similarly, the various format modifiers above can be used.

### p \<expression>
This command is also very handy for evaluating expressions right inside GDB.

```sh
(gdb) p/x ($rax*10+0x41)<<1
$8 = 0x1ae
```
```sh
(gdb) p/x $8-0xe
$9 = 0x1a0
```

### p \<symbol>
If the binary is not stripped, this command can print the location of a given symbol.

```sh
(gdb) p main
$10 = {int ()} 0x400557 <main>
```
```sh
(gdb) p free
$11 = {void (void *)} 0x7ffff7df0620 <free>
```

## x/[length][size][format] (examine)
This command displays the memory contents at a given address. More **format** modifiers are available:

- `x/o` octal
- `x/x` hexadecimal
- **`x/d` decimal - default**
- `x/u` unsigned decimal
- `x/t` binary
- `x/f` floating point
- `x/a` address
- `x/c` char
- `x/s` string
- `x/i` instruction

But this is usually not enough. The following size modifiers can be used to specify the **size** of the value in memory:

- `x/b` byte
- `x/h` halfwords (2 bytes)
- **`x/w` words (4 bytes) - default**
- `x/g` giant words (8 bytes)

Lastly, a **length** modifier can be used to specify the number of values to display. If not given, it is 1 by default.

These 3 modifiers can be combined together, e.g.

- `x/4bx 0x7fffffffe3c0`
- `x/2gd 0x7fffffffe3c8`
- `x/s 0x7fffffffe3e0`

{{< code language="c" title="my_struct" id="3" expand="Show" collapse="Hide" isCollapsed="true" >}}
// gcc -g basic.c -o basic -no-pie
#include <stdio.h>

typedef struct my_struct {
    char a;
    short b;
    int c;
    double d;
    char e[20];
    long long f;
} my_struct;

my_struct m = {
    .a = '6',
    .b = 56,
    .c = 1337,
    .d = 3.1415,
    .e = {'h', 'e', 'l', 'l', 'o', ' ', 'f', 'r', 'i', 'e', 'n', 'd', '\0'},
    .f = 0x1337133713371337
};
{{< /code >}}

```sh
(gdb) x/c &m.a
0x7fffffffe3c0:	54 '6'
```
```sh
(gdb) x/hd &m.b
0x7fffffffe3c2:	56
```
```sh
(gdb) x/w &m.c
0x7fffffffe3c4:	1337
```
```sh
(gdb) x/gf &m.d
0x7fffffffe3c8:	3.1415000000000002
```
```sh
(gdb) x/s &m.e
0x7fffffffe3d0:	"hello friend"
(gdb) x/20bx &m.e
0x7fffffffe3d0:	0x68	0x65	0x6c	0x6c	0x6f	0x20	0x66	0x72
0x7fffffffe3d8:	0x69	0x65	0x6e	0x64	0x00	0x00	0x00	0x00
0x7fffffffe3e0:	0x00	0x00	0x00	0x00
```
```sh
(gdb) x/gx &m.f
0x7fffffffe3e8:	0x1337133713371337
```

In the example above, it is convenient to directly reference the variable (`m`) and its elements (`m.a`, `m.b`). Remember to add `&` before the symbol (`&m.a`) because `x` is expecting an address.

However, this only works when the binary is compiled with debugging information (with the `-g` flag).

```bash
❯ gcc basic2.c -o basic2 -no-pie
```
```sh
(gdb) x/c &m.a
No symbol "m" in current context.
```

Anyways, this is common in reverse engineering. Earlier, `printf` printed the address of `m` which is located at `0x7fffffffe3c0`. We can call `x` on this address instead. However, it is more troublesome now to access each element in the struct, as we need to manually calculate the offset of each element.

```sh
(gdb) x/20wx 0x7fffffffe3c0
0x7fffffffe3c0:	0x00380036	0x00000539	0xc083126f	0x400921ca
0x7fffffffe3d0:	0x6c6c6568	0x7266206f	0x646e6569	0x00000000
0x7fffffffe3e0:	0x00000000	0x00000000	0x13371337	0x13371337
0x7fffffffe3f0:	0xffffe4e0	0x00007fff	0x7ec63500	0x104e20db
0x7fffffffe400:	0x00400630	0x00000000	0xf7a05b97	0x00007fff
```

In the example above, as I do not yet know the size of a struct, I chose a reasonably big number (20) of words to be displayed. I can see that the struct ends at `0x7fffffffe3ef` because it contains the last element of `m` (`0x1337133713371337`).

I can also identify the location of the other elements in the struct from the output above.

```sh
(gdb) x/c 0x7fffffffe3c0
0x7fffffffe3c0:	54 '6'
```
```sh
(gdb) x/hd 0x7fffffffe3c0+2
0x7fffffffe3c2:	56
```
```sh
(gdb) x/wd 0x7fffffffe3c0+4
0x7fffffffe3c4:	1337
```
```sh
(gdb) x/gf 0x7fffffffe3c0+8
0x7fffffffe3c8:	3.1415000000000002
```
```sh
(gdb) x/s 0x7fffffffe3c0+16
0x7fffffffe3d0:	"hello friend"
(gdb) x/20bx 0x7fffffffe3c0+16
0x7fffffffe3d0:	0x68	0x65	0x6c	0x6c	0x6f	0x20	0x66	0x72
0x7fffffffe3d8:	0x69	0x65	0x6e	0x64	0x00	0x00	0x00	0x00
0x7fffffffe3e0:	0x00	0x00	0x00	0x00
```
```sh
(gdb) x/gx 0x7fffffffe3c0+40
0x7fffffffe3e8:	0x1337133713371337
```

## set *(\<type>\*)(\<address>)=\<value>
This command modifies the contents of memory. The syntax might be daunting, but it is quite intuitive, same as C code for setting values of pointers.

1. Cast `address` as `type*`
2. Dereference `address`
3. Set the value

For example, `set *(char*)(0x7fffffffe3c0+16)=109`:

1. Cast `0x7fffffffe3c0+16` as a `char*`
2. Dereference `0x7fffffffe3c0+16` to point to a `char`
3. Set the value of the `char` as `109`

```sh
(gdb) set *(char*)(0x7fffffffe3c0+16)='m'
(gdb) x/s 0x7fffffffe3c0+16
0x7fffffffe3d0:	"mello friend"
```

Without specifying a type, GDB defaults to `int`.

```sh
(gdb) set *(0x7fffffffe3c0+4)=0xcafebeef
(gdb) x/12wx 0x7fffffffe3c0
0x7fffffffe3c0:	0x00380036	0xcafebeef	0xc083126f	0x400921ca
0x7fffffffe3d0:	0x6c6c6568	0x7266206f	0x646e6569	0x00000000
0x7fffffffe3e0:	0x0000006d	0x00000000	0x13371337	0x13371337
```