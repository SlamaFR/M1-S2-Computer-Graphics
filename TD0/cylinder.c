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
#define NBC 50                    // Nombre de cercles sur les bases

static double step = 1.0;         // Pas de rendu
static double H = 1;              // Hauteur du cylindre
static double R = .5;             // Rayon du cylindre

static G3Xpoint side[NBM][NBP];   // Points représentant le cone en lui-même
static G3Xpoint top[NBM][NBC];    // Points représentant la base supérieure
static G3Xpoint bottom[NBM][NBC]; // Points représentant la base inférieure

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void) {
    // On calcule la taille des subdivisions de l'angle theta, de la hauteur et des cercles des bases.
    double theta = 2 * PI / NBM;
    double t = H / (NBP - 1);
    double r = R / (NBC - 1);

    /* On se reporte à l'équation paramétrique d'un cone :
     *
     * ATTENTION NE PAS CONFONDRE !
     * R vaut le rayon et r vaut la taille d'une subdivision.
     *
     *   / x \            / x = R * cos(theta)
     * M | y |  tel que  <  y = R * sin(theta)
     *   \ z /            \ z = t
     *
     * Les bases sont également construites grâce à l'équation paramétrique d'un disque :
     *
     *   / x \            / x = r * cos(theta)
     * M | y |  tel que  <  y = r * sin(theta)
     *   \ z /            \ z = -1 ou 1
     *
     * Il faut pondérer nos angles et t par l'indice courant.
     * On doit également décaler le cylindre vers le bas (H/2)
     */
    for (int i = 0; i < NBM; i++) {
        for (int j = 0; j < NBP; j++) {
            side[i][j].x = R * cos(i * theta);
            side[i][j].y = R * sin(i * theta);
            side[i][j].z = (j * t) - (H / 2.);
        }
        for (int j = 0; j < NBC; j++) {
            top[i][j].x = (j * r) * cos(i * theta);
            top[i][j].y = (j * r) * sin(i * theta);
            top[i][j].z = H / 2.;
            bottom[i][j].x = (j * r) * cos(i * theta);
            bottom[i][j].y = (j * r) * sin(i * theta);
            bottom[i][j].z = -H / 2.;
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
            NormalVertex3dv(top[i][min(j + step, NBC - 1)]);
            NormalVertex3dv(top[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(top[i][j]);

            // Second triangle (NW -> SE -> NE)
            NormalVertex3dv(top[i][j]);
            NormalVertex3dv(top[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(top[min(i + step, NBM) % NBM][j]);


            // Premier triangle (SW -> SE -> NW)
            NormalVertex3dv(bottom[i][min(j + step, NBC - 1)]);
            NormalVertex3dv(bottom[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(bottom[i][j]);

            // Second triangle (NW -> SE -> NE)
            NormalVertex3dv(bottom[i][j]);
            NormalVertex3dv(bottom[min(i + step, NBM) % NBM][min(j + step, NBC - 1)]);
            NormalVertex3dv(bottom[min(i + step, NBM) % NBM][j]);
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
