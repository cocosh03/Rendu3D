//
// Created by Corentin on 15/01/2019.
//

#ifndef RENDU3D_GEOMETRY_H
#define RENDU3D_GEOMETRY_H

#include <cmath>
#include <ostream>

struct Vecteur2D{
    float x, y;
};

template <typename T> class Vecteur3D{
public :
    Vecteur3D() : x(0),y(0),z(0){}
    Vecteur3D(T xx , T yy, T zz) : x(xx),y(yy),z(zz){}
    T x,y,z;

    float norm(){
        return std::sqrt(x*x+y*y+z*z);
    }
    Vecteur3D<T> & normalize(T l = 1){
        *this = (*this)*(l/norm());
        return *this;
    }
};

typedef Vecteur3D<float> Vecteur3DF;
#endif //RENDU3D_GEOMETRY_H
