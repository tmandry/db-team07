#include <boost/test/unit_test.hpp>
#include <iostream>
#include "database.h"

static const double TOL = 0.0001;

//CONSTRUCTOR TESTS
//Default
BOOST_AUTO_TEST_CASE(default_test)
{
	Table t;
	BOOST_CHECK(t.size() == 0);
}

//With Column List
BOOST_AUTO_TEST_CASE(_constructor_no_columns_test)
{
	Table::ColumnList c;
	Table t(c);
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(constructor_one_column_test)
{
	Table::ColumnList c;
	c.push_back(make_pair("aaaa", Table::integer));
	Table t(c);
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(constructor_many_columns_test)
{

	Table::ColumnList c;
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	BOOST_CHECK(t.columns() == c);
}

//ADD_COLUMN TESTS
BOOST_AUTO_TEST_CASE(add_column_integer)
{
	Table t;
	Table::ColumnList c;
	t.add_column("test", Table::integer);
	c.push_back(make_pair("test", Table::integer));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(add_column_floating)
{
	Table t;
	Table::ColumnList c;
	t.add_column("test", Table::floating);
	c.push_back(make_pair("test", Table::floating));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(add_column_varchar)
{
	Table t;
	Table::ColumnList c;
	t.add_column("test", Table::varchar);
	c.push_back(make_pair("test", Table::varchar));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(add_column_date)
{
	Table t;
	Table::ColumnList c;
	t.add_column("test", Table::date);
	c.push_back(make_pair("test", Table::date));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(add_column_time)
{
	Table t;
	Table::ColumnList c;
	t.add_column("test", Table::time);
	c.push_back(make_pair("test", Table::time));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(add_column_test_nulls)
{
	Table::ColumnList c;
	c.push_back(make_pair("ID", Table::integer));
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	vector<string> name;
	name.push_back("ID");
	t.set_key(name);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	t.add_column("cccc", Table::integer);
	for (int i = 0; i < t.size(); i++) {
		Record r = t.at(i);
		BOOST_CHECK(r.get<int>("cccc") == NULL);
	}
}

//DEL_COLUMN TESTS
BOOST_AUTO_TEST_CASE(delete_column_size)
{
	Table::ColumnList c;
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	t.del_column("bbbb");
	c.erase(c.begin()+1);
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(delete_column_check_records)
{
	Table::ColumnList c;
	c.push_back(make_pair("ID", Table::integer));
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	t.del_column("bbbb");
	for (int i = 0; i < t.size(); i++) {
		Record r = t.at(i);
		BOOST_CHECK_THROW(r.get<int>("bbbb"), ColumnDoesNotExistError);
	}
}

BOOST_AUTO_TEST_CASE(delete_column_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	BOOST_CHECK_THROW(t.del_column("blah"), ColumnDoesNotExistError);
}

//RENAME_COLUMN TESTS
BOOST_AUTO_TEST_CASE(rename_check)
{
	Table t;
	t.add_column("test", Table::date);
	t.rename_column("test", "new test");
	Table::ColumnList c;
	c.push_back(make_pair("new test", Table::date));
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(rename_exception)
{
	Table t;
	t.add_column("test", Table::date);
	BOOST_CHECK_THROW(t.rename_column("blah", "new test"), ColumnDoesNotExistError);
}

//COLUMNS TESTS
BOOST_AUTO_TEST_CASE(columns_check_from_constructor)
{
	Table::ColumnList c;
	c.push_back(make_pair("test", Table::integer));
	Table t(c);
	BOOST_CHECK(t.columns() == c);
}

BOOST_AUTO_TEST_CASE(columns_check_from_add)
{
	Table t;
	t.add_column("test", Table::date);
	Table::ColumnList c;
	c.push_back(make_pair("test", Table::date));
	BOOST_CHECK(t.columns() == c);
}

//INDEX_FOR TESTS
/*
BOOST_AUTO_TEST_CASE(index_for_works)
{
	Table::ColumnList c;
	c.push_back(make_pair("test", Table::integer));
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	unsigned int i = t.index_for("test");
	BOOST_CHECK(i == unsigned int(0));
	i = t.index_for("aaaa");
	BOOST_CHECK(i == unsigned int(1));
	i = t.index_for("bbbb");
	BOOST_CHECK(i == unsigned int(2));
}

BOOST_AUTO_TEST_CASE(index_for_exception)
{
	Table::ColumnList c;
	c.push_back(make_pair("test", Table::integer));
	c.push_back(make_pair("aaaa", Table::integer));
	c.push_back(make_pair("bbbb", Table::integer));
	Table t(c);
	BOOST_CHECK_THROW(t.index_for("blah"), ColumnDoesNotExistError);
}
*/

//SET_KEY TESTS
BOOST_AUTO_TEST_CASE(set_key_column_exception)
{
	Table t = Table();
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("blah");
	BOOST_CHECK_THROW(t.set_key(names), ColumnDoesNotExistError);
}

BOOST_AUTO_TEST_CASE(set_key_invalid_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	vector<string> keys;
	keys.push_back("aaaa");
	BOOST_CHECK_THROW(t.set_key(keys), InvalidOperationError);
}

//KEY TESTS
BOOST_AUTO_TEST_CASE(key_none)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	BOOST_CHECK(t.key().size() == 0);
}
BOOST_AUTO_TEST_CASE(key_works)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	BOOST_CHECK(t.key() == names);
}


//SIZE TESTS
BOOST_AUTO_TEST_CASE(size_test_none)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	BOOST_CHECK(t.size() == 0);
}

BOOST_AUTO_TEST_CASE(size_test_one)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.size() == 1);
}

BOOST_AUTO_TEST_CASE(size_test_many)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	BOOST_CHECK(t.size() == 2);
}

//INSERT TESTS
BOOST_AUTO_TEST_CASE(insert_all_nulls)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	Record r1;
	BOOST_CHECK(t.size() == 0);
	BOOST_CHECK(t.count("ID") == 0);
	BOOST_CHECK(t.count("aaaa") == 0);
	BOOST_CHECK(t.count("bbbb") == 0);
	//BOOST_CHECK(t.first() == r1);
	//BOOST_CHECK(t.last() == r1);
}


// You can't insert records without values for a column according to the api,
// so these tests don't make sense
// BOOST_AUTO_TEST_CASE(insert_some_nulls)
// {
// 	Table t;
// 	t.add_column("ID", Table::integer);
// 	t.add_column("aaaa", Table::integer);
// 	t.add_column("bbbb", Table::integer);
// 	vector<string> names;
// 	names.push_back("ID");
// 	t.set_key(names);
// 	vector<pair<string, string> > v1;
// 	v1.push_back(make_pair("ID", "1"));
// 	Record r1(v1);
// 	t.insert(r1);
// 	BOOST_CHECK(t.size() == 1);
// 	BOOST_CHECK(t.count("ID") == 1);
// 	BOOST_CHECK(t.count("aaaa") == 0);
// 	BOOST_CHECK(t.count("bbbb") == 0);
// 	//BOOST_CHECK(t.first() == r1);
// 	//BOOST_CHECK(t.last() == r1);
// 	vector<pair<string, string> > v2;
// 	v2.push_back(make_pair("ID", "1"));
// 	v2.push_back(make_pair("aaaa", "1"));
// 	Record r2(v2);
// 	t.insert(r2);
// 	BOOST_CHECK(t.size() == 2);
// 	BOOST_CHECK(t.count("ID") == 2);
// 	BOOST_CHECK(t.count("aaaa") == 1);
// 	BOOST_CHECK(t.count("bbbb") == 0);
// 	//BOOST_CHECK(t.last() == r2);
// }

BOOST_AUTO_TEST_CASE(insert_no_nulls)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.size() == 1);
	BOOST_CHECK(t.count("ID") == 1);
	BOOST_CHECK(t.count("aaaa") == 1);
	BOOST_CHECK(t.count("bbbb") == 1);
	//BOOST_CHECK(t.first() == r1);
	//BOOST_CHECK(t.last() == r1);
}

