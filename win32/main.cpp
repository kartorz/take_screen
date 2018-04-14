
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include <QtGUI>

#include "mainWindow.h"
#include "language.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	int argc = 0;
	QApplication app(argc, 0);

	//QTextCodec::codecForName("GB2312");
	QTextCodec *codec = QTextCodec::codecForLocale();
    //QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    //QTextCodec::setCodecForTr(codec); 

	QString strTitle = STR_UI_02;
	MainWindow win;
	win.setGeometry(120,50, 420, 280);
	win.setMaximumSize(460,280);
	win.setWindowTitle(strTitle);
	win.show();
	app.exec();
}
  