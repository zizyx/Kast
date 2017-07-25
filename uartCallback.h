#ifndef UARTCALLBACK_H
#define UARTCALLBACK_H

class uartCallback {
	public:
	    // The prefix "cbi" is to prevent naming clashes.
	    virtual int cbiCallbackFunction(char*, int) = 0;
};


#endif