#pragma once
#include "Header.h"
#include "ICMP.h"
class Traceroute
{
private:

	SOCKET sock; // ����
	
	
public:
	bool Init(); // ���� �ʱ�ȭ
	SOCKET getSock();
};

