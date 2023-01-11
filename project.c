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
void inpute_W(char *);
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
    inpute_W(fInput);
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

void menu()
{
    while (true)
    {
        system("clear");
        int option;
        printf("\t\tscheduler simulation\n");

        switch (method)
        {
        case 0:
            printf("1 - Scheduling Method (NONE)\n");
            break;
        case 1:
            printf("1 - Scheduling Method (FCFS)\n");
            break;
        case 2:
            printf("1 - Scheduling Method (SJF)\n");
            break;
        case 3:
            printf("1 - Scheduling Method (PS)\n");
            break;
        case 4:
            printf("1 - Scheduling Method (RR)\n");
            break;
        default:
            printf("1 - Scheduling Method (NONE)\n");
            break;
        }

        switch (mode)
        {
        case 0:
            printf("2 - Preemtive Mode (OFF)\n");
            break;
        case 1:
            printf("2 - Preemtive Mode (ON)\n");
            break;
        default:
            printf("2 - Preemtive Mode (OFF)\n");
            break;
        }

        printf("3 - Result\n");
        printf("4 - End \n");
        printf("Option > ");
        scanf("%1d", &option);

        switch (option)
        {
        case 1:
            menu1();
            break;
        case 2:
            menu2();
            break;
        case 3:
            menu3();
            break;
        case 4:
            menu4();
            break;
        default:
            printf("Please select a valid option!\n");
            system("sleep 2");
            break;
        }
    }
}

void menu1()
{
    system("clear");
    int option = 0;

    if (mode == 1)
    {
        printf("1 - pls chose a scheduling method\n");
        printf("2 - Shortest-Job-First Scheduling\n");
        printf("3 - Priority Scheduling\n");
        printf("Option > ");
        scanf("%1d", &option);

        switch (option)
        {
        case 1:
            method = NONE;
            break;
        case 2:
            method = SJF;
            break;
        case 3:
            method = PS;
            break;
        default:
            printf("Please select a valid option\n");
            system("sleep 1");
            break;
        }
    }
    else
    {
        printf("1 - pls chose a scheduling method\n");
        printf("2 - First Come, First Served Scheduling\n");
        printf("3 - Shortest-Job-First Scheduling\n");
        printf("4 - Priority Scheduling\n");
        printf("5 - Round-Robin Scheduling\n");
        printf("Option > ");
        scanf("%1d", &option);
        switch (option)
        {
        case 1:
            method = NONE;
            break;
        case 2:
            method = FCFS;
            break;
        case 3:
            method = SJF;
            break;
        case 4:
            method = PS;
            break;
        case 5:
            method = RR;
            tq_menu();
            break;
        default:
            printf("Please select a valid option\n");
            system("sleep 1");
            break;
        }
    }
}

void menu2()
{
    system("clear");
    int option = 0;
    if (method == 0)
    {
        printf("(Scheduling Methods Menu would change based on your preemtive mode selection.)\n");
        printf("Please! Next time when you are choosing preemtive mode, ");
        printf("pls select the scheduling method first.\n");
        printf("1 - OFF\n");
        printf("2 - ON\n");
        printf("Option > ");
        scanf("%1d", &option);
        switch (option)
        {
        case 1:
            mode = OFF;
            break;
        case 2:
            mode = ON;
            break;
        default:
            printf("Please select a valid option\n");
            system("sleep 1");
            break;
        }
    }
    else if (method == 2 || method == 3)
    {
        printf("1 - OFF\n");
        printf("2 - ON\n");
        printf("Option > ");
        scanf("%1d", &option);
        switch (option)
        {
        case 1:
            mode = OFF;
            break;
        case 2:
            mode = ON;
            break;
        default:
            printf("Please select a valid option\n");
            system("sleep 1");
            break;
        }
    }
    else
    {
        printf("Preemtive mode is not available for selected Scheduling Method\n");
        system("sleep 1");
    }
}

void menu3()
{
    switch (method)
    {
    case 1:
        fcfs();
        printf("Press Enter return main menu.\n");
        getchar();
        getchar();
        break;
    case 2:
        if (mode == 0)
        {
            sjf_np();
            printf("Press Enter return main menu.\n");
            getchar();
            getchar();
        }
        else
        {
            sjf_p();
            printf("Press Enter return to main menu.\n");
            getchar();
            getchar();
        }
        break;

    case 3:
        if (mode == 0)
        {
            ps_np();
            printf("Press Enter to return to the main menu.\n");
            getchar();
            getchar();
        }
        else
        {
            ps_p();
            printf("Press Enter to return to the main menu.\n");
            getchar();
            getchar();
        }
        break;

    case 4:
        rr();
        printf("Press Enter to return to the main menu.\n");
        getchar();
        getchar();
        break;
    }
}

