#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main() {
    int epoll_fd, num_events, i;
    struct epoll_event events[MAX_EVENTS];

    // 创建 epoll 实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }

    // 设置需要监听的文件描述符
    // 这里使用 stdin 作为示例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fileno(stdin);

    printf("EPOLLIN is %d \n", EPOLLIN);
    printf("EPOLLIN is %d \n", EPOLLOUT);

    // 将 stdin 的事件添加到 epoll 实例中
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, event.data.fd, &event) == -1) {
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
            // 检查事件类型
            if (events[i].events & EPOLLIN) {
                // 可读事件发生
                if (events[i].data.fd == fileno(stdin)) {
                    // stdin 上有可读数据
                    char input[256];
                    fgets(input, sizeof(input), stdin);
                    printf("Input: %s", input);
                }
            }
        }
    }

    // 关闭 epoll 实例
    close(epoll_fd);

    return 0;
}

