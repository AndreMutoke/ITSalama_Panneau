
//=============================================================================================================
//==============================NEW_METHOD_BY_GODWIN_AND_ANDRE============================================
//=============================================================================================================

#include "Panel.hpp"


std::string separator2 = "c #";
//char separator[] = {' '};
char chiffres[] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9'
};

    Panel::Panel(int width, // La largeur de l'ecran, ou le nombre des leds en Largeur
                int height, // La hauteur de l'ecran, ou le nombre des Leds en Hauteur
                uint8_t pin, // La broche de sortie des donnees
                uint8_t matrixType, // Le type de la matrice
                neoPixelType ledType)
    {
        Panel::m_largeur = width; Panel::m_hauteur = height; Panel::m_pin = pin; 
        Panel::matrix = new Adafruit_NeoMatrix(Panel::m_largeur, 
                                                Panel::m_hauteur, 
                                                Panel::m_pin, 
                                                matrixType, ledType);
    }

    Panel::~Panel()
    {
        delete matrix;
    }
    void Panel::centerXY(MsgPanel& message, bool x, bool y)
    {
        if(message.getWidth() <= Panel::m_largeur)
        {
            message.setScroll(false);
            if(x && y)
            {
                message.setCoordX( (int)(Panel::m_largeur - message.getWidth())/2);
                message.setCoordY((int)(Panel::m_hauteur - message.getSize() * 7)/2);
            }
            else
            {
                if(y){
                   message.setCoordY((int)(Panel::m_hauteur - message.getSize() * 7)/2);
                }
                else if(x){
                    message.setCoordX( (int)(Panel::m_largeur - message.getWidth())/2);
                }
            }
        }
        else
        {
            static int defilement;
            if(!message.getScrollStatus())
            {
                defilement = Panel::m_largeur;
                message.setScroll(true);
            } 
            message.setCoordX(defilement);
            if(y) 
            {
                message.setCoordY((int)(Panel::m_hauteur - message.getSize() * 7)/2);
            }
            if(defilement-- < -message.getWidth())
            {
                defilement = Panel::m_largeur;
                message.setScroll(false);
            }
        }
        
    }
// IMG Handler on Panel
int Panel::power(int a, int b){
    int r=1;
    for(int i=1; i<=b; ++i){
        r *= a;
    }
    return r;
}

int Panel::whichChiffre(char c){
    int n=-1;
    for(int p =0; p< 10; p++){
        if(chiffres[p] == c){ n = p;}
    }
    return n;
}
bool Panel::isCHiffre(char c){
    bool val = false;
    for(auto i=0; i< ::strlen(chiffres); ++i){ if(c == chiffres[i]) {val = true;}}
    return val;
}
int Panel::convertCharToInt(std::string& str){
    int nbre = 0;
    for(int p=0; p<str.length(); p++)
    {
        nbre += power(10, (str.length()-1-p)) * whichChiffre(str[p]);
    }
    return nbre;
}
std::vector<std::string> Panel::initString(int w, int h){
    std::vector<std::string> str;
    std::string strInit;
    for(int i=0; i<w; i++){
        strInit.push_back(' ');
    }
    for(int i=0; i<h; i++){
        str.push_back(strInit);
    }
    return str;
}




int Panel::findSeparator(std::string& str){
    int pos;
    for(int i=0; i<str.length(); ++i){
        std::string temp;
        if(str[i] == 'c'){
            for(int j=i; j < i+3; ++j){
                temp.push_back(str[j]);
            }
            if(temp == separator2){
                pos = i;
            }

        }
    }
    return pos;
}

std::string Panel::getFirst(std::string& str){
    std::string first;
    for(int i=0; i< findSeparator(str); i++){
        first.push_back(str[i]);
    }
    return first;
}

std::string Panel::getSecond(std::string&str){
    std::string second;
    for(int i=findSeparator(str) +2; i<str.length(); i ++)
    {
        second.push_back(str[i]);
    }
    return second;
}