void menu4()
{
    if (qTime == 0)
        tq_menu();
    if (firstFCFS)
    {
        fcfs();
    }
    if (firstSjfNf)
    {
        sjf_np();
    }
    if (firstSjfP)
    {
        sjf_p();
    }
    if (firstPsNp)
    {
        ps_np();
    }
    if (firstPsP)
    {
        ps_p();
    }
    if (firstRR)
    {
        rr();
    }

    printf("%s", buffer_output);
    FILE *f = fopen(fOutput, "w");
    fprintf(f, "%s", buffer_output);
    fclose(f);

    while (header_original != NULL)
    {
        header_original = delete_front(header_original);
    }

    exit(0);
}

void tq_menu()
{
    while (true)
    {
        system("clear");
        printf("enter time quantum for Round-Robin\n");
        printf("Time Quantum > ");
        scanf("%d", &qTime);
        break;
    }
}

void inpute_W(char *fInput)
{
    FILE *finput = fopen(fInput, "r");
    int id_counter = 1;
    if (feof(finput))
    {
        printf("file is empty\n");
        exit(1);
    }
    else
    {
        while (!feof(finput))
        {
            int a, b, c;
            fscanf(finput, "%d:%d:%d\n", &a, &b, &c);
            header_original = insert_back(header_original, id_counter, a, b, c);
            id_counter++;
        }
    }
    fclose(finput);
}

void fcfs()
{
    struct node *clone_header = clone_LL(header_original);
    struct node *temp1, *temp2, *t;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bool is_first = true;
    bubble_sort(&clone_header, number_of_process, "AT");
    temp1 = clone_LL(clone_header);
    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
    t = temp2 = temp1;

    while (temp1 != NULL)
    {
        if (temp1->arrival_time <= count)
        {
            count += temp1->burst_time;
            temp1->turnaround_time = count;
            if (is_first)
            {
                if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time) < 0)
                    temp1->waiting_time = 0;
                is_first = false;
            }
            else
            {
                if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time) < 0)
                    temp1->waiting_time = 0;
            }
        }

        else
        {
            count = temp1->arrival_time;
            count += temp1->burst_time;
            temp1->turnaround_time = count;
            if ((temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time) < 0)
                temp1->waiting_time = 0;
        }

        temp1 = temp1->next;
    }

    strcpy(buff, "");
    bubble_sort(&temp2, number_of_process, "PID");
    system("clear");
    strcat(buff, "Scheduling Method: First Come First Served\n");
    strcat(buff, "Process Waiting Times:\n");
    while (temp2 != NULL)
    {
        int pid = temp2->process_id;
        int wait = temp2->waiting_time;
        average_wait += wait;
        char buff_1[20] = "";
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp2 = temp2->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstFCFS)
    {
        strcat(buffer_output, buff);
        firstFCFS = false;
    }
    printf("%s", buff);

    while (t != NULL)
    {
        t = delete_front(t);
    }
}

void sjf_np()
{

    struct node *clone_header = clone_LL(header_original);
    struct node *temp, *temp1, *t;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bubble_sort(&clone_header, number_of_process, "AT");
    bubble_sort(&clone_header, number_of_process, "SJF");
    temp = clone_LL(clone_header);
    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
    t = temp1 = temp;

    bool is_first = true;
    while (temp != NULL)
    {
        if (temp->arrival_time <= count)
        {
            count += temp->burst_time;
            temp->turnaround_time = count;
            if (is_first)
            {
                if ((temp->waiting_time = temp->turnaround_time - temp->burst_time) < 0)
                    temp->waiting_time = 0;
                is_first = false;
            }
            else
            {
                if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
                    temp->waiting_time = 0;
            }
        }

        else
        {
            count = temp->arrival_time;
            count += temp->burst_time;
            temp->turnaround_time = count;
            if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
                temp->waiting_time = 0;
        }

        temp = temp->next;
    }

    strcpy(buff, "");
    bubble_sort(&temp1, number_of_process, "PID");
    system("clear");
    strcat(buff, "Scheduling Method: Shortest Job First (Non-Preemtive)\n");
    strcat(buff, "Process Waiting Times:\n");
    while (temp1 != NULL)
    {
        int pid = temp1->process_id;
        int wait = temp1->waiting_time;
        average_wait += wait;
        char buff_1[20] = "";
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp1 = temp1->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstSjfNf)
    {
        strcat(buffer_output, buff);
        firstSjfNf = false;
    }
    printf("%s", buff);

    while (t != NULL)
    {
        t = delete_front(t);
    }
}

