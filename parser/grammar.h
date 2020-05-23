#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "private.h"
#include "json/json.h"

namespace parse {

Json::Value loadJson(std::string filename);

struct rule_t;
struct stack_t;
typedef std::shared_ptr<rule_t> rule_ptr;
typedef std::shared_ptr<stack_t> stack_ptr;

struct grammar_t {

  grammar_t(Json::Value const &json);
  // ~grammar_t ();

  // void set_item (bundles::item_ptr const& item);

  // oak::uuid_t const& uuid () const;
  stack_ptr seed() const;
  std::mutex &mutex() { return _mutex; }

private:
  struct rule_stack_t {
    rule_stack_t(rule_t const *rule, rule_stack_t const *parent = nullptr)
        : rule(rule), parent(parent) {}

    rule_t const *rule;
    rule_stack_t const *parent;
  };

  void setup_includes(rule_ptr const &rule, rule_ptr const &base,
                      rule_ptr const &self, rule_stack_t const &stack);
  rule_ptr find_grammar(std::string const &scope, rule_ptr const &base);
  // std::vector<std::pair<scope::selector_t, rule_ptr>>
  // grammar_t::injection_grammars ()
  rule_ptr add_grammar(std::string const &scope, Json::Value const &json,
                       rule_ptr const &base = rule_ptr());

  // std::vector<std::pair<scope::selector_t, rule_ptr>> injection_grammars();

  rule_ptr _rule;
  std::map<std::string, rule_ptr> _grammars;
  std::mutex _mutex;
};

typedef std::shared_ptr<grammar_t> grammar_ptr;
grammar_ptr parse_grammar(Json::Value const &json);

} // namespace parse
