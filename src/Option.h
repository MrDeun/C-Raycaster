#ifndef OPTION
#define OPTION

template <class DATA1> struct Option {
  DATA1 *result;
  bool error;

  Option() : error(true), result(nullptr) {}
  Option(DATA1 in) : error(false), result(nullptr) {
    result = new DATA1;
    *result = in;
  }
};

#endif