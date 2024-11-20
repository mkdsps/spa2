#include <stdio.h>
#include <stdlib.h>

enum {RED, BLACK};

typedef struct Proizvod {
    int sifra, brojLicitiranih;
    int niz[100];
}Proizvod;

typedef struct Node{
    Proizvod proizvod;
    int boja;
    struct Node *left, *right, *parent;
}Node;

void padding ( char ch, int n )
{
  int i;
  for ( i = 0; i < n; i++ )
    putchar ( ch );
}
void structure (  Node *root, int level )
{
  int i;
  if ( root == NULL ) {
    padding ( '\t', level );
    puts ( "~" );
  }
  else {
    structure ( root->right, level + 1 );
    padding ( '\t', level );
    printf ( "%d (%d)\n", root->proizvod.sifra, root->boja );
    structure ( root->left, level + 1 );
  }
}


Node *alociraj(int sifra){
    Node *res = (Node *)malloc(sizeof(Node));
    res->left = NULL;
    res->right = NULL;
    res->parent = NULL;
    res->boja = RED;
    
    res->proizvod.sifra = sifra;
    res->proizvod.brojLicitiranih = 0;
    res->proizvod.niz[0] = 0;
    return res;
}

void leftR(Node **root){
    Node *l = *root;
    Node *r = l->right;

    l->right = r->left;
    if (l->right) l->right->parent = l;

    r->parent = l->parent;
    if (l->parent){
        if (l->parent->left == l) l->parent->left = r;
        else l->parent->right = r;
    }

    r->left = l;
    l->parent = r;

    *root = r;
}

void rightR(Node **root){
    Node *r = *root;
    Node *l = r->left;

    r->left = l->right;
    if (r->left) r->left->parent = r;

    l->parent = r->parent;
    if (r->parent){
        if (r->parent->right == r) r->parent->right = l;
        else r->parent->left = l;
    }

    l->right = r;
    r->parent = l;

    *root = l;
}

void ispravi(Node **root, Node *noda){
    Node *uncle, *grandparent;

    while(noda->parent && noda->parent->boja == RED){
        grandparent = noda->parent->parent;
        if (grandparent -> left == noda->parent){ // roditelj je levo dete;
            uncle = grandparent->right;
            if (uncle && uncle->boja == RED){
                grandparent->boja = RED;
                uncle->boja = noda->parent->boja = BLACK;

                noda = grandparent;
            }
            else{ // red uncle
                if (noda->parent->right == noda){
                    noda = noda->parent;
                    leftR(&noda);        
                    if (noda->parent == NULL) *root = noda;
                    noda = noda->left;
                }
                grandparent = noda->parent->parent;

                grandparent->boja = RED;
                noda->parent->boja = BLACK;

                rightR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;
            }
        }
        else{ // roditelj je desno dete;
            uncle = grandparent->left;
            if (uncle && uncle->boja   == RED){
                grandparent->boja = RED;
                uncle->boja = noda->parent->boja = BLACK;
                noda = grandparent;
            }
            else{
                if (noda->parent->left == noda){
                    noda = noda->parent;
                    rightR(&noda);
                    if (noda->parent == NULL) *root = noda;

                    noda = noda->right; 
                }
                grandparent = noda->parent->parent;
                grandparent->boja = RED;
                noda->parent->boja = BLACK; 

                leftR(&grandparent);
                if (grandparent->parent == NULL) *root = grandparent;   
            }
        }
    }
    (*root)->boja = BLACK;
}

void dodaj(Node **root, Node *nov){
    Node *tr = *root, *pret = NULL;

    int sifra = nov->proizvod.sifra;

    while(tr){
        pret = tr;
        if (tr->proizvod.sifra > sifra) tr = tr->left;
        else tr = tr->right;
    }

    nov->parent = pret;

    if (pret == NULL) *root = nov;
    else if(pret->proizvod.sifra > sifra) pret->left = nov;
    else pret->right = nov;



    ispravi(root, nov);
}

Node* formiraj(){
    Node *root = NULL;
    
    int n;
    scanf("%d", &n);
    while(n--){
        int k;
        scanf("%d", &k);
        dodaj(&root, alociraj(k));
    }
    return root;
}

Node* nadji(Node *noda, int sifra){
    while(noda && noda->proizvod.sifra != sifra ){
        if ( noda->proizvod.sifra > sifra ) noda=noda->left;
        else noda = noda->right;  
    }
    return noda;
}

int MAX(int a, int b){
    if (a > b) return a;
    return b;
}
int MIN(int a, int b){
    if (a < b) return a;
    return b;
}

void ucijtajLicitacije(Node *root){
    
    while(1){
        int sifra;
        scanf("%d", &sifra);
        if (!sifra) return;

        int suma;
        scanf("%d", &suma);

        Node *pomoc = nadji(root, sifra);
        int broj = pomoc->proizvod.niz[0];
        

        pomoc->proizvod.niz[pomoc ->proizvod.brojLicitiranih++] = MIN(broj,suma);
        pomoc->proizvod.niz[0] = MAX(broj,suma);        
    }
}

void ispis(Node *root){
    if (root == NULL) return;
    ispis(root->left);
    

    printf("%d", root->proizvod.sifra);
    if (root->proizvod.niz[0]) printf(" %d", root->proizvod.niz[0]);
    printf("\n");

    ispis(root->right);
}

void izbaci(Node **root, Node *noda){
    if (noda == NULL) return;

    Node *levo = noda->left, *desno = noda->right;

    if (noda->proizvod.niz[0]){
        noda->parent = NULL;
        noda->left = NULL;
        noda->right = NULL;           
        dodaj(root, noda);
    }
    else
        free(noda);
    
    izbaci(root, levo);
    izbaci(root, desno);
}

void obilazi(Node* node, int brojac[], int *max_brojanje, int *najcesca_cena) {
        if (node == NULL) return;
               
        for (int i = 0; i < node->proizvod.brojLicitiranih; i++) {
            int cena = node->proizvod.niz[i];
            brojac[cena]++;  

            
            if (brojac[cena] > *max_brojanje) {
                *max_brojanje = brojac[cena];
                *najcesca_cena = cena;
            }
        }
        
        obilazi(node->left, brojac ,max_brojanje, najcesca_cena);
        obilazi(node->right ,brojac , max_brojanje, najcesca_cena);
}

void najcescaCena(Node* root) {
    int brojac[2000] = {0};  // Niz koji broji koliko puta se koja cena pojavila
    int max_brojanje = 0;        // Najveći broj ponavljanja
    int najcesca_cena = -1;      // Cena koja se najviše puta pojavila

    obilazi(root,brojac, &max_brojanje, &najcesca_cena);

    // Ispis rezultata
    if (najcesca_cena != -1) {
        printf("Najcesce licitirana cena je %d, koja se pojavila %d puta.\n", najcesca_cena, max_brojanje);
    } else {
        printf("Nema licitacija.\n");
    }
}

int main(){
    Node *root = formiraj();
    structure(root, 0);
    ucijtajLicitacije(root);
    ispis(root);
    printf("===========================\n");
    Node *noviRoot = NULL;
    izbaci(&noviRoot, root);
    ispis(noviRoot);
    najcescaCena(noviRoot);
} 

