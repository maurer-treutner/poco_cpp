/*
 * ThreadPoolSample.cpp
 *
 * Author: Peter Maurer
 *
 * Beispiel, das die Verwendung von Poco::Event illustriert.
 *
 * Copyright (C) 2013-2017 Maurer & Treutner GmbH & Co. KG, Leopoldhafen
 */

#include "ThreadDemo/ThreadPoolSample.h"
#include "Poco/SharedPtr.h"
#include "Poco/ThreadPool.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"

#include <limits>
#include <vector>

namespace ThreadDemo {

	using Poco::Runnable;
	using Poco::SharedPtr;
	using Poco::Thread;
	using Poco::ThreadPool;
	using Poco::Timespan;
	using Poco::Timestamp;
	using std::vector;

	// Die Klasse Worker dient in unserem Beispiel als Runnable - ihrer einzige Aufgabe
	// ist es, einen definierten Zeitraum im Thread zu verbleiben.
	//
	class SimpleWorker : public Runnable
	{
	public:
		SimpleWorker(){}

		virtual ~SimpleWorker(){}

		void run()
		{
			Thread::sleep(100);
		}
	};

	Timespan ThreadPoolSample::createThreadsFromPool(int noOfThreads, bool preallocated)
	{
		// We create the Threadpool anyway before starting time measurement
		int minCapacity=2;
		if (preallocated)
		{
			minCapacity=noOfThreads;
		}

		ThreadPool myThreadPool(minCapacity,noOfThreads,60,0);

/*		vector<Worker> workers;
		for (int i=0;i<myThreadPool.capacity();i++)
		{
			workers.push_back(Worker());
		}
*/
		SimpleWorker myWorker;

		Timestamp beforeThreadStart;

		for (int i=0;i<noOfThreads;i++)
		{
			myThreadPool.start(myWorker);
		}

		Timespan elapsedTime = beforeThreadStart.elapsed();

		myThreadPool.joinAll();

		return elapsedTime;
	}

	Timespan ThreadPoolSample::createThreadsWithSimpleThreads(int noOfThreads)
	{
		vector<SharedPtr<Thread>> simpleThreads;

		SimpleWorker myWorker;

		Timestamp beforeThreadStart;
		for (int i=0;i<noOfThreads;i++)
		{
			simpleThreads.push_back(new Thread());
		}

		for (int i=0;i<noOfThreads;i++)
		{
			SharedPtr<Thread> currentThread = simpleThreads.at(i);
			currentThread->start(myWorker);
		}

		Timespan elapsedTime = beforeThreadStart.elapsed();

		for (int i=0;i<noOfThreads;i++)
		{
			SharedPtr<Thread> currentThread = simpleThreads.at(i);
			currentThread->join();
		}

		return elapsedTime;
	}

	void ThreadPoolSample::run(std::ostream& os, std::istream& is)
	{
		const int noOfThreads = 10;
		Timespan noPreallocPoolSpan = createThreadsFromPool(noOfThreads,false);
		Timespan preallocPoolSpan = createThreadsFromPool(noOfThreads,true);
		Timespan simpleThreadSpan = createThreadsWithSimpleThreads(noOfThreads);

		std::cout<<"Creation of Threads:"<<std::endl
				 <<noOfThreads<<" Threads without preallocation of Threads in ThreadPool: "<<noPreallocPoolSpan.microseconds()<<"µs"<<std::endl
				 <<noOfThreads<<" Threads with preallocated threads in ThreadPool:        "<<preallocPoolSpan.microseconds()<<"µs"<<std::endl
				 <<noOfThreads<<" Threads not using ThradPool:                            "<<simpleThreadSpan.microseconds()<<"µs"<<std::endl;
	}
} /* namespace ThreadDemo */
