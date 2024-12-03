#pragma once
#include "Header.h"
#include "ICMP.h"
class Traceroute
{
private:
	std::vector<int> ping; // ping ����
	std::vector<std::chrono::time_point< std::chrono::high_resolution_clock>> stping; //st ping ����
	SOCKET sock; // ����
	ICMP icmpPaket; // ��Ŷ
	std::vector<std::string> ErrorMessage;// �����޼���
	
public:
	bool Init(); // ���� �ʱ�ȭ
	sockaddr_in Stadd(std::string destAddr);//���ڿ� ip��ȯ
	bool traceroute(sockaddr_in destAddr,int ttl);// traceroute�Լ� (�ڵ忡�� ���� �ݺ����� ����Ͽ� ���)
	std::vector<int> getPing(); //ping�� ��ȯ�ϴ� �Լ�
	std::string getIP(); // ip�� ��ȯ�ϴ� �Լ�
	std::string getErrorMessage(int i); // �����޽����� ��ȯ�ϴ� �Լ� 0~2���� ���� �־������
};

