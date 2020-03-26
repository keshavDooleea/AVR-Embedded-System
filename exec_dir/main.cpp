////////////////////////////////////////////////////////////////////////////////
/// \file    main.cpp
/// \author  Reetesh Dooleea
/// \author  Freddy Some
/// \author  Hicham Hamoumi
/// \author  Yacine Hamdani
///
/// Implementations du code qui gere les instructions du parcour.
/// Identification materiel :
/// PORT A : [1:6] Infrarouges.
/// PORT B : [1:2] DEL [4:6] SON [3:5] SONAR
/// PORT C : [1:8] LCD
/// PORT D : [2:4] Bouton blanc. [5:8] Moteur.
////////////////////////////////////////////////////////////////////////////////

#include <librairie.h>

/**
 * \struct  lesCompteurs
 * \brief   Les compteurs necessaires de chaque sections.
 * 
 * Regroupe les compteurs utilise 
 * tout au long du parcours.
 */
struct lesCompteur
{
    uint8_t nombreBoucle = 0;  //!<  Permet de savoir la position du Robot sur les deux boucles.
    uint8_t nombreCoupure = 0; //!<  Permet de savoir la position du Robot sur les coupures.
    uint8_t nombreSection = 0; //!<  Permet de savoir la position du Robot sur le parcours.
    uint8_t finCouloir = 0;    //!<  Permet de savoir la position du Robot sur le couloir.
};

/**
 * \enum    Etat
 * \brief   Déclaration des sections
 * 
 * Regroupe les sections du 
 * parcours.
 */
enum class Etat
{
    couloir, //!<  Section couloir .
    mur,     //!<  Section mur.
    boucle,  //!<  Section deux boucles.
    coupure, //!<  Section coupure.
    fin      //!<  Section fin.
};

/**
 * \enum    segmentCoupure
 * \brief   Déclaration des segments des coupures.
 * 
 * Regroupe les etats du la section coupures.
 * Crucial pour distinguer les fins de coupures.
 */
enum class segmentCoupure
{
    segment1,           //!<  Segments UV ou Z2.
    segment2,           //!<  Segments XY ou 45.
    transitionSegment2, //!<  Segments WX ou 34.
    transitionSegment1, //!<  Segments Z1 ou 6A.
};

/**
 * \enum    segmentCouloir
 * \brief   Déclaration des etats du couloir.
 * 
 * Regroupe les etats du la section couloir.
 * Crucial pour detecter la fin du couloir.
 */
enum class segmentCouloir
{
    droite,        //!< Virage a droite.
    gauche,        //!< Vriage a gauche.
    analyseDroite, //!< Verifie si un des capteurs est detecte .
    analyseGauche, //!< Verifie si un des capteurs est detecte .
    suiteDroite,   //!< Virage a droite si rien est detecte.
    suiteGauche,   //!< Virage a gauche si rien est detecte.
    finDuCouloir   //!< DS3 est detecte , engage la fin du couloir.
};

/**
 * \enum    segmentMur
 * \brief   Déclaration des etats du mur.
 * 
 * Regroupe les etats du la section mur.
 * Crucial pour effectue le suivie de mur 
 * et basculer au suivie de ligne.
 */
enum class segmentMur
{
    debutMur, //!< Suiveur de ligne au debut de la section.
    sonar,    //!< Effectue le suiveur de mur.
    finMur    //!< Gere le comportement du robot jusqu'a la prochaine section.
};

/**
 * declaration des variables de type volatile utilisees dans l'ISR
 * 
 */
volatile uint8_t temps;
volatile uint8_t nbBouton;
volatile bool commencer;
volatile bool interruptionExterne;
volatile int etatSonar;

/**
 * declaration des variables globales
 * 
 */
lesCompteur compteur;
LCM ecran(&DEMO_DDR, &DEMO_PORT);
uint16_t distance;
int estRentre;
Etat section;
segmentCouloir sortieCouloir;
segmentCoupure choixCoupure;
segmentMur choixMur;

