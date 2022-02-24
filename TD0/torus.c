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
static double R = 1.0;            // Rayon du trou
static double S = 0.5;            // Rayon du tore (épaisseur)

static G3Xpoint points[NBM][NBP]; // Points représentant notre tore

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void) {
    // On calcule la taille d'une subdivision de chaque angle (theta et phi)
    double theta = 2 * PI / NBM;
    double phi = 2 * PI / (NBP - 1); // -1 ici, car on veut NBP faces et pas NBP lignes

    /* On se reporte à l'équation paramétrique d'un tore :
     *
     *   / x \            / x = cos(theta) * (R + r * cos(phi))
     * M | y |  tel que  <  y = -sin(theta) * (R * r * cos(phi))
     *   \ z /            \ z = r * sin(phi)
     *
     * Il faut pondérer nos angles par l'indice courant.
     */
    for (int i = 0; i < NBM; i++) {
        for (int j = 0; j < NBP; j++) {
            points[i][j].x = cos(i * theta) * (R + S * cos(j * phi));
            points[i][j].y = -sin(i * theta) * (R + S * cos(j * phi));
            points[i][j].z = S * sin(j * phi);
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
