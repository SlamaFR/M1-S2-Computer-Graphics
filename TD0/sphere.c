/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Université Gustave Eiffel                                     =*/
/*= Code "squelette" pour prototypage avec libg3x.6c              =*/
/*=================================================================*/

#include <g3x.h>
#include <math.h>

#include "utils.h"

/* tailles de la fenêtre (en pixel) */
static int WWIDTH = 512, WHEIGHT = 512;

#define NBM 100                   // Nombre de méridiens (lignes verticales)
#define NBP 100                   // Nombre de parallèles (lignes horizontales)

static double step = 1.0;         // Pas de rendu
static double R = 1.0;            // Rayon de la sphère.

static G3Xpoint points[NBM][NBP]; // Points représentant notre sphère

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void) {
    // On calcule la taille d'une subdivision de chaque angle (theta et phi)
    double theta = 2 * PI / NBM;
    double phi = PI / (NBP - 1); // -1 ici, car on veut NBP faces et pas NBP lignes

    /* On se reporte à l'équation paramétrique d'une sphère :
     *
     *   / x \            / x = cos(theta) * sin(phi)
     * M | y |  tel que  <  y = sin(theta) * sin(phi)
     *   \ z /            \ z = cos(phi)
     *
     * Il faut pondérer nos angles par l'indice courant.
     */
    for (int i = 0; i < NBM; i++) {
        for (int j = 0; j < NBP; j++) {
            points[i][j].x = R * cos(i * theta) * sin(j * phi);
            points[i][j].y = R * sin(i * theta) * sin(j * phi);
            points[i][j].z = R * cos(j * phi);
        }
    }
}

/* la fonction de contrôle : appelée 1 seule fois, juste après <init> */
static void ctrl(void) {
    g3x_CreateScrollv_d("step", &step, 1, 10, 1, "Pas de rendu");
}

/* la fonction de dessin : appelée en boucle */
static void draw(void) {
    glPointSize(3);
    g3x_Material(G3Xr, .2, .6, .9, 1, 1);

    /* Rendu en nuage de side */

    //glBegin(GL_POINTS);
    //for (int i = 0; i < NBP; i++) {
    //    for (int j = 0; j < NBM; j++) {
    //        g3x_Vertex3dv(side[j][i]);
    //    }
    //}
    //glEnd();

    /* Rendu en facettes carrées */

    //glBegin(GL_QUADS);
    //for (int i = 0; i < NBM; i+=step) {
    //    for (int j = 0; j < NBP - 1; j+=step) {
    //        // Facette (SW -> SE -> NE -> NW)
    //        NormalVertex3dv(side[i][min(j + step, NBP - 1)]);
    //        NormalVertex3dv(side[min(i + step, NBM) % NBM][min(j + step, NBP - 1)]);
    //        NormalVertex3dv(side[min(i + step, NBM) % NBM][j]);
    //        NormalVertex3dv(side[i][j]);
    //    }
    //}
    //glEnd();

    /* Rendu en facettes triangulaires */
    // On utilisera cette méthode de rendu pour les autres solides.

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < NBM; i += step) {
        for (int j = 0; j < NBP - 1; j += step) {
            // Premier triangle (SW -> SE -> NW)
            NormalVertex3dv(points[i][min(j + step, NBP - 1)]);
            NormalVertex3dv(points[min(i + step, NBM) % NBM][min(j + step, NBP - 1)]);
            NormalVertex3dv(points[i][j]);

            // Second triangle (NW -> SE -> NE)
            NormalVertex3dv(points[i][j]);
            NormalVertex3dv(points[min(i + step, NBM) % NBM][min(j + step, NBP - 1)]);
            NormalVertex3dv(points[min(i + step, NBM) % NBM][j]);
        }
    }
    glEnd();

    /*
     * La boucle utilise des modulos au niveau des méridiens, car ceux-ci forment une boucle.
     * Le dernier méridien est en effet relié au premier.
     * Le modulo permet de retomber à 0 lors du dépassement.
     * Le min, quant à lui, permet de fixer une limite afin d'éviter de relier
     * le méridien au second, ou au troisième, etc.
     */
}

/* la fonction d'animation (facultatif) */
static void anim(void) {
}

/* la fonction de sortie  (facultatif) -- atexit() */
static void quit(void) {
}

/***************************************************************************/
/* La fonction principale : NE CHANGE JAMAIS ou presque                    */
/***************************************************************************/
int main(int argc, char **argv) {
    /* creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv, WWIDTH, WHEIGHT);

    g3x_SetInitFunction(init); /* fonction d'initialisation */
    g3x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
    g3x_SetDrawFunction(draw); /* fonction de dessin        */
    g3x_SetAnimFunction(anim); /* fonction d'animation      */
    g3x_SetExitFunction(quit); /* fonction de sortie        */

    /* lancement de la boucle principale */
    return g3x_MainStart();
    /* RIEN APRES CA */
}