void sjf_p()
{
    struct node *clone_header = clone_LL(header_original);
    struct node *temp, *temp1, *temp2;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bubble_sort(&clone_header, number_of_process, "AT");
    bubble_sort(&clone_header, number_of_process, "SJF");
    temp = temp1 = temp2 = clone_header;

    while (!is_all_done(clone_header))
    {
        struct node *in_cpu_node = find_least_left(clone_header, count);
        bool is_found = true;
        if (in_cpu_node == NULL)
        {
            temp = clone_header;
            while (temp != NULL)
            {
                if (!temp->is_terminated)
                {
                    if (temp->arrival_time > count && is_found)
                    {
                        is_found = false;
                        count = temp->arrival_time;
                        in_cpu_node = find_least_left(clone_header, count);
                        in_cpu_node->how_much_left--;
                        count++;
                        if (in_cpu_node->how_much_left == 0)
                        {
                            in_cpu_node->turnaround_time = count;
                            in_cpu_node->is_terminated = true;
                        }
                    }
                }

                temp = temp->next;
            }
        }
        else
        {
            count++;
            in_cpu_node->how_much_left--;

            if (in_cpu_node->how_much_left == 0)
            {
                in_cpu_node->turnaround_time = count;
                in_cpu_node->is_terminated = true;
            }
        }
    }

    bool is_first = true;
    while (temp1 != NULL)
    {
        if (is_first)
        {
            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
            if (temp1->waiting_time < 0)
                temp1->waiting_time = 0;

            is_first = false;
        }

        else
        {
            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
            if (temp1->waiting_time < 0)
                temp1->waiting_time = 0;
        }

        temp1 = temp1->next;
    }

    strcpy(buff, "");
    bubble_sort(&temp2, number_of_process, "PID");
    system("clear");
    strcat(buff, "Scheduling Method: Shortest Job First Scheduling (Preemtive)\n");
    strcat(buff, "Process Waiting Times:\n");
    while (temp2 != NULL)
    {
        int pid = temp2->process_id;
        int wait = temp2->waiting_time;
        average_wait += wait;
        char buff_1[20] = "";
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp2 = temp2->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstSjfP)
    {
        strcat(buffer_output, buff);
        firstSjfP = false;
    }
    printf("%s", buff);

    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
}

void ps_np()
{
    struct node *clone_header = clone_LL(header_original);
    struct node *temp, *temp1, *t;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bubble_sort(&clone_header, number_of_process, "AT");
    bubble_sort(&clone_header, number_of_process, "PS");
    temp = clone_LL(clone_header);
    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
    t = temp1 = temp;

    bool is_first = true;
    while (temp != NULL)
    {
        if (temp->arrival_time <= count)
        {
            count += temp->burst_time;
            temp->turnaround_time = count;
            if (is_first)
            {
                if ((temp->waiting_time = temp->turnaround_time - temp->burst_time) < 0)
                    temp->waiting_time = 0;
                is_first = false;
            }
            else
            {
                if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
                    temp->waiting_time = 0;
            }
        }

        else
        {
            count = temp->arrival_time;
            count += temp->burst_time;
            temp->turnaround_time = count;
            if ((temp->waiting_time = temp->turnaround_time - temp->burst_time - temp->arrival_time) < 0)
                temp->waiting_time = 0;
        }

        temp = temp->next;
    }

    strcpy(buff, "");
    bubble_sort(&temp1, number_of_process, "PID");
    system("clear");
    strcat(buff, "Scheduling Method: Priority Scheduling (Non-Preemtive)\n");
    strcat(buff, "Process Waiting Times:\n");
    while (temp1 != NULL)
    {
        int pid = temp1->process_id;
        int wait = temp1->waiting_time;
        average_wait += wait;
        char buff_1[20] = "";
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp1 = temp1->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstPsNp)
    {
        strcat(buffer_output, buff);
        firstPsNp = false;
    }
    printf("%s", buff);

    while (t != NULL)
    {
        t = delete_front(t);
    }
}

void ps_p()
{
    struct node *clone_header = clone_LL(header_original);
    struct node *temp, *temp1, *temp2;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bubble_sort(&clone_header, number_of_process, "AT");
    bubble_sort(&clone_header, number_of_process, "PS");
    temp = temp1 = temp2 = clone_header;

    while (!is_all_done(clone_header))
    {
        struct node *in_cpu_node = find_least_priority(clone_header, count);
        bool is_found = true;

        if (in_cpu_node == NULL)
        {
            temp = clone_header;
            while (temp != NULL)
            {
                if (!temp->is_terminated)
                {
                    if (temp->arrival_time > count && is_found)
                    {
                        is_found = false;
                        count = temp->arrival_time;
                        in_cpu_node = find_least_priority(clone_header, count);
                        in_cpu_node->how_much_left--;
                        count++;
                        if (in_cpu_node->how_much_left == 0)
                        {
                            in_cpu_node->turnaround_time = count;
                            in_cpu_node->is_terminated = true;
                        }
                    }
                }

                temp = temp->next;
            }
        }

        else
        {
            count++;
            in_cpu_node->how_much_left--;

            if (in_cpu_node->how_much_left == 0)
            {
                in_cpu_node->turnaround_time = count;
                in_cpu_node->is_terminated = true;
            }
        }
    }

    bool is_first = true;
    while (temp1 != NULL)
    {
        if (is_first)
        {
            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
            if (temp1->waiting_time < 0)
                temp1->waiting_time = 0;

            is_first = false;
        }

        else
        {
            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
            if (temp1->waiting_time < 0)
                temp1->waiting_time = 0;
        }
        temp1 = temp1->next;
    }

    strcpy(buff, "");
    bubble_sort(&temp2, number_of_process, "PID");
    system("clear");
    strcat(buff, "Scheduling Method: Priority Scheduling (Preemtive)\n");
    strcat(buff, "Process Waiting Times:\n");
    while (temp2 != NULL)
    {
        int pid = temp2->process_id;
        int wait = temp2->waiting_time;
        average_wait += wait;
        char buff_1[20] = "";
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp2 = temp2->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstPsP)
    {
        strcat(buffer_output, buff);
        firstPsP = false;
    }
    printf("%s", buff);
    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
}

