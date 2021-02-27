#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gene_number;
int chrom_number;
int iteration;
int generation = 0;

struct best {
    int fitness;
    int data;
    struct best *next;
};
typedef struct best bestChro;

struct Node {
    int fitness;
    struct Node *next;
    struct gene *gene;
};
struct gene {
    int data;
    struct gene *next;
};

/* Function to insert a node at the beginning of a linked list */
void appendfirst(struct Node **head_ref, int new_data) {
    /* 1. allocate node */
    struct Node *new_node = (struct Node *) malloc(sizeof(struct Node));
    new_node->gene = (struct gene *) malloc(sizeof(struct gene));
    struct Node *last = *head_ref;  /* used in step 5*/

    /* 2. put in the data  */
    new_node->gene->data = new_data;

    /* 3. This new node is going to be the last node, so make next of it as NULL*/
    new_node->next = NULL;
    new_node->gene->next = NULL;

    /* 4. If the Linked List is empty, then make the new node as head */
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;
    return;
}

void appendthen(struct gene **head_ref, int new_data) {
    /* 1. allocate node */
    struct gene *new_node = (*head_ref);
    new_node = (struct gene *) malloc(sizeof(struct gene));
    struct gene *last = *head_ref;

    /* 2. put in the data  */
    new_node->data = new_data;

    /* 3. This new node is going to be the last node, so make next of it as NULL*/
    new_node->next = NULL;

    /* 4. Traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 5. Change the next of last node */
    last->next = new_node;
    return;
}

/* This function was written to find the best chromosome */
void bestFinder(struct best * bRoot , struct gene * bestCro, int fitness){
    if(bRoot->next == NULL){
        while(bestCro->next != NULL){
            bRoot->data = bestCro->data;
            bRoot->next =  malloc(sizeof(struct best));
            bRoot->fitness = fitness;
            bRoot->next->next = NULL;
            bRoot = bRoot->next;
            bestCro = bestCro->next;
        }
        bRoot->fitness = fitness;
        bRoot->data = bestCro->data;
    }
    else{
        while(bestCro->next != NULL){
            bRoot->data = bestCro->data;
            bRoot->fitness = fitness;
            bRoot = bRoot->next;
            bestCro = bestCro->next;
        }
        bRoot->fitness = fitness;
        bRoot->data = bestCro->data;
    }
}

/* Function to print nodes in a given linked list */
void printList(struct Node *start, struct best *best) {
    printf("GENERATION: %d\n", generation);
    generation++;
    struct Node *temp = start;
    while (temp != NULL) {
        struct gene *temp2 = temp->gene;
        while (temp2 != NULL) {
            if (temp2->next == NULL) {
                printf("%d", temp2->data);
            } else {
                printf("%d:", temp2->data);
            }
            temp2 = temp2->next;
        }
        printf(" -> %d\n", temp->fitness);
        temp = temp->next;
    }

    /*Check if the first chromosome in the sorted list has a lower fitness value then the best chromosome*/
    struct gene *gTemp = start->gene;
    if(start->fitness < best->fitness){
        bestFinder(best,gTemp,start->fitness);
    }
    printf("Best chromosome found so far: ");
    struct best *initial = best;
    while (initial->next != NULL) {
            printf("%d:", initial->data);
            initial = initial->next;
    }
    printf("%d -> %d", initial->data, initial->fitness);
}

/* function to swap data of two nodes a and b*/
void swap(struct Node *a, struct Node *b) {
    int temp = a->fitness;
    a->fitness = b->fitness;
    b->fitness = temp;
    struct gene *temp2 = a->gene;
    a->gene = b->gene;
    b->gene = temp2;
}

/* Bubble sort the given linked list */
void bubbleSort(struct Node *start) {
    int swapped, i;
    struct Node *ptr1;
    struct Node *lptr = NULL;

    /* Checking for empty list */
    if (start == NULL)
        return;
    do {
        swapped = 0;
        ptr1 = start;
        while (ptr1->next != lptr) {
            if (ptr1->fitness > ptr1->next->fitness) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

/*Function that calculates each chromosome's fitness value*/
void fitness_calc(struct Node *start) {
    int i;
    struct Node *temp = start;
    while (temp != NULL) {
        int gen = gene_number;
        int fitness = 0;
        struct gene *temp2 = temp->gene;
        while (temp2 != NULL) {
            int result = 1;
            for (i = 0; i < (gen - 1); i++) {
                result = 2 * result;
            }
            fitness = fitness + (temp2->data * (result));
            gen--;
            temp2 = temp2->next;
        }
        temp->fitness = fitness;
        temp = temp->next;
    }
}

/*Function to initialize the chromosomes with genes into the linked list*/
void initializer(char *genes, struct Node **gene_list) {
    int append_count = 0;
    char *token = strtok(genes, ":");
    while (token != NULL) {
        if (append_count == 0) appendfirst(gene_list, atoi(token)); //creating a new chromosome
        else {
            struct Node *temporary = *gene_list;
            while ((temporary)->next != NULL)
                (temporary) = (temporary)->next;
            appendthen(&(temporary)->gene, atoi(token)); //adding genes
        }
        append_count++;
        token = (char *) strtok(NULL, ":");
    }
    struct Node *temporary = *gene_list;
    while ((temporary)->next != NULL)
        (temporary) = (temporary)->next;
}

/*Function to read the file to an array*/
int *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c, len;
    fseek(file, 0, SEEK_END); //to know the total bytes of the file
    len = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (file == NULL) return NULL; //could not open file
    code = malloc(sizeof(char) * len);
    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char) c;
    }
    code[n] = '\0';
    int *mutate = malloc(sizeof(int) * strlen(code));
    char *token_mutate;
    token_mutate = strtok(code, ": \n");
    int l = 0;
    while (token_mutate != NULL) {
        mutate[l] = atoi(token_mutate);
        token_mutate = strtok(NULL, ": \n");
        l++;
    }
    return mutate;
}

