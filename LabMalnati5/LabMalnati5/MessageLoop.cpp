#include "stdafx.h"
#include "MessageLoop.h"

MessageLoop::MessageLoop()
{
	valid = true;
	cout << "Starting message loop" << endl;
}


MessageLoop::~MessageLoop()
{
}

void MessageLoop::startup() {
	bool start = false;
	call_once(of_start, [&start]() {start = true; });
	if (start == false)
		throw exception();
	for (int i = 0; i < nthread; i++) {
		threads.push_back(thread{&MessageLoop::worker, this});
	}

}
void MessageLoop::shutdown() {
	bool shutdown = false;
	call_once(of_shutdown, [&shutdown]() {shutdown = true; });
	if (shutdown == false)
		throw exception();

	cout << "Closing Message loop\n";

	pq.invalidate();
	for (int i = 0; i < nthread; i++) {
		threads[i].join();
	}
}


void MessageLoop::post_message(std::shared_ptr< std::packaged_task<void()> > pt) {
	pq.push(pt);
}
void MessageLoop::post_delayed(std::shared_ptr< std::packaged_task<void()> > pt, std::chrono::milliseconds delay) {
	pq.push_delayed(pt,delay);
}


void MessageLoop::worker(/*PriorityQueue &PQ*/) {
	bool goahead = true;
	shared_ptr<packaged_task<void()>> task;

	while(goahead == true) {
		goahead = pq.pop(task);
		if (goahead == true) {
			(*task)();
		}
	}
}