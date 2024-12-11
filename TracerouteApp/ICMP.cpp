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
			qDebug() << "Packet too small to analyze!\n" ;
			return;
		}

		// IP ��� ���� ����
		const unsigned int IP_HEADER_LENGTH = (rebuff[0] & 0x0F) * 4;

		if (packetSize < IP_HEADER_LENGTH + 8) { // IP ��� + ICMP ��� Ȯ��
			qDebug() << "Packet too small for ICMP!" ;
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
	
	sockaddr_in ICMP::Stadd(std::string destAddr)
	{
		sockaddr_in destip;
		memset(&destip, 0, sizeof(destip));
		destip.sin_family = AF_INET;
		// ���ڿ��� ip�� �ƴ� �������� ��� �������ּҸ� ip�� ��ȯ ip�� ��� ���ڿ��� ip������ ��ȯ�ϰ� ����
		if (inet_pton(AF_INET, destAddr.c_str(), &destip.sin_addr) != 1) {
			struct addrinfo hint, * result;
			memset(&hint, 0, sizeof(hint));
			hint.ai_family = AF_INET;
			hint.ai_socktype = SOCK_RAW;


			int red = getaddrinfo(destAddr.c_str(), NULL, &hint, &result);
			if (red != 0) {
				return destip;// �� ip����
			}
			destip = *(sockaddr_in*)result->ai_addr;
			freeaddrinfo(result);
			inet_ntop(AF_INET, &destip.sin_addr, ip, INET_ADDRSTRLEN);
		}
		return destip;
	}

	//�۽��Լ�
	std::string ICMP::Send(SOCKET sock, int ttl,sockaddr_in destAddr,int i) {
		

		ICMPHEADER icmp;
		//���ڿ��� ip�ּҷ� ��ȯ
		
		
		//��Ŷ ����	
		CreatePacket(icmp, ttl * 10+i);
		
		//��Ŷ �۽� �� �����˻�
		if (sendto(sock, (char*)&icmp, sizeof(icmp), 0, (struct sockaddr*)&destAddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			qDebug() << "Failed send to Packet" << "\n";
			if (WSAGetLastError() == EPERM) {
				return GrantError = "������ �������� ����� �䱸";
			}
			return "ErrorCode : "+WSAGetLastError();
		}
		
			qDebug() << "Send Success ttl : "<<ttl << "\n";
		
		return "";
	}
	std::string ICMP::Receive(SOCKET sock) {
		qDebug() << "Waiting for Recevied" << "\n";
		
		//���Ź��� ��Ŷ�� �����Ϳ� ip�ּ� ����
		
		int ipsize = sizeof(ipadd);
		memset(&ipadd, 0, sizeof(ipadd));
		ipadd.sin_family = AF_INET;
		int result = recvfrom(sock,rebuff,sizeof(rebuff),0,(sockaddr*)&ipadd,&ipsize);
		if (result == -1) {
			RecvError = "TIME OUT ERROR";
			qDebug() << RecvError << WSAGetLastError() << "\n";
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
	