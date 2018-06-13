# 防止某键盘误触发

新买的键盘用了几个月，一个问题越来越明显，就是误触发，按一次，重复响应多次，随时随地敲错特别烦。

就算我按得再轻也没用。

这个问题用 win 自带的 筛选键功能无法解决。

然而键盘超出了包退期，要自己花运费寄过去修QAQ！

所以我写了这个小程序，屏蔽重复触发。

希望能帮到不小心买到类似键盘的你。

解压后，双击里面的 keyboard.exe 运行。

可以设置为开机启动，这样以后就不需要手动双击了。

下载地址：https://github.com/garfeng/keyboard/releases/latest

自己修改参数编译：

``` shell

g++ keyboard.cpp -o keyboard.exe
./keyboard.exe

```
