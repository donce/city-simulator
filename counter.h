#ifndef INC_COUNTER_H
#define INC_COUNTER_H

class counter {	
public:	
	void Init();
	double GetCount();
	counter();
private:
	__int64 counterFreq;
	__int64 counterLast;
};


#endif