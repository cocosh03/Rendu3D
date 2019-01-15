//
// Created by Corentin on 15/01/2019.
//

#ifndef RENDU3D_GEOMETRY_H
#define RENDU3D_GEOMETRY_H

#include <cmath>
#include <ostream>

template <class t> struct Vec2 {
    union {
        struct {t u, v;};
        struct {t x, y;};
        t raw[2];
    };
    Vec2() : u(0), v(0) {}
    Vec2(t _u, t _v) : u(_u),v(_v) {}
    inline Vec2<t> operator +(const Vec2<t> &V) const { return Vec2<t>(u+V.u, v+V.v); }
    inline Vec2<t> operator -(const Vec2<t> &V) const { return Vec2<t>(u-V.u, v-V.v); }
    inline Vec2<t> operator *(float f)          const { return Vec2<t>(u*f, v*f); }
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec2<t>& v);
};

typedef Vec2<int>   Vec2i;

template <class t> std::ostream& operator<<(std::ostream& s, Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

#endif //RENDU3D_GEOMETRY_H
