/*
 * Noms : Reetesh Dooleea, Freddy Some, Hicham Hamoumi, Yacine Hamdani 
 * Équipe 2647
 * Projet : main.cpp
 * Description : Ce fichier contient les codes pour les quatres sections du parcours du projet final.
 */

#include <librairie.h>

// les compteurs necessaires de chaque section
struct lesCompteur
{
    uint8_t nombreSections = 0;
    uint8_t nombreBoucle = 0;
    uint8_t nombreCoupure = 0;
    uint8_t nombreCouloir = 0;
    uint8_t nombreSection = 0;
    uint8_t finCouloir = 0;
};

// déclaration des sections
enum class Etat
{
    couloir,
    mur,
    boucle, // deux boucles
    coupure,
    fin
};

// pour distinguer les fins de la coupure
enum class segmentCoupure
{
    segment1,
    segment2,
    transitionSegment2,
    transitionSegment1,
};

//les parties de la section couloir
enum class segmentCouloir
{
    droite,
    gauche,
    analyseDroite,
    analyseGauche,
    suiteDroite,
    suiteGauche,
    finDuCouloir
};

// les parties de la section sonar
enum class segmentMur
{
    debutMur,
    sonar,
    finMur
};

// declaration des variables utilisees dans l'ISR
volatile Etat section;
volatile segmentCoupure choixCoupure;
volatile segmentMur choixMur;
volatile segmentCouloir sortieCouloir;

// les variables de type volatile
volatile uint8_t temps;
volatile uint8_t nbBouton;
volatile bool commencer;
volatile bool interruptionExterne;
volatile uint16_t distance;
volatile int etatSonar;
volatile int estRentre;

//declaration des variables globales
lesCompteur compteur;
LCM ecran(&DEMO_DDR, &DEMO_PORT);

// affiche l'etat present
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

// changement des etats
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

// faire une rotation a gauche a la fin de chaque section
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

// avancer et suivre la ligne pour aller tout droit
void suiveurDeLigne()
{
    if (!POS1 && !POS2 && POS3 && !POS4 && !POS5)
    {
        // DS3 allume
        ajustementPWM(100, 110, avant);
        delayMs(40);
    }

    //else if (!POS1 && !POS2 && POS3 || POS4 && !POS5)
    else if (!POS1 && !POS2 && (POS3 || POS4) && !POS5)
    {
        // tourner legerement a droite - DS4 allume
        ajustementPWM(55, 110, avant);
        delayMs(10);
    }

    //else if (!POS1 && POS2 || POS3 && !POS4 && !POS5)
    else if (!POS1 && (POS2 || POS3) && !POS4 && !POS5)
    {
        // tourner legerement a gauche - DS2 allume
        ajustementPWM(100, 60, avant);
        delayMs(25);
    }
}

// donnne une petite poussee au robot
void boost()
{
    ajustementPWM(130, 130, avant);
    delayMs(100);
}

// suivre la ligne afin de se rendre a l'intersection et faire un virage
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

// initialiser les registres et variables pour le fonctionnement du sonar
void initialisationSonar()
{
    cli(); // bloquer toutes les interruptions

    DDRB = 0b11111011;
    DDRC = 0xff;
    EIMSK |= (1 << INT2);  // permettre les interruptions externes a ete commente parcequon nutilise pas dinterruptions externes
    EICRA |= (1 << ISC20); // l'interruption est assigné à "any edge"

    sei(); // permet de recevoir à nouveau des interruptions
}

// section mur
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
            PORTB |= (1 << PB4); //Le PINA envoie le premier pulse durant 15us
            _delay_us(15);
            PORTB &= ~(1 << PB4);
            distance = 2.20 * temps; // on calcul la distance en prenant compte la conversion en micro secondes, la division par 58 et le prescaler
            _delay_ms(50);

            if (distance < 13)
            {
                ajustementPWM(70, 105, avant); // seloigner du mur si la distance est inferieure a 12 cm
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

// rendu a la fin du parcours
void fin()
{
    // faire la transition pour revenir au point de depart et arreter les moteurs
    transition();
    ajustementPWM(0, 0, avant);
}

// execute chaque section dependemment de l'etat
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

// bouton poussoir du breadboard pour detecter le demarrage
ISR(INT0_vect)
{
    commencer = true;
    DDRD = 0xFF;
    EIFR |= (1 << INTF0);
}

// interruption du bouton-poussoir sur le breadboard
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

// interruption du sonar
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

// initialiser les registres et variables globales
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