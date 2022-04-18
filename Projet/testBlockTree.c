#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "donneesVote.h"
#include "blockchain.h"
#include "blockTree.h"
#include "winner.h"


int main()  {
    srand(time(NULL));

    //generation de donnees
    int nbVoters = 1000, nbCandidates = 5;
    generate_random_data(nbVoters, nbCandidates);
    printf("\nOn genere %d electeurs et %d candidats.\n",nbVoters,nbCandidates);

    //lire les cles publiques des candidats et de tout le monde
    CellKey *candidates = read_public_keys("candidates.txt");
    CellKey *publicKeys = read_public_keys("keys.txt");

    //lire les declarations de vote
    CellProtected *votes = read_protected("declarations.txt");

    //rajout d'une fraude
    //TO DO

    //soumission de tous les votes et rajout dans l'arbre
    printf("\nOperation en cours : soumission des votes (c'est assez long...)\n");
    CellTree *tree = NULL;  
    int d = 3;  //nombre de bits a 0

    int i, nbFichier = 1, votesParBlock = 3;
    char nomFichier[256];
    CellProtected *current = votes;
    Protected *pr;
    Key *cleAssesseur = current->data->pKey;

    pr = current->data;
    submit_vote(pr);
    
    create_block(&tree,cleAssesseur,d);
    sprintf(nomFichier,"fichier%d",nbFichier);
    fprintf(stderr,"\nadd_block : nomFichier = %s\n",nomFichier);
    add_block(d,nomFichier);

    delete_tree(tree);
    delete_list_protected_total(votes);
    delete_list_keys(candidates);
    delete_list_keys(publicKeys);


    return 0;
}