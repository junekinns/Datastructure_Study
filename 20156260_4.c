#include <stdio.h>
#define TRUE 1 
#define FALSE 0
#define SWAP(a, b)((a) != (b) ? ((a) ^= (b), (b) ^= (a), (a) ^= (b)) : ((a), (b)))
#define MAX(a, b)((a) > (b) ? (a) : (b))
//typedef, struct, Functions Declaration
//
//int main() {
//	return 0;
//}