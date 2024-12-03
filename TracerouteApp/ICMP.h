#pragma once
#include "Header.h"
class ICMP
{
private:
	//������ �ּ� ������ۿ� 
	char rebuff[1024];
	sockaddr_in ipadd;
	std::string ICMPError;//��Ŷ ������ ����
	std::string GrantError;
	std::string RecvError;//���� �Լ� ����
	char ip[INET_ADDRSTRLEN] = "";//ip���ڿ�
	uint8_t type = NULL;//��Ŷ Ÿ��
	uint8_t code = NULL;//��Ŷ �ڵ�



	//���Ἲ�˻縦 ���� üũ�� �����Լ�
	uint16_t CalculateChecksum(uint16_t* buffer, int size);
	// ��Ŷ�� ���� ��Ŷ ���� ��ȣ�� �޾� ��Ŷ�� �ʱ�ȭ �ϰ� ����
	void CreatePacket(ICMPHEADER& icmp, uint16_t crseq);
	//��Ŷ �м� �Լ�
	void analyzePacket(int packetSize);
	
public:
	
	std::string Send(SOCKET sock, int ttl, sockaddr_in destAddr,int i);
	std::string Receive(SOCKET sock);
	std::string getIp();
	uint8_t getType();
	int getPing();
};