/**
 * Affiche la section presente .
 * 
 */
void afficherSection()
{
    ecran.clear();

    // affichage de l'etat present
    switch (section)
    {
    case Etat::couloir:
        ecran.write("LE COULOIR");
        break;

    case Etat::mur:
        ecran.write("LE MUR ");
        break;

    case Etat::boucle:
        ecran.write("LES DEUX BOUCLES");
        break;

    case Etat::coupure:
        ecran.write("LA COUPURE");
        break;

    case Etat::fin:
        ecran.write("LA FIN");
        break;
    }
}

/**
 * Permet de changer de section
 * grace au bouton poussoir .
 * 
 */
void changerSection()
{

    // passer a la section suivante
    switch (nbBouton % 4)
    {
    case 0:
        section = Etat::couloir;
        break;

    case 1:
        section = Etat::mur;
        break;

    case 2:
        section = Etat::boucle;
        break;

    case 3:
        section = Etat::coupure;
        break;
    }

    afficherSection();
}

/**
 * Faire une rotation a gauche a la
 * fin de chaque section
 * 
 */
void transition()
{
    // avancer pour aligner les roues avec la ligne adjacente gauche
    ajustementPWM(110, 110, avant);
    delayMs(900);

    // immobiliser la roue gauche et faire un virage a gauche
    while (!POS3)
    {
        ajustementPWM(90, 0, avant);
        delayMs(20);
    }
    if (POS3)
    {
        ajustementPWM(0, 0, avant);
        delayMs(500);
    }
}

/**
 * Suit les lignes du parcours
 * selon le capteur detectee.
 * 
 */
void suiveurDeLigne()
{
    if (!POS1 && !POS2 && POS3 && !POS4 && !POS5)
    {
        // DS3 allume
        ajustementPWM(100, 110, avant);
        delayMs(40);
    }

    else if (!POS1 && !POS2 && (POS3 || POS4) && !POS5)
    {
        // tourner legerement a droite - DS4 allume
        ajustementPWM(55, 110, avant);
        delayMs(10);
    }

    else if (!POS1 && (POS2 || POS3) && !POS4 && !POS5)
    {
        // tourner legerement a gauche - DS2 allume
        ajustementPWM(100, 60, avant);
        delayMs(25);
    }
}

/**
 * Donnne une petite poussee 
 * au robot.
 * 
 */
void boost()
{
    ajustementPWM(130, 130, avant);
    delayMs(100);
}

/**
 * Gere la fin des sections du 
 * parcours.
 * 
 */
void finIntersection()
{
    // avance tant qu'on n'est pas rendu a l'intersection de la section
    while (!((POS1 && POS2 && POS3 && !POS4 && !POS5) || (POS1 && POS2 && POS3 && POS4 && !POS5)))
    {
        suiveurDeLigne();
    }

    // tourner a gauche et changer de section
    transition();
    compteur.nombreSection++;
}

/**
 * Effectue le couloir.
 * 
 */
void couloir()
{
    boost();

    while (POS2 || POS3 || POS4)
    {
        suiveurDeLigne();
    }
    if (estRentre == 0)
    {
        ajustementPWM(130, 100, avant);
        delayMs(500);
        ajustementPWM(0, 0, avant);
        delayMs(500);
    }
    estRentre = 1;

    while (compteur.finCouloir != 1)
    {

        switch (sortieCouloir)
        {

        case segmentCouloir::droite:
            ajustementPWM(0, 120, avant);
            delayMs(10);
            sortieCouloir = segmentCouloir::analyseDroite;
            break;

        case segmentCouloir::analyseDroite:
            ajustementPWM(0, 120, avant);
            delayMs(700);
            if (POS3 || POS4 || POS5)
            {

                sortieCouloir = segmentCouloir::finDuCouloir;
            }
            else
            {
                sortieCouloir = segmentCouloir::suiteDroite;
            }

            break;

        case segmentCouloir::suiteDroite:
            while (!POS5)
            {
                ajustementPWM(0, 110, avant);
                delayMs(10);
            }
            sortieCouloir = segmentCouloir::gauche;

            break;

        case segmentCouloir::gauche:
            ajustementPWM(120, 0, avant);
            delayMs(100);
            sortieCouloir = segmentCouloir::analyseGauche;
            break;

        case segmentCouloir::analyseGauche:
            ajustementPWM(120, 0, avant);
            delayMs(500);
            if (POS1 || POS2 || POS3)
            {
                sortieCouloir = segmentCouloir::finDuCouloir;
            }
            else
            {
                sortieCouloir = segmentCouloir::suiteGauche;
            }

            break;

        case segmentCouloir::suiteGauche:
            while (!POS1)
            {
                ajustementPWM(100, 0, avant);
                delayMs(10);
            }
            sortieCouloir = segmentCouloir::droite;
            break;

        case segmentCouloir::finDuCouloir:
            compteur.finCouloir++;
            break;
        }
    }

    finIntersection();
    section = Etat::mur;
}

