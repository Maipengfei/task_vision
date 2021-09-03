#实现ssh免密登录远程机命令行
1）ssh-keygen 在本地生成私钥
2）ssh-copy-id ubuntu@159.75.36.245 上传公钥到远程机
3）ssh ubuntu@159.75.36.245 测试免密登录远程机

#成功后
scp ubuntu@159.75.36.245：~/Desktop/test.txt ~/Desktop
将远程机文件拷贝至本机



