#pragma once

#include <json/json.h>
#include <string>
#include <vector>

#include "grammar.h"
#include "private.h"

namespace parse {

rule_ptr convert_json(Json::Value const &json);

Json::Value rule_to_json(rule_ptr const &rule);

} // namespace parse
/*
Json::Value ruleToJson(Rule *rule);
Json::Value repositoryToJson(Repository *repository);
*/