std::vector<int > Panel::parametreImg(char **img){
    std::vector<int > temp;
    char *tab = img[0];
    std::string width; std::string height; std::string color; std::string NbVal;

    int it=0;
    for(it; tab[it] != ' '; it++) width.push_back(tab[it]);
    it++;
    for(it; tab[it] != ' '; it++) height.push_back(tab[it]);
    it++;
    for(it; tab[it] != ' '; it++) color.push_back(tab[it]);
    it++;
    for(it; tab[it] != ' '; it++) NbVal.push_back(tab[it]);
    temp.push_back(convertCharToInt(width)); /// 0
    temp.push_back(convertCharToInt(height));/// 1
    temp.push_back(convertCharToInt(color));///  2
    temp.push_back(convertCharToInt(NbVal));///  3
    return temp;
}



std::map<std::string, std::string> Panel::getMapColor(char **img)
{
    std::map<std::string , std::string> mapCouleur;
    std::vector<int > parametre = parametreImg(img);
    for(int k=1; k<parametre[2]+1; k++)
    {
        std::string col = img[k];
        //cout<<"La colonne est : "<<col<<endl;

        mapCouleur.insert(make_pair(getFirst(col), getSecond(col)));
    }
    return mapCouleur;
}
std::map<int, std::string> Panel::getMapPixel(char **img)
{
    std::map<int , std::string> mapPixel;
    std::vector<int > parametre = parametreImg(img);
    for(int k=parametre[2]+1; k<parametre[2]+parametre[1]+1; k++)
    {
        int pos = k - (parametre[2]+1);
        std::string col = img[k];
        //cout<<"La colonne est : "<<col<<endl;

        mapPixel.insert(std::pair<int, std::string>(pos, col));
    }
    return mapPixel;
}
void Panel::drawImg(char **img, std::map<std::string, std::string>& mapCouleur, int posX, int posY){
  std::vector<int > parametre = parametreImg(img);
  /// std::map<std::string, std::string> mapCouleur = getMapColor(donBosco);
  std::map<int, std::string>mapPixel = getMapPixel(img);
  std::map<int, std::string>::iterator  it2;

  for(int k=0; k<parametre[1]; ++k){

    for(it2 = mapPixel.begin(); it2!=mapPixel.end();++it2 ){
      if(it2->first == k)
        {

          int w = parametre[0]* parametre[3];
          for(int l=0; l< w; ++l){

            std::string subStr = it2->second.substr(l, parametre[3]);
            std::map<std::string, std::string>::iterator it;
            for(it = mapCouleur.begin(); it!=mapCouleur.end(); ++it){
              if(it->first.substr(0, parametre[3]) == subStr){
                const char* colorString = it->second.c_str();
                char* ptr;
                long colorValue = strtol(colorString + 1, &ptr, 16);
            
            // Définir la couleur de l'objet
                int8_t green = (colorValue >> 16) & 0xFF;
                uint8_t red = (colorValue >> 8) & 0xFF;
                uint8_t blue = colorValue & 0xFF;
                int x = posX + (int) l/2;
                int y = posY + k;
                //Serial.println("On veut afficher");
                this->matrix->drawPixel(x, y, this->matrix->Color(red, green, blue)); //.drawPixel(x, y, tft.color565(red, green, blue)); delay(1);
                this->Show();
              }
            }


          }
          
        }
    }


  }

  
}

