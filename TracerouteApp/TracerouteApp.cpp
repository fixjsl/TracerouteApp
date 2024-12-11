#include "TracerouteApp.h"

TracerouteApp::TracerouteApp(QWidget *parent)
    : QWidget(parent)
{
	trace.Init();
    ui.setupUi(this);
	QHBoxLayout* M = new QHBoxLayout(ui.scrollAreaWidgetContents);
	connect(ui.StartButton, &QPushButton::clicked, this, &TracerouteApp::TraceLoop);
}

TracerouteApp::~TracerouteApp()
{}

//초기화함수



//button 연결 함수
void TracerouteApp::TraceLoop() {
	ui.StartButton->setEnabled(false);
	QHBoxLayout* M = new QHBoxLayout(ui.scrollAreaWidgetContents);
	ui.scrollAreaWidgetContents->setLayout(M);
	InforLayout pingL;
	std::string destiproute = ui.URLEnter->text().toStdString();
	ICMP icmp;

	//문자열 ip변환
    sockaddr_in destip = icmp.Stadd(destiproute);
	ui.INfor->setText( QString::fromStdString(icmp.getIp()) + "까지의 경로추적");
	if (destip.sin_addr.s_addr == 0) { 
		qDebug() << "Invailed URL"<<"\n";
		QMessageBox msg;
		msg.setIcon(QMessageBox::Warning);
		msg.setText("잘못된 URL입니다.");
		msg.setStandardButtons(QMessageBox::Ok);
		msg.setDefaultButton(QMessageBox::Ok);
		ui.StartButton->setEnabled(true);
		return;
	}
	//ping
    std::vector<float> ping;
	std::vector<std::chrono::time_point< std::chrono::high_resolution_clock>> stping;
    
    
	//trace loop 시작
    for (int ttl = 0; ttl < Max_hop; ttl++) {
		pingL.CreateLayout();
		int j = 0;
		std::chrono::time_point< std::chrono::high_resolution_clock> start;
		//ping 배열 초기화
		ping.clear();
		stping.clear();
		//소켓 ttl설정
		if (setsockopt(trace.getSock(), IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
			qDebug() << "Failed set ttl : " << WSAGetLastError();
		}
		else qDebug() << "Success set ttl : " << ttl ;

		// 패킷 3개보내기
		for (int i = 0; i < 3; i++) {
			icmp.Send(trace.getSock(), ttl, destip, i);
			//송신 시작시간
			stping.push_back(std::chrono::high_resolution_clock::now());
		}
		//ttl 출력
		pingL.setTTL(ttl);
		M->addLayout(pingL.getMainLayout());
		//수신모듈
		while (true) {
			//수신
			std::string Rresult = icmp.Receive(trace.getSock());

			//타임아웃 확인
			if (Rresult != "") {
				if (j >= 3) {
					break;
				}
				//에러메시지
				ping.push_back(0.0);
			}
			else {
				//ping 계산
				auto end = std::chrono::high_resolution_clock::now();
				auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - stping[j]);
				int durms = static_cast<float>(dur.count());
				ping.push_back(durms);
			}
			//ping 출력
			pingL.setPing(ping[j], j);


			j++;
		}
		//ip출력
		std::string tempip;
		std::string ip = icmp.getIp();
		if (ip == tempip) {
			// 타임아웃 오류처리
			pingL.setIp("Access Failed");
		}
		else {
			//ip정상출력
			pingL.setIp(ip);
		}
		// 도착지 확인
		if ((int)icmp.getType() == 0) {
			
			ui.StartButton->setEnabled(true);
			return;
		}
		tempip = ip;


        
    }
	ui.INfor->setText("목적 ip까지 도착 실패\n");
	ui.StartButton->setEnabled(true);
	return;
}