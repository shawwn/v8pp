//
// Copyright (c) 2013-2016 Pavel Medvedev. All rights reserved.
//
// This file is part of v8pp (https://github.com/pmed/v8pp) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include "v8pp/property.hpp"
#include "v8pp/context.hpp"

#include "test.hpp"

namespace {

int get1() { return 0; }
int set1(int) { return 0; }

bool get2(v8::Isolate*) { return false; }
void set2(v8::Isolate*, int) {}

void get3(v8::Local<v8::String>, v8::PropertyCallbackInfo<v8::Value> const&) {}
void set3(v8::Local<v8::String>, v8::Local<v8::Value>, v8::PropertyCallbackInfo<void> const&) {}

struct X
{
	int get1() const { return 0; }
	int set1(int) { return 0; }

	bool get2(v8::Isolate*) { return false; }
	void set2(v8::Isolate*, int) {}

	void get3(v8::Local<v8::String>, v8::PropertyCallbackInfo<v8::Value> const&) {}
	void set3(v8::Local<v8::String>, v8::Local<v8::Value>, v8::PropertyCallbackInfo<void> const&) {}
};

// external accessors
int external_get1(const X&) { return 0; }
int external_set1(X&, int) { return 0; }

bool external_get2(const X&, v8::Isolate*) { return false; }
void external_set2(X&, v8::Isolate*, int) {}

void external_get3(const volatile X&, v8::Local<v8::String>, v8::PropertyCallbackInfo<v8::Value> const&) {}
void external_set3(volatile X&, v8::Local<v8::String>, v8::Local<v8::Value>, v8::PropertyCallbackInfo<void> const&) {}

using namespace v8pp::detail;

//property metafunctions
static_assert(is_getter<decltype(&get1), 0>::value,
	"getter function");
static_assert(is_getter<decltype(&X::get1), 0>::value,
	"getter member function");
static_assert(is_getter<decltype(&external_get1), 1>::value,
	"getter external function");

static_assert(is_setter<decltype(&set1), 0>::value,
	"setter function");
static_assert(is_setter<decltype(&X::set1), 0>::value,
	"setter member function");
static_assert(is_setter<decltype(&external_set1), 1>::value,
	"setter external function");

static_assert(is_isolate_getter<decltype(&get2), 0>::value,
	"isolate getter function");
static_assert(is_isolate_getter<decltype(&X::get2), 0>::value,
	"isolate getter member function");
static_assert(is_isolate_getter<decltype(&external_get2), 1>::value,
	"isolate getter external function");

static_assert(is_isolate_setter<decltype(&set2), 0>::value,
	"isolate setter function");
static_assert(is_isolate_setter<decltype(&X::set2), 0>::value,
	"isolate setter member function");
static_assert(is_isolate_setter<decltype(&external_set2), 1>::value,
	"isolate setter external function");

static_assert(is_direct_getter<decltype(&get3), 0>::value,
	"direct getter function");
static_assert(is_direct_getter<decltype(&X::get3), 0>::value,
	"direct getter member function");
static_assert(is_direct_getter<decltype(&external_get3), 1>::value,
	"direct getter external function");

static_assert(is_direct_setter<decltype(&set3), 0>::value,
	"direct setter function");
static_assert(is_direct_setter<decltype(&X::set3), 0>::value,
	"direct setter member function");
static_assert(is_direct_setter<decltype(&external_set3), 1>::value,
	"direct setter external function");

template<typename Get, typename Set>
void test_property(Get&& get, Set&& set)
{
	v8pp::property<Get, Set, v8pp::detail::none, v8pp::detail::none> prop(get, set);
	check_eq("prop is_readonly", prop.is_readonly, false);
	check_eq("prop getter", prop.getter, get);
	check_eq("prop setter", prop.setter, set);
}

} // namespace {

void test_property()
{
	test_property(get1, set1);
	test_property(get2, set2);
	test_property(get3, set3);
	test_property(external_get1, external_set1);
	test_property(external_get2, external_set2);
	test_property(external_get3, external_set3);
}
