#include <windows.h>
#include <stdio.h>

int sum = 0;

DWORD WINAPI MyThread(LPVOID arg) {
    int num = (int)arg;
    for(int i = 1; i <= num; i++) sum += i;
    return 0;
}

int main(int argc, char *argv[]) {
    int num = 100;
    // �����带 �����ڸ��� ����(CREATE_SUSPENDED)
    HANDLE hThread = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);
    if(hThread == NULL) return 1;

    printf("Before start Thread, result = %d\n", sum);
    ResumeThread(hThread); // ������ �����
    WaitForSingleObject(hThread, INFINITE); // �����尡 �� ����ǰ� ����
    printf("After start Thread, result = %d\n", sum);
    CloseHandle(hThread);

    return 0;
}