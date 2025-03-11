#ifndef OBSERVER_H_
#define OBSERVER_H_

// Interfaccia Observer: ogni osservatore deve implementare il metodo update()
class Observer {
public:
  virtual void update() = 0;
};

// Interfaccia Subject: fornisce metodi per registrare e rimuovere observer, e per notificarli
class Subject {
public:
  virtual void attach(Observer* o) = 0;
  virtual void detach(Observer* o) = 0;
  virtual void notify() = 0;
};


#endif // OBSERVER_H_
