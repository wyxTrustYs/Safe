import socket
import struct
import threading
import minsql
import enum
import time

class MsgType(enum.Enum):
    MD5 = 0
    Process = 1


class Server(object):
    def __init__(self):
        # 初始化套接字
        self.server = socket.socket();
        # 绑定地址
        self.server.bind(('0.0.0.0', 8000))
        # 开启监听
        self.server.listen()
        # 初始化数据库
        self.sql = minsql.Mysql()

    # 等待连接
    def accept(self):
        while True:
            # 接收连接， 并输出连接到的主机
            client, addr = self.server.accept()
            print(addr, '连入了服务器')
            # 接收客户端，并为每个客户端开启独立的线程
            t = threading.Thread(target=self.recv, args=(client,))
            t.start()

    # 查询并返回病毒库
    def search(self, client):
        MD5 = self.sql.QuerySql("SELECT * FROM MD5 ")
        Process = self.sql.QuerySql("SELECT * FROM process")
        length = len(Process)+len(MD5)
        client.send(struct.pack('i100s', length, 'start'.encode('GBK')))
        time.sleep(1)
        for num in MD5:
            client.send(struct.pack('i100s', MsgType.MD5.value, num[0].encode('GBK')))
            time.sleep(0.05)
            print(num[0])
        for num in Process:
            client.send(struct.pack('i50s', MsgType.Process.value, num[0].encode('GBK')))
            time.sleep(0.1)
            print(num[0])
            time.sleep(0.1)
        client.send(struct.pack('i100s', MsgType.MD5.value, 'finish'.encode('GBK')))

        # 处理收到的消息

    def recv(self, client):
        try:
           while True:
               message = client.recv(1024)
               user = message.decode('utf-8').rstrip('\x00')
               print(user)
               if user == 'client':
                   self.search(client)
            
        except Exception as msg:
            # 当一个客户端套接字被关闭的时候，会产生一个异常
            print(msg)
            print(client.getsockname(), '退出了服务器')
            client.close()


if __name__ == '__main__':
    server = Server()
    server.accept()
