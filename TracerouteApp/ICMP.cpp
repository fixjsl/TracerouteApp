#include "Header.h"
#include "ICMP.h"



//private

	//무결성검사를 위한 체크섬 생성함수
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
	// 패킷과 현재 패킷 생성 번호를 받아 패킷을 초기화 하고 리턴
	void ICMP::CreatePacket(ICMPHEADER& icmp, uint16_t crseq) {
		memset(&icmp, 0, sizeof(ICMPHEADER));
		icmp.Type = 8;
		icmp.Code = 0;
		icmp.id = (uint16_t)GetCurrentProcessId()+crseq;
		icmp.sequence = crseq;
		icmp.Checksum = 0;
		icmp.Checksum = CalculateChecksum((uint16_t*)&icmp, sizeof(icmp));
	}
	//패킷 분석 함수
	void ICMP::analyzePacket(int packetSize) {
		if (packetSize < 20) {  // 최소한 IP 헤더 크기 확인
			std::cerr << "Packet too small to analyze!" << std::endl;
			return;
		}

		// IP 헤더 길이 추출
		const unsigned int IP_HEADER_LENGTH = (rebuff[0] & 0x0F) * 4;

		if (packetSize < IP_HEADER_LENGTH + 8) { // IP 헤더 + ICMP 헤더 확인
			std::cerr << "Packet too small for ICMP!" << std::endl;
			return;
		}

		// ICMP 헤더 추출
		char* icmpHeader = rebuff + IP_HEADER_LENGTH;

		// ICMP Type과 Code 가져오기
		type = static_cast<uint8_t>(icmpHeader[0]);  // Type은 ICMP 헤더의 첫 바이트
		code = static_cast<uint8_t>(icmpHeader[1]);
	}
	
	//문자열 ip변환 함수


	//public
	
	//송신함수
	std::string ICMP::Send(SOCKET sock, int ttl,sockaddr_in destAddr,int i) {
		

		ICMPHEADER icmp;
		//문자열을 ip주소로 변환
		
		
		//패킷 생성	
		CreatePacket(icmp, ttl * 10+i);
		
		//패킷 송신 및 오류검사
		if (sendto(sock, (char*)&icmp, sizeof(icmp), 0, (struct sockaddr*)&destAddr, sizeof(sockaddr_in)) == SOCKET_ERROR) {
			std::cerr << "Failed send to Packet" << "\n";
			if (WSAGetLastError() == EPERM) {
				return GrantError = "관리자 권한으로 재실행 요구";
			}
			return "SendError";
		}
		
			std::cout << "Send Success ttl : "<<ttl << std::endl;
		
		return "";
	}
	std::string ICMP::Receive(SOCKET sock) {
		std::cout << "Waiting for Recevied" << std::endl;
		
		//수신받은 패킷의 데이터와 ip주소 저장
		
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
	