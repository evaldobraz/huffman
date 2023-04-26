#include <stdio.h>
#include <stdlib.h>

typedef struct priority_queue_node {
    int value;
    int priority;
    struct priority_queue_node *next;
} priority_queue_node;

typedef struct priority_queue {
    priority_queue_node *head;
} priority_queue;

void pq_init(priority_queue *pq) {
    pq->head = NULL;
}

void pq_push(priority_queue *pq, int value, int priority) {
    priority_queue_node *node = malloc(sizeof(priority_queue_node));
    node->value = value;
    node->priority = priority;
    node->next = NULL;

    if (pq->head == NULL || pq->head->priority < priority) {
        node->next = pq->head;
        pq->head = node;
    } else {
        priority_queue_node *current = pq->head;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
}

int pq_pop(priority_queue *pq) {
    if (pq->head == NULL) {
        printf("Priority Queue is empty.\n");
        return -1;
    }

    int result = pq->head->value;
    priority_queue_node *temp = pq->head;
    pq->head = pq->head->next;
    free(temp);

    return result;
}

int main() {
    priority_queue pq;
    pq_init(&pq);

    //pq_push(&pq, 5, 2);
    
    //printf("%d\n", pq_pop(&pq));
    
    return 0;
}
