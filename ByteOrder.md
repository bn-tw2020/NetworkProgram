## [1] 바이트 정렬 함수

* 메모리에 데이터를 저장할 때 바이트 순서
  * 빅 엔디안, 리틀 엔디안
  
  
### 바이트 정렬 방식을 고려해야 하는 경우

<center><img src="https://user-images.githubusercontent.com/66770613/115184665-49d6cb80-a119-11eb-8111-7e8489181c79.png" width="750" height="400"/></center>    

* 프로토콜 구현을 위해 필요한 정보
  * IP주소 -> 빅 엔디안
  * 포트 번호 -> 빅 엔디안
* 응용 프로그램이 주고 받는 데이터
  * 빅 엔디안 or 리틀 엔디안으로 통일

> 참고   
> 네트워크 바이트 정렬: 빅엔디안 방식   
> 호스트 바이트 정렬 : 시스템이 사용하는 고유한 바이트 정렬 방식   

### 바이트 정렬 함수(유닉스 호환)

```c++
u_short htons(u_short hostshort); // host-to-network-short
u_long htonl(u_long hostlong); // host-to-network long
u_short ntohs(u_short netshort); // network-to-hostshort
u_long ntohl(u_long netlong); // network-to-host long
```

### 바이트 정렬 함수(윈속 확장)

```c++
int WSAHtons(SOCKET s, u_short hostshort, u_short *lpnetshort);
int WSAHtonl(SOCKET s, u_long hostlong, u_long *lpnetlong);
int WSANtohs(SOCKET s, u_short netshort, u_short *lphostshort);
int WSANtohl(SOCKET s, u_long netlong, u_long *lphostlong);
```

### 왜 정렬 함수를 써야 하는가?

네트워크 장비들은 대부분 PC장비들하고 전혀 다르게 만들어져 있다.  
그래서 프로그래밍 관점에서 보는 것보다 시스템들을 최적화하고 시스템들이 잘 돌아가게 하게끔 빅엔디안 형태가 최적화 될 수도 있어서 빠른 처리를 위해서 사용한다.  
장비쪽에서는 빅엔디안이 편리하기에 이쪽으로 치우치고 있으며, 호스트에서는 여러 가지의 하드웨어가 다른 종류의 칩들을 보유하고 있어서 내부 마다 다르다.

### SOCKADDR_IN / SOCKADDR_IN6 Struct

<img src= "https://user-images.githubusercontent.com/66770613/115185905-ba7ee780-a11b-11eb-96f9-85a0274dae21.png" width="700" height="300" />  

---

## [2] IP 주소 변환 함수

### IPv4 주소 변환 함수

```c++
unsigned long inet_addr(const char *cp);
```

* 문자열 형태로 IPv4 주소 입력 => 32비트 숫자(네트워크 바이트 정렬)로 리턴

```c++
char *inet_ntoa(struct in_addr in);
```

* 32비트 숫자(네트워크 바이트 정렬)로 IPv4 주소 입력 => 문자열 형태로 리턴

### IPv4 or IPv6 주소 변환 함수(문자열 => 숫자)

```c++
int WSAStringToAddress(
  // 문자열 형식의 IP주소
  LPTSTR AddressString,
  // AF_INET or AF_INET6
  INT AddressFamily,
  // NULL
  LPWSAPROTOCOL_INFO lpProtocolInfo,
  // IP 주소(숫자)를 저장할 구조체; SOCKADDR_IN or SOCKADDR_IN6
  LPSOCKADDR lpAddress,
  // 주소 구조체의 길이
  LPIINT lpAddressLength);
```

```c++
int WSAAddressToString(
  // IP 주소(숫자)를 저장할 구조체; SOCKADDR_IN or SOCKADDR_IN6
  LPSOCKADDR lpAddress,
  // 주소 구조체의 길이
  DWORD dwAddressLength,
  // NULL
  LPWSAPROTOCOL_INFO lpProtocolInfo,
  // IP주소(문자열)를 저장할 버퍼
  LPTSTR lpszAddressString,
  // 버퍼의 길이
  LPDWORD lpdwAddressStringLength);
```

### 바이트 정렬 함수와 IP주소 변환 함수 예시

* 응용 프로그램이 소켓 주소 구조체를 초기화 후, 소켓 함수에 넘겨주는 경우

```c++
// 소켓 주소 구조체를 초기화한다.
SOCKADDR_IN addr;
ZeroMemory(&addr, sizeof(addr)); // 0으로 채운다.
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr("147.46.114.70");
addr.sin_port = htons(9000);

// 소켓 함수를 호출 한다.
SocketFunc(...,(SOCKADDR *)&addr, sizeof(addr), ...);
```

* 소켓 함수가 소켓 주소 구조체를 입력 
* 소켓 함수가 소켓 주소 구조체를 입력 받아
* 소켓 함수가
