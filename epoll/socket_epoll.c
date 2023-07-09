#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, epoll_fd, num_events, i;
    struct epoll_event events[MAX_EVENTS];

    // 创建 socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // 设置服务器地址和端口
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);

    // 绑定 socket 到服务器地址和端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }

    // 监听连接
    if (listen(server_fd, 10) == -1) {
        perror("listen");
        return 1;
    }

    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }

    // 将服务器 socket 添加到 epoll 实例中
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl");
        return 1;
    }

    while (1) {
        // 等待事件发生
        num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            return 1;
        }

        // 处理所有发生的事件
        for (i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_fd) {
                // 有新的连接请求
                struct sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                printf("accecpt event is : %d\n", events[i].events);
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);	
                if (client_fd == -1) {
                    perror("accept");
                    return 1;
                }
		printf("IPv4 Address: %s\n", inet_ntoa(client_addr.sin_addr));

                // 将客户端 socket 添加到 epoll 实例中
                event.events = EPOLLIN;
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("epoll_ctl");
                    return 1;
                }

                printf("New connection accepted. Client socket fd: %d\n", client_fd);
            } else {
                // 有数据可读
                char buffer[BUFFER_SIZE];
                int bytes_read = read(events[i].data.fd, buffer, BUFFER_SIZE);
                if (bytes_read <= 0) {
                    // 读取错误或连接关闭
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    printf("Connection closed. Client socket fd: %d\n", events[i].data.fd);
                    printf("Connection closed. event is : %d\n", events[i].events);
                } else {
                    // 打印接收到的数据
                    printf("Received data from client socket fd %d: %s\n", events[i].data.fd, buffer);
                    printf("Received data from client. event is : %d\n", events[i].events);
                }
            }
        }
    }

    // 关闭 epoll 实例和服务器 socket
    close(epoll_fd);
    close(server_fd);

    return 0;
}

