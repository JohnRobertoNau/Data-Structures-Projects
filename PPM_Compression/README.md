Am impartit programul in mai multi pasi:

Prima data am declarat structura "Pixels" pe care am folosit-o in main ca sa
declar matricea de pixeli in care voi citi imaginea in format binar.

Dupa aceea am declarat structura QuadTreeNode, care contine x si y (coordo-
natele fiecarui bloc stocat in arbore) o variabila grid, de tip Pixels,
un poiner catre cei patru copii ai arborelui si o variabila is_leaf,
pentru a stabili cu usurinta daca nodul nostru este frunza (is_leaf == 1),
sau nu (is_leaf == 0).


Apoi am definit structurile QueueNode si Queue, de care m-am folosit in
parcurgerea arborelui pe nivele. Urmeaza mai multe functii pentru Queue,
precum: initQueue (care creaza o coada), enqueue (care adauga un nod in
coada), dequeue (care elimina ultimul element din coada, eliberandu-i
memoria) si freeQueue (care elibereaza memoria cozii).


Dupa aceea am declarat functia calculate_values care functioneaza astfel:
Primeste ca parametrii grid, size, x, y si valori de tip unsigned char
vred (in care se va calcula si pastra valoarea pentru rosu, analog pentru
vgreen si vblue) si vmean in care se va retine si calcula media culorilor,
care va fi comparata ulterior cu factorul citit din linia de comanda.


Functia create_QuadTree construieste recursiv arborele de compresie:

-Primeste ca parametrii imaginea stocata in image, x si y (coordonatele)
width-ul si height-ul imaginii si factorul de compresie.

- creeaza un prim nod unde se atribuie campurilor nodului valorile
ce au fost date functiei

- apeleaza functia calculate_values pentru a calcula valorile descrise
anterior.

- se realizeaza o comparatie. Daca vmean > factor, inseamna ca imaginea
trebuie divizata. Se precizeaza faptul ca nu este frunza, iar apoi se
apeleaza recursiv functia pentru fiecare copil al arborelui,
cu coordonatele fiecaruia.

- daca imaginea nu (mai) trebuie divizata, se atribuie valoarea fiecarei
culori in campurile specifice pentru matricea de pixeli specifica
nodului, se precizeaza ca nodul nu mai are copii si ca este frunza.
Se returneaza radacina acestui arbore.


Pentru a calcula numarul de nivele am folosit urmatoarea strategie:
Am verificat in functia countLevels daca root-ul arborelui este nul,
iar daca da se returneaza 0. Daca nu, se verifica recursiv nivelul
fiecarui subarbore iar numarul de niveluri este maximul dintre
nivelurile celor 4 subarbori + 1 (pentru a include si nivelul
radacinii). Pentru a calcula maximul dintre cele 4, m-am folosit de
o functie vmax4 (care returneaza maximul dintre 4 valori),
folosindu-ma de o functie ajutatorea vmax2.

Am observat ca de fapt numarul de blocuri este acelasi cu numarul de
frunze al arborelui. Prin urmare, in functia countBlocks am pastrat
in k numarul de frunze gasite, apeland recursiv functia pentru fiecare
copil.

In functia sideMax am calculat latura celui mai mare bloc de imagine.
Daca nodul este frunza, returnam width-ul blocului. Daca nu,
declaram o variabila max_side in care vom pastra valorile maxime
gasite. Apelam recursiv functia pentru fiecare copil si pastram
valoarea acesteia intr-o variabila child_side, iar daca
child_side > max_side inseamna ca child_side este deocamdata cea
mai mare valoare si o retinem in max_side.

Functia levelTraverse parcurge arborele de compresie pe niveluri si
se afiseaza informatiile necesare, eliberandu-se la sfarsit memoria
cozii.

Functia free_QuadTree elibereaza recursiv memoria arborelui de
compresie.

In functia main am verificat daca primim argumente pentru cerintele
1 si 2, (adica daca argc > 4). Daca da, am citit din fisierul
PPM plasat ca argument in linia de comanda toate elementele pe care
le are. Dupa aceea mutam cursorul la inceputul imaginiisi o citim
binar, intr-o matrice de tip Pixel **grid. Apoi citim factorul
din linia de comanda si verificam ce cerinta trebuie rezolvata.
Daca se rezolva cerinta 1 (if (strcmp(argv[1], "-c1") == 0)),
se creaza arborele de compresie si se scriu in fisier informatiile
necesare, apoi se elibereaza memoria si se inchid fisierele

Daca se rezolva cerinta 2, am citit factorul, am creat arborele de
compresie si am scris toate informatiile necesare, parcurgand
arborele pe nivele. Apoi am eliberat memoria si am inchis fisierele.
