#pragma once

#include <QtWidgets/QWidget>
#include "ui_TracerouteApp.h"

class TracerouteApp : public QWidget
{
    Q_OBJECT

public:
    TracerouteApp(QWidget *parent = nullptr);
    ~TracerouteApp();

private:
    Ui::TracerouteAppClass ui;
};
