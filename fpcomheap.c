#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

typedef struct priority_queue {
    int data[MAX_SIZE];
    int size;
} priority_queue;

void pq_init(priority_queue *pq) {
    pq->size = 0;
}

void pq_push(priority_queue *pq, int value) {
    if (pq->size >= MAX_SIZE) {
        printf("Priority Queue is full.\n");
        return;
    }

    int i = pq->size;
    pq->data[i] = value;
    pq->size++;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (pq->data[p] < pq->data[i]) {
            int temp = pq->data[p];
            pq->data[p] = pq->data[i];
            pq->data[i] = temp;
            i = p;
        } else {
            break;
        }
    }
}

int pq_pop(priority_queue *pq) {
    if (pq->size <= 0) {
        printf("Priority Queue is empty.\n");
        return -1;
    }

    int result = pq->data[0];
    pq->size--;
    pq->data[0] = pq->data[pq->size];

    int i = 0;
    while (i * 2 + 1 < pq->size) {
        int left = i * 2 + 1;
        int right = i * 2 + 2;
        int max_index = i;

        if (pq->data[left] > pq->data[max_index]) {
            max_index = left;
        }

        if (right < pq->size && pq->data[right] > pq->data[max_index]) {
            max_index = right;
        }

        if (max_index != i) {
            int temp = pq->data[i];
            pq->data[i] = pq->data[max_index];
            pq->data[max_index] = temp;
            i = max_index;
        } else {
            break;
        }
    }

    return result;
}

int main() {
    priority_queue pq;
    pq_init(&pq);

    //pq_push(&pq, );
    

    //printf("%d\n", pq_pop(&pq));
    

    return 0;
}
