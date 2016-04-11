#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

template<typename T> void safe_delete(T*& a) {
  delete a;
  a = 0;
}

#endif // HELPERS_HPP_INCLUDED
