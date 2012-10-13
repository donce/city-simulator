#ifndef INC_AVERAGE_H
#define INC_AVERAGE_H


class average {

public:
	average();

	double GetAverage();
	double GetNow();
	
	void Add(double value, double time);
private:
	
	double t;//time
	double av;//average

	double now;

};


#endif