#include <stdio.h>
//Test Anything on class
void InsertionSort(int list[], int len) {
	int i, j, key;
	for (i = 1; i < len; i++) {
		key = list[i];
		for (j = i - 1; j >= 0 && list[j] > key; j--) {
			list[j + 1] = list[j];
		}
		list[i + 1] = key;
	}
}

void BubbleSort(int list[], int len) {
	int i, j, temp;

}

int main() {
	int arr[10] = { 1,3,4,2,5,6,8,7,10,9 };
	InsertionSort(arr, sizeof(arr) / sizeof(int));
	return 0;
}