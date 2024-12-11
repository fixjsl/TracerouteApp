#pragma once

#include <QtWidgets/QWidget>
#include "Traceroute.h"
#include "ui_TracerouteApp.h"
#include <qmessagebox.h>
#include "InforLayout.h"

class TracerouteApp : public QWidget
{
    Q_OBJECT

public:
    TracerouteApp(QWidget *parent = nullptr);
    ~TracerouteApp();
    void TraceLoop();
private:
   Traceroute trace;
    Ui::TracerouteAppClass ui;
};
