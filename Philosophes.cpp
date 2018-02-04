#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>

/*
	Five philosophers are at a dinner table.
	Five forks are at their disposal.
	Only two philosophers can eat at a time.
	Each philosopher needs two forks to eat.
*/

using namespace std;

mutex fourchettes[5];									// Forks are locks

class  Philosophe										// A philosopher has two forks,
{														// a name, and a counter to keep
public:													// track of his meals
	string m_name;                        
	int m_left;
	int m_right;
	int count;
};

Philosophe *create(string name, int left, int right) {  // Creating a philosopher returns a philosopher pointer 
	Philosophe *x = new Philosophe();
	x->m_name = name;
	x->m_left = left;
	x->m_right = right;
	return x;
}

void eat(void *data) {
	Philosophe* bonHomme = (Philosophe*) data;			// Assinging which philosopher is eating
	fourchettes[bonHomme->m_left].lock();				// Eating locks two forks
	fourchettes[bonHomme->m_right].lock();

	string temp = bonHomme->m_name + " mange \n";
	cout << temp;
	this_thread::sleep_for(chrono::milliseconds(500));  // Eating takes half a second
	string temp2 = bonHomme->m_name + " a fini de manger \n";
	cout << temp2;

	fourchettes[bonHomme->m_left].unlock();				// Unlocking the forks for the next thread
	fourchettes[bonHomme->m_right].unlock();
}

int main(void) {

	thread threadId[5];									// 5 threads representing 5 philosophers
	Philosophe *all[5] = {								// Creating 5 philosophers
		create("Raouf", 0 ,1),
		create("Simon", 1, 2),
		create("Roxanne", 2,3),
		create("Alexandre", 3, 4),
		create("Talel", 0, 4) };

	while (true)										// The meals never ends
	{
		for (int i = 0; i < 5; ++i) {
			threadId[i] = thread(eat, all[i]);
			all[i]->count++;							// Keeping count of meals per philosopher
		}

		for (int i = 0; i < 5; ++i)
		{
			threadId[i].join();
		}

		cout << endl;

		for (int i = 0; i < 5; ++i)
		{												// Recap of the meal
			cout << all[i]->m_name << " a eu " << all[i]->count << " repas." << endl;
		}

		cout << endl;									// Next round
		cout << "Appuyez sur enter pour continuer" << endl;
		cout << endl;

		cin.get();
	}
	return 0;
}