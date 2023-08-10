#include <Panel.hpp>
#include <MsgHandler.hpp>

#include "its_logo.h"

/**
 *  Auteur : Andre MUTOKE & Godwin LUFATAKI
 *  Description : Voici un simple tes pour la matrice a led
 *              Pour pouvoir instancier, voici les parametre utilises 
 *              Panel (
 *                      int largeur,
 *                      int hauteur, 
 *                      int pin_de_sortie : Par defaut c'est 4,
 *                      uint8_t type_de_matrice (Par defaut c'est NEO_MATRIX_BOTTOM +
                                                                NEO_MATRIX_LEFT +
                                                                NEO_MATRIX_COLUMNS +
                                                                NEO_MATRIX_ZIGZAG),
                        nePixelType type de led (Par defaut : NEO_GRB + NEO_KHZ800)
 * 
 * 
 *                  );                
 * 
 * 
 * 
*/

Panel *ecran = new Panel(32, 32);
/**
 * Vous pouvez aussi faire changer des parametres 
 * Panel *ecran = new Panel(32, 32, 6);
 * 
 * Panel *ecran = new Panel(24, 65, 4,NEO_MATRIX_BOTTOM +
                          NEO_MATRIX_LEFT +
                          NEO_MATRIX_COLUMNS +
                          NEO_MATRIX_ZIGZAG)
 * 
 *  Parmi les options il y a :
 * NEO_MATRIX_TOP         ///< Pixel 0 est au dessu de la matrice
 * NEO_MATRIX_BOTTOM       ///< Pixel 0 est en dessous de la matrice
 * NEO_MATRIX_LEFT         ///< Pixel 0 est a gauche de la matrice
 * NEO_MATRIX_RIGHT       ///< Pixel 0 est a droite de la matrice
 * NEO_MATRIX_ROWS         ///< La matrice est concu en ligne (horizontal)
 * NEO_MATRIX_COLUMNS      ///< La matrice est concu en colonne (vertical)
 * NEO_MATRIX_PROGRESSIVE  ///< Chaque ligne commence une le pixel suivant de la ligne precedente
 * NEO_MATRIX_ZIGZAG       ///< Les pixels sont se suivant nzigzag
 * 
 * 
*/
std::map<std::string, std::string> mapCouleur = ecran->getMapColor(don_bosco);

void setup()
{
    ecran->begin();
    
}
void loop()
{
    ecran->printMsg("Hello world !"); delay(5000);
    ecran->cleanPanel();
    ecran->drawImg(don_bosco, mapCouleur); delay(5000);

    //ecran->drawImg(donBosco);

}