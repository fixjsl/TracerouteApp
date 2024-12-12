#include "InforLayout.h"
#include "TracerouteApp.h"
QHBoxLayout* InforLayout::CreateLayout()
{   
    
    Ping.clear();
    TTL = new QLabel("1");
    TTL->setAlignment(Qt::AlignCenter);
    QPixmap TTi(":/TracerouteApp/TTL.png");
    TTL->setPixmap(TTi);
    TTL->setScaledContents(true);
    
    Ping.append(new QLabel("Ping 1 : "));
    Ping.append(new QLabel("Ping 2 : "));
    Ping.append(new QLabel("Ping 3 : "));
    
    Ip = new QLabel("IP : ");
    MainLayout = new QHBoxLayout();
    PingLayout = new QHBoxLayout();
    ipPingLayout = new QVBoxLayout();
    
    MainLayout->addWidget(TTL,1);

    PingLayout->addWidget(Ping[0]);
    PingLayout->addWidget(Ping[1]);
    PingLayout->addWidget(Ping[2]);

    ipPingLayout->addLayout(PingLayout);
    ipPingLayout->addWidget(Ip);

    MainLayout->addLayout(ipPingLayout,3);


    return MainLayout;
}

QLabel* InforLayout::setPing(float ping, int Packetn)
{
    
    if (ping == -1.0) {
        int p = Packetn + 1;
        std::string pi = std::to_string(p);
        std::string text = "Ping" + pi;
        text = text + ": *";
        Ping[Packetn]->setText(QString::fromStdString(text));
        z++;
    }
    else if (ping <= 1.0&&ping >0.0) {
        int p = Packetn + 1;
        std::string pi = std::to_string(p);
        std::string text = "Ping" + pi;
        text = text + "<=1";
        Ping[Packetn]->setText(QString::fromStdString(text));
    }
    else if(ping >1.0){
        int p = Packetn + 1;
        std::string pi = std::to_string(p);
        std::string text = "Ping" + pi;
        text = text + ": "+std::to_string(ping)+"ms";
        Ping[Packetn]->setText(QString::fromStdString(text));
    }
    return Ping[Packetn];
}

QLabel* InforLayout::setIp(std::string ip)
{
    if (Ip == nullptr) {
        Ip = new QLabel(); // 부모 설정
    }
    if (z == 3) {
        Ip->setText("Access Failed");
        z = 0;
    }
    else {
    ipPingLayout->addWidget(Ip);
    Ip->setText("IP : "+QString::fromStdString(ip));
    }
    
    return Ip;
}

QLabel* InforLayout::setTTL(int ttl)
{
    
    TTL->setText("ttl : "+QString::fromStdString(std::to_string(ttl+1)));
    return TTL;
}

QHBoxLayout* InforLayout::getMainLayout()
{
    
    return MainLayout;
}
