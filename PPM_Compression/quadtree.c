/* NAU John-Roberto - 314CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura unei imagini, matrice de pixeli
typedef struct Pixels {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Pixels;

// Structura arborelui cuaternar
typedef struct QuadTreeNode {
    int x, y; // coordonatele sfertului din dreapta sus
    int width, height;

    Pixels grid; // matricea de pixeli

    struct QuadTreeNode *first_child; // stanga sus
    struct QuadTreeNode *second_child; // dreapta sus
    struct QuadTreeNode *third_child;  // dreapta jos
    struct QuadTreeNode *fourth_child;  // stanga jos

    int is_leaf; // 0 = false; 1 = true
} QuadTreeNode;

// Definim structura unui nod din coada
typedef struct QueueNode {
    QuadTreeNode *node; //aici vom stoca nodurile arborelui
    struct QueueNode *next;
} QueueNode;

// Definim structura cozii
typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
} Queue;

// Functie care creaza o coada
Queue *initQueue()
{
    Queue *q = (Queue *) malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

// Functie care verifica daca coada este goala sau nu
int isEmptyQueue(Queue *q)
{
    return (q->front == NULL);
}

// Functie care adauga un element in coada
void enqueue(Queue *q, QuadTreeNode *quadNode)
{
    QueueNode *qnode;
    qnode = (QueueNode *) malloc(sizeof(QueueNode));
    qnode->node = quadNode;
    qnode->next = NULL;

    if (isEmptyQueue(q) == 0) {
        q->rear->next = qnode;
        q->rear = qnode;
    } else {
        q->rear = q->front = qnode;
    }
}

// Functie care elimina ultimul element din coada
// si elibereaza memoria
void dequeue(Queue **q)
{
    QueueNode *temp;
    if (isEmptyQueue(*q)) {
        return;
    }
    if ((*q)->front == (*q)->rear) {
        (*q)->front->next = NULL;
        (*q)->rear = NULL;
        free((*q)->front);
        (*q)->front = NULL;
    } else {
        temp = (*q)->front;
        (*q)->front = (*q)->front->next;
        free(temp);
        temp = NULL;
    }
}

// Functie care elibereaza memoria cozii
void freeQueue(Queue **q)
{
    if (*q == NULL) {
        return;
    }
    while (isEmptyQueue(*q) != 1) {
        dequeue(q);
    }
    free(*q);
    (*q) = NULL;
}

// Functie care calculeaza mean-ul si valorile pentru red, green si blue
// ale imaginii
void calculate_values(Pixels **grid, int size, int x, int y,
unsigned char *vred, unsigned char *vgreen, unsigned char *vblue, int *vmean)
{
    int i, j;
    unsigned long long sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            sum1 = sum1 + grid[i][j].red;
            sum2 = sum2 + grid[i][j].green;
            sum3 = sum3 + grid[i][j].blue;
        }
    }

    // Calculam valorile pentru red, green si blue
    (*vred) = sum1 / (size * size);
    (*vgreen) = sum2 / (size * size);
    (*vblue) = sum3 / (size * size);

    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            sum4 = sum4 + ((*vred - grid[i][j].red) * (*vred - grid[i][j].red) +
            (*vgreen - grid[i][j].green) * (*vgreen - grid[i][j].green) +
            (*vblue - grid[i][j].blue) * (*vblue - grid[i][j].blue));
        }
    }
    (*vmean) = sum4 / (3 * size * size);
}

// Functie care construieste recursiv arborele de compresie
QuadTreeNode *create_QuadTree(Pixels **image, int x, int y,
int width, int height, int factor)
{
    unsigned char vred, vgreen, vblue;
    // Se creaza un nou nod pentru imagine
    QuadTreeNode *node = (QuadTreeNode *) malloc(sizeof(QuadTreeNode));
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    
    int vmean;
    calculate_values(image, width, x, y, &vred, &vgreen, &vblue, &vmean);

    // Daca scorul, adica mean-ul este mai mare decat pregul impus,
    // se divizeaza imaginea si se precizeaza ca nu este frunza
    if (vmean > factor) {
        node->is_leaf = 0; //nu este frunza, subdivizarea continua

        // se construieste recursiv arborele de compresie
        node->first_child = create_QuadTree(image, x, y, width / 2,
        height / 2, factor); // stanga sus

        node->second_child = create_QuadTree(image, x, y + height / 2,
        width / 2, height / 2, factor); // dreapta sus

        node->third_child = create_QuadTree(image, x + width / 2,
        y + height / 2, width / 2, height / 2, factor); // dreapta jos

        node->fourth_child = create_QuadTree(image, x + width / 2,
        y, width / 2, height / 2, factor); // stanga jos
    } else {
        node->grid.red = vred;
        node->grid.green = vgreen;
        node->grid.blue = vblue;
        node->first_child = NULL;
        node->second_child = NULL;
        node->third_child = NULL;
        node->fourth_child = NULL;

        node->is_leaf = 1; // precizam ca este frunza
    }
    return node;
}

// Functie ajutatoare care determina valoarea maxima dintre 2 numere
int vmax2(int x, int y)
{
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

// Functie ajutatoare care determina valoarea maxima
// dintre 4 numere, folosind functia precedenta
int vmax4(int a, int b, int c, int d)
{
    if (vmax2(a, b) > vmax2(c, d)) {
        return vmax2(a, b);
    } else {
        return vmax2(c, d);
    }
}

// functie care numara cate nivele are arborele nostru
int countLevels(QuadTreeNode *root)
{
    if (root == NULL) {
        return 0;
    } else {
        int LV_first_child = countLevels(root->first_child);
        int LV_second_child = countLevels(root->second_child);
        int LV_third_child = countLevels(root->third_child);
        int LV_fourth_child = countLevels(root->fourth_child);

        // numarul de nivele este nivelul maxim al unuia dintre cei 4 sub-arbori
        return vmax4(LV_first_child, LV_second_child,
        LV_third_child, LV_fourth_child) + 1;
    }
}

// Functie care numara cate nivele are arborele
int countBlocks(QuadTreeNode *root)
{
    // pastram in k numarul de frunze, adica numarul de blocuri
    int k = 0;
    if (root == NULL) {
        return 0;
    } else {
        if (root->is_leaf == 1) {
            k++;
        }
        k = k + countBlocks(root->first_child);
        k = k + countBlocks(root->second_child);
        k = k + countBlocks(root->third_child);
        k = k + countBlocks(root->fourth_child);
        return k;
    }
}

// Functie care calzuleaza latura celui mai mare patrat
int sideMax(QuadTreeNode *root)
{
    if (root->is_leaf == 1) {
        return root->width;
    } else {
        // initializam max_side cu root->width pentru ca width = height
        int max_side = 0;

        // Apelam recursiv functia pentru root->first_child
        int child_side = sideMax(root->first_child);
        if (child_side > max_side) {
            max_side = child_side;
        }

        // Apelam recursiv functia pentru root->second_child
        child_side = sideMax(root->second_child);
        if (child_side > max_side) {
            max_side = child_side;
        }

        // Apelam recursiv functia pentru root->third_child
        child_side = sideMax(root->third_child);
        if (child_side > max_side) {
            max_side = child_side;
        }

        // Apelam recursiv functia pentru root->fourth_child
        child_side = sideMax(root->fourth_child);
        if (child_side > max_side) {
            max_side = child_side;
        }
        return max_side;
    }
}

// Traversarea arborelui pe nivele pentru cerinta 2
void levelTraverse(QuadTreeNode *root, FILE *fout)
{
    Queue *q = initQueue();
    enqueue(q, root);
    while (q->front != NULL) {
        QuadTreeNode *quadNode = q->front->node;

        if (quadNode->is_leaf == 0) {
            unsigned char tipul_nodului = 0;
            fwrite(&tipul_nodului, sizeof(unsigned char), 1, fout);
            dequeue(&q);
            if (quadNode->first_child != NULL) {
                enqueue(q, quadNode->first_child);
            }
            if (quadNode->second_child != NULL) {
                enqueue(q, quadNode->second_child);
            }
            if (quadNode->third_child != NULL) {
                enqueue(q, quadNode->third_child);
            }
            if (quadNode->fourth_child != NULL) {
                enqueue(q, quadNode->fourth_child);
            }
        } else {
            unsigned char tipul_nodului = 1;
            fwrite(&tipul_nodului, sizeof(unsigned char), 1, fout);
            fwrite(&quadNode->grid.red, sizeof(unsigned char), 1, fout);
            fwrite(&quadNode->grid.green, sizeof(unsigned char), 1, fout);
            fwrite(&quadNode->grid.blue, sizeof(unsigned char), 1, fout);
            dequeue(&q);
        }
    }

    // Se elibereaza memoria cozii folosite
    freeQueue(&q);
}

// Functie care elibereaza memoria unui arbore cuaternar
void free_QuadTree(QuadTreeNode *quadNode)
{
    if (quadNode == NULL) {
        return;
    }
    free_QuadTree(quadNode->first_child);
    free_QuadTree(quadNode->second_child);
    free_QuadTree(quadNode->third_child);
    free_QuadTree(quadNode->fourth_child);

    free(quadNode);
}

int main(int argc, char *argv[])
{
    int i = 0, j = 0;
    char format[3];
    int width, height, valcolor;

    // daca arc > 4 inseamna ca primim argumente
    // pentru cerintele 1 si 2
    if (argc > 4)
    {
        FILE *fin = fopen(argv[3], "rb");
        
        // citim formatul din fisierul PPM
        fscanf(fin, "%s", format);

        // citim width si height
        fscanf(fin, "%d%d", &width, &height);

        // citim valoarea maxima a culorilor
        fscanf(fin, "%d", &valcolor);

        // Pentru a muta cursorul la inceputul matricei
        fseek(fin, 1, SEEK_CUR);

        // Alocam dinamic matricea de pixeli "grid"
        // in care vom citi imaginea in format binar
        Pixels **grid = (Pixels **) malloc(height * sizeof(Pixels *));
        for (i = 0; i < height; i++) {
            grid[i] = (Pixels *) malloc(width * sizeof(Pixels));
        }

        // citim matricea in format binar
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                fread(&grid[i][j], 1, sizeof(Pixels), fin);
            }
        }
        
        int factor;
        if (strcmp(argv[1], "-c1") == 0) {
            factor = atoi(argv[2]);
            FILE *fout = fopen(argv[4], "wb");

            // Cream arborele de compresie
            QuadTreeNode *quadTree = create_QuadTree(grid, 0, 0,
            width, height, factor);

            // Se scriu informatiile necesare
            fprintf(fout, "%d\n", countLevels(quadTree));
            fprintf(fout, "%d\n", countBlocks(quadTree));
            fprintf(fout, "%d\n", sideMax(quadTree));

            // Se elibereaza memoria arborelui
            free_QuadTree(quadTree);

            // Se inchid fisierele
            fclose(fin);
            fclose(fout);
        } else if (strcmp(argv[1], "-c2") == 0) {
            factor = atoi(argv[2]);
            FILE *fout = fopen(argv[4], "wb");

            // Cream arborele
            QuadTreeNode *quadTree = create_QuadTree(grid, 0, 0,
            width, height, factor);

            // Se obtine dimensiunea si se scrie
            unsigned int size = quadTree->width;
            fwrite(&size, sizeof(unsigned int), 1, fout);

            // Se parcurgearborele cu ajutorul functiei si
            // se scriu informatiile
            levelTraverse(quadTree, fout);

            // Se elibereaza memoria arborelui
            free_QuadTree(quadTree);

            // Se inchid fisierele
            fclose(fin);
            fclose(fout);
        }

        // Se elibereaza memoria matricei in care am stocat
        // imaginea in format binar
        for (i = 0; i < height; i++) {
            free(grid[i]);
        }
        free(grid);

    }
    return 0;
}