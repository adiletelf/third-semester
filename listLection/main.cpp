#include <cstdlib>
#include <stdio.h>
using namespace std;

struct Trailer;
struct Lider
{
    int key;
    int count;
    Lider   *nx;
    Trailer *tr;
};

struct Trailer
{
    Lider *ld;
    Trailer *nx;
};

void AddEdge(Lider *& vertexes, int vertexA, int vertexB)
{
    Lider *ptrA = NULL, *ptrB = NULL;
    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);
    bool setEdge = true;
    if (ptrA && ptrB)
    {
        Trailer* trTmp = ptrA->tr;
        for (Trailer *trTmp = ptrA->tr; trTmp && trTmp->ld != ptrB; trTmp = trTmp->nx);
            setEdge = (bool)trTmp;
    }
    else
    {
        if (!ptrA)
        {
            ptrA = new Lider;
            ptrA->key = vertexA;
            ptrA->count = 0;
            ptrA->nx = NULL;
            ptrA->tr = NULL;
            ptrA->nx = vertexes;
            vertexes = ptrA;
        }

        if (!ptrB)
        {
            ptrB = new Lider;
            ptrB->key = vertexB;
            ptrB->count = 0;
            ptrB->nx = NULL;
            ptrB->tr = NULL;
            ptrB->nx = vertexes;
            vertexes = ptrB;
        }

        if (setEdge)
        {
            ptrB->count++;
            Trailer* elemTr = new Trailer;
            elemTr->ld = ptrB;
            elemTr->nx = ptrA->tr;
            ptrA->tr = elemTr;
        }
    }
}

void PrintGraph(Lider* vertexes)
{
    if (vertexes == NULL)
        printf("Graph is Empty");
    else
        while (vertexes)
        {
            Trailer* trTmp = vertexes->tr;
            while (trTmp)
            {
                printf("(%d, %d)\n", vertexes->key, trTmp->ld->key);
                trTmp = trTmp->nx;
            }
            vertexes = vertexes->nx;
        }
}


void DeleteGraph(Lider *& vertexes)
{
    while (vertexes)
    {
        Trailer* tmp = vertexes->tr;
        vertexes->tr = vertexes->tr->nx;
        delete tmp;
    }
    Lider* tmp = vertexes;
    vertexes = vertexes->nx;
    delete tmp;
}


bool ExcludeEdge(Lider *& vertexes, int vertexA, int vertexB)
{
    Lider *ptrA = NULL, *ptrB = NULL;
    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);
    if (ptrA && ptrA->tr && ptrB)
    {
        if (ptrA->tr->ld == ptrB)
        {
        Trailer *t = ptrA->tr;
        ptrA->tr = ptrA->tr->nx;
        delete t;
        ptrB->count--;
       }    
    
        else
        {
            Trailer* trTmp;
            for (trTmp = ptrA->tr; trTmp->nx && trTmp->nx->ld != ptrB; trTmp = trTmp->nx);
            if (trTmp->nx)
            {
                Trailer* t = trTmp->nx;
                trTmp->nx = trTmp->nx->nx;
                delete t;
                ptrB->count--;
            }
        }
        return true;
    } 
    return false;
}


void topologicalSort(Lider* LD)
{
    Lider* LDnew = nullptr;
    Lider* pt = LD;
    while (!pt)
    {
        if (pt->count == 0)
        {
            pt->nx = LDnew;
            LDnew = pt;
        }
        pt = pt->nx;
    }
    pt = LDnew;
    LDnew = LDnew->nx;
    Trailer* T = pt->tr;
    while (!T)
    {
        T->ld->key--;
        T = T->nx;
    }
}



int main(int argc, char** argv)
{
    FILE* graphInFile = fopen("graph.dat", "r");
    Lider* graph = NULL;
    int vertexA = 0, vertexB = 0;
    int k = 0;
    if (graphInFile)
    {
        while ((k = fscanf(graphInFile, "%d%d", &vertexA, &vertexB)) > 0)
        {
            printf("(%d, %d)\n", vertexA, vertexB);
            AddEdge(graph, vertexA, vertexB);
        }

        fclose(graphInFile);
       
    }
    else
        puts("File is not open");

    return 0;
}
