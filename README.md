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

