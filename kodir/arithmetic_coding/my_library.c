#include <stdio.h>
#include "my_library.h"

void QuickSort(symbol_with_freq q[], int begin, int end) {
		int mediana = q[begin + (end-begin)/2].frequency;

		int left = begin;
		int right = end;

		while(left <= right) {
			while(q[left].frequency > mediana) {
				left++;
			}
			while(q[right].frequency < mediana) {
				right--;
			}
			if(left <= right) {
				symbol_with_freq cup;
				cup.symbol = q[left].symbol;
				cup.frequency = q[left].frequency;

				q[left].symbol = q[right].symbol;
				q[left].frequency = q[right].frequency;

				q[right].symbol = cup.symbol;
				q[right].frequency = cup.frequency;
				
				left++;
				right--;
			}
		}
		if(left < end) QuickSort(q, left, end);
		if(begin < right) QuickSort(q, begin, right);
}

