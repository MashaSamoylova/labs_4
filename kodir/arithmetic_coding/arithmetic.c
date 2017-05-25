#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "my_library.h"

int DEL;
int FLAG = 0;
int DEBUG_MODE = 0;

void printf_b(symbol_with_freq b[], int n) {
	int i;
	for(i =0 ; i< n; i++) {
		printf("symbol: %c freq: %d\n", b[i].symbol, b[i].frequency);
	}
}

int count_frequency(char *s, symbol_with_freq b[]) {
	int i;
	int n = 2;
	b[0].symbol = '-';
	b[0].frequency = 0; //symbol of end of file, encode with it
	b[1].symbol = '\0';
	b[1].frequency = 1;
	for(i = 0; s[i]; i++) {
		int k;
		for(k = 1; k < n; k++) {
			if(s[i] == b[k].symbol) {
				b[k].frequency+=1;
				break;
			}
		}

		if(k == n) {
			b[k].symbol = s[i];
			b[k].frequency = 1;
			n++;
		}
	}

	for(i = 1; i<n; i++) {
		b[i].frequency += b[i-1].frequency;
	}
	DEL = b[n-1].frequency;
	if(DEBUG_MODE) printf_b(b,n);
	b[n].symbol = -1;
	b[n].frequency = -1;
	return 0;
}

int to_int(char *s) {
	int i;
	int value = 0;
	for(i = 0; i < 16; i++) {
		if('\0' == s[i]) {
			return -1;
		}
		if('1' == s[i]) {
			value = value | (1 << (15-i));
		}	
	}
	return value;
}

int input_bit(char* s) {
	if('1' == *s) {
		return 1;
	}
	if('0' == *s) {
		return 0;
	}
	if(FLAG) return 0;
	FLAG = 1;
	return 1;
}

void decode(char* code, symbol_with_freq b[], char* decoded_s) {
	int value;
	int length_of_code = strlen(code);
	int l[length_of_code]; //исходный рабочий интервал
	int h[length_of_code];
	h[0] = 65535;
	l[0] = 0;
	int first_qtr = h[0]/4+1;
	int half = 2*first_qtr;
	int third_qtr = 3*first_qtr;
	int range;
	int index = 0;
	int i = 1;
	int cum = 0;
	int c_index;
	value = to_int(code+index);
	index = 16;
	int bit;

	decoded_s[0] = '\0';
	int k = 0;

	while(1) {
		if(DEBUG_MODE) printf("=================================\n");
		if(-1 == value) {
			printf("ERORR\n");
			exit(0);
		}
		range = (h[i-1] - l[i-1]) + 1;
		cum = ((value - l[i-1] + 1) * DEL - 1) / range;
		
		for(c_index = 1; b[c_index].frequency <= cum; c_index++);
		if(DEBUG_MODE) printf("DECODE SYMBOL: %c\n", b[c_index].symbol);
		
		decoded_s[k] = b[c_index].symbol;
		decoded_s[k+1] = '\0';
		k++;
		
		l[i] = l[i-1] + (range*b[c_index-1].frequency)/DEL;
		h[i] = l[i-1] + (range*b[c_index].frequency)/DEL -1;

		if(DEBUG_MODE) {
			printf("value = %d\nrange = %d\n", value, range);
			printf("[%d;%d]\n", l[i], h[i]);
		}

		for(;;) {
			if(h[i] >= half) {
				if(l[i] >= half) {
					value -= half; 
					l[i] -= half;
					h[i] -= half;
				}
				else if(l[i] >= first_qtr && h[i] < third_qtr) {
					value -= first_qtr;
					l[i] -= first_qtr;
					h[i] -= first_qtr;
				}
				else break;
			}
			l[i] = 2*l[i];
			h[i] = 2*h[i] + 1;
			value = 2*value + input_bit(code+index);
			index++;
		}
		i++;
		if(DEBUG_MODE) printf("--------------------------");
		if( '\0' == b[c_index].symbol) { 
			break;
		}
	}
}

//search sembol in table of frequency
int search(symbol_with_freq b[], char c) {
	int i = 0;
	while(b[i].symbol != c) {
		if(-1 == b[i].symbol) {
			printf("NOT FOUND\n");
			exit(4567543);
		}
		i++;
	}
	return i;
}

void add_bit(int bit, char* code) {
	int n = strlen(code);
	if(bit == 0) {
		code[n] = '0';
	}
	else {
		code[n] = '1';
	}

	code[n+1] = '\0';
}

int write_bits(int bit, int bits_to_foll, char* code) {
	
	add_bit(bit, code);

	while(bits_to_foll > 0) {
		add_bit(!bit, code);
		bits_to_foll -= 1;	   	   
	}
}

int encode(char *s, symbol_with_freq b[], char* code) {
	int length_of_string = strlen(s);
	int l[length_of_string]; //исходный рабочий интервал
	int h[length_of_string];
	h[0] = 65535;
	l[0] = 0;
	int first_qtr = h[0]/4+1;
	int half = 2*first_qtr;
	int third_qtr = 3*first_qtr;
	code[0] = '\0';	
	int i = 1;
	int range = 0;
	int c_index;
	int bits_to_foll = 0;
	
	while( (i-1) < (strlen(s)+1) ) {
		if(DEBUG_MODE) printf("------------------------------\n");
		c_index = search(b, s[i-1]);


		range = h[i-1] - l[i-1] + 1;

		l[i] = l[i-1] + (range * b[c_index-1].frequency)/DEL;
		h[i] = l[i-1] + (range * b[c_index].frequency)/DEL - 1;
		if(DEBUG_MODE) {
			printf("range = %d\n", range);
			printf("[l[i];h[i]] [%d;%d]\n", l[i], h[i]);
			printf("[l[i-1];h[i-1]] [%d;%d]\n", l[i-1], h[i-1]);
		}
		for(;;) {
			if(h[i] < half) {
				write_bits(0, bits_to_foll, code);
				bits_to_foll = 0;
			}
			else if(l[i] >= half) {
				write_bits(1, bits_to_foll, code);
				bits_to_foll = 0;
				//Нормализуем интервал, уменьшая значения границ, если они находятся во второй части рабочего интервала
				l[i] -= half;
				h[i] -= half;
			}
			// Если текущий интеpвал  содеpжит сеpедину исходного, то вывод еще одного обpатного бита позже, а сейчас убpать общую часть
			else if(l[i] >= first_qtr && h[i] < third_qtr) {
				bits_to_foll +=1;
				l[i] -= first_qtr;
				h[i] -= first_qtr;
			}
			else break;
			//расширяем рабочий интервал
			l[i] = 2*l[i];
			h[i] = 2*h[i] + 1;
		}
		i++;
	}

	return 0;
}


int main(int argc, char* argv[]) { 

	char s[101];
	srand(time(NULL));
//	printf("input string: \n");	
	scanf("%s", s);

	symbol_with_freq b[strlen(s)];

	char code[500];
	char decoded_s[500];

	count_frequency(s, b);
	encode(s,b,code);
	if(DEBUG_MODE) printf("%s\n", code);
	decode(code, b, decoded_s);
	if(DEBUG_MODE)printf("%s\n", decoded_s);
	printf("%d\n", strcmp(s, decoded_s));

	return 0;
}
