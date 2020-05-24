#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
  const char *grammars[] = {"hello.json",
                            "json.json",
                            "c.json",
                            "html.json",
                            "javascript.json",
                            "coffee-script.json"
                            // "php.json",
                            "sql.json",
                            "text.json",
                            "ruby.json",
                            "scss.json",
                            "c-plus-plus.json",
                            0};

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
  std::cout << gm->document() << std::endl;
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
  std::cout << gm->document() << std::endl;

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

void dump_tokens(std::map<size_t, scope::scope_t> &scopes) {
  std::map<size_t, scope::scope_t>::iterator it = scopes.begin();
  while(it != scopes.end()) {
    size_t n = it->first;
    scope::scope_t scope = it->second;
    std::cout << n << ": " << scope.name << std::endl;
    it++;
  }
}

void test_c() {
  grammar_ptr gm;
  gm = load("test-cases/first-mate/fixtures/c.json");
  std::cout << gm->document() << std::endl;

  return;

  FILE *fp = fopen("tests/cases/test.c", "r");
  // FILE *fp = fopen("tests/cases/tinywl.c", "r");
  char str[1024];
  bool firstLine = true;

  parse::stack_ptr parser_state = gm->seed();
  while (fgets(str, 1000, fp)) {

    const char *first = str;
    const char *last = first + strlen(first);

    std::cout << str << std::endl;

    std::map<size_t, scope::scope_t> scopes;
    // stack_ptr stack 
    parser_state = parse::parse(first, last, parser_state, scopes, firstLine);
    dump_tokens(scopes);
    firstLine = false;
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