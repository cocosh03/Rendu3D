#include "tgaimage.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}

std::vector<std::string> split(const std::string &chaine, char delimiteur)
{
    std::vector<std::string> elements;
    std::stringstream ss(chaine);
    std::string sousChaine;
    while (getline(ss, sousChaine, delimiteur))
    {
        elements.push_back(sousChaine);
    }
    return elements;
}



int main(int argc, char** argv) {
    int width = 800;
    int height = 800;
    TGAImage image(width, height, TGAImage::RGB);

    std::ifstream fichier("../obj/african_head.obj");
    if (fichier){
        char delimiter = ' ';
        std::string ligne;
        std::vector<std::string> tableau;

        while (getline(fichier,ligne)){
            std::vector<std::string> listLignes = split(ligne, delimiter);
            if (listLignes.size() == 4){
                if (listLignes[0] == "v"){
                    tableau.push_back(listLignes[1]+ ' ' + listLignes[2]);
                }
            }

        }
        fichier.close();
        //image.set(float x0 = (v0.x+1.)*width/2.), flaot y0 = (v0.y+1.)*height/2.;
        for (int i = 0; i < tableau.size(); ++i) {
            std::vector<std::string> listTableau = split(tableau[i], delimiter);
            float x = (atof(listTableau[0].c_str())+1.)*width/2.;
            float y = (atof(listTableau[1].c_str())+1.)*height/2.;
            image.set(x,y,white);
        }
    }
    else{
        std::cerr << "Erreur" << std::endl;
    }


    /*std::vector<std::string> listElements;
    std::vector<std::string> elements;
    std::vector<std::string> vect = read('obj/african_head.obj', listElements());*/

    /*for (int i = 0; i < vect.size(); i++) {

            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            line(x0, y0, x1, y1, image, white);

    }

    char delimiter = ' ';*/



    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;

}