BOOST_AUTO_TEST_CASE(insert_key_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
  Record r1(v1);
	t.insert(r1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "1"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	BOOST_CHECK_THROW(t.insert(r2), KeyConflictError);
}

//BEGIN TESTS
BOOST_AUTO_TEST_CASE(begin_test)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	Record test = *t.begin();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	test = *t.begin();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
}

//END TESTS
BOOST_AUTO_TEST_CASE(end_test)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	Record test = *t.begin();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
  // you can't derefernece t.end(), t.end() points to the point in memory AFTER the last record
	// test = *t.end();
	// BOOST_CHECK(test.get<int>("ID") == 2);
	// BOOST_CHECK(test.get<int>("aaaa") == 2);
	// BOOST_CHECK(test.get<int>("bbbb") == 2);
}

//FIRST TESTS
BOOST_AUTO_TEST_CASE(first_test)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	Record test = t.first();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	test = t.first();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
}

//LAST TESTS
BOOST_AUTO_TEST_CASE(last_test)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	Record test = t.last();
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	test = t.last();
	BOOST_CHECK(test.get<int>("ID") == 2);
	BOOST_CHECK(test.get<int>("aaaa") == 2);
	BOOST_CHECK(test.get<int>("bbbb") == 2);
}

//AT TESTS
BOOST_AUTO_TEST_CASE(at_test)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	Record test = t.at(0);
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	test = t.at(0);
	BOOST_CHECK(test.get<int>("ID") == 1);
	BOOST_CHECK(test.get<int>("aaaa") == 1);
	BOOST_CHECK(test.get<int>("bbbb") == 1);
	test = t.at(1);
	BOOST_CHECK(test.get<int>("ID") == 2);
	BOOST_CHECK(test.get<int>("aaaa") == 2);
	BOOST_CHECK(test.get<int>("bbbb") == 2);
}

