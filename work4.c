#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	int ID;
	int PRIORITY;
	int CPUTIME;
	int ALLTIME;
	char *STATE;
	struct node *next;
}PCB, *PCBLIST;

char *STATE[] = { "RUNNING", "READY", "FINISH" };

void ready_push(PCBLIST h, PCB* pro)
{
	PCBLIST q = h, p = h->next;

	while (p != NULL && p->PRIORITY > pro->PRIORITY)
	{
		q = p;
		p = p->next;
	}

	PCBLIST s = (PCB*)malloc(sizeof(PCB));
	*s = *pro;
	s->next = p;
	q->next = s;
}

PCB* ready_pop(PCBLIST h)
{
	PCBLIST q = h;
	PCB* pro = q->next;
	q->next = q->next->next;
	return pro;
}

void ready_update(PCBLIST h)
{
	PCBLIST q = h->next;
	while (q != NULL)
	{
		q->PRIORITY++;
		q = q->next;
	}
}

void ready_record(PCBLIST h, PCB* record)
{
	printf("	READY_QUEUE: ");
	PCBLIST q = h->next;
	if (q == NULL)
		printf("NULL");
	while (q != NULL)
	{
		printf("->%d", q->ID);

		record[q->ID] = *q;

		q = q->next;
	}
	printf("\n");
}

void running_record(PCB* pro, PCB* record)
{

	if (pro == NULL)
	{
		printf("	RUNNINGPROG: NULL\n");
		return;
	}
	if (pro->ALLTIME == 0) return;

	pro->PRIORITY -= 3;
	pro->CPUTIME += 1;
	pro->ALLTIME -= 1;

	printf("	RUNNINGPROG: %d\n", pro->ID);

	record[pro->ID] = *pro;

}

void classify_QUEUE(PCBLIST h, PCBLIST r)
{
	PCBLIST q = h->next;
	while (q != NULL)
	{
		if (strcmp(q->STATE, STATE[1]) == 0)
			ready_push(r, q);
		q = q->next;
	}

}

void initial_PCB(PCBLIST h)
{
	int       id[] = { 0, 1, 2, 3, 4 };
	int priority[] = { 9, 38, 30, 29, 0 };
	int  cputime[] = { 0, 0, 0, 0, 0 };
	int  alltime[] = { 3, 3, 6, 3, 4 };
	char  *state[] = { "READY", "READY", "READY", "READY", "READY" };

	PCBLIST p, q = h;

	for (int i = 0; i < 5; i++)
	{
		p = (PCB*)malloc(sizeof(PCB));
		p->ID = id[i];
		p->PRIORITY = priority[i];
		p->CPUTIME = cputime[i];
		p->ALLTIME = alltime[i];
		p->STATE = state[i];
		p->next = NULL;

		q->next = p;
		q = q->next;
	}
}

void print_table(PCB* record)
{
	printf("==============================================================\n");
	printf("ID       \t%-8d %-8d %-8d %-8d %-8d\n",
		record[0].ID, record[1].ID, record[2].ID, record[3].ID, record[4].ID);
	printf("PRIORITY \t%-8d %-8d %-8d %-8d %-8d\n",
		record[0].PRIORITY, record[1].PRIORITY, record[2].PRIORITY, record[3].PRIORITY, record[4].PRIORITY);
	printf("CPUTIME  \t%-8d %-8d %-8d %-8d %-8d\n",
		record[0].CPUTIME, record[1].CPUTIME, record[2].CPUTIME, record[3].CPUTIME, record[4].CPUTIME);
	printf("ALLTIME  \t%-8d %-8d %-8d %-8d %-8d\n",
		record[0].ALLTIME, record[1].ALLTIME, record[2].ALLTIME, record[3].ALLTIME, record[4].ALLTIME);
	printf("STATE    \t%-8s %-8s %-8s %-8s %-8s\n",
		record[0].STATE, record[1].STATE, record[2].STATE, record[3].STATE, record[4].STATE);

	printf("\n");
}

int main()
{
	PCBLIST PROCESS = (PCB*)malloc(sizeof(PCB));       // the initial process list
	PROCESS->next = NULL;
	PCBLIST READY_QUEUE = (PCB*)malloc(sizeof(PCB));   // the ready queue 
	READY_QUEUE->next = NULL;

	initial_PCB(PROCESS);   // init by the data
	classify_QUEUE(PROCESS, READY_QUEUE);   // init ready queue by the init process

	PCB record[5];     // record the process infomation
	PCB* pro = NULL;   // the running process
	int times = 0;     // count the times

	printf("\n第%d个时间片后:\n", times);   // TIMES INFO
	running_record(pro, record);           // RUNNINGPROG
	ready_record(READY_QUEUE, record);     // READY_QUEUE
	print_table(record);                   // TABLE

	while (1)
	{
		if (READY_QUEUE->next == NULL && pro == NULL) break;   // no process in all queue

		if (pro == NULL)   // no process in running, make one process to run
		{
			if (READY_QUEUE->next != NULL)
				pro = ready_pop(READY_QUEUE);
			// change the running
			pro->STATE = STATE[0];
			pro->CPUTIME = 0;
		}

		times++;
		printf("\n第%d个时间片后:\n", times); // TIMES INFO

		// update and record the running
		running_record(pro, record);         // RUNNINGPROG

		// update and record the ready
		ready_update(READY_QUEUE);
		ready_record(READY_QUEUE, record);   // READY_QUEUE

		// change the running to finish
		if (pro->ALLTIME == 0)
		{
			pro->STATE = STATE[2];
			record[pro->ID] = *pro;
			free(pro);
			pro = NULL;
		}

		print_table(record);                 // TABLE

	}

	printf("\nEND\n");

	return 0;
}


