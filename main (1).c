#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int sequence;
    struct node * next;
}Student;

typedef struct queue {
    Student* front;
    Student* back;
    int nodeCount;
    int k; // number of skips
    int th; // threshold
}queue;

int isEmpty(queue* p)
{
    if (p->front == NULL) {
        return 0;
    }
    return 1;
}

Student* createStudent(int sequence)
{
    Student* n = malloc(sizeof(Student));
    n->sequence = sequence;
    n->next = NULL;

    return n;
}

void enqueue(queue* p, Student* node)
{
    if (p->front == NULL) {
        p->front = node;
        p->back = node;
        p->front->next = p->back;
    }
    else {
        p->back->next = node;
        p->back = node;
        p->back->next = p->front;
    }

}

void createReverseCircle(queue *q)
{
    Student* current = q->front;
    Student* prev = NULL;
    Student* next;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    q->back->next = q->front;
}

void rearrangeCircle(queue* q)
{
    Student* oldFront = q->front;
    Student* prev = q->front;
    q->front->next = q->front;

    while (q->front != q->back) {
        q->front->next = prev;
        prev = q->front;
        q->front->next = q->front;
    }
    q->front->next = prev;
    q->back = oldFront;
}

queue* phase1(queue* p, int index)
{
    Student* temp;
    Student* walk = p->front;

    printf("Group for Garage# %d\n", index+1);

    while (p->nodeCount > p->th) {
        if (walk->next->sequence % p->k == 0) {
            temp = walk->next;

            if (temp == p->back) {
                p->back = walk;
                walk->next = p->front;
                walk->next = NULL;
            }
            else {
                walk->next = temp->next;
            }

            if (temp == p->front) {
                p->front = p->front->next;
                p->back->next = p->front;
            }
            printf("Student# %d eliminated\n", temp->sequence);
            free(temp);
            p->nodeCount--;
            walk = walk->next;
        }
        else {
            walk = walk->next;
        }
    }
    return p;
}

Student* dequeue(queue* p)
{
    if (isEmpty(p) == 0) {
        return NULL;
    }
    
    Student* temp = p->front;

    p->front = p->front->next;

    p->nodeCount--;

    if (p->nodeCount == 0) {
        p->front = NULL;
        p->back = NULL;
    }
    return temp;
}

int peek(queue* p)
{
    if (isEmpty(p) == 1) {
        int value = p->front->sequence;
        return value;
    }
    return -1;
}

int maxFinder(queue* p[])
{
    int value = -1;
    int max_index = -1;

    for (int i = 0; i < 10; ++i) {
        if (isEmpty(p[i]) == 1) {
            if (value < peek(p[i])) {
                value = p[i]->front->sequence;
                max_index = i;
            }
        }
    }
    printf("Eliminated student %d from group from garage %d",
           value, max_index+1);
    return max_index;
}

queue** phase2(queue* p[])
{
    printf("Phase2 elimination");
    for (int i = 0; i < 10; ++i) {
        if (p[i] != NULL) {
            int index = maxFinder(p);
            Student* eliminatedStudent = dequeue(p[index]);
            if (eliminatedStudent != NULL) {
                free(eliminatedStudent);
            }
            p[index]->nodeCount--;
        }
    }
    return p;
}

void display(queue* p)
{
    if (isEmpty(p) == 0) {
        return;
    }

    Student* t = p->front;

    while (t != p->back) {
        printf(" %d", t->sequence);
        t = t->next;
    }
    printf(" %d", t->sequence);
}

void freeMemory(queue* p)
{
    for (int i = 0; i < 10; ++i) {
        if (isEmpty(&p[i]) == 1) {
            Student* current = p[i].front;
            while (current != NULL) {
                Student* next = current->next;
                free(current);
                current = next;
            }
        }
        free(&p[i]);
    }
}

void main()
{
    queue* total_garages[10] = {NULL};

    int garages, students, k, th, index, next_index;

    scanf("%d\n", &garages);
    scanf("%d\n", &index);

    for (int g = 0; g < garages; ++g) {
        for (int i = 0; i < 10; ++i) {
            if (index-1 == i) {
                total_garages[i] = malloc(sizeof(queue));
                scanf("%d\n", &students);
                for (int s = students; s > 0; --s) {
                    enqueue(total_garages[index-1], createStudent(s));
                }
            }

            scanf("%d", &k);
            scanf("%d", &th);

            if (total_garages[i] != NULL) {
                total_garages[i]->k = k;
                total_garages[i]->th = th;
                total_garages[i]->nodeCount = students;
            }
            
    
            scanf("%d", &next_index);
            index = next_index;
        }
    }
    printf("Initial status of nonempty queues\n");
    for (int i = 0; i < 10; ++i) {
        if (total_garages[i] != NULL) {
            createReverseCircle(total_garages[i]);
            printf("%d", i+1);
            display(total_garages[i]);
        }
    }
    printf("\nAfter ordering status of nonempty queues\n");
    for (int i = 0; i < 10; ++i) {
        if (total_garages[i] != NULL) {
            rearrangeCircle(total_garages[i]);
            printf("%d", i+1);
            display(total_garages[i]);
        }
    }
    printf("\nPhase1 elimination\n");
    for (int i = 0; i < 10; ++i) {
        if (total_garages[i] != NULL) {
            phase1(total_garages[i], i);
        }
    }
    phase2(&total_garages[0]);
    for (int i = 0; i < 10; ++i) {
        if (total_garages[i]->nodeCount == 1 && total_garages[i] != NULL) {
            printf("Student %d from the group for garage %d is the winner!\n",
                   total_garages[i]->front->sequence, i+1);
        }
    }
    for (int i = 0; i < 10; ++i) {
        if (total_garages[i] != NULL) {
            freeMemory(total_garages[i]);
        }
    }
}
