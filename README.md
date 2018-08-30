# ChatServer

一款聊天软件的后端实现

需要安装的依赖

[libevent](https://github.com/libevent/libevent) 

[jsoncpp](https://github.com/open-source-parsers/jsoncpp)

[glog](https://github.com/google/glog)

[Crypto++](https://github.com/weidai11/cryptopp)

[libmysqlclient](https://dev.mysql.com/downloads/connector/cpp/)

使用方法:

	先创建四张数据库表
	
		users,messages,messages_flg,friends
	
	将Main目录下的配置文件复制到bin目录下
	
	make 
	
	cd bin
	
	./test



配置文件包括:

	线程数: worker
	
	端口号: port
	
	日志文件: log_file