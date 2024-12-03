#include "Traceroute.h"
#include "ICMP.h"
#include "Header.h"

bool Traceroute::Init() {
	//소켓 초기화
	WSADATA win;
	if (WSAStartup(MAKEWORD(2, 2), &win) != 0) {
		std::cerr << "Failed Init" << "\n";
	}
	//타임아웃 시간 생성
	int timeout = 5000;
	//소켓 생성
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	//소켓 오류 설정
	if (sock == INVALID_SOCKET) {
		std::cerr << "Failed Socket create" << "\n";
		std::cerr << "Error : " << WSAGetLastError() << "\n";
		return false;
	}
	else {
		// 타임아웃 시간 설정
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		std::cout << "Sucess create Socket" << "\n";
		return true;
	}
	
}

sockaddr_in Traceroute::Stadd(std::string destAddr)
{
	sockaddr_in destip;
	memset(&destip, 0, sizeof(destip));
	destip.sin_family = AF_INET;
	// 문자열이 ip가 아닌 도메인일 경우 도메인주소를 ip로 변환 ip일 경우 문자열을 ip값으로 변환하고 리턴
	if (inet_pton(AF_INET, destAddr.c_str(), &destip.sin_addr) != 1) {
		struct addrinfo hint, * result;
		memset(&hint, 0, sizeof(hint));
		hint.ai_family = AF_INET;
		hint.ai_socktype = SOCK_RAW;
		

		int red = getaddrinfo(destAddr.c_str(), NULL, &hint, &result);
		if (red != 0) {
			ErrorMessage.push_back("invalid domain");
			return destip;// 빈 ip리턴
		}
		destip = *(sockaddr_in*)result->ai_addr;
		freeaddrinfo(result);
	}
	return destip;
}

bool Traceroute::traceroute(sockaddr_in destAddr, int ttl)
{
	int i, j = 0;
	std::chrono::time_point< std::chrono::high_resolution_clock> start;
	//에러메시지 배열 초기화
	ErrorMessage.clear();
	//ping 배열 초기화
	ping.clear();
	stping.clear();
	//소켓 ttl설정
	if (setsockopt(sock, IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
		std::cout << "Failed set ttl : " << WSAGetLastError() << std::endl;
	}
	else std::cout << "Success set ttl : "<<ttl << std::endl;
	
	// 패킷 3개보내기
	for (i = 0; i < 3; i++) {
		icmpPaket.Send(sock, ttl , destAddr, i);
		//송신 시작시간
		stping.push_back( std::chrono::high_resolution_clock::now());
	}
	//수신모듈
	while (true) {
	
	std::string Rresult = icmpPaket.Receive(sock);
	
	//타임아웃 확인
	if (Rresult != "") {
		if (j >= 3) {
			std::cout << "\nrecived end" << std::endl;
			break;
		}
		ErrorMessage.push_back("Access Failed");
		ping.push_back(0);
	}
	else {
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - stping[j]);
		int durms = static_cast<int>(dur.count());
		ping.push_back(durms);
	}
	//ping ip출력
	if (ping[j] == 0) {
		std::cout << "pint : *\t";
	}
	else std::cout <<"ping : "<<ping[j] << "\t";
	
	
	j++;
	}
	std::string ip = getIP();
	std::cout << "ip : "<<ip << std::endl;
	std::cout << "\n";
	// 도착지 확인
	if ((int)icmpPaket.getType() == 0) {
		return true;
	}
	else return false;
		
		
		
	
	
	
}

std::vector<int> Traceroute::getPing()
{
	return ping;
}



std::string Traceroute::getIP()
{
	return icmpPaket.getIp();
}

std::string Traceroute::getErrorMessage(int i) {
	return ErrorMessage[i];
}





