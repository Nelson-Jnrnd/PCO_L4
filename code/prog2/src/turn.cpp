/* -----------------------------------------------------------
Laboratoire : 04 - Gestion de ressources
Fichier     : turn.cpp
Auteur(s)   : Jeanrenaud Nelson, Mirabile Théo
Date        : 06/12/2021

But  : Implémentation du système de virages (ou segments)
----------------------------------------------------------- */

#include "turn.h"
#include "ctrain_handler.h"

Turn::Turn(int contact1, int contact2, QList<Switch> switches, Danger safety)
: contact1(contact1), contact2(contact2), safety(safety)
{
    // Pour chaque aiguillage renseigné, on le rajoute au tour
    for(Switch s : switches){
        afficher_message(qPrintable(QString("Populating...")));
        this->switches.append(s);
    }
}

int Turn::getEntry(bool isClockwise)
{
    return isClockwise ? contact1 : contact2;
}

int Turn::getExit(bool isClockwise)
{
    return isClockwise ? contact2 : contact1;
}

void Turn::activate()
{
    // On active les aiguillages du tour
    for(Switch s : qAsConst(switches)){
        diriger_aiguillage(s.number, s.angle, 0);
    }
}

SharedSectionInterface::EntryPoint Turn::getEntryPoint(bool isClockwise)
{
    return isClockwise ? SharedSectionInterface::EntryPoint::EA : SharedSectionInterface::EntryPoint::EB;
}

Turn::Danger Turn::getSafety()
{
    return safety;
}
