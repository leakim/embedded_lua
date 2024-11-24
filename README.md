# embedded_lua
Embedded lua demo


```
$ make
...

$ cat demo.lua
myInt1 = 42
myStr1 = "hello 1"
myStr2 = "hello 2"
myInt2 = 43
myMap = {
    piKey = 3.14
}
myTable = {}
myTable[1]="firstTableEntry"
myInt3 = myInt2 * myInt1

$ ./demo
rc = 0
rc = 0
[0] [string] myInt1 = [number] 42
[1] [string] myStr1 = [string] hello 1
[2] [string] myStr2 = [string] hello 2
[3] [string] myMap = [table] table
[4] [string] myInt3 = [number] 1806
[5] [string] myTable = [table] table
[6] [string] myInt2 = [number] 43
```
