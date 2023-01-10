#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define BUFF_SIZE 999999

enum CPUMethods
{
    NONE,
    FCFS,
    SJF,
    PS,
    RR
} method = NONE;
enum PMode
{
    OFF,
    ON
} mode = OFF;

int qTime;
char *fInput = NULL;
char *fOutput = NULL;
char buff[BUFF_SIZE];
char buffer_output[BUFF_SIZE * 6];
bool firstFCFS = true;
bool firstSjfNf = true;
bool firstSjfP = true;
bool firstPsNp = true;
bool firstPsP = true;
bool firstRR = true;
char *exe;

struct node
{
    int process_id;
    int burst_time;
    int arrival_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int first_response;
    int how_much_left;
    int time_slices;
    int last_slice_burst;
    bool is_terminated;
    bool in_cpu;
    struct node *next;
};
struct node *header_original = NULL;

struct node *create_node(int, int, int, int);
struct node *insert_back(struct node *, int, int, int, int);
struct node *delete_front(struct node *);
void display_LL(struct node *);
struct node *clone_LL();

void useOutPut();
void menu();
void menu1();
void menu2();
void menu3();
void menu4();
void tq_menu();
void write_input_to_LL(char *);
void fcfs();
void sjf_np();
void sjf_p();
void ps_np();
void ps_p();
void rr();
int process_counter(struct node *);
struct node *swap_nodes(struct node *, struct node *);
void bubble_sort(struct node **, int, char *);
bool is_all_done(struct node *);
bool is_previous_ones_done(struct node *, int);
struct node *find_least_left(struct node *, int);
struct node *find_least_priority(struct node *, int);

int main(int argc, char *argv[])
{
    exe = argv[0];
    int lines = 0;

    while ((lines = getopt(argc, argv, "f:o:")) != -1)
    {
        switch (lines)
        {
        case 'f':
            fInput = optarg;
            break;
        case 'o':
            fOutput = optarg;
            break;
        default:
            useOutPut();
            break;
        }
    }

    if (fInput == NULL || fOutput == NULL)
    {
        useOutPut();
    }

    FILE *finput = fopen(fInput, "r");
    if (finput == NULL)
    {
        printf("input file does not exists.\n");
        exit(1);
    }
    fclose(finput);
    write_input_to_LL(fInput);
    menu();
    return 0;
}

struct node *create_node(int pid, int burst_time, int arrival_time, int priority)
{
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    memset(temp, '\0', sizeof(struct node));
    temp->process_id = pid;
    temp->burst_time = burst_time;
    temp->arrival_time = arrival_time;
    temp->priority = priority;
    temp->waiting_time = 0;
    temp->turnaround_time = 0;
    temp->how_much_left = burst_time;
    temp->first_response = 0;
    temp->time_slices = 0;
    temp->last_slice_burst = 0;
    if (temp->burst_time == 0)
        temp->is_terminated = false;
    temp->in_cpu = false;
    temp->next = NULL;
    return temp;
}
struct node *insert_back(struct node *header, int id, int burst_time, int arrival_time, int priority)
{
    struct node *temp = create_node(id, burst_time, arrival_time, priority);
    struct node *header_temp;
    if (header == NULL)
    {
        header = temp;
        return header;
    }
    header_temp = header;
    while (header_temp->next != NULL)
        header_temp = header_temp->next;
    header_temp->next = temp;
    return header;
}

struct node *delete_front(struct node *header)
{
    struct node *temp;
    if (header == NULL)
    {
        return header;
    }
    temp = header;
    header = header->next;
    memset(temp, '\0', sizeof(struct node));
    free(temp);
    return header;
}

void display_LL(struct node *header)
{
    struct node *temp = header;
    while (temp != NULL)
    {
        int a, b, c, d, e, f, g, h, i, j;
        bool t;
        a = temp->process_id;
        b = temp->burst_time;
        c = temp->arrival_time;
        d = temp->priority;
        e = temp->waiting_time;
        f = temp->turnaround_time;
        g = temp->how_much_left;
        h = temp->first_response;
        i = temp->time_slices;
        j = temp->last_slice_burst;

        printf("ID:%d\tBurst:%d\tArrival:%d\tPriority:%d\tWait:%d\tTurn:%d\tLeft:%d\tResponse:%d\tSlices:%d\tLastSlice:%d\n", a, b, c, d, e, f, g, h, i, j);
        temp = temp->next;
    }

    getchar();
    getchar();
}

struct node *clone_LL(struct node *header)
{
    struct node *header_temp = header;
    struct node *clone_header = NULL;
    while (header_temp != NULL)
    {
        int pid = 0, burst = 0, arrival = 0, priority = 0;
        pid = header_temp->process_id;
        burst = header_temp->burst_time;
        arrival = header_temp->arrival_time;
        priority = header_temp->priority;
        clone_header = insert_back(clone_header, pid, burst, arrival, priority);
        header_temp = header_temp->next;
    }
    return clone_header;
}

void useOutPut()
{
    printf("Use: %s -f <input filename> -o <output filename>\n", exe);
    exit(1);
}
