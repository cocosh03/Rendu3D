#include "tgaimage.h"
#include "geometry.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>


const TGAColor white = TGAColor(255, 255, 255, 255);
//const TGAColor rouge = TGAColor(255, 0, 0, 255);
//const TGAColor random = TGAColor(rand()%255, rand()%255, rand()%255, 255);

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

std::vector<std::string> split(const std::string &chaine, char delimiteur){
    std::vector<std::string> elements;
    std::stringstream ss(chaine);
    std::string sousChaine;
    while (getline(ss, sousChaine, delimiteur))
    {
        elements.push_back(sousChaine);
    }
    return elements;
}

void pixel(int x, int y, TGAImage &image, TGAColor color){ //colorier un seul pixel
    image.set(x, y, color);
}

float cross_product2D(Vecteur2D v1, Vecteur2D v2){
    float p = v1.x * v2.y - v2.x * v1.y;
    return p;
}

float dot_product_2D(Vecteur2D v1, Vecteur2D v2){
    float p = v1.x * v2.x + v1.y * v2.y;
    return p;
}

float dot_product_3D(Vecteur3D v1, Vecteur3D v2){
    float p = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return p;
}

void rasterisation(Vecteur2D v1, Vecteur2D v2, Vecteur2D v3, TGAImage &image, TGAColor color){
    int maxX = (int) std::max(v1.x, std::max(v2.x, v3.x));
    int minX = (int) std::min(v1.x, std::min(v2.x, v3.x));
    int maxY = (int) std::max(v1.y, std::max(v2.y, v3.y));
    int minY = (int) std::min(v1.y, std::min(v2.y, v3.y));

    Vecteur2D vs1{v2.x - v1.x, v2.y - v1.y};
    Vecteur2D vs2{v3.x - v1.x, v3.y - v1.y};
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            Vecteur2D q{x - v1.x, y - v1.y};

            float s = cross_product2D(q, vs2) / cross_product2D(vs1, vs2);
            float t = cross_product2D(vs1, q) / cross_product2D(vs1, vs2);

            if ((s >= 0) && (t >= 0) && (s + t <= 1)) { /* inside triangle */
                pixel(x, y, image, color);
            }
        }
    }
}

void triangle(Vecteur2D  v1, Vecteur2D v2, Vecteur2D v3, TGAImage &image, TGAColor color) {
    if (v1.y > v2.y) std::swap(v1, v2);
    if (v1.y > v3.y) std::swap(v1, v3);
    if (v2.y > v3.y) std::swap(v2, v3);
    //v3.y > v2.y > v1.y
    rasterisation(v1, v2, v3, image, color);
}