BOOST_AUTO_TEST_CASE(at_test_exceptions)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK_THROW(t.at(t.size()), RowDoesNotExistError);
	BOOST_CHECK_THROW(t.at(-1), RowDoesNotExistError);
}

//COUNT TESTS
BOOST_AUTO_TEST_CASE(count_test_none)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	BOOST_CHECK(t.count("ID") == 0);
	BOOST_CHECK(t.count("aaaa") == 0);
	BOOST_CHECK(t.count("bbbb") == 0);
}

// You can't insert records without values for a column according to the api,
// so these tests don't make sense
// BOOST_AUTO_TEST_CASE(count_test_nulls)
// {
// 	Table t;
// 	t.add_column("ID", Table::integer);
// 	t.add_column("aaaa", Table::integer);
// 	t.add_column("bbbb", Table::integer);
// 	vector<string> names;
// 	names.push_back("ID");
// 	t.set_key(names);
// 	Record r1;
// 	t.insert(r1);
// 	BOOST_CHECK(t.count("ID") == 0);
// 	BOOST_CHECK(t.count("aaaa") == 0);
// 	BOOST_CHECK(t.count("bbbb") == 0);
// 	vector<pair<string, string> > v2;
// 	v2.push_back(make_pair("ID", "1"));
// 	Record r2(v2);
// 	t.insert(r2);
// 	BOOST_CHECK(t.count("ID") == 1);
// 	BOOST_CHECK(t.count("aaaa") == 0);
// 	BOOST_CHECK(t.count("bbbb") == 0);
// 	vector<pair<string, string> > v3;
// 	v3.push_back(make_pair("ID", "1"));
// 	v3.push_back(make_pair("aaaa", "1"));
// 	Record r3(v3);
// 	t.insert(r3);
// 	BOOST_CHECK(t.count("ID") == 2);
// 	BOOST_CHECK(t.count("aaaa") == 1);
// 	BOOST_CHECK(t.count("bbbb") == 0);
// }

BOOST_AUTO_TEST_CASE(count_test_regular)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.count("ID") == 1);
	BOOST_CHECK(t.count("aaaa") == 1);
	BOOST_CHECK(t.count("bbbb") == 1);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "2"));
	v2.push_back(make_pair("bbbb", "2"));
	Record r2(v2);
	t.insert(r2);
	BOOST_CHECK(t.count("ID") == 2);
	BOOST_CHECK(t.count("aaaa") == 2);
	BOOST_CHECK(t.count("bbbb") == 2);
}

BOOST_AUTO_TEST_CASE(count_test_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	BOOST_CHECK_THROW(t.count("blah"), ColumnDoesNotExistError);
}

//SUM TESTS
BOOST_AUTO_TEST_CASE(sum_works)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	t.add_column("cccc", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "2"));
	v1.push_back(make_pair("cccc", "0"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.sum<int>("ID") == 1);
	BOOST_CHECK(t.sum<int>("aaaa") == 1);
	BOOST_CHECK(t.sum<int>("bbbb") == 2);
	BOOST_CHECK(t.sum<int>("cccc") == 0);
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("aaaa", "0"));
	v2.push_back(make_pair("bbbb", "-1"));
	v2.push_back(make_pair("cccc", "0"));
	Record r2(v2);
	t.insert(r2);
	BOOST_CHECK(t.sum<int>("ID") == 3);
	BOOST_CHECK(t.sum<int>("aaaa") == 1);
	BOOST_CHECK(t.sum<int>("bbbb") == 1);
	BOOST_CHECK(t.sum<int>("cccc") == 0);
}

BOOST_AUTO_TEST_CASE(sum_column_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::integer);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "1"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK_THROW(t.sum<int>("blah"), ColumnDoesNotExistError);
}

BOOST_AUTO_TEST_CASE(sum_numeric_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::varchar);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "blah"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK_THROW(t.sum<int>("aaaa"), InvalidOperationError);
}

