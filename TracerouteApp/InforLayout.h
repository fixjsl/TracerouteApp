#pragma once
#include <QtWidgets/QWidget>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
class InforLayout
{
public:
	void CreateLayout();
	void setPing(float ping, int Packetn);
	void setIp(std::string ip);
	void setTTL(int ttl);
	QVBoxLayout* getMainLayout();
private:
	QVBoxLayout* MainLayout;
	QHBoxLayout* ipPingLayout;
	QVBoxLayout* PingLayout;
	QLabel* TTL;
	QVector<QLabel*> Ping;
	QLabel* Ip;

};

