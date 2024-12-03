#pragma once
#include <iostream>
#include <cstring> 
#include <cstdint>
#include <chrono> 
#include <thread>
#include <future>
#include <vector>

//IPHEADER 구조체 정의
struct iphdr {
	uint8_t ihl : 4;
	uint8_t version : 4;
	uint8_t tos;
	uint16_t tot_len;
	uint16_t id;           // id
	uint16_t frag_off;
	uint8_t ttl;           // TTL
	uint8_t protocol;      // 프로토콜
	uint16_t check;        // 체크섬
	uint32_t saddr;        // 출발 주소
	uint32_t daddr;        // 도착 주소
};
//ICMPHEADER구조체 정의
struct ICMPHEADER
{
	uint8_t Type;
	uint8_t Code;
	uint16_t Checksum;
	uint16_t id;
	uint16_t sequence;
};

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "ws2_32.lib")



#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/time.h>

#endif
