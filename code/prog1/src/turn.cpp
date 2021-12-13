#include "turn.h"
#include "ctrain_handler.h"

Turn::Turn(int contact1, int contact2, QList<Switch> switches, Danger safety)
: contact1(contact1), contact2(contact2), safety(safety)
{
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

void Turn::Activate()
{
    for(Switch s : qAsConst(switches)){
        diriger_aiguillage(s.number, s.angle, 0);
    }
}

Turn::Danger Turn::getSafety()
{
    return safety;
}
