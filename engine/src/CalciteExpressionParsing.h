/*
 * CalciteExpressionParsing.h
 *
 *  Created on: Aug 7, 2018
 *      Author: felipe
 */

#ifndef CALCITEEXPRESSIONPARSING_H_
#define CALCITEEXPRESSIONPARSING_H_

#include "cudf/legacy/binaryop.hpp"
#include "cudf/types.h"
#include "gdf_wrapper/gdf_wrapper.cuh"
#include "parser/expression_utils.hpp"
#include <string>
#include <vector>
class blazing_frame;

gdf_binary_operator_exp get_binary_operation(std::string operator_string);

gdf_unary_operator get_unary_operation(std::string operator_string);

size_t get_index(std::string operand_string);

// interprets the expression and if is n-ary and logical, then returns their corresponding binary version
std::string expand_if_logical_op(std::string expression);

std::string clean_calcite_expression(std::string expression);

std::vector<std::string> get_tokens_in_reverse_order(const std::string & expression);

// NOTE call this function after use get_tokens_in_reverse_order ... TODO refactos this approach
void fix_tokens_after_call_get_tokens_in_reverse_order_for_timestamp(
	blazing_frame & inputs, std::vector<std::string> & tokens);

gdf_agg_op get_aggregation_operation(std::string operator_string);

std::string get_string_between_outer_parentheses(std::string operator_string);

gdf_dtype infer_dtype_from_literal(const std::string & token);

gdf_dtype get_output_type_expression(blazing_frame * input, gdf_dtype * max_temp_type, std::string expression);

gdf_dtype get_aggregation_output_type(gdf_dtype input_type, gdf_agg_op aggregation, bool have_groupby);

gdf_dtype get_type_from_string(std::string scalar_string);
gdf_scalar get_scalar_from_string(std::string scalar_string, gdf_dtype type, gdf_dtype_extra_info extra_info);

std::string aggregator_to_string(gdf_agg_op operation);

// takes an expression and given a starting index pointing at either ( or [, it finds the corresponding closing char )
// or ]
int find_closing_char(const std::string & expression, int start);

// takes a comma delimited list of expressions and splits it into separate expressions
// if the flag trim is true, leading and trailing spaces are removed
std::vector<std::string> get_expressions_from_expression_list(std::string & combined_expressions, bool trim = true);

bool is_type_signed(gdf_dtype type);

bool is_type_float(gdf_dtype type);
bool is_type_integer(gdf_dtype type);
bool is_date_type(gdf_dtype type);
gdf_dtype get_output_type(gdf_dtype input_left_type, gdf_dtype input_right_type, gdf_binary_operator_exp operation);
gdf_dtype get_output_type(gdf_dtype input_left_type, gdf_unary_operator operation);

// this function takes two data types and returns the a common data type that the both can be losslessly be converted to
// the function returns true if a common type is possible, or false if there is no common type
// this function assumes that common types are decimal, float, datetime and string. You cannot convert across these
// general types.
void get_common_type(gdf_dtype type1,
	gdf_dtype_extra_info info1,
	gdf_dtype type2,
	gdf_dtype_extra_info info2,
	gdf_dtype & type_out,
	gdf_dtype_extra_info & info_out);

std::string get_named_expression(std::string query_part, std::string expression_name);

bool contains_evaluation(std::string expression);

#endif /* CALCITEEXPRESSIONPARSING_H_ */
