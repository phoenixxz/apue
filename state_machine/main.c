/******************************
    > File Name: main.c
    > Author:xuxz
    > Mail: xxz1983@gmail.com 
    > Created Time: Fri 14 Jul 2023 07:18:50 AM PDT
 *****************************/
#include <stdio.h>

// 定义状态枚举
typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_FINISHED
} State;

int main() {
    State currentState = STATE_IDLE;

    while (1) {
        switch (currentState) {
            case STATE_IDLE:
                printf("Idle state\n");
                // 在此处添加状态转换逻辑
                currentState = STATE_ACTIVE;
                break;

            case STATE_ACTIVE:
                printf("Active state\n");
                // 在此处添加状态转换逻辑
                currentState = STATE_FINISHED;
                break;

            case STATE_FINISHED:
                printf("Finished state\n");
                // 在此处添加状态转换逻辑
                currentState = STATE_IDLE;
                break;
        }
    }

    return 0;
}

