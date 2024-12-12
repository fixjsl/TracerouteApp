#include "TracerouteApp.h"
#include "QQQ.h"
static TraceThread* work = new TraceThread();
static TaskQueue taskQueue;
QThread* mul;
TracerouteApp::TracerouteApp(QWidget *parent)
    : QWidget(parent)
{
	trace.Init();
    ui.setupUi(this);
	ui.scrollAreaWidgetContents->setMinimumSize(600, 800);
	connect(ui.StartButton, &QPushButton::clicked, this, &TracerouteApp::TraceLoop);
}

TracerouteApp::~TracerouteApp()
{}

void TracerouteApp::ClearLayout(QLayout* layout) {
	if (!layout) return;

	// 레이아웃에 있는 모든 항목을 순차적으로 제거합니다.
	QLayoutItem* item;
	while ((item = layout->takeAt(0)) != nullptr) {
		// 위젯이 있다면 삭제
		if (item->widget()) {
			delete item->widget();
		}
		// 레이아웃이 있다면 재귀적으로 비우기
		if (item->layout()) {
			ClearLayout(item->layout());
		}
		delete item;
	}
}



//button 연결 함수
void TracerouteApp::TraceLoop() {
	ui.StartButton->setEnabled(false);
	// 기존 레이아웃을 비웁니다.
	ClearLayout(ui.scrollL->layout());
	qDebug() << "whatthef" << "\n";
	//문자열 받아오기
	std::string destiproute = ui.URLEnter->text().toStdString();
	//공백 확인
	if (destiproute == ""){
		QMessageBox::warning(nullptr, "Error", "Invailed URL");
	ui.StartButton->setEnabled(true);
	return;
}	
	ui.INfor->setText("Destination DoMain : " + QString::fromStdString(destiproute));
	//멀티스레딩으로 traceroute 실행
	mul = new QThread(this);

	
	work->setDestadd(destiproute);
	work->setTraceSock(trace);

	work->moveToThread(mul);
	mul->start();
	//ui동기화를 위한 함수 연결
	if (!connect(mul, &QThread::started, work, &TraceThread::StartTT, Qt::QueuedConnection)) {
		QMessageBox::warning(nullptr, "URL ERROR", "Invailed URL");
	}
	connect(work, &TraceThread::ChangeIp, this, &TracerouteApp::ChangeIp);
	connect(work, &TraceThread::ChangePing, this, &TracerouteApp::ChangePing);
	
	connect(work, &TraceThread::updateTraceLayout, this, &TracerouteApp::updateTraceLayout);
	connect(work, &TraceThread::URLError, this, &TracerouteApp::URLError);
	connect(work, &TraceThread::TimeoutError, this, &TracerouteApp::TimeoutError);
	connect(work, &TraceThread::comple, this, &TracerouteApp::comple);
	connect(mul, &QThread::finished,mul, &QObject::deleteLater);

	


}
void TracerouteApp::updateTraceLayout(int ttl,  InforLayout* pingL)

{	
	taskQueue.addTask([=]() {
		qDebug() << "CreateLayout" << "\n";
		pingL->CreateLayout();
		pingL->setTTL(ttl);
		ui.scrollL->addLayout(pingL->getMainLayout());
		});
}

void TracerouteApp::ChangeIp(std::string ip, InforLayout* pingL)
{	
	taskQueue.addTask([=]() {
		qDebug() << "ipf" << "\n";
		pingL->setIp(ip); });
}

void TracerouteApp::ChangePing(float pingres , InforLayout* pingL, int j)
{	
	taskQueue.addTask([=]() {
		qDebug() << "pingf" << "\n";
		pingL->setPing(pingres, j); });
}

void TracerouteApp::URLError(const QString& me)
{	
	taskQueue.addTask([=]() {
		qDebug() << "whatthef" << "\n";
		QMessageBox::warning(nullptr, "URL ERROR", "Invailed URL"); });
}

void TracerouteApp::TimeoutError(const QString& me, InforLayout* pingL)
{	
	taskQueue.addTask([=]() {  // 포인터를 그냥 캡처
		qDebug() << "whatthef" << "\n";
		pingL->setIp("Access Failed");
		} );
}

void TracerouteApp::comple(const QString& me)
{	
	taskQueue.addTask([=]() {
		qDebug() << "complete" << "\n";
		ui.INfor->setText(me); });
	work->deleteLater(); // 워커 객체 삭제 예약
	mul->quit();         // 스레드 이벤트 루프 종료
	mul->wait();         // 스레드 종료 대기
	delete mul;          // 스레드 객체 삭제
	mul = nullptr;
	ui.StartButton->setEnabled(true);
}

   
