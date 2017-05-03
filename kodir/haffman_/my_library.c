#include <stdio.h>
#include "my_library.h"
#include "string.h"

void QuickSort(struct node *q[], int begin, int end) {
		int mediana = (*q[begin + (end-begin)/2]).frequency;

		int left = begin;
		int right = end;

		while(left <= right) {
			while((*q[left]).frequency > mediana) {
				left++;
			}
			while((*q[right]).frequency < mediana) {
				right--;
			}
			if(left <= right) {
				struct node cup;
				cup.symbol = (*q[left]).symbol;
				cup.frequency = (*q[left]).frequency;

				(*q[left]).symbol = (*q[right]).symbol;
				(*q[left]).frequency = (*q[right]).frequency;

				(*q[right]).symbol = cup.symbol;
				(*q[right]).frequency = cup.frequency;
				
				left++;
				right--;
			}
		}
		if(left < end) QuickSort(q, left, end);
		if(begin < right) QuickSort(q, begin, right);
}

//array a for a boolean vector, position - its clear, value - may be one or zero
int SetValue(BoolVector *v, int position, int value) {
	
	if(position > (CAPACITY * 32)) {
		printf("Мы так не договаривались\n");
		return -1;
	};

	if(value != 0 && value != 1) {
		printf("Да ты гонишь\n");
		return -1;
	}

	int block = position / 32; //known that we use int and nothing other. And no one cant it change:(
	int i = position % 32;
	if(value){
		v->mass[block] = (v->mass[block] | (1 << i));
	}
	else{
		v->mass[block] = (v->mass[block] & ~(1 << i));
	}	
	return 0;
}

int GetValue(BoolVector *v, int position) {
	if(position > v-> length) {
		printf("ты пьян, иди домой\n");
		return -1;
	}
	int block = position / 32;
	int i = position % 32;
	if(v -> mass[block] & (1<<i) ){
		return 1;
	}
	else return 0;
}

int ConcatVector(BoolVector *v, char* b) {
	if( (v->length + strlen(b)) > CAPACITY ) {
		printf("пфф..нет\n");
		return -1;
	}

	int i;
	for(i = 0; b[i]; i++) {
		if('1' == b[i]) {
			SetValue(v, v->length, 1);
			v->length++;
		}
		if('0' == b[i]) {
			SetValue(v, v->length, 0);
			v->length++;
		}
	}

	return 0;

}

void PrintBool(BoolVector *v){
	printf("n = %d\n", v -> length);
	int i;
	for(i = v -> length-1; i >= 0; i--){
		printf("%d", GetValue(v, i));
	}
	printf("\n");
}

