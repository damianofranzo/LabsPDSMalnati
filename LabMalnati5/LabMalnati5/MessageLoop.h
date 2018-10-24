#pragma once
#include "PriorityQueue.h"
#include <iostream>
#include <thread>
class MessageLoop
{
public:
	//int nthread = thread::hardware_concurrency() - 1;
	int nthread = 1;

	PriorityQueue pq;
	once_flag of_start,of_shutdown;
	vector<thread> threads;
	bool valid;

	void startup();
	void shutdown();
	void worker();
	void post_message(std::shared_ptr< std::packaged_task<void()> > pt);
	void post_delayed(std::shared_ptr< std::packaged_task<void()> > pt, std::chrono::milliseconds delay);


	MessageLoop();
	~MessageLoop();
	static void st_worker(PriorityQueue PQ) {
		bool goahead = true;
		shared_ptr<packaged_task<void()>> task;

		while(goahead == true) {
		goahead = PQ.pop(task);
		if (goahead == true)
			task->get_future().get();
	}
	}
};