/**
 * Effectue les coupures.
 * 
 */
void coupure()
{
    boost();

    while (compteur.nombreCoupure < 4)
    {
        switch (choixCoupure)
        {
        case segmentCoupure::segment1:

            // Fait le suiveur tant que le capteur capte la ligne.
            while (POS1 || POS2 || POS3 || POS4 || POS5)
            {
                suiveurDeLigne();
            }

            ajustementPWM(120, 120, avant);
            delayMs(10);

            // Changement du premier ou troisieme segment
            while (!POS5)
            {
                trouverNote(81);
                ajustementPWM(0, 105, avant);
                delayMs(20);
            }

            if (POS5)
            {
                choixCoupure = segmentCoupure::transitionSegment2;
            }
            break;

        case segmentCoupure::transitionSegment2:

            delayMs(400);
            ajustementPWM(115, 40, avant);
            delayMs(300);
            compteur.nombreCoupure++;
            choixCoupure = segmentCoupure::segment2;
            arreterSon();
            break;

        case segmentCoupure::segment2:

            // Fait le suiveur tant que le capteur capte la ligne.
            while (POS1 || POS2 || POS3 || POS4 || POS5)
            {
                suiveurDeLigne();
            }

            // Changement du deuxieme ou quatrieme segment
            while (!POS1)
            {
                trouverNote(60);
                ajustementPWM(85, 0, avant);
                delayMs(30);
            }

            if (POS1)
            {
                choixCoupure = segmentCoupure::transitionSegment1;
            }
            break;

        case segmentCoupure::transitionSegment1:

            delayMs(400);
            ajustementPWM(30, 130, avant);
            delayMs(250);
            compteur.nombreCoupure++;
            choixCoupure = segmentCoupure::segment1;
            arreterSon();
            break;
        }
    }

    finIntersection();
    section = Etat::couloir;
}

/**
 * Effectue les deux boucles.
 * 
 */
