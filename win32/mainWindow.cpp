/*
 *  This is the main window
 *
 *  Author: LiQiong Lee
 *
 *  Changes:
 *     2013/07/08 : First creation. 
 */

#define CPLUS_SOURCE
#include "cmdutil.h"
#include "net/idc.h"

#include "MainWindow.h"
#include "language.h"

#include <QSlider>
#include <Windows.h>

extern "C" void take_screen(void);

#define MSG_ERROR(str) \
	{ \
		QMessageBox msgBox(this); \
		msgBox.setText(str); \
		msgBox.setWindowTitle("Error"); \
		msgBox.exec(); \
	} 

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
	QToolBar* toolBar = new QToolBar;
	{
	#define ADD_TOOLBUTTON(lable, fun) {\
		CToolButton * btn = new CToolButton(lable, toolBar); \
		connect(btn, SIGNAL(clicked()), this, SLOT(fun)); \
		toolBar->addWidget(btn);\
		toolBar->addSeparator(); \
		}

		ADD_TOOLBUTTON(STR_BUTTON_01, OnBtnProjectionClicked())
		ADD_TOOLBUTTON(STR_BUTTON_02, OnBtnPauseClicked())
		//ADD_TOOLBUTTON(STR_BUTTON_03, OnBtnTakeShotClicked())
		ADD_TOOLBUTTON(STR_BUTTON_05, OnBtnScanningClicked())
		ADD_TOOLBUTTON(STR_BUTTON_04, OnBtnExitClicked())
	}

	QGroupBox* gridGroupBox = new QGroupBox(STR_UI_01);
	{
		QGridLayout *layout = new QGridLayout;
		QLabel* ipLabel = new QLabel(STR_LABLE_01);
		m_ipInput = new QLineEdit;
		//m_ipInput->setInputMask(QString("000.000.000.000"));
		//ipInput->setValidator( new QRegExpValidator( QRegExp("[0-255].[0-255].[0-255].[0-255]" , Qt::CaseInsensitive), this));
		layout->addWidget(ipLabel, 1, 0, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(m_ipInput, 1, 1, Qt::AlignLeft | Qt::AlignTop);
		QLabel* lblPort = new QLabel(STR_LABLE_02);
		m_portInput = new QLineEdit("8001");
		layout->addWidget(lblPort, 1,2, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(m_portInput, 1,3, Qt::AlignLeft | Qt::AlignTop);

		QLabel* lblRate = new QLabel(STR_LABLE_03);
		QSlider *srRate = new QSlider(Qt::Horizontal,this);
		srRate->setMinimum(1);
		srRate->setMaximum(60);
		srRate->setMinimumWidth(130);

		m_srRateVal = new QLabel("15");
		srRate->setValue(15);
		connect(srRate, SIGNAL(valueChanged(int)),this,SLOT(OnDisplaySrFrameRateValue(int)));

		layout->addWidget(lblRate, 2,0, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(srRate, 2,1, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(m_srRateVal, 2, 2, Qt::AlignLeft | Qt::AlignTop);

		QLabel* lblQuality = new QLabel(STR_LABLE_04);
		QSlider *srQuality = new QSlider(Qt::Horizontal,this);
		srQuality->setMinimum(1);
		srQuality->setMaximum(100);
		srQuality->setValue(50);
		srQuality->setMinimumWidth(130);
		connect(srQuality, SIGNAL(valueChanged(int)),this,SLOT(OnDisplaySrQualityValue(int)));
		m_srQualityVal = new QLabel("50");

		layout->addWidget(lblQuality, 3,0, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(srQuality, 3,1, Qt::AlignLeft | Qt::AlignTop);
		layout->addWidget(m_srQualityVal, 3,2, Qt::AlignLeft | Qt::AlignTop);

		m_listWtIP = new QListWidget(this);
		connect(m_listWtIP, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(OnListItemDoubleClicked(QListWidgetItem *)));
		layout->addWidget(m_listWtIP, 4, 0, 3, 4, Qt::AlignLeft | Qt::AlignTop);

		layout->setColumnStretch(1, 140);
		layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		gridGroupBox->setLayout(layout);

	}

	{
		m_statusBar = new QStatusBar(this); 
		m_statusBar->setMaximumHeight(30);
		//QLabel* m_statusInfo = new QLabel("Hello");
	}
    QVBoxLayout *layout = new QVBoxLayout;
    //layout->setSpacing(0);
    //layout->setMargin(0);
	layout->addWidget(toolBar);
	layout->addWidget(gridGroupBox);
	layout->addWidget(m_statusBar);
    setLayout(layout);
    //setMouseTracking(true);
	m_coreJob = NULL;

	m_systray = new QSystemTrayIcon(this);
	QIcon icon("projector.png"); 
	m_systray->setIcon(icon);
	m_systray->show();

	connect(m_systray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(OnSysTrayActivated(QSystemTrayIcon::ActivationReason)));

	m_statusBar->showMessage(STR_UIINFO_WELCOME);
	m_statusInfo.append(STR_UIINFO_CONNING);
	m_statusInfo.append(STR_UIINFO_NOACK);
	m_statusInfo.append(STR_UIINFO_PROCTION);
	m_statusInfo.append(STR_UIINFO_ERROR);
	m_statusInfo.append(STR_UIINFO_SCANNING);
	m_statusInfo.append(STR_UIINFO_WELCOME);

	m_messageProc = new MessageProcJob(this);
	m_coreJob = new CoreJob(this);

	m_messageProc->start();
	m_coreJob->start();
	OnBtnScanningClicked();
}

MainWindow::~MainWindow()
{
	delete m_coreJob;
	delete m_messageProc;
}

void MainWindow::OnBtnProjectionClicked()
{
	QString ip = m_ipInput->text();
	QString strPort = m_portInput->text();
	int port = strPort.toInt();

	if(ip.size() < 6)
	{
		MSG_ERROR(STR_MSG_01);
		return;
	}
	if(port<8001 || port>9000)
	{
		MSG_ERROR(STR_MSG_02);
		return;
	}

	setopt_ip(ip.toAscii().constData(), port);
	 
//	QPushButton *button = parentWidget->findChild<QPushButton *>("button1");

	m_coreJob->startProjection();
}
void MainWindow::OnBtnPauseClicked()
{
	m_coreJob->stopProjection();
	//m_statusBar->showMessage(STR_UIINFO_DONE);
	m_statusBar->showMessage(STR_UIINFO_WELCOME);
}
void MainWindow::OnBtnExitClicked()
{
	m_coreJob->abort();
	m_messageProc->abort();

	m_systray->hide();
    QCoreApplication::exit();
}

void MainWindow::OnBtnTakeShotClicked()
{
	//showMinimized();
	hide();
	Sleep(500);
	setopt_shot("jpg", NULL);
	take_screen();
}

void MainWindow::OnBtnScanningClicked()
{
	if(m_coreJob->getCurJobType() == 1)
	{
		MSG_ERROR(STR_MSG_03);
		return;
	}

	m_listWtIP->clear();
	m_coreJob->scanDevices();
	m_statusBar->showMessage(STR_UIINFO_SCANNING);
}

void MainWindow::OnGetDevices()
{
	struct device* pde;
	int size;
	size = idc_get_devices(&pde);
	for(;size>0;size--)
	{
		QString ip(pde[size-1].ip);
		m_listWtIP->addItem(ip);
	}
	m_statusBar->showMessage(STR_UIINFO_WELCOME);
}

void MainWindow::OnDisplaySrQualityValue(int val)
{ 
	OnDisplaySliderValue(val, m_srQualityVal);
	setopt_quality(val);
}
void MainWindow::OnDisplaySrFrameRateValue(int val)
{
	OnDisplaySliderValue(val, m_srRateVal);
	setopt_framerate(val);
}

void MainWindow::OnDisplaySliderValue(int val, QLabel* lbl)
{
	QString str;
	str.setNum(val);
	lbl->setText(str);
}
void MainWindow::OnListItemDoubleClicked(QListWidgetItem * item)
{
	//item->setCheckState(Qt::Checked);
	m_ipInput->setText(item->text());
}

void MainWindow::OnSysTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	showNormal();
}

int MainWindow::OnMessage(message* msg)
{
	switch(msg->id)
	{
		case SET_UISTATUS_STRING:
			if(msg->arg_int < m_statusInfo.size())
			{
				const QString& strMsg = m_statusInfo.at(msg->arg_int);
				m_statusBar->showMessage(strMsg);
			}
			break;
		default:
			break;
	}
	return 0;
}



bool MainWindow::winEvent(MSG * message, long * result)
{
	if(message->message == WM_SIZE)
	{
		if(message->wParam == SIZE_MINIMIZED)
			hide();
		goto EXIT;
	} 
	if(message->message == WM_CLOSE)
	{
		OnBtnPauseClicked();
		goto EXIT;
	}

EXIT:
	return false;
}

#if 0
void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_Backspace)
    {
		DialogQuit dlgConfirm(400,150,this);
		if(dlgConfirm.exec() == QMessageBox::Ok)
			Quit();
    }
	return QWidget::keyPressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	//(event->y()
	//set_mouse_position(event->x(), event->y());
	return QWidget::mouseMoveEvent(event);
}
#endif


