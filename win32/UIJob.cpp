/*
 *  This file is for threads.
 *
 *  Changes:
 *     JUL 29, 2013 : Create. (LiQiong Lee) 
 */

#define CPLUS_SOURCE
#include "UIJob.h"
#include "message.h"
#include "net/idc.h"

extern "C" void take_screen(void);
extern "C" void stop_taking_screen(void);

UIJob::UIJob(MainWindow* owner)
{
	m_owner = owner;
	m_reqAbort=0;
	m_thread = NULL;
//	m_isRunning =  false;
	m_jobInterval = 200;
}
UIJob::~UIJob()
{
	abort();
	m_thread->quit();
    m_thread->wait();
	delete m_thread;
}

void UIJob::start()
{
	if(!m_thread || !m_thread->isRunning())
	{
//		m_isRunning = true;
		m_thread = new QThread(this);
		connect(m_thread, SIGNAL(started()), this, SLOT(doWork()));
		connect(m_thread, SIGNAL(finished()), this, SLOT(exit()));
		//connect(m_thread, SIGNAL(terminated()), this, SLOT()
		moveToThread(m_thread);
		m_thread->start();
	}
}

void UIJob::doWork()
{
	do{
		OnJob();
		Sleep(m_jobInterval); 
		//todo, using Semaphore insteadof Sleep(200).
	}while(!m_reqAbort);
}

void UIJob::abort()
{
	if(m_thread && m_thread->isRunning())
	{
		m_reqAbort = 1;
		OnExit();
		m_thread->quit();
		m_thread->wait();
	}
	//m_isRunning = false;
}

void UIJob::exit()
{
}

CoreJob::CoreJob(MainWindow* owner) : UIJob(owner)
{
	m_jobInterval = 0;
	m_whattodo = -1;  //Do nothing
	m_semJob = new QSemaphore(1); 
}

void CoreJob::OnJob()
{
	if(m_whattodo == 1)
	{
		take_screen();//Has a while loop inner.
		m_whattodo = -1;
	}else if(m_whattodo == 2)
	{
		idc_scan_devices();
		m_owner->OnGetDevices();

		m_whattodo = -1;
		
	}else
	{
		m_semJob->acquire(1);
	}
}
void CoreJob::OnExit()
{
	if(m_whattodo == 1)
	{
		stop_taking_screen();
	}else
	{
		m_semJob->release(1);
	}
}

void CoreJob::startProjection()
{
	m_whattodo = 1;
	m_semJob->release(1);
}
void CoreJob::stopProjection()
{
	OnExit();
	m_whattodo = -1;
}

void CoreJob::scanDevices()
{
	m_whattodo = 2;
	m_semJob->release(1);
}


MessageProcJob::MessageProcJob(MainWindow* owner) : UIJob(owner)
{

}

void MessageProcJob::OnJob()
{
	int ret;
	message msg;
	ret = get_message(MSG_TARGET_ALL, &msg);
	//todo using QSemaphore insteadof polling in a loop.
	if(ret != -1)
	{
		if(msg.target_id == MSG_TARGET_UI)
		{
			m_owner->OnMessage(&msg);
		}
	}
}
