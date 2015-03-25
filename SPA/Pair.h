#pragma once
template <typename T>

class Pair
{
private:
	T firstItem;
	T secondItem;
public:
	Pair(void);
	~Pair(void);

	void setFirst(T);
	void setSecond(T);
	
	T getFirst();
	T getSecond();
};