void boucle()
{
    boost();

    while (compteur.nombreBoucle < 12)
    {
        suiveurDeLigne();

        // conditions de la boucle
        if ((POS1 && POS2 && POS3 && !POS4 && !POS5) || (POS1 && POS2 && POS3 && POS4 && !POS5) || (POS1 && POS2 && POS3 && POS4 && POS5))
        {
            while ((POS1 && POS2 && POS3 && !POS4 && !POS5) || (POS1 && POS2 && POS3 && POS4 && !POS5) || (POS1 && POS2 && POS3 && POS4 && POS5))
            {
            }
            compteur.nombreBoucle++;

            // grosse boucle
            if (compteur.nombreBoucle == 3 || compteur.nombreBoucle == 4 || compteur.nombreBoucle == 5 || compteur.nombreBoucle == 6 || compteur.nombreBoucle == 7)
            {
                uint8_t pointMort = 0;
                uint8_t vitesseVirage = 95;

                ajustementPWM(vitesseVirage, pointMort, avant);
                delayMs(500);

                while (!POS3)
                {
                    ajustementPWM(vitesseVirage, pointMort, avant);
                    delayMs(20);
                }
                if (POS3)
                {
                    ajustementPWM(0, 0, avant);
                    delayMs(150);
                }

                ajustementPWM(140, 140, avant);
                delayMs(300);

                suiveurDeLigne();
            }

            // petite boucle
            else if (compteur.nombreBoucle == 8 || compteur.nombreBoucle == 9 || compteur.nombreBoucle == 10)
            {
                uint8_t pointMort = 0;
                uint8_t vitesseVirage = 100;

                ajustementPWM(vitesseVirage, pointMort, avant);
                delayMs(400);

                while (!POS3)
                {
                    ajustementPWM(vitesseVirage, pointMort, avant);
                    delayMs(20);
                }
                if (POS3)
                {
                    ajustementPWM(0, 0, avant);
                    delayMs(150);
                }

                ajustementPWM(140, 140, avant);
                delayMs(300);

                suiveurDeLigne();
            }

            // ignorer le croisement et avancer tout droit
            else if (compteur.nombreBoucle == 1 || compteur.nombreBoucle == 2 || compteur.nombreBoucle == 11 || compteur.nombreBoucle == 12)
            {

                suiveurDeLigne();
            }
        }

        // DS1 allume
        else if (POS1 && !POS2 && !POS3 && !POS4 && !POS5)
        {
            ajustementPWM(110, 50, avant);
            delayMs(10);
        }

        // DS5 allume
        else if (!POS1 && !POS2 && !POS3 && !POS4 && POS5)
        {
            delayMs(15);
            ajustementPWM(50, 110, avant);
        }
    }

    finIntersection();
    section = Etat::coupure;
}

/**
 * Initialise les registres et
 * variables pour le fonctionnement du sonar .
 * 
 */
void initialisationSonar()
{
    cli(); // bloquer toutes les interruptions

    DDRB = 0b11111011;
    DDRC = 0xff;
    EIMSK |= (1 << INT2);  // permettre les interruptions externes a ete commente parcequon nutilise pas dinterruptions externes
    EICRA |= (1 << ISC20); // l'interruption est assigné à "any edge"

    sei(); // permet de recevoir à nouveau des interruptions
}

/**
 * Effectue le mur.
 * 
 */
void suiveurDeMur()
{
    boost();

    initialisationSonar();

    switch (choixMur)
    {
    case segmentMur::debutMur:
        while (POS2 || POS3 || POS4)
        {
            suiveurDeLigne();
        }
        choixMur = segmentMur::sonar;
        break;

    case segmentMur::sonar:
        while (!POS1 && !POS2 && !POS3 && !POS4 && !POS5)
        {
            PORTB |= (1 << PB4); // le PINA envoie le premier pulse durant 15us
            _delay_us(15);
            PORTB &= ~(1 << PB4);
            distance = 2.20 * temps; // on calcul la distance en prenant compte la conversion en micro secondes, la division par 58 et le prescaler
            _delay_ms(50);

            if (distance < 13)
            {
                ajustementPWM(70, 105, avant); // s'eloigner du mur si la distance est inferieure a 12 cm
                PORTB = ROUGE;
            }

            else if (distance > 17)
            {
                ajustementPWM(105, 60, avant); // se rapprocher du mur si la distance est superieure a 18 cm
                PORTB = ROUGE;
            }

            else if ((13 <= distance) && (distance <= 17))
            {
                PORTB = VERT;
                ajustementPWM(105, 105, avant); // aller tout droit si le robot se situe dans un intervalle adequat
            }
        }

        if (POS1 || POS5)
        {
            ajustementPWM(100, 100, avant);
            delayMs(400);
        }
        PORTB = ETEINT;
        choixMur = segmentMur::finMur;
        break;

    case segmentMur::finMur:
        while (!POS3) // cherche la ligne
        {
            ajustementPWM(0, 80, avant);
            delayMs(200);
        }

        if (POS3)
        {
            ajustementPWM(0, 0, avant);
            delayMs(200);
        }
        ajustementPWM(130, 130, avant);
        delayMs(100);

        finIntersection();
        section = Etat::boucle;
        break;
    }
}