BOOST_AUTO_TEST_CASE(sum_type_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("aaaa", Table::date);
	t.add_column("bbbb", Table::integer);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("aaaa", "2013/02/01"));
	v1.push_back(make_pair("bbbb", "1"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK_THROW(t.sum<char>("aaaa"), InvalidOperationError);
}

//MIN TESTS
BOOST_AUTO_TEST_CASE(min_no_entries)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK(t.min<int>("ID") == NULL);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
  // you cannot insert records that do not have the same columns as the table
  // *Record r1;
	// t.insert(r1);
	// BOOST_CHECK(t.min<int>("ID") == NULL);
}

BOOST_AUTO_TEST_CASE(min_works)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("float", "2.5"));
	v1.push_back(make_pair("vchar", "blah"));
	v1.push_back(make_pair("date", "2013/02/01"));
	v1.push_back(make_pair("time", "04:20:01"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.min<int>("ID") == 1);
	BOOST_CHECK_CLOSE(t.min<float>("float"), 2.5, TOL);
	BOOST_CHECK(t.min<string>("vchar") == "blah");
	BOOST_CHECK(t.min<string>("date") == "2013/02/01");
	BOOST_CHECK(t.min<string>("time") == "04:20:01");
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("float", "1.5"));
	v2.push_back(make_pair("vchar", "all"));
	v2.push_back(make_pair("date", "2012/01/02"));
	v2.push_back(make_pair("time", "05:20:01"));
	Record r2(v2);
	t.insert(r2);
	BOOST_CHECK(t.min<int>("ID") == 1);
	BOOST_CHECK_CLOSE(t.min<float>("float"), 1.5, TOL);
	BOOST_CHECK(t.min<string>("vchar") == "all");
	BOOST_CHECK(t.min<string>("date") == "2012/01/02");
	BOOST_CHECK(t.min<string>("time") == "04:20:01");
}

BOOST_AUTO_TEST_CASE(min_column_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK_THROW(t.min<int>("test"), ColumnDoesNotExistError);
}

BOOST_AUTO_TEST_CASE(min_type_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK_THROW(t.min<char>("date"), InvalidTypeError);
}

//MAX TESTS
BOOST_AUTO_TEST_CASE(max_no_entries)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK(t.min<int>("ID") == NULL);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
  // you cannot insert records that do not have the same columns as the table
  // *Record r1;
	// t.insert(r1);
	// BOOST_CHECK(t.max<int>("ID") == NULL);
}

BOOST_AUTO_TEST_CASE(max_works)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	vector<string> names;
	names.push_back("ID");
	t.set_key(names);
	vector<pair<string, string> > v1;
	v1.push_back(make_pair("ID", "1"));
	v1.push_back(make_pair("float", "2.5"));
	v1.push_back(make_pair("vchar", "blah"));
	v1.push_back(make_pair("date", "2013/02/01"));
	v1.push_back(make_pair("time", "04:20:01"));
	Record r1(v1);
	t.insert(r1);
	BOOST_CHECK(t.max<int>("ID") == 1);
	BOOST_CHECK_CLOSE(t.max<float>("float"), 2.5, TOL);
	BOOST_CHECK(t.max<string>("vchar") == "blah");
	BOOST_CHECK(t.max<string>("date") == "2013/02/01");
	BOOST_CHECK(t.max<string>("time") == "04:20:01");
	vector<pair<string, string> > v2;
	v2.push_back(make_pair("ID", "2"));
	v2.push_back(make_pair("float", "1.5"));
	v2.push_back(make_pair("vchar", "all"));
	v2.push_back(make_pair("date", "2012/01/02"));
	v2.push_back(make_pair("time", "05:20:01"));
	Record r2(v2);
	t.insert(r2);
	BOOST_CHECK(t.max<int>("ID") == 2);
	BOOST_CHECK_CLOSE(t.max<float>("float"), 2.5, TOL);
	BOOST_CHECK(t.max<string>("vchar") == "blah");
	BOOST_CHECK(t.max<string>("date") == "2013/02/01");
	BOOST_CHECK(t.max<string>("time") == "05:20:01");
}

BOOST_AUTO_TEST_CASE(max_column_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK_THROW(t.max<int>("test"), ColumnDoesNotExistError);
}

BOOST_AUTO_TEST_CASE(max_type_exception)
{
	Table t;
	t.add_column("ID", Table::integer);
	t.add_column("float", Table::floating);
	t.add_column("vchar", Table::varchar);
	t.add_column("date", Table::date);
	t.add_column("time", Table::time);
	BOOST_CHECK_THROW(t.min<char>("date"), InvalidTypeError);
}
