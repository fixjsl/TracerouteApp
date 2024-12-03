#pragma once
#include "Header.h"
#include "ICMP.h"
class Traceroute
{
private:
	std::vector<int> ping; // ping 보관
	std::vector<std::chrono::time_point< std::chrono::high_resolution_clock>> stping; //st ping 보관
	SOCKET sock; // 소켓
	ICMP icmpPaket; // 패킷
	std::vector<std::string> ErrorMessage;// 에러메세지
	
public:
	bool Init(); // 소켓 초기화
	sockaddr_in Stadd(std::string destAddr);//문자열 ip변환
	bool traceroute(sockaddr_in destAddr,int ttl);// traceroute함수 (코드에서 직접 반복문을 사용하여 사용)
	std::vector<int> getPing(); //ping을 반환하는 함수
	std::string getIP(); // ip를 반환하는 함수
	std::string getErrorMessage(int i); // 에러메시지를 반환하는 함수 0~2까지 직접 넣어줘야함
};

