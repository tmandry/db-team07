#include <boost/test/unit_test.hpp>
#include <iostream>
#include "database.h"

BOOST_AUTO_TEST_SUITE(database_test)

BOOST_AUTO_TEST_CASE(table_except_test)
{
	Database a;
	BOOST_CHECK_THROW(a.table("SHOULDNOTEXIST"), TableDoesNotExistError)
}
BOOST_AUTO_TEST_CASE(add_test)
{
	Database a;
	//Create a simple table to add
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::integer));
	Table* test = new Table(t);
	a.add_table("Test", test);
	BOOST_CHECK(a.table("Test") == test);	//See if table was added
}

BOOST_AUTO_TEST_CASE(add_except_test)
{
	Database a;
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::varchar));
	Table* test = new Table(t);
	a.add_table("Test", test);
	BOOST_CHECK_THROW(a.add_table("Test", test), InvalidOperationError);
}

BOOST_AUTO_TEST_CASE(ifexists_test)
{
	Database a;
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::varchar));
	Table* test = new Table(t);
	a.add_table("Test", test);
	BOOST_CHECK(a.table_if_exists("Test") == test);
	BOOST_CHECK(a.table_if_exists("DNE") == NULL);
}
BOOST_AUTO_TEST_CASE(drop_test)
{
	Database a;
	//Create a simple table to add
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::integer));
	Table* test = new Table(t);
	Table* test2 = new Table(t);
	a.add_table("Test2", test2);
	a.add_table("Test", test);
	a.drop_table("Test");		//Drop a Table
	std::vector<std::string> comp;
	comp.push_back("Test2");
	BOOST_CHECK(a.table_names() == comp);
}

BOOST_AUTO_TEST_CASE(drop_except_test)
{
	Database a;
	BOOST_CHECK_THROW(a.drop_table("Test"), TableDoesNotExistError);
}


BOOST_AUTO_TEST_CASE(add_complex_test)
{
	Database a;
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::integer));
	//Make table more complex
	Table* test = new Table(t);
	test->add_column("A", Table::floating);
	test->add_column("DF", Table::varchar);
	//Make Database more complex
	a.add_table("Test", test);
	Table* test2 = new Table(t);
	a.add_table("T2", test2);
	t.push_back(make_pair("E", Table::integer));
	Table* test3 = new Table(t);
	a.add_table("T3", test3);
	//Check to see if tables are added properly
	BOOST_CHECK(a.table("Test") == test);
	BOOST_CHECK(a.table("T2") == test2);
	BOOST_CHECK(a.table("T3") == test3);
}

BOOST_AUTO_TEST_CASE(drop_complex_test)
{

	Database a;
	Table::ColumnList t;
	t.push_back(make_pair("T", Table::integer));
	//Make table more complex
	Table* test = new Table(t);
	test->add_column("A", Table::floating);
	test->add_column("DF", Table::varchar);
	//Make Database more complex
	a.add_table("Test", test);
	Table* test2 = new Table(t);
	a.add_table("T2", test2);
	t.push_back(make_pair("E", Table::integer));
	Table* test3 = new Table(t);
	a.add_table("T3", test3);
	//Drop one and check for effect on others
	a.drop_table("Test");
	BOOST_CHECK(a.table("T2") == test2);
	BOOST_CHECK(a.table("T3") == test3);
	//Build vector to test against
	std::vector<std::string> comp;
	comp.push_back("T2");
	comp.push_back("T3");
	BOOST_CHECK(a.table_names() == comp);	//Check to see if table names returns properly
	BOOST_CHECK(a.table_if_exists("Test") == NULL);
	BOOST_CHECK(a.table_if_exists("T2") == test2);
}

BOOST_AUTO_TEST_CASE(save_load_test)
{
	Database a;
	Table* t1 = new Table();
	Table* t2 = new Table();
	t1->add_column("Test", Table::date);
	t1->add_column("T2", Table::integer);
	t1->add_column("T3", Table::varchar);
	t2->add_column("U1", Table::integer);
	t2->add_column("U2", Table::varchar);
	t2->add_column("U3", Table::integer);
	a.add_table("Test1", t1);
	a.add_table("Test2", t2);
	a.save("Test.a");
	Database b;
	b.load("Test.a");
	BOOST_REQUIRE(a.table_names() == b.table_names());
}

BOOST_AUTO_TEST_CASE(load_except_test)
{
	Database a;
	BOOST_CHECK_THROW(a.load("fileshouldnotexist.eeee"), IOError);
}

BOOST_AUTO_TEST_CASE(merge_test)
{
	//First database
	Database a;
	Table* t1 = new Table();
	Table* t2 = new Table();
	t1->add_column("Test", Table::date);
	t1->add_column("T2", Table::integer);
	t1->add_column("T3", Table::varchar);
	t2->add_column("U1", Table::integer);
	t2->add_column("U2", Table::varchar);
	t2->add_column("U3", Table::integer);
	a.add_table("Test", t1);
	a.add_table("SecondTest", t2);
	//Second database
	Database b;
	Table* u1 = new Table();
	Table* u2 = new Table();
	u1->add_column("Whee", Table::date);
	u1->add_column("W2", Table::integer);
	u1->add_column("W3", Table::varchar);
	u2->add_column("V1", Table::integer);
	u2->add_column("V2", Table::varchar);
	u2->add_column("V3", Table::integer);
	b.add_table("Test2", u1);
	b.add_table("Test3", u2);
	//Merge databases and see if new tables exist
	a.merge(b);
	BOOST_CHECK(a.table_if_exists("Test2") != NULL);
	BOOST_CHECK(a.table_if_exists("Test3") != NULL);
	Database *c = a.copy();
	BOOST_CHECK(c->table("Test2") == a.table("Test2"));
}
BOOST_AUTO_TEST_SUITE_END()