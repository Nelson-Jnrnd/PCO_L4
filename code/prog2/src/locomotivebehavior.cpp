/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : locomotivebehavior.cpp
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Implémentation du comportement d'une locomotive
----------------------------------------------------------- */

//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include <iostream>

/*!
 * \brief Fonction exécutée lors du lancement d'une locomotive
 */
void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");


    // Valeur bidon pour le 1er tour
    int previousCornerContact = -1;


    while(1) {

        // Récupère le prochain virage du tour courant
        Turn* currentTurn = track.nextTurn(isClockwise);

        // Si le prochain virage est défini...
        if(currentTurn){

            // On récupère le contact d'entrée du virage courant
            int currentCornerContact = currentTurn->getEntry(isClockwise);
            loco.afficherMessage("Current turn contact :" + QString::number(currentCornerContact));

            // Si on vient de quitter la file d'attente pour l'accès à la section partagée
            // On vérifie pas l'entrée du virage, il y a des grandes chances qu'on l'aie dépassé en freinant
            if(!wokeUp){
                // Si le contact de sortie du virage précédent est le même que
                // le contact d'entrée du virage courant, on est déjà dessus
                if(previousCornerContact != currentCornerContact){
                        attendre_contact(currentCornerContact);
                }
                currentTurn->activate();
            }
            else{
                wokeUp = false;
                afficher_message(qPrintable(QString("The engine no. %1 ignore contact").arg(loco.numero())));
            }
            // On récupère le niveau de danger du virage actuel
            Turn::Danger futurDangerLevel = currentTurn->getSafety();

            // Si l'on a dépassé le nombre de tours maximum, on ignore la requête d'accès qui va être passée par la suite
            // car on va effectuer un demi-tour; sans cela les locomotives vont s'interbloquer en raison de cette requête
            if((currentLapCount !=  MAX_LAPS + 1 or track.getTurnNo(currentTurn) != 0)){

            // Selon le niveau de danger du virage courant...
            switch(futurDangerLevel){

                // Dans le cas où l'on entre dans la section critique du côté A, on effectue une requête
                // cela n'est le cas seulement dans un sens de rotation horaire, autrement on en sort.
                case Turn::Danger::requestEA:
                if(isClockwise)
                   sharedSection->request(loco, id, currentTurn->getEntryPoint(isClockwise));
                break;

                // Dans le cas où l'on entre dans la section critique du côté B, on effectue une requête
                // cela n'est le cas seulement dans un sens de rotation anti-horaire, autrement on en sort.
                case Turn::Danger::requestEB:
                if(!isClockwise)
                    sharedSection->request(loco, id, currentTurn->getEntryPoint(isClockwise));
                break;

                default:
                break;
                }
            }

            // On récupère ensuite le contact de sortie du virage courant
            previousCornerContact = currentTurn->getExit(isClockwise);


            attendre_contact(previousCornerContact);



            manageDangers(currentTurn);

        // Si le prochain virage est indéfini, c'est que le tour est terminé
        } else {

            loco.afficherMessage("Track complete (next turn is undefined)");

            // On incrémente le nombre de tours effectués
            // Si le nombre de tours atteint le maximum...
            if (currentLapCount++ > MAX_LAPS){
                // On change de sens
                loco.inverserSens();
                isClockwise = !isClockwise;

                // On réinitialise les tours effectués
                currentLapCount = 1;

            } else {
                // Sinon, on repart pour un tour dans le même sens
                track.newLap(isClockwise);
            }
        }
    }
}

/*!
 * \brief Affiche le message de lancement d'une locomotive
 */
void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

/*!
 * \brief Affiche le message de complétion d'une locomotive
 */
void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

/*!
 * \brief Cette fonction détermine les actions à effectuer en fonction du niveau de danger du virage courant
 * \param currentTurn le virage courant pour lequel il faut analyser les dangers
 */
void LocomotiveBehavior::manageDangers(Turn* currentTurn)
{
    // Récupère les informations du virage suivant
    Turn* nextTurn = track.peekNextTurnLoopBack(isClockwise);

    // S'il y'a un virage suivant (on n'est donc pas à la fin d'un tour)
    if (nextTurn) {

        // Récupère les niveaux de danger du virage courant et suivant
        Turn::Danger futurDangerLevel = nextTurn->getSafety();
        Turn::Danger currentDangerLevel = currentTurn->getSafety();

        // Si les niveaux de danger diffèrent entre le virage courant et le suivant, et que le nombre de tours effectué arrive au max...
        if (futurDangerLevel != currentDangerLevel and (currentLapCount != MAX_LAPS + 1 or track.getTurnNo(currentTurn) != 0)) {

            // Si on essaie d'accéder à la section partagée depuis le point d'entrée A (dans le sens horaire, autrement on en sort)...
            if (currentDangerLevel == Turn::Danger::requestEA and isClockwise) {
                // On accède à la section partagée
                sharedSection->getAccess(loco, id);
                wokeUp = true;                

            // Ou alors, on essaie d'accéder depuis le point d'entrée B (dans le sens anti-horaire, autrement on en sort)
            } else if (currentDangerLevel == Turn::Danger::requestEB and !isClockwise){
                // On accède à la section partagée
                sharedSection->getAccess(loco, id);
                wokeUp = true;
            }

        }

        // Si le prochain virage n'est plus du même type de danger...
        if (futurDangerLevel != currentDangerLevel) {
            // Dans le cas d'un danger "shared", cela signifie que l'on quitte la section partagée
            if (currentDangerLevel == Turn::Danger::shared) {
                sharedSection->leave(loco, id);
            }
        }
    }
}