std::vector<int > Panel::getFisrtAndLastDelimitator(std::string pathImgXPM)
{
    std::vector<int > buff;

    int initPos =0, finalPos = 0;
    int round=-1;

    for(int i=0; i< (int) pathImgXPM.length(); ++i)
    {
        if(round == -1)
        {
            if(pathImgXPM[i] == '"'){
                initPos = i; round++;
            }
        }
    }
    int round2=-1;
    for(int i= (int) pathImgXPM.length() - 1; i>=0; --i)
    {
        if(round2 == -1)
        {
            if(pathImgXPM[i] == '"'){
                finalPos = i; round2++;
            }
        }
    }
    if((round2 == 0) && (round == 0)){
        buff.push_back(initPos); buff.push_back(finalPos);
        return buff;
    }
    buff.push_back(-1); buff.push_back(-1);

    return buff;

}
//std::vector<std::string > Panel::getImgStrings(std::string& pathImg)
std::vector<std::string > Panel::getImgStrings(fs::File& Fichier)
{
    
    std::vector<std::string > buffer;
   // std::string path = pathImg; 
    //const char* path = pathImg.c_str();
    //fs::File Fichier = SPIFFS.open(path);
    
    //if(Fichier.available()){*/

        while(Fichier.available())
        {
            std::string strLigne = Fichier.readStringUntil('\n').c_str(); //int tours=0;
                std::vector<int > initAndFinal = getFisrtAndLastDelimitator(strLigne);
                if((initAndFinal[0] != -1) || (initAndFinal[1] != -1))
                {
                    //std::cout<< initAndFinal[0]<< " ::: "<< initAndFinal[1]<<std::endl;
                    int dim = initAndFinal[1] - initAndFinal[0] - 1; int debut = initAndFinal[0] + 1;
                    buffer.push_back(strLigne.substr(debut, dim));

                }
           

        }
            Fichier.close();
            return buffer;
       
    //}
    //else
    //{
        //std::cout<<"Error to read file"<<std::endl;
    //}

}

void Panel::creerImg(){
    const int width = 100;
 const int height = 100;
 const char * bacGroundColor = "#FFFFFF";
 const char * fillCOlor = "#000000";
 const int centreX = 50;
 const int centreY = 50;
 const int radius = 40;

// Allocation pour la matrice Pixel

    char **pixels = new char*[height];
    for(int y=0; y<height; ++y){
        pixels[y] = new char[width];
        std::memset(pixels[y], ' ', width);
    }

    // Dessiner le cercle
    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            int dx = i - centreX;
            int dy = j - centreY;

            double distance = std::sqrt(dx*dx + dy*dy);
            if(distance <= radius){
                pixels[i][j] = 'b';
            }else{
                pixels[i][j] = 'w';
            }
        }
    }

    // Ecrire cela dans un fichier xpm
    /*std::ofstream outfile("circle.xpm");*/
    //outfile<< "/* XPM */"<<std::endl;
    /*outfile<<"static char *circle = {"<<std::endl;

    outfile<<"\""<<width<<" "<<height<<" "<<"2 1\","<<std::endl;
    outfile<<"\""<<bacGroundColor[0]<< " c " <<bacGroundColor<<"\", "<<std::endl;
    outfile<<"\""<<fillCOlor[0]<< " c " <<fillCOlor<<"\", "<<std::endl;

    for(int y=0; y<height; ++y){
            outfile<<"\"";
        for(int x=0; x<width; ++x){
            outfile<<pixels[y][x];
        }
        outfile<<"\","<<std::endl;
    }

    outfile<<"};"<<std::endl;
    outfile.close();
;;
    for(int i=0; i<height;++i){
        delete pixels[i];
    }
    delete pixels;

    //std::cout<<"Le fichier circle.xpm a ete creer avec succes"<<std::endl;*/

}

//void Panel::drawImg(std::string pathImg, int posX, int posY) //ORIGINAL
int Panel::parametreImgWidth(fs::File& pathImg){
    //std::vector<std::string > images = getImgStrings(pathImg);
    //std::vector<int > parametre = parametreImg(img);

    //std::vector<int > parametre;
    //std::string tab = imag[0];
    //std::string width;// std::string height; std::string color; std::string NbVal;

    //int it=0;
    //for(it; images[0][it] != ' '; it++){ width.push_back(images[0][it]);}
    
    

    return 0;//convertCharToInt(width);

}

