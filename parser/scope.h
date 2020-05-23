#pragma once

#include <memory>
#include <vector>

namespace scope {

struct scope_t {
  scope_t();
  scope_t(char const *scope);
  scope_t(std::string const &scope);
  ~scope_t();

  scope_t(scope_t &&rhs);
  scope_t(scope_t const &rhs);
  scope_t &operator=(scope_t &&rhs);
  scope_t &operator=(scope_t const &rhs);

  bool has_prefix(scope_t const &rhs) const;

  void push_scope(std::string const &atom);
  void pop_scope();
  std::string const &back() const;
  size_t size() const;
  bool empty() const;
  size_t hash() const;

  bool operator==(scope_t const &rhs) const;
  bool operator!=(scope_t const &rhs) const;
  bool operator<(scope_t const &rhs) const;

  // explicit operator bool() const;
  // explicit operator std::string() const;

  std::string name;
};

typedef scope_t context_t;
typedef scope_t selector_t;

} // namespace scope
