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
	//���ڿ� ip��ȯ
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
	//���� ttl����
	if (setsockopt(trace.getSock(), IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
		qDebug() << "Failed set ttl : " << WSAGetLastError();
	}
	else qDebug() << "Success set ttl : " << ttl;


	// ��Ŷ 3��������
	for (int i = 0; i < 3; i++) {
		icmp.Send(trace.getSock(), ttl, destip, i);
		//�۽� ���۽ð�
		stping.push_back(std::chrono::high_resolution_clock::now());
	}
	int j = 0;
	int k = 0;
	//���Ÿ��
	while (true) {
		//����
		std::string Rresult = icmp.Receive(trace.getSock());

		//Ÿ�Ӿƿ� Ȯ��
		if (Rresult != "") {
			if (j >2) {
				break;
			}
			
			//�����޽���
			pingres.push_back(-1.0);
			k++;
		}
		else {
			//ping ���
			auto end = std::chrono::high_resolution_clock::now();
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - stping[j]);
			float durms = static_cast<float>(dur.count());
			pingres.push_back(durms);
		}
		//ping ���
		
		emit ChangePing(pingres[j], pingL, j);
		
		j++;
	}
	emit ChangeIp(icmp.getIp(), pingL);
}

