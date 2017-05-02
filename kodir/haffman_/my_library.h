#define CAPACITY 1024

struct node{
	char symbol;
	int frequency;
	struct node *left;
	struct node *right;
};

typedef struct {
	unsigned char symbol;
	char code[9];
} symbol_code;

typedef struct {
	unsigned mass[CAPACITY];
	int length;
} BoolVector;


void QuickSort(struct node *q[], int begin, int end);
int SetValue(BoolVector *v, int i, int value);
int ConcatVector(BoolVector *v, char* b);
void PrintBool(BoolVector *v);

