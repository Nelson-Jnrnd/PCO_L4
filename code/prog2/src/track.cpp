/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : track.cpp
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Implémentation du système de tours (tracks)
----------------------------------------------------------- */

#include "track.h"

Track::Track() :turns(QList<Turn*>()), currentTurn(turns) {}

Track::~Track()
{
    for(Turn* t : qAsConst(turns)){
        delete t;
    }
}


void Track::addTurn(int contact1, int contact2, QList<Switch> switches, Turn::Danger safety)
{
    // On rajoute le nouveau virage à la liste
    turns.append(new Turn(contact1, contact2, switches, safety));

    // On fait pointer l'itérateur de virage courant sur le début de la liste
    currentTurn = turns;
}

Turn* Track::nextTurn(bool isClockwise)
{
    // Si on est dans un sens horaire
    if (isClockwise) {
        // On retourne le virage suivant s'il existe
        if (currentTurn.hasNext()) {
            return currentTurn.next();

        // Sinon on retourne un pointeur nul
        } else {
            return nullptr;
        }
    // Sinon, on est dans un sens anti-horaire
    } else{
        // On retourne le virage précédent s'il existe
        if (currentTurn.hasPrevious()) {
            return currentTurn.previous();

        // Sinon on retourne un pointeur nul
        } else {
            return nullptr;
        }
    }
}

Turn* Track::peekNextTurn(bool isClockwise)
{
    // Si on est dans un sens horaire
    if (isClockwise) {
        // On retourne les informations du virage suivant s'il existe
        if (currentTurn.hasNext()) {
            return currentTurn.peekNext();

        // Sinon on retourne un pointeur nul
        } else {
            return nullptr;
        }

    // Sinon, on est dans un sens anti-horaire
    } else {
        // On retourne les informations du virage précédent s'il existe
        if (currentTurn.hasPrevious()) {
            return currentTurn.peekPrevious();

        // Sinon on retourne un pointeur nul
        } else {
            return nullptr;
        }
    }
}

Turn *Track::peekNextTurnLoopBack(bool isClockwise)
{
    // Si on est dans un sens horaire
    if(isClockwise){
        // On retourne les informations du virage suivant s'il existe
        if(currentTurn.hasNext()){
            return currentTurn.peekNext();
        // Sinon, on retourne le premier virage du tour
        } else{
            QListIterator<Turn*> tmp(turns);
            return tmp.peekNext();
        }
    // Sinon, on est dans un sens anti-horaire
    } else{
        // On retourne les informations du virage précédent s'il existe
        if(currentTurn.hasPrevious()){
            return currentTurn.peekPrevious();
        // Sinon, on retourne le dernier virage du tour
        } else{
            QListIterator<Turn*> tmp(turns);
            tmp.toBack();
            return tmp.peekPrevious();
        }
    }
}

void Track::newLap(bool isClockwise)
{
    // Si on est dans un sens horaire, on retourne le premier virage
    if(isClockwise){
        currentTurn.toFront();
    }
    // Sinon, on retourne le dernier
    else{
        currentTurn.toBack();
    }

}

void Track::initTrack(bool isClockwise)
{
    // Si on est dans un sens horaire,
    if(isClockwise){
        // On récupère le premier virage
        currentTurn.toFront();
        // Et on rajoute les tours suivants
        while(!currentTurn.hasNext()){
            currentTurn.toFront();
        }

    // Sinon, on fait l'inverse
    } else{
        currentTurn.toBack();
        while (!currentTurn.hasPrevious()) {
            currentTurn.toBack();
        }
    }
}


int Track::getTurnNo(Turn *turn)
{
    return turns.indexOf(turn);
}

int Track::getNbTurns()
{
    return turns.count();
}
