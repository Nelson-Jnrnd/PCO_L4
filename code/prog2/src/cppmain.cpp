/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : cppmain.cpp
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Programme 2 - Fichier principal
----------------------------------------------------------- */

//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//                                         //


#include "ctrain_handler.h"

#include "locomotive.h"
#include "locomotivebehavior.h"
#include "sharedsectioninterface.h"
#include "sharedsection.h"

// Locomotives :
// Vous pouvez changer les vitesses initiales, ou utiliser la fonction loco.fixerVitesse(vitesse);
// Laissez les numéros des locos à 0 et 1 pour ce laboratoire

// Locomotive A
static Locomotive locoA(7 /* Numéro (pour commande trains sur maquette réelle) */, 8 /* Vitesse */);
// Locomotive B
static Locomotive locoB(42 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);

//Arret d'urgence
void emergency_stop()
{
    afficher_message("\nSTOP!");

    // Arrête les locomotives
    locoA.arreter();
    locoB.arreter();

    // Fixe leur vitesse à zéro
    locoA.fixerVitesse(0);
    locoB.fixerVitesse(0);
}


//Fonction principale
int cmain()
{
    /************
     * Maquette *
     ************/

    //Choix de la maquette (A ou B)
    selection_maquette(MAQUETTE_A /*MAQUETTE_B*/);

    /**********************************
     * Initialisation des aiguillages *
     **********************************/

    // Initialisation des aiguillages
    // Positiion de base donnée comme exemple, vous pouvez la changer comme bon vous semble
    // Vous devrez utiliser cette fonction pour la section partagée pour aiguiller les locos
    // sur le bon parcours (par exemple à la sortie de la section partagée) vous pouvez l'
    // appeler depuis vos thread des locos par ex.
    diriger_aiguillage(1,  TOUT_DROIT, 0);
    diriger_aiguillage(2,  DEVIE     , 0);
    diriger_aiguillage(3,  DEVIE     , 0);
    diriger_aiguillage(4,  TOUT_DROIT, 0);
    diriger_aiguillage(5,  TOUT_DROIT, 0);
    diriger_aiguillage(6,  TOUT_DROIT, 0);
    diriger_aiguillage(7,  TOUT_DROIT, 0);
    diriger_aiguillage(8,  DEVIE     , 0);
    diriger_aiguillage(9,  TOUT_DROIT, 0);
    diriger_aiguillage(10, DEVIE     , 0);
    diriger_aiguillage(11, TOUT_DROIT, 0);
    diriger_aiguillage(12, TOUT_DROIT, 0);
    diriger_aiguillage(13, DEVIE     , 0);
    diriger_aiguillage(14, TOUT_DROIT, 0);
    diriger_aiguillage(15, DEVIE     , 0);
    diriger_aiguillage(16, TOUT_DROIT, 0);
    diriger_aiguillage(17, TOUT_DROIT, 0);
    diriger_aiguillage(18, TOUT_DROIT, 0);
    diriger_aiguillage(19, TOUT_DROIT, 0);
    diriger_aiguillage(20, DEVIE     , 0);
    diriger_aiguillage(21, DEVIE     , 0);
    diriger_aiguillage(22, TOUT_DROIT, 0);
    diriger_aiguillage(23, TOUT_DROIT, 0);
    diriger_aiguillage(24, TOUT_DROIT, 0);
    // diriger_aiguillage(/*NUMERO*/, /*TOUT_DROIT | DEVIE*/, /*0*/);

    /***************************
     * Création des deux tours *
     ***************************/

    // Tour effectué par la loco A (les virages sont donnés pour un sens horaire)
    Track trackLocoA;
    {
        trackLocoA.addTurn(34, 33, {{DEVIE, 21}}, Turn::requestEA);
        trackLocoA.addTurn(33, 32, {{TOUT_DROIT, 23}, {DEVIE, 20}}, Turn::clear);
        trackLocoA.addTurn(25, 24, {{DEVIE, 15}, {TOUT_DROIT, 17}}, Turn::shared);
        trackLocoA.addTurn(24, 19, {{DEVIE, 13}, {TOUT_DROIT, 14}}, Turn::shared);
        trackLocoA.addTurn(13, 15, {{DEVIE, 10}, {TOUT_DROIT, 9}}, Turn::shared);
        trackLocoA.addTurn(15, 14, {{TOUT_DROIT, 11}, {DEVIE, 8}}, Turn::shared);
        trackLocoA.addTurn(7, 6, {{DEVIE, 3}, {TOUT_DROIT, 5}}, Turn::clear);
        trackLocoA.addTurn(6, 5, {{DEVIE, 2}}, Turn::requestEB);
    }

    // Tour effectué par la loco B (les virages sont donnés pour un sens horaire)
    Track trackLocoB;
    {
        trackLocoB.addTurn(31, 30, {{TOUT_DROIT, 22}}, Turn::requestEA);
        trackLocoB.addTurn(29, 28, {{TOUT_DROIT, 19}}, Turn::clear);
        trackLocoB.addTurn(22, 21, {{TOUT_DROIT, 16}}, Turn::shared);
        trackLocoB.addTurn(20, 19, {{TOUT_DROIT, 13}}, Turn::shared);
        trackLocoB.addTurn(13, 12, {{TOUT_DROIT, 10}}, Turn::shared);
        trackLocoB.addTurn(11, 10, {{TOUT_DROIT, 7}}, Turn::shared);
        trackLocoB.addTurn(4, 3, {{TOUT_DROIT, 4}}, Turn::clear);
        trackLocoB.addTurn(2, 1, {{TOUT_DROIT, 1}}, Turn::requestEB);
    }

    /********************************
     * Position de départ des locos *
     ********************************/

       // Exemple de position de départ
       locoA.fixerPosition(5, 34);

       // Loco 1
       // Exemple de position de départ
       locoB.fixerPosition(31, 1);

    /***********
     * Message *
     **********/

    // Affiche un message dans la console de l'application graphique
    afficher_message("Hit play to start the simulation...");

    /*********************
     * Threads des locos *
     ********************/

    // Création de la section partagée
    std::shared_ptr<SharedSectionInterface> sharedSection = std::make_shared<SharedSection>();

    // Création du thread pour la loco 0
    std::unique_ptr<Launchable> locoBehaveA = std::make_unique<LocomotiveBehavior>(locoA, sharedSection, trackLocoA, 2, false, SharedSectionInterface::LocoId::LA);
    // Création du thread pour la loco 1
    std::unique_ptr<Launchable> locoBehaveB = std::make_unique<LocomotiveBehavior>(locoB, sharedSection, trackLocoB, 2, true, SharedSectionInterface::LocoId::LB);

    // Lanchement des threads
    afficher_message(qPrintable(QString("Lancement thread loco A (numéro %1)").arg(locoA.numero())));
    locoBehaveA->startThread();
    afficher_message(qPrintable(QString("Lancement thread loco B (numéro %1)").arg(locoB.numero())));
    locoBehaveB->startThread();

    // Attente sur la fin des threads
    locoBehaveA->join();
    locoBehaveB->join();

    //Fin de la simulation
    mettre_maquette_hors_service();

    return EXIT_SUCCESS;
}
