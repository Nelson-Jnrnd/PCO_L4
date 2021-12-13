//    ___  _________    ___  ___  ___   __ //
//   / _ \/ ___/ __ \  |_  |/ _ \|_  | / / //
//  / ___/ /__/ /_/ / / __// // / __/ / /  //
// /_/   \___/\____/ /____/\___/____//_/   //
//

#include "locomotivebehavior.h"
#include "ctrain_handler.h"
#include <iostream>

void LocomotiveBehavior::run()
{
    //Initialisation de la locomotive
    loco.allumerPhares();
    loco.demarrer();
    loco.afficherMessage("Ready!");

    /* A vous de jouer ! */

    // Vous pouvez appeler les méthodes de la section partagée comme ceci :
    //sharedSection->request(loco);
    //sharedSection->getAccess(loco);
    //sharedSection->leave(loco);

    // Valeur bidon pour le 1er tour
    int previousCornerContact = -1;
    while(1) {
        // Récupère le prochain virage du tour courrant
        Turn* currentTurn = track.nextTurn(isClockwise);
        if(currentTurn){
            int currentCornerContact = currentTurn->getEntry(isClockwise);
            // Si on vient de quitter la file d'attente pour l'accès à la section partagée
            // On vérifie pas l'entrée du virage, il y a des grandes chances qu'on l'ai dépassé en freinant
            if(!wokeUp){
                // Si le contact de sortie du virage précédent est le même que
                // le contact d'entrée du virage courrant, on est déjà dessus
                if(previousCornerContact != currentCornerContact){
                        attendre_contact(currentCornerContact);
                }
                currentTurn->Activate();
            }
            else{
                wokeUp = false;
                afficher_message(qPrintable(QString("The engine no. %1 ignore contact").arg(loco.numero())));
            }

            previousCornerContact = currentTurn->getExit(isClockwise);
            attendre_contact(previousCornerContact);

            manageDangers(currentTurn);
        } else{
            // Si le prochain virage est indéfini, c'est que le tour est terminé
            if(currentLapCount++ > MAX_LAPS){
                // On change de sense
                loco.inverserSens();
                isClockwise = !isClockwise;
                currentLapCount = 1;
            } else{
                track.newLap(isClockwise);
            }
        }
    }
}

void LocomotiveBehavior::printStartMessage()
{
    qDebug() << "[START] Thread de la loco" << loco.numero() << "lancé";
    loco.afficherMessage("Je suis lancée !");
}

void LocomotiveBehavior::printCompletionMessage()
{
    qDebug() << "[STOP] Thread de la loco" << loco.numero() << "a terminé correctement";
    loco.afficherMessage("J'ai terminé");
}

void LocomotiveBehavior::manageDangers(Turn* currentTurn)
{
    Turn* nextTurn = track.peekNextTurn(isClockwise);
    if(nextTurn){
        Turn::Danger futurDangerLevel = nextTurn->getSafety();
        Turn::Danger currentDangerLevel = currentTurn->getSafety();
        if(futurDangerLevel > Turn::Danger::clear
                and currentDangerLevel != Turn::Danger::shared){
            // On entre dans une nouvelle zone de danger...
            switch (futurDangerLevel) {
            case Turn::Danger::slow:
                loco.fixerVitesse(3);
                break;
            case Turn::Danger::shared:
                sharedSection->access(loco);
                wokeUp = true;
                break;
            default:
            break;
            }
        } else if(currentDangerLevel == Turn::Danger::shared
                  and futurDangerLevel != Turn::Danger::shared){
            // On quite une zone de danger shared
            sharedSection->leave(loco);
        }
        if(currentDangerLevel == Turn::Danger::slow
                  and futurDangerLevel != Turn::Danger::slow){
            // On quite un zone de danger slow
            loco.fixerVitesse(12);
        }
    }
}
