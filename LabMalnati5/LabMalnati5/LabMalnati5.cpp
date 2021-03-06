// LabMalnati5.cpp: definisce il punto di ingresso dell'applicazione console.
//
#include "stdafx.h"
#include <thread>
#include <iostream>
#include "stdafx.h"
#include "MessageLoop.h"
#include "PriorityQueue.h"

//using namespace std;

int main()
{
	MessageLoop ml;
	//thread t(&MessageLoop::worker, ml, (PriorityQueue*) 0);
	//thread t(foo); // it works..

	ml.startup();

	try {
		ml.startup();
	}
	catch (...) {
		std::cout << "Exception caught, you cannot do startup two times\n";
	}

//	typedef T another_name_for_T;
	typedef std::packaged_task<void()> task;

	/*auto t3 = std::make_shared< task >([]() {std::cout << "third message\n"; });
	auto t4 = std::make_shared< task >([]() {std::cout << "4th message\n"; });
	auto t5 = std::make_shared< task >([]() {std::cout << "5th message\n"; });
	auto t6 = std::make_shared< task >([]() {std::cout << "6th message\n"; });

	ml.post_message(t3);
	ml.post_message(t4);
	ml.post_message(t5);
	ml.post_message(t6);*/


	auto t1 = std::make_shared< task >([]() {
		std::cout << "second message\n";
		throw 1; });
	auto t3 = std::make_shared< task >([]() {std::cout << "third message\n"; });
	auto t2 = std::make_shared< task >([&ml, t3]() {
		std::cout << "first message\n";
		ml.post_delayed(t3, std::chrono::milliseconds(1500)); });
	ml.post_delayed(t1, std::chrono::milliseconds(1000));
	ml.post_message(t2);
	auto f1 = t1->get_future();
	try {
		f1.get();
	}
	catch (...) {
		std::cout << "Exception caught in t1\n";
	}

	auto t4 = std::make_shared< task >([]() {std::cout << "4th message\n"; });

	ml.post_delayed(t4, std::chrono::milliseconds(5000));

	//std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	ml.shutdown();

	system("pause");
}

