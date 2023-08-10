
//=============================================================================================================
//==============================NEW_METHOD_BY_GODWIN_AND_ANDRE============================================
//=============================================================================================================
#ifndef PANEL_H_INCLUDED
#define PANEL_H_INCLUDED
#include <Arduino.h>
#include <MsgHandler.hpp>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <c++/8.4.0/vector>
#ifndef PSTR
    #define PSTR // Make Arduino Due happy
#endif

#define CENTER 0
#define LEFT 1
#define RIGHT 2
#define NONE 4

//#include <c++/8.4.0/iostream>
//#include <c++/8.4.0/string>
#include <c++/8.4.0/cstdlib>
#include <c++/8.4.0/cstring>
// #include <c++/8.4.0/cmath>
#include <c++/8.4.0/map>
//#include <SPIFFS.h>
#include "FS.h"

class Panel{
private:
    int m_largeur; // La largeur de l'ecran, qui correspond au nombre des Leds en Largeur
    int m_hauteur; // La hauteur de l'ecran qui correspond au nombre des Leds en Hauteur
    uint8_t m_pin; // La broche de sortie
    Adafruit_NeoMatrix *matrix; // L'objet representant la matrice

    int power(int a, int b);

    int whichChiffre(char c);
    bool isCHiffre(char c);
    int convertCharToInt(std::string& str);
    std::vector<std::string> initString(int w, int h);


    int findSeparator(std::string& str);
    std::string getFirst(std::string& str);
    std::string getSecond(std::string&str);

    std::vector<int > getFisrtAndLastDelimitator(std::string pathImgXPM);
    //std::vector<std::string > getImgStrings(std::string& pathImg);ORIGINAL
    std::vector<std::string > getImgStrings(fs::File &Fichier);
    //std::vector<std::string > Panel::getImgStrings(const char* pathImg);

    

public:
    Panel(int width, // La largeur de l'ecran, ou le nombre des leds en Largeur
           int height, // La hauteur de l'ecran, ou le nombre des Leds en Hauteur
           uint8_t pin = (uint8_t)4, // La broche de sortie des donnees
           uint8_t matrixType = (uint8_t) NEO_MATRIX_BOTTOM +
                                          NEO_MATRIX_LEFT +
                                          NEO_MATRIX_COLUMNS +
                                          NEO_MATRIX_ZIGZAG, // Le type de la matrice
           neoPixelType ledType = (neoPixelType) NEO_GRB + NEO_KHZ800); // le type des Leds
    ~Panel();


    void begin(){matrix->begin();}
    void cleanPanel(){matrix->clear();}
    void setBrightness(uint8_t bright){matrix->setBrightness(bright);}
    void Show(){matrix->show();}
    void setCursor(int x, int y){matrix->setCursor(x,y);}
    void setTextSize(int sizeTxt){matrix->setTextSize(sizeTxt);}
    void wrapMsg(bool wrap){matrix->setTextWrap(false);}
    void setXY(MsgPanel& message, Coord position){message.setCoord(position);}
    void printMsg(MsgPanel& message){
        Panel::matrix->print(message.getMsg().c_str());
    }
    void printMsg(const char *message){
        Panel::matrix->print(message);
    }
    void printMsg(char *message){
        Panel::matrix->print(message);
    }
    int getHeight(){return m_hauteur;}
    int getWidth(){return m_largeur;}

    void fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t rayon,uint16_t color){Panel::matrix->drawCircle(x0, y0, rayon, color);}
    void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size)
    {
        Panel::matrix->drawChar( x, y, c, color, bg, size);
    }
    
    void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
    {
        Panel::matrix->drawFastVLine(x, y, h, color);
    }
    void drawHLine(int16_t x, int16_t y, int16_t h, uint16_t color)
    {
        Panel::matrix->drawFastHLine(x, y, h, color);
    }

    void setTextColor(uint16_t txtcolor, uint16_t backgnd = (uint16_t) 0U);
    void fillPanel(uint8_t allPanel=0);
    void centerXY(MsgPanel& message, bool x = true, bool y = true);
    void centerXY(MsgPanel tabMsg[],int sizeTab, bool x = true, bool y = true);
    void centerXY(std::vector<MsgPanel> tabMsg, bool x = true, bool y = true);
    void setXY(MsgPanel& message, int x, int y);
    void multiPrint(std::vector<MsgPanel> tabMsg, uint posX = NONE);
    void multiPrint(MsgPanel tabMsg[],int sizeTab, uint posX = NONE);
    void setPrintMsg(MsgPanel& message);
    void printInline(const char tabChar[], int sizeMsg, int spaceChar, uint16_t couleur = BLEU, bool y = false, int posY = 0);
    void printInline(const char tabChar[], int sizeMsg, int spaceChar, uint16_t couleur[], bool y = false, int posY = 0);
    //void printInline(std::vector<MsgPanel> tabMsg, int spaceMsg, bool y = false, int posY = 0);
    MsgColor rgbToInt(GRB grbTxt, GRB grbBackgnd = {0,0,0});
    uint16_t intRgbToInt(uint8_t green, uint8_t red, uint8_t blue){return Panel::matrix->Color(green, red, blue);}

    std::vector<int > parametreImg(char **img);
    int parametreImgWidth(fs::File& pathImg);

    std::map<std::string, std::string> getMapColor(char **img);
    std::map<int, std::string> getMapPixel(char **img);
    void drawImg(char **img, std::map<std::string, std::string>& mapCouleur, int posX=0, int posY=0);
    void creerImg();

    void drawImg(fs::File& pathImg, int posX=0, int posY=0);
//void drawImg(std::string pathImg/* Preciser l'ecran tft, neoMatrix, ... */, int posX=0, int posY=0);ORIGINAL

};

#endif // PANEL_H_INCLUDE