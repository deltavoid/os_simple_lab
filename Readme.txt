

OS Simple Lab

基于RISC-V实现的操作系统小实验，以展示操作系统的基本原理。

目前已经完成的有
    os_timer_interrupt (src/os_timer_interrupt1) 时钟中断
    os_task_switch(src/os_task_switch) M态的任务切换
    os_user_task_switch(src/os_user_task_switch) U态的任务切换
    os_user_syscall(src/so_user_syscall) U态向M态的系统调用

所有实验只需进入相应文件夹下直接make即可，有的实验输出结果在log.txt中。

实验需要配置riscv32位环境，具体参见https://github.com/chyyuu/ucore_os_lab/tree/riscv32-priv-1.10。

