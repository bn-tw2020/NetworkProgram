/*
이벤트 : 사건 발생을 다른 스레드에 알릴 때 사용

1. 이벤트를 비신호 상태로 생성
2. 한 스레드가 작업을 진행하고, 나머지 스레드는 이벤트에 대해 Wait*() 함수를 호출해
   이벤트가 신호 상태가 될 때까지 대기
3. 스레드가 작업을 완료하면 이벤트를 신호 상태로 바꿈
4. 기다리고 있던 스레드 중 하나 혹은 전부가 깨어남
*/

#include <windows.h>
#include <stdio.h>

#define BUFSIZE 10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg) {
    DWORD retval;

    for(int k = 1; k <= 500; k++) {
        // 읽기 완료 대기
        retval = WaitForSingleObject(hReadEvent, INFINITE);
        if(retval != WAIT_OBJECT_0) break;

        // 공유 버퍼에 데이터 저장
        for(int i = 0; i < BUFSIZE; i++) buf[i] = k;

        // 쓰기 완료 알림
        SetEvent(hWriteEvent);
    }
    return 0;
}

DWORD WINAPI ReadThread(LPVOID arg) {
    DWORD retval;
    while(1) {
        // 쓰기 완료 대기
        retval = WaitForSingleObject(hWriteEvent, INFINITE);
        if(retval != WAIT_OBJECT_0) break;

        // 읽은 데이터 출력
        printf("Thread %4d: ", GetCurrentThreadId());
        for(int i = 0; i < BUFSIZE; i++) printf("%3d ", buf[i]);
        printf("\n");

        // 버퍼 초기화
        ZeroMemory(buf, sizeof(buf));

        // 읽기 완료 알림
        SetEvent(hReadEvent);
    }
}

int main(int argc, char argv[]) {
    // 두개의 자동 리셋 이벤트 생성(각각 비신호, 신호 상태)
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if(hWriteEvent == NULL) return 1;
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    if(hReadEvent == NULL) return 1;

    // 세 개의 스레드 생성
    HANDLE hThread[3];
    hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
    hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

    // 세 개의 스레드 종료 대기
    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    // 이벤트 제거
    CloseHandle(hWriteEvent);
    CloseHandle(hReadEvent);
    return 0;
}