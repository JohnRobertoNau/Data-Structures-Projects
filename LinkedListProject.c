/* NAU John-Roberto - 314CC */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_INSTR 100 //lungimea maxima a randului cu instructiunea

// Definim structura unui nod dintr-o lista
typedef struct Node {
    char val;
    struct Node *urm;
    struct Node *ant;
} Node;

// Definim structura benzii
typedef struct Banda {
    Node *sant;
    Node *deget;
} Banda;

// Definim structura stivei
typedef struct Stiva {
    Node *deget;
    struct Stiva *urm;
} Stiva;

// Definim structura unui nod din coada
typedef struct nodCoada {
    char *operatie;
    struct nodCoada *urm;
} nodCoada;

// Definim structura cozii
typedef struct Coada {
    nodCoada *fata;
    nodCoada *spate;
} Coada;

// Definim o functie care creeaza un nod cu o anumita valoare
Node* creeaza_nod(char val)
{
    Node* nod = (Node *) malloc(sizeof(Node));
    nod->val = val;
    nod->urm = NULL;
    nod->ant = NULL;
    return nod;
}

// Definim o functie care creaza o banda
Banda *creeaza_banda()
{
    Banda *banda;
    banda = (Banda *) malloc(sizeof(Banda));
    banda->sant = creeaza_nod('0');
    banda->deget = creeaza_nod('#');
    banda->sant->urm = banda->deget;
    banda->deget->ant = banda->sant;
    return banda;
}

// Definim functia show care afiseaza banda si specifica pozitia degetului
void show(Banda *banda, FILE *out)
{
    Node *nod = banda->sant->urm;
    while (nod != NULL)
    {
        if (nod == banda->deget)
        {
            fprintf(out, "|%c|", nod->val);
        }
        else
        {
            fprintf(out,"%c", nod->val);
        }
        nod = nod->urm;
    }
    fprintf(out, "\n");
}

// Functie care muta degetul la stanga si returneaza 0 daca nu se poate muta la stanga
char move_left(Banda **banda)
{
    if ((*banda)->deget->ant != (*banda)->sant)
    {
        (*banda)->deget = (*banda)->deget->ant;
        return 1;
    }
    return 0;
}

// Functie care muta degetul la dreapta
void move_right(Banda **banda)
{
    if ((*banda)->deget->urm != NULL) 
    {
        (*banda)->deget = (*banda)->deget->urm;
    }
    else // Daca suntem la ultimul nod din dreapta, mai cream unul
    {
        Node *nod;
        nod = creeaza_nod('#');
        (*banda)->deget->urm = nod;
        nod->ant = (*banda)->deget;
        (*banda)->deget = nod;
    }
}

// Functie care muta degetul la stanga pana cand gaseste un anumit element
void move_left_char(Banda **banda, char c, FILE *out)
{
    Node *copie;
    copie = (*banda)->deget;
    if ((*banda)->deget->val == c) // Daca valoarea pe care o cautam este cea de pe deget, iesim din functie
    {
        return;
    }
    else
    {
        while ((*banda)->deget->ant != (*banda)->sant)
        {
            if ((*banda)->deget->val == c)
            {
                break;
            }
            (*banda)->deget = (*banda)->deget->ant;
        }
        if ((*banda)->deget->ant == (*banda)->sant && (*banda)->deget->val != c) //Daca am ajuns in capăt si nu am gasit valoarea
        {
            fprintf(out, "ERROR\n");
            (*banda)->deget = copie;
        }
    }
    
}

// Functie care muta degetul la dreapta pana cand gaseste un anumit element
void move_right_char(Banda **banda, char c)
{
    if ((*banda)->deget->val == c) // Daca valoarea cautata este pe deget, iesim din functie
    {
        return;
    }
    while ((*banda)->deget->urm != NULL)
    {
        (*banda)->deget = (*banda)->deget->urm;
        if ((*banda)->deget->val == c)
        {
            break;
        }
    }
    if ((*banda)->deget->urm == NULL && (*banda)->deget->val != c)
    {
        move_right(banda);
    }
}

// Functie care insereaza pe pozitia din stanga a degetului o anumita valoare si muta degetul pe acea valoare
void insert_left(Banda **banda, char c, FILE *out)
{
    if ((*banda)->deget->ant == (*banda)->sant)
    {
        fprintf(out, "ERROR\n");
    }
    else
    {
        Node *nod;
        nod = creeaza_nod(c);
        Node *ant = (*banda)->deget->ant;
        (*banda)->deget->ant = nod;
        nod->urm = (*banda)->deget;
        nod->ant = ant;
        ant->urm = nod;
        (*banda)->deget = nod;
    }
}