void rr()
{
    struct node *clone_header = clone_LL(header_original);
    struct node *temp1, *temp2, *temp3;
    int count = 0;
    float average_wait = 0.0f;
    int number_of_process = process_counter(clone_header);
    bool is_first = true;
    bool previous_ones_done = false;
    bubble_sort(&clone_header, number_of_process, "AT");
    temp1 = temp2 = temp3 = clone_header;

    while (temp3 != NULL)
    {
        temp3->time_slices = temp3->burst_time / qTime;
        temp3->last_slice_burst = temp3->burst_time % qTime;
        temp3 = temp3->next;
    }

    while (!is_all_done(clone_header))
    {
        temp1 = clone_header;
        is_first = true;
        while (temp1 != NULL)
        {
            if (!temp1->is_terminated)
            {
                if (temp1->arrival_time <= count)
                {
                    if (is_first)
                    {
                        if (temp1->time_slices == 0)
                        {
                            count += temp1->last_slice_burst;
                            if (temp1->last_slice_burst != 0)
                                temp1->turnaround_time = count;
                            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time;
                            if (temp1->waiting_time < 0)
                                temp1->waiting_time = 0;
                            temp1->is_terminated = true;
                        }
                        else
                        {
                            count += qTime;
                            temp1->turnaround_time = count;
                            temp1->time_slices--;
                        }
                        is_first = false;
                    }

                    else
                    {
                        if (temp1->time_slices == 0)
                        {
                            count += temp1->last_slice_burst;
                            if (temp1->last_slice_burst != 0)
                                temp1->turnaround_time = count;
                            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
                            if (temp1->waiting_time < 0)
                                temp1->waiting_time = 0;
                            temp1->is_terminated = true;
                        }
                        else
                        {
                            count += qTime;
                            temp1->turnaround_time = count;
                            temp1->time_slices--;
                        }
                    }
                }

                else
                {
                    previous_ones_done = is_previous_ones_done(clone_header, count);
                    if (previous_ones_done)
                    {
                        count = temp1->arrival_time;
                        if (temp1->time_slices == 0)
                        {
                            count += temp1->last_slice_burst;
                            if (temp1->last_slice_burst != 0)
                                temp1->turnaround_time = count;
                            temp1->waiting_time = temp1->turnaround_time - temp1->burst_time - temp1->arrival_time;
                            if (temp1->waiting_time < 0)
                                temp1->waiting_time = 0;
                            temp1->is_terminated = true;
                        }
                        else
                        {
                            count += qTime;
                            temp1->turnaround_time = count;
                            temp1->time_slices--;
                        }
                    }
                }
            }

            is_first = false;
            temp1 = temp1->next;
        }
    }

    strcpy(buff, "");
    bubble_sort(&clone_header, number_of_process, "PID");
    temp2 = clone_header;
    system("clear");
    snprintf(buff, 999, "Scheduling Method: Round-Robin (Time quantum: %d)\n", qTime);
    strcat(buff, "Process Waiting Times:\n");
    while (temp2 != NULL)
    {
        int pid = temp2->process_id;
        int wait = temp2->waiting_time;
        average_wait += wait;
        char buff_1[20];
        snprintf(buff_1, 19, "PS%d: %d ms\n", pid, wait);
        strcat(buff, buff_1);
        temp2 = temp2->next;
    }
    average_wait /= number_of_process;
    char buff_2[40];
    snprintf(buff_2, 39, "Average Waiting Time: %.3f ms\n\n", average_wait);
    strcat(buff, buff_2);
    if (firstRR)
    {
        strcat(buffer_output, buff);
        firstRR = false;
    }
    printf("%s", buff);
    while (clone_header != NULL)
    {
        clone_header = delete_front(clone_header);
    }
}
