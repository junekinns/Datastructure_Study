#include <stdio.h>
#include <ctype.h>
#define TRUE 1 
#define FALSE 0
#define MAX_QUEUE_SIZE 20
#define NEXTIDX(p)(p == (MAX_QUEUE_SIZE) ? (0):(p+1))
//typedef, struct, Functions Declaration
typedef char Data;
typedef struct _cQueue { //Circular Queue
	int front;
	int rear;
	Data queArr[MAX_QUEUE_SIZE+1]; //Circular Queue�̱⶧���� ���ڸ��� ������������ ���ܵд�.
}CQueue;
typedef CQueue Queue;
void QueueInit(Queue * pq);
int QIsEmpty(Queue * pq);
int Enqueue(Queue * pq, Data data);
Data Dequeue(Queue * pq);
Data QPeek(Queue * pq);
//typedef, struct, Functions Declaration

void QueueInit(Queue * pq) {
	pq->front = 0;
	pq->rear = 0;
}
int QIsEmpty(Queue * pq) {
	if (pq->front == pq->rear)
		return TRUE;
	else
		return FALSE;
}
int Enqueue(Queue * pq, Data data) { //ADDQUEUE�� �������θ� �˷��ֱ� ���� ��ȯ���� �ִ�(TRUE, FALSE)
	if (NEXTIDX(pq->rear) == pq->front) {
		printf("Fail : Queue Full  ");
		return FALSE;
	}
	pq->rear = NEXTIDX(pq->rear); //���� ť �̱� ������ pq->rear++ �̶�� �ϸ� �ȵȴ�. 
	pq->queArr[pq->rear] = data;
	return TRUE;
}
Data Dequeue(Queue * pq) {
	if (QIsEmpty(pq)) {
		printf("Queue Empty  ");
		return;
	}
	pq->front = NEXTIDX(pq->front);
	return pq->queArr[pq->front];
}
Data QPeek(Queue * pq) {
	if (QIsEmpty(pq)) {
		printf("Queue Empty");
		return;
	}
	return pq->queArr[NEXTIDX(pq->front)];
}
int main() {
	Queue q;
	QueueInit(&q);
	char ch; int num; int qsize = 0;
	while ((ch = getchar()) != EOF) { //EOF�� �Է¹����� ���α׷� ����.
		if (!isdigit(ch) && ch != '\n') {
			printf("ADDQUEUE(%c)  ", ch);
			if (Enqueue(&q, ch)) {
				++qsize;
				printf("Queue Size = %d/%d\n", qsize, MAX_QUEUE_SIZE);
			}
			else {
				printf("Queue Size = %d/%d\n", qsize, MAX_QUEUE_SIZE);
			}
		}
		else if(isdigit(ch)){
			/*char strNum[3]; //MAX_QUEUE_SIZE�� �ڸ��� + 1 ��ŭ ���ڰ� ���� �� �ִ�.
			while ((ch = getchar()) != '\n') {
				strNum
			}*/
			num = ch - '0';
			for (int i = 0; i < num; i++) {
				printf("DELETEQUEUE() = ");
				if (!QIsEmpty(&q)) {
					--qsize;
					printf("%c  ",Dequeue(&q));
					printf("Queue Size = %d/%d\n", qsize, MAX_QUEUE_SIZE);
				}
				else {
					printf("Fail : ");
					Dequeue(&q);
					printf("Queue Size = %d/%d\n", qsize, MAX_QUEUE_SIZE);
					break;
				}
			}
		}
		else
		{
			continue;
		}
	}
	return 0;
}