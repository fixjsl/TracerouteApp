#include "TracerouteApp.h"
#include "Traceroute.h"
TracerouteApp::TracerouteApp(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

TracerouteApp::~TracerouteApp()
{}



void TraceLoop(int Max_hop,std::string destiproute) {
    sockaddr_in destip;
    std::vector<int> ping;
    std::string routeip;
    Traceroute tr;
    for (int i = 0; i < Max_hop; i++) {
        if (tr.traceroute(destip, i) == true) {
            break;
        }
        //ttl출력
        //ip출력
        //ping출력
    }
}