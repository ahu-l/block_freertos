  bootloader的程序放在flash的0x8000000位置处，大小分配24kb<br>
  手环主体代码放在flash的0x8006000处，大小分配40kb<br>
  bootloader的代码会执行跳转指令<br>
  具体参考资料：
  
([stm32远程烧录程序](https://blog.csdn.net/m0_74676415/article/details/139887445?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522ECCB408C-772D-4168-B9F7-34F8979A0ADF%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=ECCB408C-772D-4168-B9F7-34F8979A0ADF&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-11-139887445-null-null.142^v100^pc_search_result_base9&utm_term=stm32%E8%BF%9C%E7%A8%8B%E6%9B%B4%E6%96%B0%E7%A8%8B%E5%BA%8F&spm=1018.2226.3001.4187))

([stm32远程实现跳转](https://blog.csdn.net/qq_26043945/article/details/124139821))

([stm32的多级菜单显示](https://blog.csdn.net/wyhnbkls/article/details/139375000?spm=1001.2014.3001.5506))

([stm32的菜单框运动详解](https://blog.csdn.net/weixin_45633002/article/details/136953093?spm=1001.2014.3001.5506))
