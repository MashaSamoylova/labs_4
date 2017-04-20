struct node{
		unsigned char symbol;
			int frequency;
				struct node *left;
					struct node *right;
};

typedef struct {
		unsigned char symbol;
			char code[9];
} symbol_code;


void QuickSort(struct node *q[], int begin, int end);