int main(int argc, char** argv) {

    TGAColor random = TGAColor(rand()%255, rand()%255, rand()%255, 255);
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
                    vertices.push_back(listLignes[1] + ' ' + listLignes[2] + ' ' + listLignes[3]);
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

        for (int i = 0; i < edges.size(); ++i) {

            std::vector<std::string> listArc = split(edges[i], delimiter);

            Vecteur2D v1{}, v2{}, v3{};
            Vecteur3D v3D1{}, v3D2{}, v3D3{};

            int point1 = (atoi(listArc[0].c_str()) - 1.);
            int point2 = (atoi(listArc[1].c_str()) - 1.);
            int point3 = (atoi(listArc[2].c_str()) - 1.);

            std::vector<std::string> coordonnees1 = split(vertices[point1], delimiter);
            v1.x = (atof(coordonnees1[0].c_str()) + 1.) * width / 2.;
            v1.y = (atof(coordonnees1[1].c_str()) + 1.) * height / 2.;

            std::vector<std::string> coordonnees2 = split(vertices[point2], delimiter);
            v2.x = (atof(coordonnees2[0].c_str()) + 1.) * width / 2.;
            v2.y = (atof(coordonnees2[1].c_str()) + 1.) * height / 2.;

            std::vector<std::string> coordonnees3 = split(vertices[point3], delimiter);
            v3.x = (atof(coordonnees3[0].c_str()) + 1.) * width / 2.;
            v3.y = (atof(coordonnees3[1].c_str()) + 1.) * height / 2.;
            random = TGAColor(rand()%255, rand()%255, rand()%255, 255);

            int point3D1 = (atoi(listArc[0].c_str()) -1.); //x
            int point3D2 = (atoi(listArc[1].c_str()) -1.); //y
            int point3D3 = (atoi(listArc[2].c_str()) -1.); //z

            std::vector<std::string> coordonnees3D1 = split(vertices[point3D1], delimiter);
            v3D1.x = atof(coordonnees3D1[0].c_str());
            v3D1.y = atof(coordonnees3D1[1].c_str());
            v3D1.z = atof(coordonnees3D1[2].c_str());

            std::vector<std::string> coordonnees3D2 = split(vertices[point3D2], delimiter);
            v3D2.x = atof(coordonnees3D2[0].c_str());
            v3D2.y = atof(coordonnees3D2[1].c_str());
            v3D2.z = atof(coordonnees3D2[2].c_str());

            std::vector<std::string> coordonnees3D3 = split(vertices[point3D3], delimiter);
<<<<<<< HEAD
            v3D3.x = atof(coordonnees3D3[0].c_str());
            v3D3.y = atof(coordonnees3D3[1].c_str());
            v3D3.z = atof(coordonnees3D3[2].c_str());
=======
            v3D3.x = atof(coordonnees3D1[0].c_str());
            v3D3.y = atof(coordonnees3D1[1].c_str());
            v3D3.z = atof(coordonnees3D1[2].c_str());

            /*Vecteur3D v1v2 = {v3D2.x - v3D1.x, v3D2.y - v3D1.y, v3D2.z - v3D1.z};
            Vecteur3D v1v3 = {v3D3.x - v3D1.x, v3D3.y - v3D1.y, v3D3.z - v3D1.z};*/

            Vecteur3DF n = {(v3D2.y - v3D1.y) * (v3D3.z - v3D1.z) - (v3D2.z - v3D1.z) * (v3D3.y - v3D1.y),
                           (v3D2.z - v3D1.z) * (v3D3.x - v3D1.x) - (v3D2.x - v3D1.x) * (v3D3.z - v3D1.z),
                           (v3D2.x - v3D1.x) * (v3D3.y - v3D1.y) - (v3D2.y - v3D1.y) * (v3D3.x - v3D1.x)};
            n.normalize();
            Vecteur3DF lumiere{0, 0, -1};
            float intensity = n.x*lumiere.x + n.y*lumiere.y +n.z*lumiere.z;
	    if (intensity > 0){
	      triangle(v1, v2, v3, image, random)TGAColor(intensity*255, intensity*255, intensity*255, 255));
	    }
>>>>>>> 68960057a1e5958a81c842dd77a8c38fcb1a612f

            Vecteur3D v3v1 = {v3D3.x - v3D1.x, v3D3.y - v3D1.y, v3D3.z - v3D1.z};
            Vecteur3D v2v1 = {v3D2.x - v3D1.x, v3D2.y - v3D1.y, v3D2.z - v3D1.z};

            Vecteur3D n = {v3v1.y*v2v1.z-v3v1.z*v2v1.y, v3v1.z*v2v1.x-v3v1.x*v2v1.z, v3v1.x*v2v1.y-v3v1.y*v2v1.x};
            float norm = std::sqrt(n.x*n.x+n.y*n.y+n.z*n.z);

<<<<<<< HEAD
            n = {n.x/norm, n.y/norm, n.z/norm};
            Vecteur3D lumiere{0, 0, -1};
            float intensity = n.x*lumiere.x + n.y*lumiere.y +n.z*lumiere.z;
            if (intensity > 0){
            triangle(v1, v2, v3, image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
            }
=======
            /*for (int i = 0; i < edges.size(); ++i) {
                Vecteur2D coord_ecran[3];
                for (int j = 0; j < 3; ++j) {
                    coord_ecran[j].x = (v1.x+1.)*width/2.;
                    coord_ecran[j].y = (v1.y+1.)*height/2.;
                }
            }*/
>>>>>>> 68960057a1e5958a81c842dd77a8c38fcb1a612f
        }
    }
    else{
        std::cerr << "Erreur" << std::endl;
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
