#### 简介

lab0_0是在屏幕上打印出Hello World。

#### quick start

```
make			# 编译
make run		# 运行
make clean		# 清除编译结果
```

#### 出现的问题

##### 1. 无法在qemu中运行。

- 问题描述：qemu提示" HTIF tohost must be 8 bytes"，然而对tohost的定义是8字节的`volatile extern uint64_t tohost;`。