# C编译器

### 简介
如你所见，这是一个简单的C编译器，根据下列教程所编写：
<br>https://norasandler.com/2017/11/29/Write-a-Compiler.html
<br>完成了第一阶段,通过了该repo中的stage1测试 **(更新中...)**
<br>*repo*网址：https://github.com/nlsandler/write_a_c_compiler

### 使用方法
#### 1
```commandline
cd /path/to/stage
```
#### 2
```commandline
path/to/your/compiler.exe NAME.c
```
正确的样例会通过，并生成汇编文件和可执行文件 ，不通过的会报错

### Attention
因项目设置，构建完成的编译器会在 *part1* 的 **cmake-build-debug** 中，显示为 *part.exe(eg, part1.exe,part2.exe)*
<br>须在测试环境中安装32位gcc
```commandline
sudo apt install gcc-multilib
```

### 测试环境
*kali-linux-2022.3-vmware-amd64*

```commandline
gcc -m32 NAME.c -o NAME
./NAME
echo $?
```


### 问题

可能导致内存泄露,没有正确 *delete*