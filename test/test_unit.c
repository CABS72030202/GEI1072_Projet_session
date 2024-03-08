#include "../lib/acutest.h"
#include<stdio.h>
#include "../src/tools.h"
#include "../test/test_unit.h"



void check_produit(void) 
{
    // Cas de test 1 : Tester produit avec des nombres positifs
  
    TEST_CHECK(produit(2, 3) == 6);

    // Cas de test 2 : Tester produit avec un nombre négatif
   
    TEST_CHECK(produit(-4, 5) == -20);

    // Cas de test 3 : Tester produit avec deux nombres négatifs
   
    TEST_CHECK_(produit(-2, -3)== 6,"Test échoué : attendu %d mais obtenu %d",6 ,produit(-2, -3));

    // Ajouter plus de cas de test au besoin...

    // Vous pouvez utiliser TEST_CHECK_ pour fournir plus de détails sur un test échoué
    // TEST_CHECK_(result == expected, "Message supplémentaire si le test échoue");

    // Exemple :
    // TEST_CHECK_(result == expected, "Test échoué : attendu %d mais obtenu %d", attendu, result);
}


TEST_LIST = {
    {"check_produit", check_produit},
    {0}
};