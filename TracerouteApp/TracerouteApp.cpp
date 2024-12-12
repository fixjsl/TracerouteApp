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
	QLayoutItem* temp;
	if ((temp = layout->takeAt(0)) != nullptr) {
		if (temp->widget()) {
			delete temp->widget();
		}
		if (temp->layout()) {
			delete temp->layout();
		}
		delete temp;
	}
}



//button ���� �Լ�
void TracerouteApp::TraceLoop() {
	ui.StartButton->setEnabled(false);
	// ���� ���̾ƿ��� ���ϴ�.
	
	if (ui.scrollL) {
		ClearLayout(ui.scrollL);
	}
	QVBoxLayout* nee = new QVBoxLayout(ui.verticalLayoutWidget);
	qDebug() << "whatthef" << "\n";
	//���ڿ� �޾ƿ���
	std::string destiproute = ui.URLEnter->text().toStdString();
	//���� Ȯ��
	if (destiproute == ""){
		QMessageBox::warning(nullptr, "Error", "Invailed URL");
	ui.StartButton->setEnabled(true);
	return;
}	
	ui.INfor->setText("Destination DoMain : " + QString::fromStdString(destiproute));
	//��Ƽ���������� traceroute ����
	mul = new QThread(this);

	
	work->setDestadd(destiproute);
	work->setTraceSock(trace);

	work->moveToThread(mul);
	mul->start();
	//ui����ȭ�� ���� �Լ� ����
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
	taskQueue.addTask([=]() {  // �����͸� �׳� ĸó
		qDebug() << "whatthef" << "\n";
		pingL->setIp("Access Failed");
		} );
}

void TracerouteApp::comple(const QString& me)
{	
	taskQueue.addTask([=]() {
		qDebug() << "complete" << "\n";
		ui.INfor->setText(me); });
	work->deleteLater(); // ��Ŀ ��ü ���� ����
	mul->quit();         // ������ �̺�Ʈ ���� ����
	mul->wait();         // ������ ���� ���
	delete mul;          // ������ ��ü ����
	mul = nullptr;
	ui.StartButton->setEnabled(true);
}

   
