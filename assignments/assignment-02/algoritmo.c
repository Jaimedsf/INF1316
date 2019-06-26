#include "algoritmo.h"

int LRU(Frame* listFrames, int lenList, int* nextIndexes) {
	int index, max_time = 0x0FFFFFFF;

	for (int i = 0; i < lenList; ++i)
		if (listFrames[i].time < max_time) {
			index = i;
			max_time = listFrames[i].time;
		}
		
	return index;
}

int NRU(Frame* listFrames, int lenList, int* nextIndexes) {

	int ind1 = -1, ind2 = -1, ind3 = -1;

	for (int i = 0; i < lenList; ++i) {
		if ((listFrames[i].flagR == '0') && (listFrames[i].flagW == '0'))
			return i;
		else if ((listFrames[i].flagR == '0') && (listFrames[i].flagW == '1'))
			ind1=i;
		else if ((listFrames[i].flagR == '1') && (listFrames[i].flagW == '0'))
			ind2 = i;			
		else if ((listFrames[i].flagR == '1') && (listFrames[i].flagW == '1'))
			ind3 = i;	
	}
	
	if(ind1 != -1)
		return ind1;
	else if (ind2 != -1)
		return ind2;
	else
		return ind3;
}

int NOVO(Frame* listFrames, int lenList, int* nextIndexes) {
	return 0;
}

