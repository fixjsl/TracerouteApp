#include "InforLayout.h"

void InforLayout::CreateLayout()
{   
    TTL = new QLabel("1");
    TTL->setAlignment(Qt::AlignCenter);
    QPixmap TTi(": / TracerouteApp / TTL.png");
    TTL->setPixmap(TTi);
    TTL->setScaledContents(true);
    Ping[0] = new QLabel("Ping 1 : ");
    Ping[1] = new QLabel("Ping 2 : ");
    Ping[2] = new QLabel("Ping 3 : ");
    Ip = new QLabel("IP : ");

    MainLayout->addWidget(TTL,1);

    PingLayout->addWidget(Ping[0]);
    PingLayout->addWidget(Ping[1]);
    PingLayout->addWidget(Ping[2]);

    ipPingLayout->addLayout(PingLayout);
    ipPingLayout->addWidget(Ip);

    MainLayout->addLayout(ipPingLayout,3);


    return;
}

void InforLayout::setPing(float ping, int Packetn)
{
    if (ping == 0.0) {
        Packetn += 1;
        std::string pi = std::to_string(Packetn);
        std::string text = "Ping" + pi;
        text = text + ": *";
        Ping[Packetn]->setText(QString::fromStdString(text));
    }
    else if (ping <= 1.0) {
        Packetn += 1;
        std::string pi = std::to_string(Packetn);
        std::string text = "Ping" + pi;
        text = text + "<=1";
        Ping[Packetn]->setText(QString::fromStdString(text));
    }
    else {
        Packetn += 1;
        std::string pi = std::to_string(Packetn);
        std::string text = "Ping" + pi;
        text = text + ": "+std::to_string(ping)+"ms";
        Ping[Packetn]->setText(QString::fromStdString(text));
    }
}

void InforLayout::setIp(std::string ip)
{
    Ip->setText(QString::fromStdString(ip));
}

void InforLayout::setTTL(int ttl)
{
    TTL->setText(QString::fromStdString(std::to_string(ttl)));
}

QVBoxLayout* InforLayout::getMainLayout()
{
    return MainLayout;
}
