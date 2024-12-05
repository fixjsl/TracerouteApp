#include "Header.h"
#include "ICMP.h"



//private

	//���Ἲ�˻縦 ���� üũ�� �����Լ�
	uint16_t ICMP::CalculateChecksum(uint16_t* buffer, int size) {
		uint32_t checksum = 0;
		while (size > 1)
		{
			checksum += *buffer++;
			size -= 2;
		}
		if (size == 1) {
			checksum += *(uint8_t*)buffer;
		}

		checksum = (checksum & 0xFFFF) + (checksum >> 16);
		checksum += (checksum >> 16);

		return (uint16_t)~checksum;

	}
	// ��Ŷ�� ���� ��Ŷ ���� ��ȣ�� �޾� ��Ŷ�� �ʱ�ȭ �ϰ� ����
	void ICMP::CreatePacket(ICMPHEADER& icmp, uint16_t crseq) {
		memset(&icmp, 0, sizeof(ICMPHEADER));
		icmp.Type = 8;
		icmp.Code = 0;
		icmp.id = (uint16_t)GetCurrentProcessId()+crseq;
		icmp.sequence = crseq;
		icmp.Checksum = 0;
		icmp.Checksum = CalculateChecksum((uint16_t*)&icmp, sizeof(icmp));
	}
	//��Ŷ �м� �Լ�
	void ICMP::analyzePacket(int packetSize) {
		if (packetSize < 20) {  // �ּ��� IP ��� ũ�� Ȯ��
			std::cerr << "Packet too small to analyze!" << std::endl;
			return;
		}

		// IP ��� ���� ����
		const unsigned int IP_HEADER_LENGTH = (rebuff[0] & 0x0F) * 4;

		if (packetSize < IP_HEADER_LENGTH + 8) { // IP ��� + ICMP ��� Ȯ��
			std::cerr << "Packet too small for ICMP!" << std::endl;
			return;
		}

		// ICMP ��� ����
		char* icmpHeader = rebuff + IP_HEADER_LENGTH;

		// ICMP Type�� Code ��������
		type = static_cast<uint8_t>(icmpHeader[0]);  // Type�� ICMP ����� ù ����Ʈ
		code = static_cast<uint8_t>(icmpHeader[1]);
	}
	
	//���ڿ� ip��ȯ �Լ�


	//public
	
	//�۽��Լ�
	std::string ICMP::Send(SOCKET sock, int ttl,sockaddr_in destAddr,int i) {
		

		ICMPHEADER icmp;
		//���ڿ��� ip�ּҷ� ��ȯ
		
		
		//��Ŷ ����	
		CreatePacket(icmp, ttl * 10+i);
		
		//��Ŷ �۽� �� �����˻�
		if (sendto(sock, (char*)&icmp, sizeof(icmp), 0, (struct sockaddr*)&destAddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			std::cerr << "Failed send to Packet" << "\n";
			if (WSAGetLastError() == EPERM) {
				return GrantError = "������ �������� ����� �䱸";
			}
			return "SendError";
		}
		
			std::cout << "Send Success ttl : "<<ttl << std::endl;
		
		return "";
	}
	std::string ICMP::Receive(SOCKET sock) {
		std::cout << "Waiting for Recevied" << std::endl;
		
		//���Ź��� ��Ŷ�� �����Ϳ� ip�ּ� ����
		
		int ipsize = sizeof(ipadd);
		memset(&ipadd, 0, sizeof(ipadd));
		ipadd.sin_family = AF_INET;
		int result = recvfrom(sock,rebuff,sizeof(rebuff),0,(sockaddr*)&ipadd,&ipsize);
		if (result == -1) {
			RecvError = "TIME OUT ERROR";
			std::cout << RecvError << WSAGetLastError() << std::endl;
			return RecvError;
		}
			
		inet_ntop(AF_INET, &ipadd.sin_addr, ip, INET_ADDRSTRLEN);
		analyzePacket(sizeof(rebuff));
			
		
		
		return "";
		
	}

	std::string ICMP::getIp()
	{
		return ip;
	}

	uint8_t ICMP::getType()
	{
		return type;
	}

	int ICMP::getPing()
	{
		return 0;
	}
	