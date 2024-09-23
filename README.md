  bootloader的程序放在flash的0x8000000位置处，大小分配24kb<br>
  手环主体代码放在flash的0x8006000处，大小分配40kb<br>
  bootloader的代码会执行跳转指令<br>
  具体参考资料：
  
([stm32远程烧录程序](https://blog.csdn.net/m0_74676415/article/details/139887445?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522ECCB408C-772D-4168-B9F7-34F8979A0ADF%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=ECCB408C-772D-4168-B9F7-34F8979A0ADF&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-11-139887445-null-null.142^v100^pc_search_result_base9&utm_term=stm32%E8%BF%9C%E7%A8%8B%E6%9B%B4%E6%96%B0%E7%A8%8B%E5%BA%8F&spm=1018.2226.3001.4187))

([stm32远程实现跳转](https://blog.csdn.net/qq_26043945/article/details/124139821))

([stm32的多级菜单显示](https://blog.csdn.net/wyhnbkls/article/details/139375000?spm=1001.2014.3001.5506))

([stm32的菜单框运动详解](https://blog.csdn.net/weixin_45633002/article/details/136953093?spm=1001.2014.3001.5506))

([github readme基本语法](https://blog.csdn.net/Strive_For_Future/article/details/120956016?ops_request_misc=%257B%2522request%255Fid%2522%253A%252261F4BCEE-5F5A-4DBF-BF3C-7A0AF7F3EAD7%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=61F4BCEE-5F5A-4DBF-BF3C-7A0AF7F3EAD7&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduend~default-2-120956016-null-null.142^v100^pc_search_result_base9&utm_term=github%E7%9A%84readme%E6%80%8E%E4%B9%88%E5%86%99&spm=1018.2226.3001.4187))

([git常用命令](https://blog.csdn.net/qq_51800384/article/details/135729505?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522CB149C0D-2D5D-446E-89A8-8C21F16D1FD6%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=CB149C0D-2D5D-446E-89A8-8C21F16D1FD6&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-1-135729505-null-null.142))

(补充：删除某个已经存在的远程仓库别名：git remote rm origin)


分支的作用
你branch 一个新分支，会继承master文件，然后你切换工作区到新分支，创建的文件切换到master分支看不见，相当于两个版本，当你新分支觉得稳定的时候可以merge，讲新分支的文件全部复制到master，然后提交到github。<br>
还有一个就是你在本地创建的分支不会影响到github的分支，然后你可以使用命令来在github创建分支也可以删除分支。<br>
