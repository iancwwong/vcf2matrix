/**
 * Implementation of the Monitor class 
*/
#include "Monitor.h"

#include "Converter.h" 		/* Conversion of a single SNP into a ParsedSNP */
#include "Debug.h"

/* Constructor, destructor */
Monitor::Monitor() {
	this->toParse = new queue<string>();
	this->toParse_lock = new mutex();
	this->toParse_empty_cv = new condition_variable();
	this->empty_toParse = true;			/* toParse is empty during init */
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
	/* Synchronisation */
	this->toParse_lock->lock();
	this->toParse->push(data);
	this->empty_toParse = false;
	this->toParse_lock->unlock();
}

/* Indicates previous procedure is completed. Used to terminate this class'
executeParse() thread. */
void Monitor::signalPrevProcComplete() {

	/* Wait until list is empty */
	std::unique_lock<std::mutex> emptyToParse_lock(*this->toParse_lock);
	this->toParse_empty_cv->wait(emptyToParse_lock, [&] {
		return this->empty_toParse;
	});
	this->prevProcComplete = true;
}

/* Thread that starts and manages parsing */
void Monitor::executeParse() {

	/* Create and run the threads that will parse data */
	thread * threads = new thread[this->numThreads];
	for (int i = 0; i < this->numThreads; i++) {
		threads[i] = thread(&Monitor::parseThread, this,
							this->toParse, this->toParse_lock, 
							this->toParse_empty_cv, &this->empty_toParse,
							this->writer, &this->prevProcComplete,
							this->af, this->cs);
	}

	/* Wait for threads to self-terminate */
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}

	/* Clean up */
	cout << "Cleaning up threads..." << endl;
	delete [] threads;

}

/* Thread that does the parsing */
void Monitor::parseThread(queue<string> * toParse, mutex * toParseLock,
				condition_variable * toParse_empty_cv, bool * empty_toParse,
				Writer * writer,
				bool * prevProcComplete,
				int alleleFreq, int confScore) {

	/* Keep running as long as the previous process has not yet been completed */
	while (!(*prevProcComplete)) {

		/* Obtain the first item to parse */
		string toParseStr = "";
		toParseLock->lock();
		if (toParse->size() > 0) {
			toParseStr = toParse->front();
			toParse->pop();
		}

		/* Case when list is empty - notify the cv (in case prev process has completed) */
		else {
			*empty_toParse = true;
			toParse_empty_cv->notify_one();
		}
		toParseLock->unlock();

		/* Convert the obtained string */
		if (toParseStr.length() > 0) {
			Converter c;
			ParsedSNP * pSNP = c.convert(toParseStr, alleleFreq, confScore);

			/* Add to toWrite if not invalid SNP */
			if (pSNP != NULL) {
				writer->addToWriteData(pSNP);
			}
		}

		// PrintDebugStatement("Monitor::ParseThread", "Stil converting...");
	}

}
