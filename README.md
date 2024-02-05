# C编译器

### 简介
如你所见，这是一个简单的C编译器，根据下列教程所编写：
<br>https://norasandler.com/2017/11/29/Write-a-Compiler.html
<br>完成了第一阶段,通过了该repo中的stage1测试

### 使用方法
#### 1
```commandline
cd /path/to/stage
```
#### 2
```commandline
/path/to/your/compiler.exe NAME.c
```
正确的样例会通过，并生成汇编文件和可执行文件 ，不通过的会报错

### Attention
因项目设置，构建完成的编译器会在每个 part 的 **cmake-build-debug** 中，显示为 part.exe