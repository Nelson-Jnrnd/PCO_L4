#include "track.h"

Track::Track() :turns(QList<Turn*>()), currentTurn(turns)
{
}

void Track::AddTurn(int contact1, int contact2, QList<Switch> switches, Turn::Danger safety)
{
    turns.append(new Turn(contact1, contact2, switches, safety));
}


Turn* Track::nextTurn(bool isClockwise)
{
    if(isClockwise){
        if(currentTurn.hasNext()){
            return currentTurn.next();
        } else{
            return nullptr;
        }
    } else{
        if(currentTurn.hasPrevious()){
            return currentTurn.previous();
        } else{
            return nullptr;
        }
    }
}
// TODO maybe c'est factorisable avec nextTurn
Turn *Track::peekNextTurn(bool isClockwise)
{
    if(isClockwise){
        if(currentTurn.hasNext()){
            return currentTurn.peekNext();
        } else{
            return nullptr;
        }
    } else{
        if(currentTurn.hasPrevious()){
            return currentTurn.peekPrevious();
        } else{
            return nullptr;
        }
    }
}

void Track::newLap(bool isClockwise)
{
    if(isClockwise)
        currentTurn = turns;
    else{
        while(currentTurn.hasNext()) currentTurn.next();
    }

}