// Functie care insereaza pe pozitia din dreapta a degetului o anumita valoare si muta degetul pe acea valoare
void insert_right(Banda **banda, char c)
{
    Node *nod;
    nod = creeaza_nod(c);
    if ((*banda)->deget->urm != NULL)
    {
        Node *urm = (*banda)->deget->urm;
        nod->urm = urm;
        urm->ant = nod;
    }
    nod->ant = (*banda)->deget;
    (*banda)->deget->urm = nod;
    (*banda)->deget = nod;
}

// Functie care afiseaza elementul de pe pozitia degetului
void show_current(Banda *banda, FILE *out)
{
    fprintf(out,"%c\n", banda->deget->val);
}

// Functie care insereaza un element pe pozitia degetului
void write_caracter(Banda **banda, char c)
{
    (*banda)->deget->val = c;
}

Stiva *nodnouStiva(Node *deget)
{
    Stiva *nod;
    nod = (Stiva *) malloc(sizeof(Stiva));
    nod->deget = deget;
    nod->urm = NULL;
    return nod;
}

int isEmptyStack(Stiva *root)
{
    return (root == NULL);
}

// Functie care adauga un element in stiva
void pushStack(Stiva **root, Node *deget)
{
    Stiva *nod = nodnouStiva(deget);
    nod->urm = (*root);
    (*root) = nod;
}

// Functie care elimina un element din stiva si elibereaza memoria
void popStack(Stiva **root)
{
    if (isEmptyStack(*root))
    {
        return;
    }
    Stiva *temp = (*root);
    (*root) = (*root)->urm;
    free(temp);
}

// Functie care returneaza ultimul nod din stiva, fara sa-l elimine
Node *peekStack(Stiva *root)
{
    if (isEmptyStack(root))
    {
        exit(1);
    }
    return root->deget;
}

Coada *initQueue()
{
    Coada *q = (Coada *) malloc(sizeof(Coada));
    q->fata = NULL;
    q->spate = NULL;
    return q;
}

int isEmptyQueue(Coada *q)
{
    return (q->fata == NULL);
}

void enqueue(Coada *q, char *s)
{
    nodCoada *nod;
    nod = (nodCoada *) malloc(sizeof(nodCoada));
    nod->operatie = (char *) malloc(strlen(s) + 1);
    strcpy(nod->operatie, s);
    nod->urm = NULL;

    if (isEmptyQueue(q) == 0)
    {
        q->spate->urm = nod;
        q->spate = nod;
    }
    else
    {
        q->spate = q->fata = nod;
    }
}

// Functie care elimina din coada si elibereaza memoria
void dequeue(Coada **q)
{
    nodCoada *temp;
    if (isEmptyQueue(*q))
    {
        return;
    }
    temp = (*q)->fata;
    free((*q)->fata->operatie);
    (*q)->fata = (*q)->fata->urm;
    free(temp);
}

char *peekQueue(Coada *q)
{
    return q->fata->operatie;
}

// Functie pentru a elibera memoria din structura Banda
void freeBanda(Banda *banda)
{
    Node *temp = banda->sant;
    Node *urm;
    while (temp != NULL)
    {
        urm = temp->urm;
        free(temp);
        temp = urm;
    }
    free(banda);
}

// Functie pentru eliberarea memoriei Stivei
void freeStack(Stiva **stiva)
{
    while (isEmptyStack(*stiva) != 1)
    {
        popStack(stiva);
    }
    free(*stiva);
    (*stiva) = NULL; //pentru a nu pointa spre o zona de memorie dezalocata
}

// Functie pentru eliberarea memoriei Cozii
void freeQueue(Coada **coada)
{
    if (*coada == NULL)
    {
        return;
    }
    while (isEmptyQueue(*coada) != 1)
    {
        dequeue(coada);
    }
   free(*coada);
   (*coada) = NULL;
}

