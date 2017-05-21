/**
 * Implementation of the Monitor class 
*/
#include "Monitor.h"

/* Constructor, destructor */
Monitor::Monitor() {
	this->toParse = new queue<string>();
	this->toParse_lock = new mutex();
	this->writer = nullptr;				/* default */
	this->numThreads = 1;				/* default */
	this->af = 0;						/* default */
	this->cs = 0;						/* default */
	this->prevProcComplete = false;		/* initial val */
}

Monitor::~Monitor() {
	delete this->toParse;
	delete this->toParse_lock;
}

/* Preparation functions */
void Monitor::setWriter(Writer * writer) {
	this->writer = writer;
}

void Monitor::setParseParameters(int alleleFreq, int confScore) {
	this->af = alleleFreq;
	this->cs = confScore;
}

void Monitor::setNumThreads(int numThreads) {
	this->numThreads = numThreads;
}

/* Add data to the toParse vector */
void Monitor::addToParseData(string data) {
	/* Utilise locks to prevent concurrency issues */
	this->toParse_lock->lock();
	this->toParse->push(data);
	this->toParse_lock->unlock();
}

/* Indicates previous procedure is completed. Used to terminate this class'
executeParse() thread. */
void Monitor::signalPrevProcComplete() {
	this->prevProcComplete = true;
}

/* Thread that starts and manages parsing */
void Monitor::executeParse() {

	/* Create and run the threads that will parse data */
	thread * threads = new thread[this->numThreads];
	for (int i = 0; i < this->numThreads; i++) {
		threads[i] = thread(&Monitor::parseThread, this,
							this->toParse, this->toParse_lock, 
							this->writer, &this->prevProcComplete,
							this->af, this->cs);
	}

	/* Wait for threads to self-terminate */
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}

	/* Clean up */
	delete threads;

}

/* Thread that does the parsing */
void Monitor::parseThread(queue<string> * toParse, mutex * toParseLock,
				Writer * writer,
				bool * prevProcComplete,
				int alleleFreq, int confScore) {

	/* Keep running as long as the previous process has not yet been completed */
	while (!(*prevProcComplete)) {

		/* ToDo... */
	}

}
