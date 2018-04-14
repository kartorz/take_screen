#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QtGui>
#include <QVector>

#include "message.h"
#include "UIJob.h"
//#include <QSystemTrayIcon>

class CoreJob;
class MessageProcJob;

class CToolButton : public QToolButton
{
     Q_OBJECT

public:
	CToolButton(const QString &text, QWidget *parent = 0)
	{
		setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		//setToolButtonStyle(Qt::ToolButtonTextOnly);
		setText(text);
		// setAutoRaise(true);
	}

    QSize sizeHint() const
	{
		QSize size = QToolButton::sizeHint();
		size.rheight() += 25;
		size.rwidth() = qMax(size.width(), size.height());
		return size;
	}
};


class MainWindow: public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	int OnMessage(message* msg);
	void OnGetDevices();

protected slots:
    void OnBtnProjectionClicked();
	void OnBtnPauseClicked();
	void OnBtnExitClicked();
	void OnBtnTakeShotClicked();
	void OnBtnScanningClicked();

	void OnDisplaySrFrameRateValue(int);
	void OnDisplaySrQualityValue(int);

	void OnSysTrayActivated(QSystemTrayIcon::ActivationReason reason);
	void OnListItemDoubleClicked(QListWidgetItem * item);

protected :
	//void keyPressEvent(QKeyEvent* event);
	//void mouseMoveEvent(QMouseEvent* event);
	//void showEvent(QShowEvent * event);
	//void paintEvent(QPaintEvent * event);
	bool winEvent( MSG * message, long * result);
private:
	QLineEdit* m_ipInput;
	QLineEdit* m_portInput;
	QLabel*    m_srRateVal;
	QLabel*    m_srQualityVal;
	QListWidget * m_listWtIP;
	QStatusBar* m_statusBar;
	QVector<QString> m_statusInfo;

	QSystemTrayIcon* m_systray;

	CoreJob* m_coreJob;
	MessageProcJob* m_messageProc;

	void OnDisplaySliderValue(int val, QLabel* lbl);
};

#endif