/*
임계 영역
한 프로세스에서 스레드 간 공유 자원을 접근 할 수 있도록 동기화 접근
*/

#include <windows.h>
#include <stdio.h>

#define MAXCNT 100000000
int g_count = 0;
CRITICAL_SECTION cs;

DWORD WINAPI MyThread1(LPVOID arg) {
    for(int i = 0; i < MAXCNT; i++) {
        EnterCriticalSection(&cs);
        g_count += 2;
        LeaveCriticalSection(&cs);
    }
    return 0;
}

DWORD WINAPI MyThread2(LPVOID arg) {
    for(int i = 0; i < MAXCNT; i++) {
        EnterCriticalSection(&cs);
        g_count -= 2;
        LeaveCriticalSection(&cs);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    // 임계영역 초기화
    InitializeCriticalSection(&cs);
    
    // 두개의 스레드 생성
    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
    
    // 두개의 스레드 종료 대기
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

    // 임계영역 삭제
    DeleteCriticalSection(&cs);

    // 결고 출력
    printf("g_count = %d\n", g_count);
    return 0;
}
