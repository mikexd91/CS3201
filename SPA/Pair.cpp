#include "Pair.h"
template <typename T>
Pair::Pair(void){
}

template <typename T>
Pair::~Pair(void){
}

template <typename T>
void Pair::setFirst(T item){
	this->firstItem = item;
}

template <typename T>
void Pair::setSecond(T item){
	this->secondItem = item;
}

template <typename T>
T Pair::getFirst(){
	return this->firstItem;
}

template <typename T>
T Pair::getSecond(){
	return this->secondItem;
}