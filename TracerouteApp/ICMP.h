#pragma once
#include "Header.h"
class ICMP
{
private:
	//도착지 주소 저장버퍼와 
	char rebuff[1024];
	sockaddr_in ipadd;
	std::string ICMPError;//패킷 데이터 에러
	std::string GrantError;
	std::string RecvError;//수신 함수 에러
	char ip[INET_ADDRSTRLEN] = "";//ip문자열
	uint8_t type = NULL;//패킷 타입
	uint8_t code = NULL;//패킷 코드



	//무결성검사를 위한 체크섬 생성함수
	uint16_t CalculateChecksum(uint16_t* buffer, int size);
	// 패킷과 현재 패킷 생성 번호를 받아 패킷을 초기화 하고 리턴
	void CreatePacket(ICMPHEADER& icmp, uint16_t crseq);
	//패킷 분석 함수
	void analyzePacket(int packetSize);
	
public:
	
	std::string Send(SOCKET sock, int ttl, sockaddr_in destAddr,int i);
	std::string Receive(SOCKET sock);
	std::string getIp();
	uint8_t getType();
	int getPing();
};

