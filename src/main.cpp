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
    TGAImage image(900, 900, TGAImage::RGB);

    std::ifstream fichier("../obj/african_head.obj");
    if (fichier) {
        char delimiter = ' ';
        std::string ligne;
        std::vector<std::string> vertices;
        std::vector<std::string> edges;


        while (getline(fichier, ligne)) {
            std::vector<std::string> listLignes = split(ligne, delimiter);
            if (listLignes.size() == 4) {
                if (listLignes[0] == "v") {
                    vertices.push_back(listLignes[1] + ' ' + listLignes[2]);
                }
                if (listLignes[0] == "f") {
                    char limiter = '/';
                    std::string coordonees;
                    for (int i = 1; i < 4; i++) {
                        std::vector<std::string> temp = split(listLignes[i], limiter);
                        coordonees += temp[0] + ' ';
                    }
                    edges.push_back(coordonees);
                }
            }

        }
        fichier.close();

        /*for (int i = 0; i < vertices.size(); ++i) { //Pour déssiner les points
            std::vector<std::string> listTableau = split(vertices[i], delimiter);
            float x = (atof(listTableau[0].c_str())+1.)*width/2.;
            float y = (atof(listTableau[1].c_str())+1.)*height/2.;
            image.set(x,y,white);
        }*/

        for (int i = 0; i < edges.size(); ++i) {

            std::vector<std::string> listArc = split(edges[i], delimiter);

            int point1 = (atoi(listArc[0].c_str()) - 1.);
            int point2 = (atoi(listArc[1].c_str()) - 1.);
            int point3 = (atoi(listArc[2].c_str()) - 1.);

            std::vector<std::string> coordonnees1 = split(vertices[point1], delimiter);
            float x1 = (atof(coordonnees1[0].c_str()) + 1.) * width / 2.;
            float y1 = (atof(coordonnees1[1].c_str()) + 1.) * height / 2.;

            std::vector<std::string> coordonnees2 = split(vertices[point2], delimiter);
            float x2 = (atof(coordonnees2[0].c_str())+1.)*width/2.;
            float y2 = (atof(coordonnees2[1].c_str())+1.)*height/2.;

            std::vector<std::string> coordonnees3 = split(vertices[point3], delimiter);
            float x3 = (atof(coordonnees3[0].c_str())+1.)*width/2.;
            float y3 = (atof(coordonnees3[1].c_str())+1.)*height/2.;

            line(x1, y1, x2, y2, image, white);
            line(x1, y1, x3, y3, image, white);
            line(x2, y2, x3, y3, image, white);
        }
    }
    else{
        std::cerr << "Erreur" << std::endl;
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;

}