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
#define NBC 50                    // Nombre de cercles sur la base

static double step = 1.0;         // Pas de rendu
static double H = 2.0;            // Hauteur du cone.
static double R = 1.0;            // Rayon de la base.

static G3Xpoint side[NBM][NBP];   // Points représentant le cone en lui-même
static G3Xpoint base[NBM][NBC];   // Points représentant la base.

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void) {
    // On calcule la taille d'une subdivision de l'angle theta, de cone et des cercles de la base.
    double theta = 2 * PI / NBM;
    double t = H / (NBP - 1);
    double r = R / (NBC - 1);

    /* On se reporte à l'équation paramétrique d'un cone :
     *
     *   / x \            / x = (1-t)/2 * cos(theta)
     * M | y |  tel que  <  y = (1-t)/2 * sin(theta)
     *   \ z /            \ z = t
     *
     * (1 - t) / 2 avec t dans [-1; 1[.
     * Dans notre cas, t est dans [0; H[ donc ça devient :
     *     ((H/2) - t) / H
     * <=> ((H/2) - (t - (H/2))) / H
     * <=> ((H/2) - t + (H/2)) / H
     * <=> (H - t) / H
     *
     * La base est également construite grâce à l'équation paramétrique d'un disque :
     *
     *   / x \            / x = r * cos(theta)
     * M | y |  tel que  <  y = r * sin(theta)
     *   \ z /            \ z = -1
     *
     * Il faut pondérer nos angles et t par l'indice courant.
     */
    for (int i = 0; i < NBM; i++) {
        for (int j = 0; j < NBP; j++) {
            side[i][j].x = ((H - (j * t)) / H) * R * cos(i * theta);
            side[i][j].y = ((H - (j * t)) / H) * R * sin(i * theta);
            side[i][j].z = (j * t) - (H / 2.);
        }
        for (int j = 0; j < NBC; j++) {
            base[i][j].x = (j * r) * cos(i * theta);
            base[i][j].y = (j * r) * sin(i * theta);
            base[i][j].z = -H / 2.;
        }
    }
}

/* la fonction de contrôle : appelée 1 seule fois, juste après <init> */
static void ctrl(void) {
    g3x_CreateScrollv_d("step", &step, 1, 10, 1, "e");
}

/* la fonction de dessin : appelée en boucle */
static void draw(void) {
    glPointSize(3);
    g3x_Material(G3Xr, .2, .6, .9, 1, 1);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < NBM; i += step) {
        for (int j = 0; j < NBP; j += step) {
            // Premier triangle (SW -> SE -> NW)
            NormalVertex3dv(side[i][min(j + step, NBP - 1)]);
            NormalVertex3dv(side[min(i + step, NBM) % NBM][min(j + step, NBP - 1)]);
            NormalVertex3dv(side[i][j]);

            // Second triangle (NW -> SE -> NE)
            NormalVertex3dv(side[i][j]);
            NormalVertex3dv(side[min(i + step, NBM) % NBM][min(j + step, NBP - 1)]);
            NormalVertex3dv(side[min(i + step, NBM) % NBM][j]);
        }
        for (int j = 0; j < NBC; j += step) {
            // Premier triangle (SW -> SE -> NW)
            NormalVertex3dv(base[i][min(j + step, NBC - 1)]);
            NormalVertex3dv(base[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(base[i][j]);

            // Second triangle (NW -> SE -> NE)
            NormalVertex3dv(base[i][j]);
            NormalVertex3dv(base[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(base[min(i + step, NBM) % NBM][j]);
        }
    }
    glEnd();
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