/**
 * Instruction rendu a la fin
 * du parcour.
 * 
 */
void fin()
{
    // faire la transition pour revenir au point de depart et arreter les moteurs
    transition();
    ajustementPWM(0, 0, avant);
}

/**
 * Execute chaque section dependemment de l'etat , 
 * si le bouton poussoir interupt est appuye.
 * 
 */
void gererSection()
{
    // boucle tant que les quatres sections n'ont pas ete parcourues
    while (section != Etat::fin)
    {
        // gere chaque partie de la section
        if (commencer)
        {

            switch (section)
            {
            case Etat::couloir:
                couloir();
                break;

            case Etat::mur:
                suiveurDeMur();
                break;

            case Etat::boucle:
                boucle();
                break;

            case Etat::coupure:
                coupure();
                break;

            case Etat::fin:
                fin();
                break;
            }

            // si le parcours est fait
            if (compteur.nombreSection > 3)
            {
                section = Etat::fin;
            }

            afficherSection();
        }
    }
}

/**
 * Bouton poussoir interupt pour detecter 
 * le demarrage.
 *
 * \param [in] INT0_vect
 *        Vecteur d'interuption 0.
 * 
 * \return Une interuption si le bouton est preser.
 */
ISR(INT0_vect)
{
    commencer = true;
    DDRD = 0xFF;
    EIFR |= (1 << INTF0);
}

/**
 * Interruption du bouton-poussoir 
 * sur le breadboard pour changer d'etat.
 *
 * \param [in] INT1_vect
 *        Vecteur d'interuption 1.
 * 
 * \return Une interuption si le bouton est preser.
 */

ISR(INT1_vect)
{
    // vérifier anti-rebond
    _delay_ms(30);

    // incrémentation de l'etat
    if (!(PIND & 0x08))
    {
        if (!(PIND & 0x08))
        {
            nbBouton++;
            interruptionExterne = true;
        }
    }

    // enregistrer les autres interruptions
    EIFR |= (1 << INTF1);
}

/**
 * Interruption du timer pour le sonar
 *
 * \param [in] INT2_vect
 *        Vecteur d'interuption 2.
 * 
 * \return Une interuption lorsque le sonar detect un signal. 
 */

ISR(INT2_vect)
{
    if (etatSonar == 0)
    {
        partirMinuterie2(); // on demarre la minuterie 2 quand on recoit une premiere
        etatSonar = 1;
    }
    else
    {
        arreterMinuterie2(); // on arrete la minuterie 2 quand on recoit un une deuxieme interruption
        temps = TCNT2;
        etatSonar = 0;
    }
}

/**
 * Initialiser les registres 
 * et variables globales.
 * 
 */
void initialisation()
{
    // Initialisation des PORTS
    DDRA = ENTREE;
    DDRB = 0x03;
    DDRD = ENTREE; // PORT A est en mode entrée
    afficherSection();
    initialisationBoutonExterne(); // on initialise le bouton blanc sur le breadboard
    initialisationBoutonInt();

    // variables sonar
    temps = 0;
    distance = 0;
    etatSonar = 0;
    estRentre = 0;

    // variables pour le depart du parcours
    nbBouton = 0;
    commencer = false;
    interruptionExterne = true;

    // variables des enums
    choixCoupure = segmentCoupure::segment1;
    choixMur = segmentMur::debutMur;
    sortieCouloir = segmentCouloir::droite;
}

int main()
{
    initialisation();

    // bouton blanc du breadboard pour changement des etats
    while (!commencer)
    {
        if (interruptionExterne)
        {
            changerSection();
            interruptionExterne = false;
        }
    }

    // attendre deux secondes avant lancer le programme
    delayMs(2000);

    gererSection(); // effectuer les instructions de chaque etat

    return 0;
}