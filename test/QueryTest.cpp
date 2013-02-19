#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>
#include "database.h"

static const double TOL = 0.0001;

BOOST_AUTO_TEST_CASE( query_test1 )
{
	/*
		In this test case we will make a small 2x2 table by first making 2
		records and then pushing them into the table
		Then we will run multiple queries on this table and test
		that the queries produced the correct output
	*/
	Database d;

	// make a new table and add some columns to it and set the key
	Table* t1 = new Table();
	t1->add_column("first_column", Table::varchar);
	t1->add_column("second_column", Table::varchar);
	vector<string> the_key;
	the_key.push_back("first_column");
	t1->set_key(the_key);

	// make some records by making a vector of pairs and then insert them
	vector<pair<string, string> > ent1;
	ent1.push_back(make_pair("first_column","c1_r1"));
	ent1.push_back(make_pair("second_column","c1_r2"));
	Record r1(ent1);
	t1->insert(r1);
	vector<pair<string, string> > ent2;
	ent2.push_back(make_pair("first_column","c1_r2"));
	ent2.push_back(make_pair("second_column","c2_r2"));
	Record r2(ent2);
	t1->insert(r2);
	d.add_table("table1",t1);

	/* table1 looks like:
			first_column	second_column
		r1	c1_r1			c2_r1
		r2	c1_r2			c2_r2

		Now on this table we run a query just getting the top left hand entry of table
	*/

	Table* query_table1 = d.query("first_column","table1","first_column = 'c1_r1'");
	/* table from query:
		first_column
	r1	c1_r1

	*/

	Record query_table1_rec = query_table1->at(0);
	BOOST_CHECK( query_table1->size() == 1 );
	BOOST_CHECK( query_table1->count("first_column") == 1 );
	BOOST_CHECK( query_table1_rec.get<string>("first_column") ==  "c1_r1");

	/*
		we run another query on table1 to get the whole first column of table1
	*/

	Table* query2_table = d.query("first_column","table1","first_column = 'c1_r1' OR first_column = 'c1_r2'");
	/* table from query:
		first_column
	r1	c1_r1
	r2	c1_r2
	*/
	Record query2_table_rec1 = query2_table->at(0);
	Record query2_table_rec2 = query2_table->at(1);
	BOOST_CHECK( query2_table->size() == 2 );
	BOOST_CHECK( query2_table->count("first_column") == 2 );
	BOOST_CHECK( query2_table_rec1.get<string>("first_column") == "c1_r1");
	BOOST_CHECK( query2_table_rec2.get<string>("first_column") == "c1_r2");

	/*
		now we run a query on the table created by query2 to get the top left hand entry
	*/
	Table* query3_table = d.query("first_column","query2_table","first_column = 'c1_r1'");
	/* table from query:
		first_column
	r1	c1_r1
	*/
	Record query3_table_rec1 = query3_table->at(0);
	BOOST_CHECK(query3_table->size() == 1);
	BOOST_CHECK(query3_table->count("first_column") == 1);
	BOOST_CHECK(query3_table_rec1.get<string>("first_column") == "c1_r1");

	/***********
	Delete from and update will be tested from here until the next test block,
	since we can't compare the entry we just deleted to anything, I will check the
	next size (number of rows) in the table after deleting

	Delete from will be tested on table1 which still looks like:
			first_column	second_column
		r1	c1_r1			c2_r1
		r2	c1_r2			c2_r2
	*****/
	d.delete_from("table1", "second_column = 'c2_r2'");
	BOOST_CHECK(d.table("table1")->size() == 1);

	// wsine we just deleted the second row the only thing we can update is the 1st row
	d.update("table1", "first_column = 'c1_r1'", "second_column = 'c02_r01'");
	Record update_check = d.table("table1")->at(0);
	BOOST_CHECK(update_check.get<string>("second_column") == "c02_r01");


	//// Exception testing
	BOOST_CHECK_THROW(d.query("column1", "tabledne", "age > 2"), TableDoesNotExistError);
	BOOST_CHECK_THROW(d.delete_from("tabledne", "age > 2"), TableDoesNotExistError);
	BOOST_CHECK_THROW(d.update("column1", "tabledne", "age > 2"), TableDoesNotExistError);

	BOOST_CHECK_THROW(d.query("first_column", "table1", "age >><<< 2"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.delete_from("table1", "first_column ===! 2"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.update("table1", "first_column() = 2", "second_column = ()'abc'"), QuerySyntaxError);

}

BOOST_AUTO_TEST_CASE( query_test2 )
{
	/*
		Now we will make a 3x3 table to run queries on
		with the dates in the table being the same
	*/

	Database d;
	// make columns
	Table::ColumnList columns;
	columns.push_back(make_pair("column1",Table::integer));
	columns.push_back(make_pair("column2",Table::varchar));
	columns.push_back(make_pair("column3",Table::date));

	// make table and set the key
	Table *t1 = new Table(columns);
	vector<string> the_key;
	the_key.push_back("column2");
	t1->set_key(the_key);

	// add records
	vector<pair<string, string>> rec1;
	rec1.push_back(make_pair("column1", "1"));
	rec1.push_back(make_pair("column2", "record1"));
	rec1.push_back(make_pair("column3", "2013/01/01"));
	Record r1(rec1);
	t1->insert(r1);

	vector<pair<string, string>> rec2;
	rec1.push_back(make_pair("column1", "2"));
	rec1.push_back(make_pair("column2", "record2"));
	rec1.push_back(make_pair("column3", "2013/01/01"));
	Record r2(rec2);
	t1->insert(r2);

	vector<pair<string, string>> rec3;
	rec1.push_back(make_pair("column1", "3"));
	rec1.push_back(make_pair("column2", "record3"));
	rec1.push_back(make_pair("column3", "2013/01/01"));
	Record r3(rec3);
	t1->insert(r3);
	d.add_table("table1", t1);

	/*table 1 looks like:
			column1	column2	column3
		r1	1		record1	2013/01/01
		r2	2		record2	2013/01/01
		r3	3		record3	2013/01/01
	*/
	// the first query will check the dates and if the date is equal to 2013/01/01
	// then it will return all the tables columns of that record
	Table* q1_table1 = d.query("*","table1","column3 = '2013/01/01'");
	/*q1_table1 looks like :
			column1	column2	column3
		r1	1		record1	2013/01/01
		r2	2		record2	2013/01/01
		r3	3		record3	2013/01/01
	*/

	// look at every entry and make sure it matches
	Record q1_table1_r1 = q1_table1->at(0);
	BOOST_CHECK(q1_table1_r1.get<int>("column1") == 1);
	BOOST_CHECK(q1_table1_r1.get<string>("column2") == "record1");
	BOOST_CHECK(q1_table1_r1.get<string>("column3") == "2013/01/01");
	Record q1_table1_r2 = q1_table1->at(1);
	BOOST_CHECK(q1_table1_r2.get<int>("column1") == 2);
	BOOST_CHECK(q1_table1_r2.get<string>("column2") == "record2");
	BOOST_CHECK(q1_table1_r2.get<string>("column3") == "2013/01/01");
	Record q1_table1_r3 = q1_table1->at(2);
	BOOST_CHECK(q1_table1_r3.get<int>("column1") == 3);
	BOOST_CHECK(q1_table1_r3.get<string>("column2") == "record3");
	BOOST_CHECK(q1_table1_r3.get<string>("column3") == "2013/01/01");

	// the nexy query will be on table 1 again, this time we want to return the records
	// where column1 is greater than or equal to 2
	Table* q2_table1 = d.query("*", "table1", "column1 >= 2");
	/*q2_table1 looks like:
		column1	column2	column3
	r1	2		record2	2013/01/01
	r2	3		record3	2013/01/01

	*/

	Record q2_table1_r1 = q2_table1->at(0);
	BOOST_CHECK(q2_table1_r1.get<int>("column1") == 2);
	BOOST_CHECK(q2_table1_r1.get<string>("column2") == "record2");
	BOOST_CHECK(q2_table1_r1.get<string>("column3") == "2013/01/01");
	Record q2_table1_r2 = q2_table1->at(1);
	BOOST_CHECK(q2_table1_r2.get<int>("column1") == 3);
	BOOST_CHECK(q2_table1_r2.get<string>("column2") == "record3");
	BOOST_CHECK(q2_table1_r2.get<string>("column3") == "2013/01/01");

	// the next query on table1 will be to return column1 of table1 unless column2 of table1 equals 'record2'
	Table* q3_table1 = d.query("column1", "table1", "column2 != 'record2'");
	/*q3_table1 looks like:
		column1
	r1	1
	r2	3
	*/

	Record q3_table1_r1 = q3_table1->at(0);
	BOOST_CHECK(q3_table1_r1.get<int>("column1") == 1);
	Record q3_table1_r2 = q3_table1->at(1);
	BOOST_CHECK(q3_table1_r2.get<int>("column1") == 3);

	/****** testing delete and update from here until the next test case on table1
			column1	column2	column3
		r1	1		record1	2013/01/01
		r2	2		record2	2013/01/01
		r3	3		record3	2013/01/01
	**/

	// lets try the first record and last record
	d.delete_from("table1", "column1 < 2 OR column2 = 'record3'");
	BOOST_CHECK(d.table("table1")->size() == 1);

	// then lets update the only existing records date
	d.update("table1", "column2 = 'record2'", "column3 = 2013/02/02");
	Record update_table1_r1 = d.table("table1")->at(0);
	BOOST_CHECK(update_table1_r1.get<string>("column3") == "2013/02/02");

	/// Exception testing
	BOOST_CHECK_THROW(d.query("column1", "table2", "column1 > 1"), TableDoesNotExistError);
	BOOST_CHECK_THROW(d.delete_from("Table1", "column1 > 1"), TableDoesNotExistError);
	BOOST_CHECK_THROW(d.update("table", "column1 > 1", "column2 = 'yolo'"), TableDoesNotExistError);

	BOOST_CHECK_THROW(d.query("column1", "table1", "column1 (>) 1"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.delete_from("Table1", "column1 =! 1"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.update("table", "column1 > 1", "column2 =< 'yolo'"), QuerySyntaxError);
}

BOOST_AUTO_TEST_CASE( query_test3 )
{
	// now we are going to have a 5x5 table (one for each type)
	// that we are going to run a lot of queries on
	Database d;
	// make columns
	Table::ColumnList columns;
	columns.push_back(make_pair("first_name",Table::varchar));
	columns.push_back(make_pair("age",Table::integer));
	columns.push_back(make_pair("birthdate",Table::date));
	columns.push_back(make_pair("weight",Table::floating));
	columns.push_back(make_pair("time_entered",Table::time));

	Table *t1 = new Table(columns);
	vector<string> the_key;
	the_key.push_back("first_name");
	t1->set_key(the_key);

	// add records
	vector<pair<string, string>> rec1;
	rec1.push_back(make_pair("first_name", "Pam"));
	rec1.push_back(make_pair("age", "64"));
	rec1.push_back(make_pair("birthdate", "1948/09/26"));
	rec1.push_back(make_pair("weight", "148.7"));
	rec1.push_back(make_pair("time_entered","03:59:00"));
	Record r1(rec1);
	t1->insert(r1);

	vector<pair<string, string>> rec2;
	rec2.push_back(make_pair("first_name", "George"));
	rec2.push_back(make_pair("age", "33"));
	rec2.push_back(make_pair("birthdate", "1979/10/18"));
	rec2.push_back(make_pair("weight", "170.3"));
	rec2.push_back(make_pair("time_entered","04:02:00"));
	Record r2(rec2);
	t1->insert(r2);

	vector<pair<string, string>> rec3;
	rec3.push_back(make_pair("first_name", "Linda"));
	rec3.push_back(make_pair("age", "59"));
	rec3.push_back(make_pair("birthdate", "1954/02/21"));
	rec3.push_back(make_pair("weight", "133.5"));
	rec3.push_back(make_pair("time_entered","04:03:00"));
	Record r3(rec3);
	t1->insert(r3);

	vector<pair<string, string>> rec4;
	rec4.push_back(make_pair("first_name", "Angela"));
	rec4.push_back(make_pair("age", "53"));
	rec4.push_back(make_pair("birthdate", "1959/06/07"));
	rec4.push_back(make_pair("weight", "140.1"));
	rec4.push_back(make_pair("time_entered","04:05:00"));
	Record r4(rec4);
	t1->insert(r4);

	vector<pair<string, string>> rec5;
	rec5.push_back(make_pair("first_name", "Mildred"));
	rec5.push_back(make_pair("age", "79"));
	rec5.push_back(make_pair("birthdate", "1933/10/05"));
	rec5.push_back(make_pair("weight", "137.2"));
	rec5.push_back(make_pair("time_entered","04:06:00"));
	Record r5(rec5);
	t1->insert(r5);
	d.add_table("table1", t1);

	/*table1 looks like:
		first_name	age		birthdate	weight	time_entered
	r1	Pam			64		1948/09/26	148.7	03:59:00
	r2	George		33		1979/10/18	170.3	04:02:00
	r3	Linda		59		1954/02/21	133.5	04:03:00
	r4	Angela		53		1959/06/07	140.1	04:05:00
	r5	Mildred		79		1933/10/05	137.2	04:06:00
	*/

	// first query we want to see the name of people under 40 AND weigh more than 150
	Table* q1_table1 = d.query("first_name", "table1", "age < 40 AND weight > 150");
	// this table show be one entry (George)
	Record q1_table1_r1 = q1_table1->at(0);
	BOOST_CHECK(q1_table1_r1.get<string>("first_name") == "George");

	// second query we want to people older than 40 but only unless they weight less than 140 OR if the person's name is George
	Table* q2_table1 = d.query("*", "table1", "(age > 40 AND weight < 140) OR first_name = 'George'");
	/*q2_table1 should look like:
		first_name	age		birthdate	weight	time_entered
	r1	George		33		1979/10/18	170.3	04:02:00
	r2	Linda		59		1954/02/21	133.5	04:03:00
	r3	Mildred		79		1933/10/05	137.2	04:06:00
	*/
	Record q2_table1_r1 = q2_table1->at(0);
	BOOST_CHECK(q2_table1_r1.get<string>("first_name") == "George");
	BOOST_CHECK(q2_table1_r1.get<int>("age") == 33);
	BOOST_CHECK(q2_table1_r1.get<string>("birthdate") == "1979/10/18");
	BOOST_CHECK_CLOSE(q2_table1_r1.get<float>("weight"), 170.3, TOL);
	BOOST_CHECK(q2_table1_r1.get<string>("time_entered") == "04:02:00");
	Record q2_table1_r2 = q2_table1->at(1);
	BOOST_CHECK(q2_table1_r2.get<string>("first_name") == "Linda");
	BOOST_CHECK(q2_table1_r2.get<int>("age") == 59);
	BOOST_CHECK(q2_table1_r2.get<string>("birthdate") == "1954/02/21");
	BOOST_CHECK_CLOSE(q2_table1_r2.get<float>("weight"), 133.5, TOL);
	BOOST_CHECK(q2_table1_r2.get<string>("time_entered") == "04:03:00");
	Record q2_table1_r3 = q2_table1->at(2);
	BOOST_CHECK(q2_table1_r3.get<string>("first_name") == "Mildred");
	BOOST_CHECK(q2_table1_r3.get<int>("age") == 79);
	BOOST_CHECK(q2_table1_r3.get<string>("birthdate") == "1933/10/05");
	BOOST_CHECK_CLOSE(q2_table1_r3.get<float>("weight"), 137.2, TOL);
	BOOST_CHECK(q2_table1_r3.get<string>("time_entered") == "04:06:00");

	// the next query we want the name of people who were
	// entered after 04:00:00 AND their bday is before 1960/01/01
	Table* q3_table1 = d.query("first_name", "table1", "time_entered > 04:00:00 AND birtdate < 1960/01/01");
	// this should produce Linda, Angela, and Mildred
	Record q3_table1_r1 = q3_table1->at(0);
	BOOST_CHECK(q3_table1_r1.get<string>("first_name") == "Linda");
	Record q3_table1_r2 = q3_table1->at(1);
	BOOST_CHECK(q3_table1_r2.get<string>("first_name") == "Angela");
	Record q3_table1_r3 = q3_table1->at(2);
	BOOST_CHECK(q3_table1_r3.get<string>("first_name") == "Mildred");

	// complex query time
	// we want users younger than 60 but only if the weight less than 140 (Angela) OR
	// the user was entered before 04:00:00 AND the user was born on 1948/09/26 (Pam) OR
	// the user's name is George
	Table* q4_table1 = d.query("*", "table1",
		"(age < 60 AND weight < 140) OR (time_entered < 04:00:00 AND birthdate = 1948/09/26) OR (first_name='George')");
	/*q4_table1 should look like:
		first_name	age		birthdate	weight	time_entered
	r1	Pam			64		1948/09/26	148.7	03:59:00
	r2	George		33		1979/10/18	170.3	04:02:00
	r3	Angela		53		1959/06/07	140.1	04:05:00
	*/
	Record q4_table1_r1 = q4_table1->at(0);
	BOOST_CHECK(q4_table1_r1.get<string>("first_name") == "Pam");
	BOOST_CHECK(q4_table1_r1.get<int>("age") == 64);
	BOOST_CHECK(q4_table1_r1.get<string>("birthdate") == "1948/09/26");
	BOOST_CHECK_CLOSE(q4_table1_r1.get<float>("weight"), 148.7, TOL);
	BOOST_CHECK(q4_table1_r1.get<string>("time_entered") == "03:59:00");
	Record q4_table1_r2 = q4_table1->at(1);
	BOOST_CHECK(q4_table1_r2.get<string>("first_name") == "George");
	BOOST_CHECK(q4_table1_r2.get<int>("age") == 33);
	BOOST_CHECK(q4_table1_r2.get<string>("birthdate") == "1979/10/18");
	BOOST_CHECK_CLOSE(q4_table1_r2.get<float>("weight"), 170.3, TOL);
	BOOST_CHECK(q4_table1_r2.get<string>("time_entered") == "04:02:00");
	Record q4_table1_r3 = q4_table1->at(2);
	BOOST_CHECK(q4_table1_r3.get<string>("first_name") == "Angela");
	BOOST_CHECK(q4_table1_r3.get<int>("age") == 53);
	BOOST_CHECK(q4_table1_r3.get<string>("birthdate") == "1959/06/07");
	BOOST_CHECK_CLOSE(q4_table1_r3.get<float>("weight"), 140.1, TOL);
	BOOST_CHECK(q4_table1_r3.get<string>("time_entered") == "04:05:00");

	/**********
		now we get to delete and update again, table looks like this for a refresher:
		first_name	age		birthdate	weight	time_entered
	r1	Pam			64		1948/09/26	148.7	03:59:00
	r2	George		33		1979/10/18	170.3	04:02:00
	r3	Linda		59		1954/02/21	133.5	04:03:00
	r4	Angela		53		1959/06/07	140.1	04:05:00
	r5	Mildred		79		1933/10/05	137.2	04:06:00
	*/

	// first lets delete the record if the user was entered before 4:03
	d.delete_from("table1", "time_entered < 04:03:00");
	BOOST_CHECK(d.table("table1")->size() == 3);

	// now let update weight to just be 130 if it is aleady below 140
	d.update("table1", "weight < 140", "weight = 130.0");
	Record update1_table1_r1 = d.table("table1")->at(0);
	BOOST_CHECK_CLOSE(update1_table1_r1.get<float>("weight"), 130.0, TOL);
	Record update1_table1_r2 = d.table("table1")->at(1);
	BOOST_CHECK_CLOSE(update1_table1_r2.get<float>("weight"), 140.1, TOL);
	Record update1_table1_r3 = d.table("table1")->at(2);
	BOOST_CHECK_CLOSE(update1_table1_r3.get<float>("weight"), 130.0, TOL);

	// now lets delete if the weight is equal to 130
	d.delete_from("table1", "weight = 130.0");
	BOOST_CHECK(d.table("table1")->size() == 1); // only Angela should be left

	// exception testing
	BOOST_CHECK_THROW(d.query("first_name", "table1", "weight % 140"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.delete_from("table1", "first_name == 'Angela'"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.update("table1", "weight << 150", "first_name = 'Bill'"), QuerySyntaxError);
}

BOOST_AUTO_TEST_CASE( query_test5 )
{
	// In this we will create 2 small tables with a lot of records
	// so that we can test IN, EXISTS, ALL, ANY
	Database d;
	Table* t1 = new Table();
	t1->add_column("student", Table::varchar);
	t1->add_column("id", Table::integer);
	t1->add_column("gpa", Table::floating);
	vector<string> key;
	key.push_back("column2");
	t1->set_key(key);

	vector<pair<string, string>> rec1;
	rec1.push_back(make_pair("student", "Ruth"));
	rec1.push_back(make_pair("id", "335"));
	rec1.push_back(make_pair("id", "2.7"));
	Record r1(rec1);
	t1->insert(r1);
	vector<pair<string, string>> rec2;
	rec2.push_back(make_pair("student", "Grace"));
	rec2.push_back(make_pair("id", "538"));
	rec2.push_back(make_pair("gpa", "3.5"));
	Record r2(rec2);
	t1->insert(r2);
	vector<pair<string, string>> rec3;
	rec3.push_back(make_pair("student", "Mario"));
	rec3.push_back(make_pair("id", "415"));
	rec3.push_back(make_pair("gpa", "2.7"));
	Record r3(rec3);
	t1->insert(r3);
	vector<pair<string, string>> rec4;
	rec4.push_back(make_pair("student", "Becky"));
	rec4.push_back(make_pair("id", "130"));
	rec4.push_back(make_pair("gpa","3.8"));
	Record r4(rec4);
	t1->insert(r4);
	vector<pair<string, string>> rec5;
	rec5.push_back(make_pair("student", "Thomas"));
	rec5.push_back(make_pair("id", "914"));
	rec5.push_back(make_pair("gpa", "3.6"));
	Record r5(rec5);
	t1->insert(r5);
	vector<pair<string, string>> rec6;
	rec6.push_back(make_pair("student", "Kenneth"));
	rec6.push_back(make_pair("id", "730"));
	rec6.push_back(make_pair("gpa", "3.9"));
	Record r6(rec6);
	t1->insert(r6);
	vector<pair<string, string>> rec7;
	rec7.push_back(make_pair("student", "Johnny"));
	rec7.push_back(make_pair("id", "780"));
	rec7.push_back(make_pair("gpa", "3.4"));
	Record r7(rec7);
	t1->insert(r7);
	vector<pair<string, string>> rec8;
	rec8.push_back(make_pair("student", "Joseph"));
	rec8.push_back(make_pair("id", "971"));
	rec8.push_back(make_pair("gpa", "2.4"));
	Record r8(rec8);
	t1->insert(r8);
	d.add_table("students",t1);
	/* table1
		student	id		gpa
	r1	Ruth	335		2.7
	r2	Grace	538		3.5
	r3	Mario	415		2.7
	r4	Becky	130		3.8
	r5	Thomas	914		3.6
	r6	Kenneth	730		3.9
	r7	Johnny	780		3.4
	r8	Joseph	971		2.4
	*/

	Table* t2 = new Table();
	t2->add_column("id", Table::integer);
	vector<string>key1;
	key1.push_back("id");
	t2->set_key(key1);
	vector<pair<string, string>> rec01;
	rec01.push_back(make_pair("id", "538"));
	Record r01(rec01);
	t2->insert(r01);
	vector<pair<string, string>> rec02;
	rec02.push_back(make_pair("id", "130"));
	Record r02(rec02);
	t2->insert(r02);
	vector<pair<string, string>> rec03;
	rec03.push_back(make_pair("id", "914"));
	Record r03(rec03);
	t2->insert(r03);
	vector<pair<string, string>> rec04;
	rec04.push_back(make_pair("id", "730"));
	Record r04(rec04);
	t2->insert(r04);
	d.add_table("good_students", t2);
	// this table contains id's for Grace, Becky, Thomas, and Kenneth and is only one column

	Table* t3 = new Table();
	t3->add_column("gpa", Table::floating);
	vector<string>key2;
	key2.push_back("gpa");
	t3->set_key(key2);
	vector<pair<string, string>> rec001;
	rec001.push_back(make_pair("gpa", "3.9"));
	Record r001(rec001);
	t3->insert(r001);
	vector<pair<string, string>> rec002;
	rec002.push_back(make_pair("gpa", "3.8"));
	Record r002(rec002);
	t3->insert(r002);
	vector<pair<string, string>> rec003;
	rec003.push_back(make_pair("gpa", "3.6"));
	Record r003(rec003);
	t3->insert(r003);
	d.add_table("good_gpa", t3);
	// this table contains gpa for Becky, Thomas, and Kenneth

	// first we will test the IN function
	// we want the names of the student IN the good_students table
	Table* q1_table1 = d.query("student", "students", "id IN good_students");
	// q1_table will be Grace, Becky, Thomas, and Kenneth since their id is in the good_students table

	Record q1_table1_r1 = q1_table1->at(0);
	BOOST_CHECK(q1_table1_r1.get<string>("student") == "Grace");
	Record q1_table1_r2 = q1_table1->at(1);
	BOOST_CHECK(q1_table1_r2.get<string>("student") == "Becky");
	Record q1_table1_r3 = q1_table1->at(2);
	BOOST_CHECK(q1_table1_r3.get<string>("student") == "Thomas");
	Record q1_table1_r4 = q1_table1->at(3);
	BOOST_CHECK(q1_table1_r4.get<string>("student") == "Kenneth");

	// now we want a table of students whos gpa is lower than ALL of the gpas in good_gpa
	Table* q2_table1 = d.query("student", "students", "gpa < ALL(good_gpa)");
	// this table will contain Ruth, Grace, Mario, Johnny, and Joseph

	Record q2_table1_r1 = q2_table1->at(0);
	BOOST_CHECK(q2_table1_r1.get<string>("student") == "Ruth");
	Record q2_table1_r2 = q2_table1->at(1);
	BOOST_CHECK(q2_table1_r2.get<string>("student") == "Grace");
	Record q2_table1_r3 = q2_table1->at(2);
	BOOST_CHECK(q2_table1_r3.get<string>("student") == "Mario");
	Record q2_table1_r4 = q2_table1->at(3);
	BOOST_CHECK(q2_table1_r4.get<string>("student") == "Johnny");
	Record q2_table1_r5 = q2_table1->at(4);
	BOOST_CHECK(q2_table1_r5.get<string>("student") == "Joseph");

	// now we want a table of students id's that NOT equal to ANY ids in the good_student table
	Table* q3_table1 = d.query("id", "students", "id != ANY(good_students)");
	// this should be Ruth, Mario, Johnny and Joseph's ids

	Record q3_table1_r1 = q3_table1->at(0);
	BOOST_CHECK(q3_table1_r1.get<int>("id") == 335);
	Record q3_table1_r2 = q3_table1->at(1);
	BOOST_CHECK(q3_table1_r2.get<int>("id") == 415);
	Record q3_table1_r3 = q3_table1->at(2);
	BOOST_CHECK(q3_table1_r3.get<int>("id") == 780);
	Record q3_table1_r4 = q3_table1->at(3);
	BOOST_CHECK(q3_table1_r4.get<int>("id") == 971);

	/*** delete and update time the first table look like:
	table1
		student	id		gpa
	r1	Ruth	335		2.7
	r2	Grace	538		3.5
	r3	Mario	415		2.7
	r4	Becky	130		3.8
	r5	Thomas	914		3.6
	r6	Kenneth	730		3.9
	r7	Johnny	780		3.4
	r8	Joseph	971		2.4

	then one table (good_students) contains id's of Grace, Becky, Thomas, and Kenneth
	the other table (good_gpa) contains gpas 3.9, 3.8, 3.7
	*/
	// first lets kick out all the students who aren't good_students
	d.delete_from("students", "id NOT EXISTS(good_students)");
	BOOST_CHECK(d.table("students")->size() == 4);

	// then lets kick out the students who dont have a good gpa
	d.delete_from("students", "gpa < ALL(good_gpa)");
	BOOST_CHECK(d.table("students")->size() == 3); // Grace should be gone

	// now if a student has a gpa of 3.9 or higher we can round them up to 4.0
	d.update("students", "gpa >= 3.9", "gpa = 4.0");
	Record check_update = d.table("students")->at(2); // kenneth should be last entry
	BOOST_CHECK_CLOSE(check_update.get<float>("gpa"), 4.0, TOL);


	// exception testing
	BOOST_CHECK_THROW(d.query("student","students","student = ''Ruth'"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.delete_from("good_students", "id !!= 500"), QuerySyntaxError);
	BOOST_CHECK_THROW(d.update("good_gpa", "gpa >= 3.5", "gpa == 2.0"), QuerySyntaxError);
}




///////////////////////////////////////////////////////////


