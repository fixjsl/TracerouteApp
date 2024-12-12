#pragma once
#include <QThread>
#include <QString>
#include <QLabel>
#include <QMutex>
#include <QWaitCondition>
#include <chrono>
#include <vector>
#include "TracerouteApp.h"
class TraceThread :public QThread
{
	Q_OBJECT

public:
	TraceThread(QObject* parent = nullptr) : QThread(parent) {}
	~TraceThread() {}

	void setDestadd(std::string destip);
	void setTraceSock(Traceroute t);
public slots:
	void StartTT() ;
protected:
	
	
private:
	void Traceloop(int ttl, ICMP &icmp, sockaddr_in destip, std::vector<float> &pingres,InforLayout* pingL);
signals:
	void updateTraceLayout(int ttl,  InforLayout* pingL);
	void ChangeIp(std::string ip, InforLayout* pingL);
	void ChangePing(float pingres, InforLayout* pingL, int j);
	void URLError(const QString& me);
	void TimeoutError(const QString& me, InforLayout* pingL);
	void comple(const QString& me);
private:
	std::string t_destip;
	Traceroute trace;
};

