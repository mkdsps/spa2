#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int broj, trecaCif;
    struct node *left, *right;
} node;

int trecaCif(int broj){
    return broj/100 % 10;
}

node *alociraj(int broj){
    node *res = (node *)malloc(sizeof(node));
    res->left = NULL;
    res->right =NULL;
    res->broj = broj;
    res->trecaCif = trecaCif(broj);

    return res;
}

void dodaj(node **root, int broj){
    node *nov = alociraj(broj);
    node *tr = *root, *pret = NULL;

    if (*root == NULL){
        *root = nov;    
        return;
    } 

    while (tr){
        pret = tr;
        if (tr->trecaCif > nov->trecaCif) tr = tr->left;
        else tr = tr->right;
    }

    if (pret->trecaCif > nov->trecaCif) pret->left = nov;
    else pret->right = nov;
}

node *formiraj(){
    node *root = NULL;

    int n, k;
    scanf("%d", &n);
    while (n--)
    {
        scanf("%d", &k);
        dodaj(&root, k);
    }
    return root;
}

void ispis(node *noda){
    if (noda == NULL) return;

    ispis(noda->left);
    printf("%d\n", noda->broj);
    ispis(noda->right);
}

int prebroj(node* noda, int thr){
    if (noda == NULL) return 0;
    
    int zbir = 0;

    if (noda->trecaCif >= 3 && noda->trecaCif <= 7){
        int levo = prebroj(noda->left, thr);
        int desno = prebroj(noda->right, thr);

        if (noda->broj>thr) return levo + desno + 1;
        return levo + desno;
    }
    else if(noda->trecaCif < 3)
        return prebroj(noda->right, thr);
    else
        return prebroj(noda->left , thr);
}

int main(){
    node *root = formiraj();
    ispis(root);

    int x; 
    scanf("%d", &x);
    printf("%d",  prebroj(root, x));
     
}