#pragma once
#include "Header.h"
#include "ICMP.h"
class Traceroute
{
private:

	SOCKET sock; // 소켓
	
	
public:
	bool Init(); // 소켓 초기화
	SOCKET getSock();
};

