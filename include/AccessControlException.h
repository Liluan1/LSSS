#pragma once
#include <exception>

class AccessControlException: public std::exception {
  private:
    const char *message;

  public:
    AccessControlException(const char *str) : message{str} {}
    const char *what() const noexcept { return message; }
};