void Panel::drawImg(fs::File& pathImg, int posX, int posY)
{
    std::vector<std::string > img = getImgStrings(pathImg);
    //std::vector<int > parametre = parametreImg(img);

    std::vector<int > parametre;
    std::string tab = img[0];
    std::string width; std::string height; std::string color; std::string NbVal;

    int it=0;
    for(it; tab[it] != ' '; it++){ width.push_back(tab[it]);}
    it++;
    for(it; tab[it] != ' '; it++){ height.push_back(tab[it]);}
    it++;
    for(it; tab[it] != ' '; it++){ color.push_back(tab[it]);}
    it++;
    for(it; tab[it] != ' '; it++){ NbVal.push_back(tab[it]);}
    parametre.push_back(convertCharToInt(width)); /// 0
    parametre.push_back(convertCharToInt(height));/// 1
    parametre.push_back(convertCharToInt(color));///  2
    parametre.push_back(convertCharToInt(NbVal));///  3


// std::map<std::string, std::string> mapCouleur = getMapColor(donBosco);

    std::map<std::string , std::string> mapCouleur;
    //std::vector<int > parametre = parametreImg(img);
    for(int k=1; k<parametre[2]+1; k++)
    {
        std::string col = img[k].c_str();
        //cout<<"La colonne est : "<<col<<endl;

        mapCouleur.insert(make_pair(getFirst(col), getSecond(col)));
    }
//std::map<int, std::string>mapPixel = getMapPixel(img);

    std::map<int , std::string> mapPixel;
    //std::vector<int > parametre = parametreImg(img);
    for(int k=parametre[2]+1; k<parametre[2]+parametre[1]+1; k++)
    {
        int pos = k - (parametre[2]+1);
        std::string col = img[k];
        //std::cout<<"La colonne est : "<<col<<std::endl;

        mapPixel.insert(std::pair<int, std::string>(pos, col));
    }
std::map<int, std::string>::iterator  it2;

    for(int k=0; k<parametre[1]; ++k){

        for(it2 = mapPixel.begin(); it2!=mapPixel.end();++it2 ){
            if(it2->first == k)
            {
                int w = parametre[0]* parametre[3];
                for(int l=0; l< w; ++l){

                    std::string subStr = it2->second.substr(l, parametre[3]);
                    std::map<std::string, std::string>::iterator it;
                    for(it = mapCouleur.begin(); it!=mapCouleur.end(); ++it){
                        if(it->first.substr(0, parametre[3]) == subStr){
                            const char* colorString = it->second.c_str();
                            char* ptr;
                            long colorValue = strtol(colorString + 1, &ptr, 16);

                        // D�finir la couleur de l'objet
                            int8_t green = (colorValue >> 16) & 0xFF;
                            uint8_t red = (colorValue >> 8) & 0xFF;
                            uint8_t blue = colorValue & 0xFF;
                            int x = posX + (int) l/2;
                            int y = posY + k;
                            this->matrix->drawPixel(x, y, this->matrix->Color(red, green, blue)); //.drawPixel(x, y, tft.color565(red, green, blue)); delay(1);
                        }
                    }
                }
            }
        }
    }
    this->Show();
}

    void Panel::centerXY(std::vector<MsgPanel> tabMsg, bool x, bool y)
    {
        for(int indice=0; indice <tabMsg.size(); indice++)
        {
            Panel::centerXY(tabMsg[indice], x, y);
        }
    }
    void Panel::centerXY(MsgPanel tabMsg[], int sizeTab, bool x, bool y)
    {
        for(int indice=0; indice < sizeTab; indice++)
        {
            Panel::centerXY(tabMsg[indice], x, y);
        }
    }
    void Panel::setXY(MsgPanel& message, int x, int y)
    {
        Coord position = {x,y};
        message.setCoord(position);
    }

    //PERMET D'INITALISER LA POSITION DES MESSAGES SUR PLUSIEURS LIGNES DISTINCTES DE L'ECRAN 
    void Panel::multiPrint(std::vector<MsgPanel> tabMsg, uint posX)
    {
        //INITIALISATION DES INTERLIGNES ENTRE MESSAGES 
        int heigthInterval, heigthTabMsg = 0;
        for(int i = 0; i < tabMsg.size(); i++){
            heigthTabMsg += 7 * tabMsg[i].getSize();
        } 
        heigthInterval = (Panel::m_hauteur -heigthTabMsg) / (tabMsg.size() + 1); //CALCUL DE L'INTERVAL EN Y 
        tabMsg[0].setCoordY(heigthInterval);
        for(int indice=1; indice < tabMsg.size(); indice++)
        {
            //int intervalMsgY = tabMsg[indice-1].getCoord().yPos + 7  + 7 * tabMsg[0].getSize();
            //origineY + hauteurChar(=7) * tailleChar + espaceOuInterligne(=7)
            tabMsg[indice].setCoordY(tabMsg[indice-1].getCoord().yPos + 7 * tabMsg[0].getSize() + heigthInterval);
        }
        if(posX == CENTER)
        {
            for(int indice=0; indice<tabMsg.size(); indice++)
            {
                Panel::centerXY(tabMsg[indice], true, false);
            }
        }
        if(posX == LEFT)
        {
            for(int indice=0; indice<tabMsg.size(); indice++)
            {
                tabMsg[indice].setCoordX(0);
            }
        }
        if(posX == RIGHT)
        {
            for(int indice=0; indice<tabMsg.size(); indice++)
            {
                tabMsg[indice].setCoordX(Panel::m_largeur - tabMsg[indice].getWidth());
            }
        }
        for(int indice=0; indice < tabMsg.size(); indice++)
        {
            setPrintMsg(tabMsg[indice]);
        }
        
    }
    
    void Panel::multiPrint(MsgPanel tabMsg[],int sizeTab, uint posX)
    {
        int heigthInterval, heigthTabMsg = 0;
        for(int i = 0; i < sizeTab; i++){
            heigthTabMsg += 7 * tabMsg[i].getSize();
        } 
        heigthInterval = (Panel::m_hauteur -heigthTabMsg) / (sizeTab + 1); //CALCUL DE L'INTERVAL EN Y 
       
        tabMsg[0].setCoordY(heigthInterval);
        for(int indice=1; indice < sizeTab; indice++)
        {
            //int intervalMsgY = tabMsg[indice-1].getCoord().yPos + 7  + 7 * tabMsg[0].getSize();
            tabMsg[indice].setCoordY(tabMsg[indice-1].getCoord().yPos + 7  + 7 * tabMsg[0].getSize());
        }
        if(posX == CENTER)
        {
            for(int indice=0; indice < sizeTab; indice++)
            {
                Panel::centerXY(tabMsg[indice], true, false);
            }
        }
        if(posX == LEFT)
        {
            for(int indice=0; indice < sizeTab; indice++)
            {
                tabMsg[indice].setCoordX(0);
            }
        }
        if(posX == RIGHT)
        {
            for(int indice=0; indice < sizeTab; indice++)
            {
                tabMsg[indice].setCoordX(Panel::m_largeur - tabMsg[indice].getWidth());
            }
        }

        for(int indice=0; indice < sizeTab; indice++)
        {
            setPrintMsg(tabMsg[indice]);
        }
        
    }
    void Panel::setPrintMsg(MsgPanel& message)
    {
        Panel::setTextColor(message.getColor().txtColor,message.getColor().bkgndColor);
        Panel::setTextSize(message.getSize());
        Panel::setCursor(message.getCoord().xPos,message.getCoord().yPos);
        Panel::printMsg(message);
    }
    void Panel::printInline(const char tabChar[], int sizeMsg, int spaceChar, uint16_t couleur, bool y , int posY){
        uint nbChar = strlen(tabChar);
        uint widthMsg = nbChar * 5 * sizeMsg + spaceChar * (nbChar - 1);
        int x0;
        int y0;
        /*for(int i=0; i < nbChar; i++){
            widthMsg += tabChar[i].getNbChar() * 5 * sizeMsg;
        }*/
        if(widthMsg <= Panel::m_largeur)
            x0 = (Panel::m_largeur - widthMsg) / 2;// posX de depart a part de laquelle on imprimera sur l'ecran
        else   
            x0 = 0; 
    //L'ideal serait de faire en sorte que cela puisse defiler
    //MAIS nous avons principalement creer cette foonction pour eviter les debordement du message sur l'ecran
        if(y)
            y0 = (Panel::m_hauteur - 7 * sizeMsg) / 2;
        else
            y0 = posY;
        //Panel::setTextColor(couleur);
        //Panel::setTextSize(sizeMsg);
        //Panel::setCursor(x0, y0);
        for(int i=0; i < nbChar; i++){
            Panel::matrix->drawChar(x0, y0, tabChar[i],
                            couleur, 0, sizeMsg);
           //Panel::printMsg(tabChar[i]);
           x0 +=  5 * sizeMsg + spaceChar;
        }
    }
    void Panel::printInline(const char tabChar[], int sizeMsg, int spaceChar, uint16_t couleur[], bool y , int posY){
        uint nbChar = strlen(tabChar);
        uint widthMsg = nbChar * 5 * sizeMsg + spaceChar * (nbChar - 1);//uint widthMsg = nbChar * (5 * sizeMsg + spaceChar - 1);
        int x0;
        int y0;
        /*for(int i=0; i < nbChar; i++){
            widthMsg += tabChar[i].getNbChar() * 5 * sizeMsg;
        }*/
        if(widthMsg <= Panel::m_largeur)
            x0 = (Panel::m_largeur - widthMsg) / 2;// posX de depart a part de laquelle on imprimera sur l'ecran
        else   
            x0 = 0; 
    //L'ideal serait de faire en sorte que cela puisse defiler
    //MAIS nous avons principalement creer cette foonction pour eviter les debordement du message sur l'ecran
        if(y)
            y0 = (Panel::m_hauteur - 7 * sizeMsg) / 2;
        else
            y0 = posY;
        for(int i=0; i < nbChar; i++){
            Panel::matrix->drawChar(x0, y0, tabChar[i], couleur[i], 0, sizeMsg);
            x0 +=  5 * sizeMsg + spaceChar;
        }
    }
    /*void Panel::printInline(std::vector<MsgPanel> tabMsg, int spaceMsg, bool y , int posY){

        uint widthMsg = nbChar * (5 * sizeMsg + spaceMsg - 1);
        int x0;
        int y0;
        tabMsg[0].pushback(addElement);
        if(widthMsg <= Panel::m_largeur)
            x0 = (Panel::m_largeur - widthMsg) / 2;// posX de depart a part de laquelle on imprimera sur l'ecran
        else   
            x0 = 0; 
    //L'ideal serait de faire en sorte que cela puisse defiler
    //MAIS nous avons principalement creer cette foonction pour eviter les debordement du message sur l'ecran
        if(y)
            y0 = (Panel::m_hauteur - 7 * sizeMsg) / 2;
        else
            y0 = posY;
        Panel::setTextColor(couleur);
        Panel::setTextSize(sizeMsg);
        Panel::setCursor(x0, y0);
        for(int i=0; i < nbChar; i++){
           Panel::printMsg(tabChar[i]);
           x0 +=  5 * sizeMsg + spaceChar;
        }
    }*/
    MsgColor Panel::rgbToInt(GRB grbTxt, GRB grbBackgnd)
    {
        MsgColor colorMessage;
        colorMessage.txtColor = Panel::matrix->Color(grbTxt.g,grbTxt.r,grbTxt.b);
        colorMessage.bkgndColor = Panel::matrix->Color(grbBackgnd.g, grbBackgnd.r, grbBackgnd.b);
        return colorMessage;

    }
    void  Panel::fillPanel(uint8_t allPanel)
    {
        matrix->fillScreen(allPanel);
    }
    void Panel::setTextColor(uint16_t txtcolor, uint16_t backgnd)
    {
        Panel::matrix->setTextColor(txtcolor, backgnd);
    }
    void Panel::fillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
    {
        Panel::matrix->fillRect(x, y, width, height, color);
    }
