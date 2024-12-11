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

//�ʱ�ȭ�Լ�



//button ���� �Լ�
void TracerouteApp::TraceLoop() {
	ui.StartButton->setEnabled(false);
	QHBoxLayout* M = new QHBoxLayout(ui.scrollAreaWidgetContents);
	ui.scrollAreaWidgetContents->setLayout(M);
	InforLayout pingL;
	std::string destiproute = ui.URLEnter->text().toStdString();
	ICMP icmp;

	//���ڿ� ip��ȯ
    sockaddr_in destip = icmp.Stadd(destiproute);
	ui.INfor->setText( QString::fromStdString(icmp.getIp()) + "������ �������");
	if (destip.sin_addr.s_addr == 0) { 
		qDebug() << "Invailed URL"<<"\n";
		QMessageBox msg;
		msg.setIcon(QMessageBox::Warning);
		msg.setText("�߸��� URL�Դϴ�.");
		msg.setStandardButtons(QMessageBox::Ok);
		msg.setDefaultButton(QMessageBox::Ok);
		ui.StartButton->setEnabled(true);
		return;
	}
	//ping
    std::vector<float> ping;
	std::vector<std::chrono::time_point< std::chrono::high_resolution_clock>> stping;
    
    
	//trace loop ����
    for (int ttl = 0; ttl < Max_hop; ttl++) {
		pingL.CreateLayout();
		int j = 0;
		std::chrono::time_point< std::chrono::high_resolution_clock> start;
		//ping �迭 �ʱ�ȭ
		ping.clear();
		stping.clear();
		//���� ttl����
		if (setsockopt(trace.getSock(), IPPROTO_IP, IP_TTL, (const char*)&ttl, sizeof(ttl)) < 0) {
			qDebug() << "Failed set ttl : " << WSAGetLastError();
		}
		else qDebug() << "Success set ttl : " << ttl ;

		// ��Ŷ 3��������
		for (int i = 0; i < 3; i++) {
			icmp.Send(trace.getSock(), ttl, destip, i);
			//�۽� ���۽ð�
			stping.push_back(std::chrono::high_resolution_clock::now());
		}
		//ttl ���
		pingL.setTTL(ttl);
		M->addLayout(pingL.getMainLayout());
		//���Ÿ��
		while (true) {
			//����
			std::string Rresult = icmp.Receive(trace.getSock());

			//Ÿ�Ӿƿ� Ȯ��
			if (Rresult != "") {
				if (j >= 3) {
					break;
				}
				//�����޽���
				ping.push_back(0.0);
			}
			else {
				//ping ���
				auto end = std::chrono::high_resolution_clock::now();
				auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - stping[j]);
				int durms = static_cast<float>(dur.count());
				ping.push_back(durms);
			}
			//ping ���
			pingL.setPing(ping[j], j);


			j++;
		}
		//ip���
		std::string tempip;
		std::string ip = icmp.getIp();
		if (ip == tempip) {
			// Ÿ�Ӿƿ� ����ó��
			pingL.setIp("Access Failed");
		}
		else {
			//ip�������
			pingL.setIp(ip);
		}
		// ������ Ȯ��
		if ((int)icmp.getType() == 0) {
			
			ui.StartButton->setEnabled(true);
			return;
		}
		tempip = ip;


        
    }
	ui.INfor->setText("���� ip���� ���� ����\n");
	ui.StartButton->setEnabled(true);
	return;
}