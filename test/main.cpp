// Copyright (c) 2022 Dennis Hezel
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include <test.hpp>
#include <test/framework.hpp>
#include <test_cat.hpp>
#include <test_comparison.hpp>
#include <test_constructor.hpp>
#include <test_copy_assignment.hpp>
#include <test_dev10_646556_construct_tuple_from_const.hpp>
#include <test_dev10_661739_tuple_copy_ctors.hpp>
#include <test_dev11_0343056_pair_tuple_ctor_sfinae.hpp>
#include <test_dev11_0607540_pair_tuple_rvalue_references.hpp>
#include <test_move_assignment.hpp>
#include <test_structured_binding.hpp>
#include <test_swap.hpp>

int main()
{
    using namespace test;

    // test_comparison
    run_test<&test_empty_tuple_equality_compare<LambdaTuple>>();
    run_test<&test_empty_tuple_equality_compare<StdTuple>>();
    run_test<&test_equality_compare_value_value<LambdaTuple>>();
    run_test<&test_equality_compare_value_value<StdTuple>>();
    run_test<&test_equality_compare_value_lref<LambdaTuple>>();
    run_test<&test_equality_compare_value_lref<StdTuple>>();
    run_test<&test_equality_compare_value_const_lref<LambdaTuple>>();
    run_test<&test_equality_compare_value_const_lref<StdTuple>>();
    run_test<&test_equality_compare_value_rref<LambdaTuple>>();
    run_test<&test_equality_compare_value_rref<StdTuple>>();
    run_test<test_not_equality_comparable>();

    // test_constructor
    run_test<&test_empty_tuple_default_constructor<LambdaTuple>>();
    run_test<&test_empty_tuple_default_constructor<StdTuple>>();
    run_test<&test_default_constructor<LambdaTuple>>();
    run_test<&test_default_constructor<StdTuple>>();
    run_test<&test_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_lref_arg_constructor<StdTuple>>();
    run_test<&test_const_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_const_lref_arg_constructor<StdTuple>>();
    run_test<&test_rref_arg_constructor<LambdaTuple>>();
    run_test<&test_rref_arg_constructor<StdTuple>>();
    run_test<&test_converting_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_converting_lref_arg_constructor<StdTuple>>();
    run_test<&test_converting_const_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_converting_const_lref_arg_constructor<StdTuple>>();
    run_test<&test_converting_rref_arg_constructor<LambdaTuple>>();
    run_test<&test_converting_rref_arg_constructor<StdTuple>>();
    run_test<&test_copy_constructor<LambdaTuple>>();
    run_test<&test_copy_constructor<StdTuple>>();
    run_test<&test_const_lref_copy_constructor<LambdaTuple>>();
    run_test<&test_const_lref_copy_constructor<StdTuple>>();
    run_test<&test_rref_copy_constructor<LambdaTuple>>();
    run_test<&test_rref_copy_constructor<StdTuple>>();
    run_test<&test_rref_copy_constructor_does_not_move<LambdaTuple>>();
    run_test<&test_rref_copy_constructor_does_not_move<StdTuple>>();
    run_test<&test_implicit_converting_copy_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_copy_constructor<StdTuple>>();
    run_test<&test_implicit_converting_lref__arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_lref__arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_const_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_const_lref_arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_copy_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_copy_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_lref_arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_const_lref_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_const_lref_arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_move_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_move_constructor<StdTuple>>();
    run_test<&test_implicit_converting_rref_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_rref_arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_move_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_move_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_value_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_value_arg_constructor<StdTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_rref_arg_constructor<LambdaTuple>>();
    run_test<&test_implicit_converting_tuple_of_tuple_rref_arg_constructor<StdTuple>>();
    run_test<&test_no_unwrap_reference_wrapper_constructor>();
    run_test<&test_copy_list_initialization>();

    // test_copy_assignment
    run_test<&test_copy_assignment_value_value<LambdaTuple>>();
    run_test<&test_copy_assignment_value_value<StdTuple>>();
    run_test<&test_copy_assignment_const_value_value<LambdaTuple>>();
    run_test<&test_copy_assignment_const_value_value<StdTuple>>();
    run_test<&test_copy_assignment_lref_value<LambdaTuple>>();
    run_test<&test_copy_assignment_lref_value<StdTuple>>();
    run_test<&test_copy_assignment_const_lref_value<LambdaTuple>>();
    run_test<&test_copy_assignment_const_lref_value<StdTuple>>();
    run_test<&test_copy_assignment_value_lref<LambdaTuple>>();
    run_test<&test_copy_assignment_value_lref<StdTuple>>();
    run_test<&test_copy_assignment_const_value_lref<LambdaTuple>>();
    run_test<&test_copy_assignment_const_value_lref<StdTuple>>();
    run_test<&test_copy_assignment_lref_lref<LambdaTuple>>();
    run_test<&test_copy_assignment_lref_lref<StdTuple>>();
    run_test<&test_copy_assignment_const_ref_ref<LambdaTuple>>();
    run_test<&test_copy_assignment_const_ref_ref<StdTuple>>();
    run_test<&test_copy_assignment_rref_lref<LambdaTuple>>();
    run_test<&test_copy_assignment_rref_lref<StdTuple>>();
    run_test<&test_copy_assignment_rref_value<LambdaTuple>>();
    run_test<&test_copy_assignment_rref_value<StdTuple>>();
    run_test<&test_copy_assignment_lref_rref<LambdaTuple>>();
    run_test<&test_copy_assignment_lref_rref<StdTuple>>();

    // test_move_assignment
    run_test<&test_move_assignment_value_value<LambdaTuple>>();
    run_test<&test_move_assignment_value_value<StdTuple>>();
    run_test<&test_move_assignment_const_value_value<LambdaTuple>>();
    run_test<&test_move_assignment_const_value_value<StdTuple>>();
    run_test<&test_move_assignment_lref_value<LambdaTuple>>();
    run_test<&test_move_assignment_lref_value<StdTuple>>();
    run_test<&test_move_assignment_const_lref_value<LambdaTuple>>();
    run_test<&test_move_assignment_const_lref_value<StdTuple>>();
    run_test<&test_move_assignment_value_lref<LambdaTuple>>();
    run_test<&test_move_assignment_value_lref<StdTuple>>();
    run_test<&test_move_assignment_const_value_lref<LambdaTuple>>();
    run_test<&test_move_assignment_const_value_lref<StdTuple>>();
    run_test<&test_move_assignment_lref_lref<LambdaTuple>>();
    run_test<&test_move_assignment_lref_lref<StdTuple>>();
    run_test<&test_move_assignment_const_ref_ref<LambdaTuple>>();
    run_test<&test_move_assignment_const_ref_ref<StdTuple>>();
    run_test<&test_move_assignment_rref_lref<LambdaTuple>>();
    run_test<&test_move_assignment_rref_lref<StdTuple>>();
    run_test<&test_move_assignment_rref_value<LambdaTuple>>();
    run_test<&test_move_assignment_rref_value<StdTuple>>();
    run_test<&test_move_assignment_lref_rref<LambdaTuple>>();
    run_test<&test_move_assignment_lref_rref<StdTuple>>();

    // test_move_assignment
    run_test<&test_swap_value<LambdaTuple>>();
    run_test<&test_swap_value<StdTuple>>();
    run_test<&test_swap_lref<LambdaTuple>>();
    run_test<&test_swap_lref<StdTuple>>();
    run_test<&test_swap_const_lref<LambdaTuple>>();
    run_test<&test_swap_const_lref<StdTuple>>();
    run_test<&test_unswappable_const_lref<LambdaTuple>>();
    run_test<&test_unswappable_const_lref<StdTuple>>();
    run_test<&test_unswappable_immovable<LambdaTuple>>();
    run_test<&test_unswappable_immovable<StdTuple>>();

    // Microsoft STL
    run_test<&test_Dev10_646556_construct_tuple_from_const>();
    run_test<&test_Dev10_661739_tuple_copy_ctors>();
    run_test<&test_Dev11_0343056_pair_tuple_ctor_sfinae>();
    run_test<&test_Dev11_0607540_pair_tuple_rvalue_references>();

    // test_structured_binding
    run_test<&test_value_structured_binding<LambdaTuple>>();
    run_test<&test_value_structured_binding<StdTuple>>();
    run_test<&test_lref_structured_binding<LambdaTuple>>();
    run_test<&test_lref_structured_binding<StdTuple>>();
    run_test<&test_lef_structured_binding_from_const<LambdaTuple>>();
    run_test<&test_lef_structured_binding_from_const<StdTuple>>();
    run_test<&test_rref_structured_binding<LambdaTuple>>();
    run_test<&test_rref_structured_binding<StdTuple>>();
    run_test<&test_rref_structured_binding_from_move<LambdaTuple>>();
    run_test<&test_rref_structured_binding_from_move<StdTuple>>();
    run_test<&test_rref_structured_binding_from_const_move<LambdaTuple>>();
    run_test<&test_rref_structured_binding_from_const_move<StdTuple>>();

    // test
    run_test<&test_constexpr>();
    run_test<&test_concepts>();
    run_test<&test_sizeof>();
    run_test<&test_tuple_element_rref>();
    run_test<&test_forward_as_tuple>();
    run_test<&test_tie>();
    run_test<&test_make_tuple>();

    // test_cat
    run_test<&test_tuple_cat>();

    print_test_results();
    return context.failed_tests;
}