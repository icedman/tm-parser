#include <iostream>

#include "grammar.h"

using namespace parse;

int main(int argc, char **argv) {
  Json::Value root = loadJson("test-cases/first-mate/fixtures/hello.json");
  grammar_ptr gm = parse_grammar(root);
  return 0;
}