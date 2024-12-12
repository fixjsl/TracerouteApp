#include "TraceThread.h"
#include "TracerouteApp.h"

void TraceThread::setDestadd(std::string destip)
{
	t_destip = destip;
}

void TraceThread::setTraceSock(Traceroute t)
{
	trace = t;
}

void TraceThread::StartTT() 
{
	ICMP icmp;
	//문자열 ip변환
	sockaddr_in destip = icmp.Stadd(t_destip);

	
	if (destip.sin_addr.s_addr == 0) {
		qDebug() << "Invailed URL" << "\n";
		emit URLError("Invailed URL");
		return;
	}
	InforLayout pingL;
	for (int i = 0; i < Max_hop; i++) {
		std::vector<float> pingres;
		
		emit updateTraceLayout(i,&pingL);
		Traceloop(i, icmp, destip, pingres,&pingL);

		//UI update
		
		if ((int)icmp.getType() == 0) {
			qDebug() << "compleste Traceroute" << "\n";
			emit comple("Trace complete");
			return;
		}

	}
	emit comple("Failed Reach");
	return;
}

void TraceThread::Traceloop(int ttl, ICMP &icmp, sockaddr_in destip, std::vector<float> &pingres, InforLayout* pingL)
{	
	std::vector<std::chrono::time_point< std::chrono::high_resolution_clock>> stping;
	//소켓 ttl설정
	if (setsockopt(trace.getSock(), IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
		qDebug() << "Failed set ttl : " << WSAGetLastError();
	}
	else qDebug() << "Success set ttl : " << ttl;


	// 패킷 3개보내기
	for (int i = 0; i < 3; i++) {
		icmp.Send(trace.getSock(), ttl, destip, i);
		//송신 시작시간
		stping.push_back(std::chrono::high_resolution_clock::now());
	}
	int j = 0;
	int k = 0;
	//수신모듈
	while (true) {
		//수신
		std::string Rresult = icmp.Receive(trace.getSock());

		//타임아웃 확인
		if (Rresult != "") {
			if (j >2) {
				break;
			}
			
			//에러메시지
			pingres.push_back(-1.0);
			k++;
		}
		else {
			//ping 계산
			auto end = std::chrono::high_resolution_clock::now();
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - stping[j]);
			float durms = static_cast<float>(dur.count());
			pingres.push_back(durms);
		}
		//ping 출력
		
		emit ChangePing(pingres[j], pingL, j);
		
		j++;
	}
	emit ChangeIp(icmp.getIp(), pingL);
}

