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
	QHBoxLayout* CreateLayout();
	QLabel* setPing(float ping, int Packetn);
	QLabel* setIp(std::string ip);
	QLabel* setTTL(int ttl);
	QHBoxLayout* getMainLayout();
private:
	QHBoxLayout* MainLayout;
	QVBoxLayout* ipPingLayout;
	QHBoxLayout* PingLayout;
	QLabel* TTL;
	QVector<QLabel*> Ping;
	QLabel* Ip;
	int z = 0;

};

