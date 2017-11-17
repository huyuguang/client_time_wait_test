# client_time_wait_test
测试客户端TIME_WAIT问题

当需要对外频繁建立tcp短连接时，会遇到TIME_WAIT问题。

测试代码分client和server。server就是个最简单的echo server，总是client方关闭连接以产生TIME_WAIT问题。

client的逻辑是，并发建立n个连接，例如10000个连接，每个连接都发送100字节，接收100字节，然后关闭连接。
循环以上逻辑m次。

代码主要测试这么几点：
1，SO_REUSEPORT是否会有帮助？
2，SO_LINGER是否会有帮助？
3，tcp_tw_reuse是否会有帮助？

每次测试之间间隔2分钟，并且通过netstat确认此时已经没有WAIT_TIME状态的socket。

结论是：
1，tcp_tw_reuse有帮助。
2，SO_LINGER(1,0)有帮助。
3，SO_REUSEPORT没有帮助。

usage:
 client dstip loop_count connections_count reuseport[0|1] linger[0|1]



```
设置tcp_tw_reuse=0

使用reuseport，但不硬关闭连接

[huyuguang@dtrans1 ~/reuseport]$ ./reuseport.exe client 123.183.215.109 10 10000 1 0
loop: 0, finished_count: 10000, failed_count: 0, time(s): 1
loop: 1, finished_count: 10000, failed_count: 0, time(s): 8
loop: 2, finished_count: 8232, failed_count: 1768, time(s): 15
loop: 3, finished_count: 0, failed_count: 10000, time(s): 27
loop: 4, finished_count: 5165, failed_count: 4835, time(s): 13
loop: 5, finished_count: 7629, failed_count: 2371, time(s): 8
loop: 6, finished_count: 7259, failed_count: 2741, time(s): 16
loop: 7, finished_count: 8179, failed_count: 1821, time(s): 15
loop: 8, finished_count: 2080, failed_count: 7920, time(s): 24
loop: 9, finished_count: 8197, failed_count: 1803, time(s): 13


不使用reuseport，不硬关闭连接

[huyuguang@dtrans1 ~/reuseport]$ ./reuseport.exe client 123.183.215.109 10 10000 0 0
loop: 0, finished_count: 10000, failed_count: 0, time(s): 3
loop: 1, finished_count: 10000, failed_count: 0, time(s): 8
loop: 2, finished_count: 8232, failed_count: 1768, time(s): 15
loop: 3, finished_count: 0, failed_count: 10000, time(s): 27
loop: 4, finished_count: 6141, failed_count: 3859, time(s): 14
loop: 5, finished_count: 8548, failed_count: 1452, time(s): 8
loop: 6, finished_count: 7555, failed_count: 2445, time(s): 16
loop: 7, finished_count: 5988, failed_count: 4012, time(s): 21
loop: 8, finished_count: 5451, failed_count: 4549, time(s): 14
loop: 9, finished_count: 7423, failed_count: 2577, time(s): 10

不使用reuseport，硬关闭连接

[huyuguang@dtrans1 ~/reuseport]$ ./reuseport.exe client 123.183.215.109 10 10000 0 1
loop: 0, finished_count: 10000, failed_count: 0, time(s): 4
loop: 1, finished_count: 10000, failed_count: 0, time(s): 1
loop: 2, finished_count: 10000, failed_count: 0, time(s): 0
loop: 3, finished_count: 10000, failed_count: 0, time(s): 1
loop: 4, finished_count: 10000, failed_count: 0, time(s): 3
loop: 5, finished_count: 10000, failed_count: 0, time(s): 1
loop: 6, finished_count: 10000, failed_count: 0, time(s): 0
loop: 7, finished_count: 10000, failed_count: 0, time(s): 1
loop: 8, finished_count: 10000, failed_count: 0, time(s): 0
loop: 9, finished_count: 10000, failed_count: 0, time(s): 0


设置tcp_tw_reuse=1

[huyuguang@dtrans1 ~/reuseport]$ ./reuseport.exe client 123.183.215.109 10 10000 0 0
loop: 0, finished_count: 10000, failed_count: 0, time(s): 0
loop: 1, finished_count: 10000, failed_count: 0, time(s): 5
loop: 2, finished_count: 10000, failed_count: 0, time(s): 1
loop: 3, finished_count: 10000, failed_count: 0, time(s): 4
loop: 4, finished_count: 10000, failed_count: 0, time(s): 1
loop: 5, finished_count: 10000, failed_count: 0, time(s): 2
loop: 6, finished_count: 10000, failed_count: 0, time(s): 2
loop: 7, finished_count: 10000, failed_count: 0, time(s): 4
loop: 8, finished_count: 10000, failed_count: 0, time(s): 1
loop: 9, finished_count: 10000, failed_count: 0, time(s): 4
```
