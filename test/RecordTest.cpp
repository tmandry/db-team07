#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>
#include "database.h"

static const double TOL = 0.0001;


BOOST_AUTO_TEST_CASE( record_test1 )
{
	// make columns
	Table::ColumnList columns;
	columns.push_back(make_pair("first_name",Table::varchar));
	columns.push_back(make_pair("age",Table::integer));
	columns.push_back(make_pair("birthdate",Table::date));
	columns.push_back(make_pair("weight",Table::floating));
	columns.push_back(make_pair("time_entered",Table::time));

	Table t1(columns);
	vector<string> the_key;
	the_key.push_back("first_name");
	t1.set_key(the_key);

	// add records
	vector<pair<string, string>> rec1;
	rec1.push_back(make_pair("first_name", "Pam"));
	rec1.push_back(make_pair("age", "64"));
	rec1.push_back(make_pair("birthdate", "1948/09/26"));
	rec1.push_back(make_pair("weight", "148.7"));
	rec1.push_back(make_pair("time_entered","03:59:00"));
	Record r1(rec1);
	t1.insert(r1);

	Record temp_rec1 = t1.at(0);
	BOOST_CHECK( temp_rec1.get<string>("first_name") == "Pam");
	BOOST_CHECK( temp_rec1.get<int>("age") == 64);
	BOOST_CHECK( temp_rec1.get<string>("birthdate") == "1948/09/26");
	BOOST_CHECK_CLOSE( temp_rec1.get<float>("weight"), 148.7, TOL);
	BOOST_CHECK( temp_rec1.get<string>("time_entered") == "03:59:00");

	temp_rec1.set("first_name", "Bill");
	temp_rec1.set("age", 21);
	temp_rec1.set("birthdate", "1992/01/01");
	temp_rec1.set("weight", 180.3);
	temp_rec1.set("time_entered", "06:57:00");

	BOOST_CHECK( temp_rec1.get<string>("first_name") == "Bill");
	BOOST_CHECK( temp_rec1.get<int>("age") == 21);
	BOOST_CHECK( temp_rec1.get<string>("birthdate") == "1992/01/01");
	BOOST_CHECK_CLOSE( temp_rec1.get<float>("weight"), 180.3, TOL);
	BOOST_CHECK( temp_rec1.get<string>("time_entered") == "06:57:00");

	// exception testing
	BOOST_CHECK_THROW( temp_rec1.get<string>("last_name"),  ColumnDoesNotExistError);
	BOOST_CHECK_THROW( temp_rec1.set("last_name", "What last name?"), ColumnDoesNotExistError);
	// Record::set was not specified to do any type checking
  // BOOST_CHECK_THROW( temp_rec1.set("birthdate", 11111), InvalidTypeError);

	// this is really all we need since record is tested so much throughout the testing (especially query)
}
