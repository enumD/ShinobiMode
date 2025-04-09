// IControl.h
#ifndef ICONTROL_H
#define ICONTROL_H

class IStartStop
{
  public:
    virtual ~IStartStop() = default; // Distruttore virtuale per la gestione corretta della memoria

    // Metodi virtuali puri
    virtual void start() = 0;     // Metodo per avviare
    virtual void stop() = 0;      // Metodo per fermare
    virtual bool isRunning() = 0; // Metodo per verificare se è in esecuzione
};

#endif // ICONTROL_H
