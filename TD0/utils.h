#ifndef __H_UTILS
#define __H_UTILS

#include <g3x.h>

/**
 * Renvoie le minimum entre a et b.
 */
int min(int a, int b) {
    return a < b ? a : b;
}

/**
 * Dessine un side et une normale aux coordonnées fournies.<br/>
 * La normale sert notamment à calculer les réflexions de lumières.
 */
void NormalVertex3dv(G3Xcoord coord) {
    g3x_Normal3dv(coord);
    g3x_Vertex3dv(coord);
}

#endif