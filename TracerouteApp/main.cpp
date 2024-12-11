#include "TracerouteApp.h"
#include <QtWidgets/QApplication>
void GrnadtICMP() {
	system("netsh advfirewall firewall add rule name=\"Allow ICMP\" protocol=icmpv4 dir=in action=allow");
}
int checkICMPGR(const std::string& ruleName) {
	std::string command = "powershell.exe Get-NetFirewallRule | Where-Object {$_.DisplayName -like '*" + ruleName + "*'}";
	int result = system(command.c_str());
	return result == 0;
}
int main(int argc, char *argv[])
{
	if (!checkICMPGR("Allow ICMPv4-In")) {
		GrnadtICMP();
	}

    QApplication a(argc, argv);
    TracerouteApp w;
    w.show();
    return a.exec();
}
