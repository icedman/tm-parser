#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>

#include "grammar.h"
#include "parse.h"

using namespace parse;

grammar_ptr load(std::string path) {
  Json::Value json = loadJson(path);
  return parse_grammar(json);
}

void read_and_parse_a_grammar(const char *grammar, const char *out) {
  grammar_ptr gm;
  gm = load(grammar);

  std::string path;
  path = "tests/results/read/";
  path += out;
  // gm.save(path);
  // gm.dump();

  path = "tests/results/parsed/";
  path += out;
  // gm.save(path);
}

void test_read_and_parse() {
  const char *grammars[] = {
      "hello.json", "json.json", "c.json", "html.json", "javascript.json", "coffee-script.json"
      // "php.json",
      "sql.json", "text.json", "ruby.json", "scss.json", "c-plus-plus.json", 0};

  for (int i = 0;; i++) {
    if (grammars[i] == 0) {
      break;
    }
    std::string path = "test-cases/first-mate/fixtures/";
    path += grammars[i];
    read_and_parse_a_grammar(path.c_str(), grammars[i]);
  }
}

void test_hello() {
  grammar_ptr gm;
  gm = load("test-cases/first-mate/fixtures/hello.json");
  // gm.dump();

// 01234 6789Xab
// hello world!

  const char *first = "hello world!";
  const char *last = first + strlen(first);

  std::map<size_t, scope::scope_t> scopes;
  stack_ptr stack = parse::parse(first, last, gm->seed(), scopes, true);
}

void test_coffee() {
  grammar_ptr gm;
  gm = load("test-cases/suite1/fixtures/coffee-script.json");
  // gm.dump();

  Json::Value tests = loadJson("test-cases/first-mate/tests.json");
  for (int i = 0; i < (int)tests.size(); i++) {
    Json::Value t = tests[i];
    std::string scopeName = t["grammarScopeName"].asString();
      // std::cout << scopeName << std::endl;
      // continue;

    if (scopeName == "source.c") {
      std::cout << t << std::endl;
      continue;

      Json::Value lines = t["lines"];

      for (int j = 0; j < (int)lines.size(); j++) {
        Json::Value tl = lines[j];
        if (!tl.null) {
          std::string str = tl["line"].asString();
          std::cout << tl << std::endl;
        }
      }

    }
  }
}

void test_c() {
  grammar_ptr gm;
  gm = load("test-cases/first-mate/fixtures/c.json");
  // gm.dump();

  FILE *fp = fopen("tests/cases/test.c", "r");
  char str[1024];
  while(fgets(str, 1000, fp)) {


  const char *first = str;
  const char *last = first + strlen(first);

  std::map<size_t, scope::scope_t> scopes;
  stack_ptr stack = parse::parse(first, last, gm->seed(), scopes, true);

    std::cout << str << std::endl;
    // gm.tokenizeLine(str, false);
  }

  fclose(fp);
}

int main(int argc, char **argv) {
  // test_read_and_parse();
  // test_hello();
  // test_coffee();
  test_c();
  return 0;
}