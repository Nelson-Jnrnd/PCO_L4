//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "ctrain_handler.h"

#include "locomotive.h"
#include "locomotivebehavior.h"
#include "sharedsectioninterface.h"
#include "sharedsection.h"

// Locomotives :
// Vous pouvez changer les vitesses initiales, ou utiliser la fonction loco.fixerVitesse(vitesse);
// Laissez les numéros des locos à 0 et 1 pour ce laboratoire

// Locomotive A
static Locomotive locoA(7 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);
// Locomotive B
static Locomotive locoB(42 /* Numéro (pour commande trains sur maquette réelle) */, 12 /* Vitesse */);

//Arret d'urgence
void emergency_stop()
{
    // TODO

    afficher_message("\nSTOP!");
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

    /********************************
     * Position de départ des locos *
     ********************************/


    Track trackLocoB;
    {
        trackLocoB.AddTurn(31, 30, {{TOUT_DROIT, 22}}, Turn::clear);
        trackLocoB.AddTurn(29, 28, {{TOUT_DROIT, 19}}, Turn::clear);
        trackLocoB.AddTurn(22, 21, {{TOUT_DROIT, 16}}, Turn::shared);
        trackLocoB.AddTurn(20, 19, {{TOUT_DROIT, 13}}, Turn::shared);
        trackLocoB.AddTurn(13, 12, {{TOUT_DROIT, 10}}, Turn::shared);
        trackLocoB.AddTurn(11, 10, {{TOUT_DROIT, 7}}, Turn::shared);
        trackLocoB.AddTurn(4, 3, {{TOUT_DROIT, 4}}, Turn::clear);
        trackLocoB.AddTurn(2, 1, {{TOUT_DROIT, 1}}, Turn::clear);
    }
    Track trackLocoA;
    {
        trackLocoA.AddTurn(6, 7, {{DEVIE, 3}, {TOUT_DROIT, 5}}, Turn::clear);
        trackLocoA.AddTurn(14, 15, {{TOUT_DROIT, 11}, {DEVIE, 8}}, Turn::shared);
        trackLocoA.AddTurn(15, 13, {{DEVIE, 10}, {TOUT_DROIT, 9}}, Turn::shared);
        trackLocoA.AddTurn(19, 24, {{DEVIE, 13}, {TOUT_DROIT, 14}}, Turn::shared);
        trackLocoA.AddTurn(24, 25, {{DEVIE, 15}, {TOUT_DROIT, 17}}, Turn::shared);
        trackLocoA.AddTurn(32, 33, {{TOUT_DROIT, 23}, {DEVIE, 20}}, Turn::clear);
        trackLocoA.AddTurn(33, 34, {{DEVIE, 21}}, Turn::clear);
        trackLocoA.AddTurn(5, 6, {{DEVIE, 2}}, Turn::clear);
    }
    // Loco 0
    // Exemple de position de départ
    locoA.fixerPosition(5, 6);

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
    std::unique_ptr<Launchable> locoBehaveA = std::make_unique<LocomotiveBehavior>(locoA, sharedSection, trackLocoA, 2, false/*, autres paramètres ...*/);
    // Création du thread pour la loco 1
    std::unique_ptr<Launchable> locoBehaveB = std::make_unique<LocomotiveBehavior>(locoB, sharedSection, trackLocoB, 2, true /*, autres paramètres ...*/);

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
