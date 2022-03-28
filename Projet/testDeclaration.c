#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "outilsMathematiques.h"
#include "generationNbPremier.h"
#include "generationClef.h"
#include "testMillerRabin.h"
#include "rsa.h"
#include "manipulationClefs.h"
#include "signature.h"
#include "declaration.h"

void print_long_vector(long *result, int size) { 
    printf("Vector: [");
    for (int i=0; i<size; i++){
        printf("%ld \t", result[i]);
    }
    printf("]\n"); 
}

int main(void)  {
    srand(time(NULL));

    //Testing Init Keys
    Key *pKey = (Key *)malloc(sizeof(Key));
    Key *sKey = (Key *)malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 3, 7);
    printf("pKey: %lx, %lx \n", pKey->m, pKey->n);
    printf("sKey: %lx, %lx \n", sKey->m, sKey->n);

    //Testing Key Serialization
    char *chaine = key_to_str(pKey);
    printf("key_to_str: %s \n", chaine);
    Key *k = str_to_key(chaine);
    printf("str_to_key: %lx, %lx \n", k->m, k->n);

    //Testing signature
    //Candidate keys:
    Key *pKeyC = (Key *)malloc(sizeof(Key));
    Key *sKeyC = (Key *)malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 3, 7);

    //Declaration:
    char *mess = key_to_str(pKeyC);
    printf("%s vote pour %s\n", key_to_str(pKey), mess);
    Signature *sgn = sign(mess, sKey);
    printf("signature: ");
    print_long_vector(sgn->content, sgn->size);
    chaine = signature_to_str(sgn);
    printf("signature_to_str: %s\n", chaine);
    sgn = str_to_signature(chaine);
    printf("str_to_signature: ");
    print_long_vector(sgn->content, sgn->size);

    //Testing protected:
    Protected *pr = init_protected(pKey, mess, sgn);
    //Verification:
    if (verify(pr)) {
        printf("Signature valide\n");
    } else {
        printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr);
    printf("protected_to_str: %s\n", chaine);
    pr = str_to_protected(chaine);
    printf("str_to_protected: %s %s %s\n", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sgn));

    free(pKey);
    free(sKey);
    free(pKeyC);
    free(sKeyC);
    return 0;
}