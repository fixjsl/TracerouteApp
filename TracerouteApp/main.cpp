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
void myMessageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
	{
		Q_UNUSED(context);
		Q_UNUSED(type);
		// QString을 std::wstring으로 변환
		std::wstring wmsg = msg.toStdWString();

		// OutputDebugStringW로 유니코드 문자열 출력
		OutputDebugStringW(wmsg.c_str());
	}
int main(int argc, char *argv[])
{
	if (!checkICMPGR("Allow ICMPv4-In")) {
		GrnadtICMP();
	}
	qInstallMessageHandler(myMessageHandler);
	qRegisterMetaType<InforLayout>("InforLayout");
	qRegisterMetaType<std::vector<float>>("std::vector<float>");
	qRegisterMetaType<std::shared_ptr<std::vector<float>>>("std::shared_ptr<std::vector<float>>");

    QApplication a(argc, argv);
    TracerouteApp w;
    w.show();
    return a.exec();
}
