# 課題解説
課題用に構築されたlinux isoファイルが渡される。
頑張って`level〇+1 user`の`.pass`を見つけることが目的。
`.pass` は必ず`~/`にある。
これをlevel13まで繰り返す。

# level0

## ログイン
`ssh level0@192.168.0.143 -p 4242`
pw `level0`

## ファイルをローカルに移動
`scp -P 4242 level0@192.168.0.143:level0 level0`

##　ファイルの権限を確認
```
level0@RainFall:~$ ls -l
total 732
-rwsr-x---+ 1 level1 users 747441 Mar  6  2016 level0
```

### 実ユーザー (Real User ID: RUID)
実ユーザーはプログラムを実行したユーザーのid。

### 実効ユーザー (Effective User ID: EUID)
実効ユーザーは、ファイルの所有者のuid。

suid (s in owner権限位置): プログラム実行時、実行者は一時的にファイル所有者 (この場合 level1) の権限で実行できる。
このため、このプログラムは`level1`権限で実行され、`.pass`にアクセス可能となる。

## 動作確認
`source.c`のコードを見ると`if (convertedValue == 423)`の条件がある。

これによりargvに423を渡せばlevel1のshが起動できる。

```
level0@RainFall:~$ ./level0 423
$ cat /home/user/level1/.pass
1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a
```

## memo
setresuid()の呼び出し後:実効ユーザー、実ユーザー、保存されたuidがすべてlevel1に統一される。
実装依存だけど`system()`はセキュリティ対策として、euidをruidにリセットする挙動する。
一方、`execv`は現在のeuidを引き継ぐからsetしなくてもlevel1を実行できそうだから不要そうではある
詳しく調べてないからわからん。