// Functia pentru optiunea de execute
void execute(Coada *coada, Banda **banda, Stiva **stiva_undo, Stiva **stiva_redo, FILE *out)
{
    if (strncmp(peekQueue(coada),"MOVE_LEFT_CHAR", strlen("MOVE_LEFT_CHAR")) == 0)
    {
        pushStack(stiva_undo, (*banda)->deget);
        move_left_char(banda, peekQueue(coada)[strlen("MOVE_LEFT_CHAR") + 1], out);
    }
    else if (strncmp(peekQueue(coada),"MOVE_LEFT", strlen("MOVE_LEFT")) == 0)
    {
        pushStack(stiva_undo, (*banda)->deget);
        int ok = move_left(banda);
        if (ok == 0) 
        {
            popStack(stiva_undo); //daca nu s-a schimbat pozitia degetului, stergem ultima adaugare din stiva_undo
        }
    }

    if (strncmp(peekQueue(coada),"MOVE_RIGHT_CHAR", strlen("MOVE_RIGHT_CHAR")) == 0)
    {
        pushStack(stiva_undo, (*banda)->deget);
        move_right_char(banda, peekQueue(coada)[strlen("MOVE_RIGHT_CHAR") + 1]);
    }
    else if (strncmp(peekQueue(coada),"MOVE_RIGHT", strlen("MOVE_RIGHT")) == 0)
    {
        pushStack(stiva_undo, (*banda)->deget);
        move_right(banda);
    }
    
    if (strncmp(peekQueue(coada),"WRITE", strlen("WRITE")) == 0)
    {
        write_caracter(banda, peekQueue(coada)[strlen("WRITE") + 1]);
        freeStack(stiva_undo);
        freeStack(stiva_redo);
    }
    if (strncmp(peekQueue(coada),"INSERT_LEFT", strlen("INSERT_LEFT")) == 0)
    {
        insert_left(banda, peekQueue(coada)[strlen("INSERT_LEFT") + 1], out);
        freeStack(stiva_undo);
        freeStack(stiva_redo);
    }
    if (strncmp(peekQueue(coada),"INSERT_RIGHT", strlen("INSERT_RIGHT")) == 0)
    {
        insert_right(banda, peekQueue(coada)[strlen("INSERT_RIGHT") + 1]);
        freeStack(stiva_undo);
        freeStack(stiva_redo);
    }
    dequeue(&coada);
}

// Functie pentru optiunea undo
void undo(Banda **banda, Stiva **stiva_undo, Stiva **stiva_redo)
{
    pushStack(stiva_redo, (*banda)->deget);
    (*banda)->deget = peekStack(*stiva_undo);
    popStack(stiva_undo);
}

// Functie pentru optiunea redo
void redo(Banda **banda, Stiva **stiva_undo, Stiva **stiva_redo)
{
    pushStack(stiva_undo, (*banda)->deget);
    (*banda)->deget = peekStack(*stiva_redo);
    popStack(stiva_redo);
}

int main()
{
    Banda *banda = creeaza_banda();
    FILE *in = fopen("tema1.in", "r");
    FILE *out = fopen("tema1.out", "w");

    Stiva *stiva_undo = NULL;
    Stiva *stiva_redo = NULL;
    Coada *coada=initQueue();

    int n = 0,i = 0;
    char instr[MAX_INSTR]; // sirul care contine instructiunea din fiecare linie

    fscanf(in,"%d", &n);
    fgets(instr,MAX_INSTR,in);

    for (i = 0; i < n; i++)
    {
        fgets(instr, MAX_INSTR, in);
        if (strncmp(instr,"MOVE_LEFT_CHAR", strlen("MOVE_LEFT_CHAR")) == 0)
        {
            enqueue(coada, instr);
        }
        else if (strncmp(instr,"MOVE_LEFT", strlen("MOVE_LEFT")) == 0)
        {
            enqueue(coada, instr);
        }

        if (strncmp(instr,"MOVE_RIGHT_CHAR", strlen("MOVE_RIGHT_CHAR")) == 0)
        {
            enqueue(coada, instr);
        }
        else if (strncmp(instr,"MOVE_RIGHT", strlen("MOVE_RIGHT")) == 0)
        {
            enqueue(coada, instr);
        }

        if (strncmp(instr,"WRITE", strlen("WRITE"))==0)
        {
            enqueue(coada, instr);
        }
        if (strncmp(instr,"INSERT_LEFT", strlen("INSERT_LEFT")) == 0)
        {
            enqueue(coada, instr);
        }
        if (strncmp(instr,"INSERT_RIGHT", strlen("INSERT_RIGHT")) == 0)
        {
            enqueue(coada, instr);
        }
        if (strncmp(instr, "SHOW_CURRENT", strlen("SHOW_CURRENT")) == 0)
        {
            show_current(banda, out);
        }
        else if (strncmp(instr, "SHOW", strlen("SHOW")) == 0)
        {
            show(banda, out);
        }
        if (strncmp(instr, "UNDO", strlen("UNDO")) == 0)
        {
            undo(&banda, &stiva_undo, &stiva_redo);
        }
        if (strncmp(instr, "REDO", strlen("REDO")) == 0)
        {
            redo(&banda, &stiva_undo, &stiva_redo);
        }
        if (strncmp(instr, "EXECUTE", strlen("EXECUTE")) == 0)
        {
            execute(coada, &banda, &stiva_undo, &stiva_redo, out);
        }
    }
    fclose(in);
    fclose(out);
    freeBanda(banda);
    freeQueue(&coada);
    freeStack(&stiva_undo);
    freeStack(&stiva_redo);
    return 0;
}