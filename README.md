# C编译器

### 简介
简单的C编译器，根据下列教程所编写：
<br>https://norasandler.com/2017/11/29/Write-a-Compiler.html
<br>完成到第八阶段,通过了该repo中的stage1-8测试 
<br>*repo*网址：https://github.com/nlsandler/write_a_c_compiler
<br>个人练习项目，如果你也在跟随该教程，希望能帮到你。水平有限，请谅解
### 使用方法
```commandline
cd /path/to/stage
path/to/your/compiler.exe NAME.c
```
正确的样例会通过，并生成汇编文件和可执行文件 ，不通过的会报错

### Attention
<br>须在测试环境中安装32位gcc
```commandline
sudo apt install gcc-multilib
```
stage_8 中的 continue_empty_post.c 因未实现模而搁置
### 测试环境
*kali-linux-2022.3-vmware-amd64*

```commandline
gcc -m32 NAME.c -o NAME
./NAME
echo $?
```

### 问题

可能导致内存泄露,没有正确 *delete*