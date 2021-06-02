---
categories:
- Scripting
date: "2020-05-08"
slug: scripting/syntax
tags:
- scripting
title: Syntax
weight: 101
---

GDB provides a simple yet powerful scripting interface that is very helpful while doing reverse engineering. A GDB script file can contain a collection of commands, used with the following constructs. Usually I call it *GDB script*, but apparently GDB docs calls them *command files*.

## source \<file>

The file name/extension does not matter. A simple GDB script looks like this.

{{< code language="gdb" title="gdbscript" id="1" expand="Show" collapse="Hide" isCollapsed="false" >}}
set $sum=0

break *foo
commands
    silent
    printf "Arg: %d\n", $rdi
    set $sum=$sum+$rdi
    c
end

printf "Sum: %d\n", $sum
{{< /code >}}

To execute the script, simply call `source ./gdbscript`.

```sh
(gdb) source ./commands2
Breakpoint 1 at 0x400497: file gdbscript.c, line 5.
Arg: 1
Arg: 2
Arg: 100
[Inferior 1 (process 1249) exited normally]
Sum: 103
```

## set \$\<variable>=\<value>

Variables. Created with this bash-like syntax.

```sh
(gdb) set $a=1
(gdb) p $a
$1 = 1
```
```sh
(gdb) set $b=$rdi*8
(gdb) p $b
$2 = 8
```
```sh
(gdb) set $c=$a+$b
(gdb) p $c
$3 = 9
```

> **Remember the `$`!**

## printf

Better printing than plain `p`. (Assuming that you already know how `printf` in C works)

```sh
(gdb) printf "%d + %d = %d\n", $a, $b, $c
1 + 8 = 9
```
```sh
(gdb) set $d='h'
(gdb) printf "%c\n", $d
h
```
```sh
(gdb) x/s 0x00007fffffffe778
0x7fffffffe778:	"LANGUAGE=en_US:en"
(gdb) printf "export %s\n", 0x00007fffffffe778
export LANGUAGE=en_US:en
```

> **Remember to end with a `\n`!**

## if

Conditionals. `if-else-end`.

```sh
if $rdi > 10
    printf "Big: %d\n", $rdi
else
    printf "Small: %d\n", $rdi
end
```

> `else if` is not supported!

## while

Loops. GDB scripting interface only supports `while` loops.

```sh
set $i=0
while $i < 3
    set $i++
    printf "i: %d\n", $i

    # do stuff
end
```

> Remember to do `set $i++`!

## commands

To define commands when a breakpoint is hit. The `silent` command is very helpful to suppress GDB's output when a breakpoint is hit. The `commands` block can be ended with `cont`/`c`, `nexti`/`ni` or any other command that resumes execution.

```sh
break *main
commands
    silent
    printf "In main!\n"
    cont
end
```

Note that all commands defined after `cont`/`ni`/`si`/etc will be ignored. This is because there might be another breakpoint afterwards, and it will be ambigious for GDB to know which commands to execute.

For example, `printf "Continued\n"` in the following script will be ignored.

```sh
break *main
commands
    silent
    printf "In main!\n"
    cont
    printf "Continued\n"
end
```

> To let the program continue execution after the breakpoint commands, remember to end the `commands` block with `cont`/`c`!

#### Potential mistake

The following example script does not work (using **demo.c**). Notice that the `commands` block does not end with `c`. One might expect the following script to hit the breakpoint 3 times, because of the `while` loop that calls `c` 3 times. However, it only hits once.

{{< code language="c" title="demo.c" id="4" expand="Show" collapse="Hide" isCollapsed="true" >}}
// gcc -g demo.c -o demo -no-pie
#include <stdio.h>

void foo(int a)
{
}

int main()
{
    foo(1);
    foo(2);
    foo(100);

    return 0;
}
{{< /code >}}

```sh
set $sum=0

break *foo
commands
    printf "Arg: %d\n", $rdi
    set $sum+=$rdi
end

r

set $i=0
while $i < 3
    c
    set $i++
    printf "i: %d\n", $i
end

printf "Sum: %d\n", $sum
```
```sh
(gdb) source ./commands
Breakpoint 1 at 0x400497: file gdbscript.c, line 5.
Arg: 1
i: 1
i: 2
[Inferior 1 (process 1157) exited normally]
i: 3
Sum: 1
```

The breakpoint command (`printf "Arg: %d\n", $rdi`) is only executed once, although we can see that the while loop has ran 3 times (as `i` is printed from 1 to 3).

Now, consider this script without the `while` loop, and the `commands` block ending with `c`.

```sh
set $sum=0

break *foo
commands
    silent
    printf "Arg: %d\n", $rdi
    set $sum+=$rdi
    c
end

r

printf "Sum: %d\n", $sum
```
```sh
(gdb) source ./commands2
Breakpoint 1 at 0x400497: file gdbscript.c, line 5.
Arg: 1
Arg: 2
Arg: 100
[Inferior 1 (process 1249) exited normally]
Sum: 103
```

This time, the breakpoint is hit at every call to `foo`.

I'm not sure about the exact underlying behaviour, but moral of the story: just don't manually `continue` when breakpoint commands are defined, to avoid unnecessary headache.