/*Function to mutate each chromosome's gene specified in mutate file*/
void mutation(struct Node *start, int loop) {
    int counter;
    struct Node *temp = start;
    while (temp != NULL) { //traverse chromosomes
        counter = 1;
        struct gene *temp2 = temp->gene;
        while (temp2 != NULL) { //traverse genes
            if (counter == loop) {
                if (temp2->data == 1) temp2->data = 0; //change the data
                else temp2->data = 1;
            }
            counter++;
            temp2 = temp2->next;
        }
        temp = temp->next;
    }
}

/*Function to do all the processes to the chromosomes*/
void do_All_The_Things(int *select, int *xover, int *mutate, struct Node **gene_list, struct best *best) {
    int i, m, j, k, a = 0, b, x = 0, first_index, second_index, m_index = 0, to_change, control = 0, x_index = 0;
    for (j = 0; j < iteration; j++) { //iterate over xover and mutate file
        first_index = xover[x_index]; //get the first gene's index
        x_index++;
        second_index = xover[x_index]; //get the second gene's index
        x_index++;
        to_change = mutate[m_index];
        m_index++;
        for (i = 0; i < chrom_number; i++) { //iterate over the selection file
            if (control == 0) {
                a = select[x]; //get the first chromosome's index
                control++;
                x++;
            } else {
                b = select[x]; //get the second chromosome's index
                struct Node *temp = *gene_list;
                struct Node *temp2 = *gene_list;
                if (b != 1) {
                    for (m = 0; m < b - 1; m++) {
                        temp = temp->next; //go to first chromosome
                    }
                }
                if (a != 1) {
                    for (m = 0; m < a - 1; m++) {
                        temp2 = temp2->next; //go to second chromosome
                    }
                }
                struct gene *temp3 = temp->gene;
                struct gene *temp4 = temp2->gene;
                for (k = 1; k < gene_number + 1; k++) { //traverse the genes according to the indexes taken
                    if (k >= first_index && k <= second_index) {
                        int swap = temp3->data; //cross over the datas of the genes across the chromosomes
                        temp3->data = temp4->data;
                        temp4->data = swap;
                    }
                    temp3 = temp3->next;
                    temp4 = temp4->next;
                }
                control = 0;
                x++;
            }
        }
        mutation(*gene_list, to_change); //send the processed linked list to the mutation function to mutate all the genes
        fitness_calc(*gene_list); //calculate the fitness values of each chromosome
        bubbleSort(*gene_list); //sort out all the chromosome according to their fitness values
        printList(*gene_list, best); //print the list
        printf("\n");
    }
}

/*Function to read the files and send them to required functions*/
void readCommands() {
    char *token;
    FILE *pop = fopen("population", "r"); //open population file
    struct Node *gene_list = NULL;
    bestChro * bRoot = (bestChro *)malloc(sizeof(bestChro));
    bRoot->next = NULL;
    if (pop != NULL) {
        char line[128]; /* or other suitable maximum line size */
        while (fgets(line, sizeof line, pop) != NULL) /* read a line */
        {
            token = strtok(line, "\n");
            initializer(token, &gene_list);
        }
        fitness_calc(gene_list);
        bubbleSort(gene_list);
        struct gene * gTemp = gene_list->gene;
        bestFinder(bRoot, gTemp , gene_list->fitness);
        printList(gene_list, bRoot);
        printf("\n");
        fclose(pop);
    } else {
        perror("population"); /* why didn't the file open? */
        exit(1);
    }
    int *selection_file, *xover_file, *mutate_file;
    selection_file = readFile("selection");
    xover_file = readFile("xover");
    mutate_file = readFile("mutate");
    do_All_The_Things(selection_file, xover_file, mutate_file, &gene_list, bRoot);
}

int main(int argc, char *argv[]) {
    gene_number = atoi(argv[1]);
    chrom_number = atoi(argv[2]);
    iteration = atoi(argv[3]);
    readCommands();
    return 0;
}
