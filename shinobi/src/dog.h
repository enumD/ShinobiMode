#pragma once

#include "Modes.h"

class Dog : public Modes
{

  public:
    explicit Dog();

    ~Dog() override;

    // Delete copy and move constructors and assignment operators: Used to prevent Dog to be copied
    // and moved, because it is not thread safe
    Dog(const Dog &) = delete;
    Dog &operator=(const Dog &) = delete;
    Dog(Dog &&) = delete;
    Dog &operator=(Dog &&) = delete;
    // End of

  private:
    void _thread_func() override;
};

