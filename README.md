#Tiny 网络库


##开发环境

    1.Ubuntu 12.04 LTS
    2.G++ 4.6.4


##使用方法见example文件夹

    1.只使用TcpServer，适用于计算任务很轻的情况
    2.在代码中组合TcpServer和ThreadPool，  
    把计算任务（和Tcp回发）分发到线程池中，  
    适合于计算任务较重的情况。
    

##安装方法

    执行make，生成tiny头文件夹和静态库libtiny.a  
    将tiny安装到/usr/include/下，将libtiny.a放置/usr/lib/下  
    编译的时候需要加上-std=c++0x -ltiny



