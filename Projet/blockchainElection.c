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
    //TO DO ???

    //soumission de tous les votes et rajout dans l'arbre
    printf("\nOperation en cours : soumission des votes\n");
    CellTree *tree = NULL;  
    int d = 3;  //nombre de bits a 0

    int i, nbFichier = 1, votesParBlock = 10;
    char nomFichier[256];
    CellProtected *current = votes;
    Protected *pr;
    Key *cleAssesseur = NULL;
    
    while (current)   {

        //On copie la cle de l'assesseur afin de pouvoir liberer completement le block et la liste de cles plus tard
        cleAssesseur = copie_key(current->data->pKey); //la cle de l'assesseur est la cle du premier a voter dans le block
        
        //On cree Pending_votes.txt en soumettant le bon nombre de votes
        i=0;
        while (current && i<votesParBlock)   {
            pr = current->data;
            submit_vote(pr);
            current = current->next;
            i++;
        }

        create_block(&tree, cleAssesseur, d);       //On cree un block a partir de Pending_votes.txt, puis on ecrit le block dans Pending_block.txt
        sprintf(nomFichier,"fichier%d",nbFichier);
        add_block(d,nomFichier);                    //On ajoute le contenu du fichier Pending_block.txt au repertoire Blockchain
        nbFichier++;
    }
    printf("\nFin de l'operation de soumission des votes\n");

    //Ayant enregistre les fichiers dans Blockchain on peut supprimer l'arbre de construction
    printf("\nAffichage de l'arbre de construction :\n");
    print_tree(tree);
    delete_tree(tree);

    //Lecture du repertoire Blockchain, re-creation et affichage de l'arbre
    tree = read_tree();
    printf("\nAffichage de l'arbre provenant du repertoire Blockchain :\n");
    print_tree(tree);
  
    //determination du gagnant
    Key *gagnant = compute_winner_BT(tree, candidates, publicKeys,nbCandidates*2,nbVoters*2);

    char *g = key_to_str(gagnant);
    printf("\n%s a gagne\n\n",g);

    free(g);
    delete_tree(tree);
    delete_list_protected_total(votes);
    delete_list_keys(candidates);
    delete_list_keys(publicKeys);


    return 0;
}