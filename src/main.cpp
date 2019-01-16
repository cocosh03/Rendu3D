#include "tgaimage.h"
#include "geometry.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor random = TGAColor(rand()%255, rand()%255, rand()%255, 255);

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

void triangle(Vecteur2D  v1, Vecteur2D v2, Vecteur2D v3, TGAImage &image, TGAColor color){
    color = random;
    line(v1.x, v1.y, v2.x, v2.y, image, color);
    line(v2.x, v2.y, v3.x, v3.y, image, color);
    line(v1.x, v1.y, v3.x, v3.y, image, color);

    /*if (v1.y==v2.y && v1.y==v3.y) return; // i dont care about degenerate triangles
    if (v1.y>v2.y) std::swap(v1, v2);
    if (v1.y>v3.y) std::swap(v1, v3);
    if (v2.y>v3.y) std::swap(v2, v3);
    int total_height = v3.y-v1.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>v2.y-v1.y || v2.y==v1.y;
        int segment_height = second_half ? v3.y-v2.y : v2.y-v1.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? v2.y-v1.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vecteur2D A =               v1 + (v3-v1)*alpha;
        Vecteur2D B = second_half ? v2 + (v3-v2)*beta : v1 + (v2-v1)*beta;
        if (A.x>B.x) std::swap(A, B);
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, v1.y+i, color); // attention, due to int casts t0.y+i != A.y
        }
    }*/
}

int main(int argc, char** argv) {

    int width = 800;
    int height = 800;
    TGAImage image(width, height, TGAImage::RGB);

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

        //Pour dessiner les points
        /*for (int i = 0; i < vertices.size(); ++i) {
            std::vector<std::string> listTableau = split(vertices[i], delimiter);
            float x = (atof(listTableau[0].c_str())+1.)*width/2.;
            float y = (atof(listTableau[1].c_str())+1.)*height/2.;
            image.set(x,y,white);
        }*/

        for (int i = 0; i < edges.size(); ++i) {

            std::vector<std::string> listArc = split(edges[i], delimiter);

            Vecteur2D v1{}, v2{}, v3{};

            int point1 = (atoi(listArc[0].c_str()) - 1.);
            int point2 = (atoi(listArc[1].c_str()) - 1.);
            int point3 = (atoi(listArc[2].c_str()) - 1.);

            std::vector<std::string> coordonnees1 = split(vertices[point1], delimiter);
            v1.x = (atof(coordonnees1[0].c_str()) + 1.) * width / 2.;
            v1.y = (atof(coordonnees1[1].c_str()) + 1.) * height / 2.;

            std::vector<std::string> coordonnees2 = split(vertices[point2], delimiter);
            v2.x = (atof(coordonnees2[0].c_str())+1.)*width/2.;
            v2.y = (atof(coordonnees2[1].c_str())+1.)*height/2.;

            std::vector<std::string> coordonnees3 = split(vertices[point3], delimiter);
            v3.x = (atof(coordonnees3[0].c_str())+1.)*width/2.;
            v3.y = (atof(coordonnees3[1].c_str())+1.)*height/2.;

            triangle(v1, v2, v3, image,white);

            /*Vec2i t0[3] = {Vec2i(x1,y1),   Vec2i(x2,y2),  Vec2i(x3,y3)};
            triangle(t0[0], t0[1], t0[2], image, red);*/
        }
    }
    else{
        std::cerr << "Erreur" << std::endl;
    }

/*  Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    triangle(t0[0], t0[1], t0[2], image, red);
    triangle(t1[0], t1[1], t1[2], image, white);
    triangle(t2[0], t2[1], t2[2], image, green);*/

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;

}