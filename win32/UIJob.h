/*
 *  This file is for threads.
 *
 *  Changes:
 *     JUL 29, 2013 : Create. (LiQiong Lee) 
 */


#ifndef _UIJOB_H_
#define _UIJOB_H_

#include <QSemaphore>

#include "MainWindow.h"
class MainWindow;

class UIJob: public QObject
{
	Q_OBJECT

public slots:
	void doWork();
	void exit();
	
public:
	UIJob(MainWindow* owner);
	virtual ~UIJob();

	void start();
	void abort();

	virtual void OnExit() {};
	virtual void OnJob() {};

protected:
	MainWindow* m_owner;
	QThread *m_thread;
	//bool m_isRunning;
	int m_jobInterval;
	int m_reqAbort;
};

class CoreJob: public UIJob
{
public :
	CoreJob(MainWindow* owner);

	virtual void OnJob();
	virtual void OnExit();
	
	void stopProjection();
	void startProjection();

	void scanDevices();
	int  getCurJobType() {return m_whattodo;}

private:
	QSemaphore* m_semJob;      // a semaphore that guards 5 resources
	int m_whattodo;
};

class MessageProcJob: public UIJob
{
public :
	MessageProcJob(MainWindow* owner);
	virtual void OnJob();
//	virtual void OnExit();
};


#endif