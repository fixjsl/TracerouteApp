#pragma once

#include <QtWidgets/QWidget>
#include "Traceroute.h"
#include "ui_TracerouteApp.h"
#include <qmessagebox.h>
#include "InforLayout.h"
#include "TraceThread.h"
#include <qthread.h>


class TracerouteApp : public QWidget
{
    Q_OBJECT

public:
    TracerouteApp(QWidget *parent = nullptr);
    ~TracerouteApp();
    void ClearLayout(QLayout* layout);
    void TraceLoop();
    Traceroute trace;
    QMutex mutex;
    QString sharedData;
    
    
public slots:
    void updateTraceLayout(int ttl, InforLayout* pingL);
    void ChangeIp(std::string ip, InforLayout* pingL);
    void ChangePing(float pingres, InforLayout* pingL, int j);
    void URLError(const QString& me);
    void TimeoutError(const QString& me, InforLayout* pingL);
    void comple(const QString& me);
private:
    int z = 0;
    Ui::TracerouteAppClass ui;
};
