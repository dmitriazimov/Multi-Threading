#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <string>
#include <vector>

/*
	A producer stores data in a container in increments.
	A consumer simultaneously reads from the container and clears is.
*/

using namespace std;

vector<string> el_superDuperPoem = { 
"If you want to wear those shoes",
"Put them on.",
"There is no agreement of purposes,",
"In the first place.",
"You will do as you wish.",
"And I know this!",

"If you want to declare you are right,",
"When wrong stares me in the face.",
"And you want me to believe,",
"What you perceive.",
"Do it!",
"There is no argument.",

"Whatever you choose to do from now on...",
"That is up to you.",
"Conditions of programming,",
"Have been accepted as they are!",
"You see things as you wish them to exist.",
"And I choose to refrain from becoming a conformist.",

"However...",
"Let's come to admit, ",
"We are both in conflict...",
"With the current nonsense that is being dished!",
"And getting out of it provides neither one of us an exit!" };
vector<string>::iterator el_iterator;

int main()
{
	condition_variable el_alarm;
	mutex el_mutex;
	queue<string> el_queue;
	bool el_isNotified = false;
	bool el_haveData = true;

	thread el_producer([&el_mutex, &el_queue, &el_alarm, &el_isNotified, &el_haveData]() {

		// The producer reads the poem verse by verse
		for(el_iterator = el_superDuperPoem.begin(); el_iterator != el_superDuperPoem.end(); el_iterator++)
		{
			this_thread::sleep_for(chrono::milliseconds(500)); // Half a second intervals between verses
			lock_guard<mutex> lock(el_mutex);				   // Taking possession of the mutex for the duration of the scope before releasing
			el_queue.push(*el_iterator);					   // Placing the verse in a queue
			el_isNotified = true;							   // Allowing the consumer to be notified
			el_alarm.notify_one();							   // Unlocking the consumer tread waiting for this condition
		}

		lock_guard<mutex> lock(el_mutex);
		el_isNotified = true;
		el_haveData = false;								   // Declaring the producer has nothing else to store
		el_alarm.notify_one();
	});

	thread el_consumer([&el_mutex, &el_queue, &el_alarm, &el_isNotified, &el_haveData]() {

		while (el_haveData)					   // If the producer still has something to store
		{
			unique_lock<mutex> lock(el_mutex); // Taking unique ownership of the mutex, allowing the use of el_alarm condition variable

			while (!el_isNotified) 
			{
				el_alarm.wait(lock);		   // Waiting until the thread is notified
			}

			while (!el_queue.empty())		   // Reading and cleaning the verse container
			{
				cout <<  el_queue.front() << endl;
				el_queue.pop();
			}

			el_isNotified = false;
		}
	});

	el_producer.join();
	el_consumer.join();


	cin.get();
	cin.get();

	return 0;
}