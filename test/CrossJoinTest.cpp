#include <boost/test/unit_test.hpp>
#include <iostream>
#include "database.h"


BOOST_AUTO_TEST_SUITE(joins_test)
BOOST_AUTO_TEST_CASE(crossjoins_test1)
{
	Table* a = new Table();
	Table* b = new Table();
	//Create first table
	a->add_column("V1", Table::integer);
	std::vector<std::string> k;
	k.push_back("V1");
	a->set_key(k);
	Record r1;
	r1.set("V1", "1");
	Record r2;
	r2.set("V1", "2");
	a->insert(r1);
	a->insert(r2);
	//Create second table
	b->add_column("D1", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("D1");
	b->set_key(k2);
	Record r21;
	r21.set("D1", "ABCD");
	Record r22;
	r22.set("D1", "EFGH");
	b->insert(r21);
	b->insert(r22);
	Table c = a->cross_join(*b);
	BOOST_CHECK(c.size() == 4);
}


BOOST_AUTO_TEST_CASE(crossjoins_test2)
{
	Table* a = new Table();
	Table* b = new Table();
	//Create first table
	a->add_column("V1", Table::integer);
	a->add_column("V2", Table::integer);
	a->add_column("V3", Table::varchar);
	std::vector<std::string> k;
	k.push_back("V1");
	a->set_key(k);
	Record r11;
	r11.set("V1", "1");
	r11.set("V2", "2");
	r11.set("V3", "A");
	Record r12;
	r12.set("V1", "2");
	r12.set("V2", "3");
	r12.set("V3", "B");
	a->insert(r11);
	a->insert(r12);

	//Create second table
	b->add_column("D1", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("D1");
	b->set_key(k2);
	Record r21;
	r21.set("D1", "ABCD");
	Record r22;
	r22.set("D1", "EFGH");
	Record r23;
	r23.set("D1", "IJKL");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);

	Table c = a->cross_join(*b);
	BOOST_CHECK(c.size() == 6);
}

BOOST_AUTO_TEST_CASE(crossjoins_test3)
{
	Table* a = new Table();
	Table* b = new Table();
	//Create first table
	a->add_column("V1", Table::integer);
	a->add_column("V2", Table::integer);
	a->add_column("V3", Table::varchar);
	std::vector<std::string> k;
	k.push_back("V1");
	a->set_key(k);
	Record r11;
	r11.set("V1", "1");
	r11.set("V2", "2");
	r11.set("V3", "A");
	Record r12;
	r12.set("V1", "2");
	r12.set("V2", "3");
	r12.set("V3", "B");
	a->insert(r11);
	a->insert(r12);

	//Create second table
	b->add_column("D1", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("D1");
	b->set_key(k2);
	Record r21;
	r21.set("D1", "ABCD");
	Record r22;
	r22.set("D1", "EFGH");
	Record r23;
	r23.set("D1", "IJKL");
	Record r24;
	r24.set("D1", "MNOP");
	Record r25;
	r25.set("D1", "QRST");
	Record r26;
	r26.set("D1", "UVWY");
	Record r27;
	r27.set("D1", "Z123");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	b->insert(r24);
	b->insert(r25);
	b->insert(r26);
	b->insert(r27);

	Table c = a->cross_join(*b);
	BOOST_CHECK(c.size() == 14);
}

BOOST_AUTO_TEST_CASE(crossjoins_test4)
{
	Table* a = new Table();
	Table* b = new Table();
	//Create first table
	a->add_column("V1", Table::integer);
	a->add_column("V2", Table::integer);
	a->add_column("V3", Table::varchar);
	std::vector<std::string> k;
	k.push_back("V1");
	a->set_key(k);
	Record r11;
	r11.set("V1", "1");
	r11.set("V2", "2");
	r11.set("V3", "A");
	Record r12;
	r12.set("V1", "2");
	r12.set("V2", "3");
	r12.set("V3", "B");
	Record r13;
	r13.set("V1", "3");
	r13.set("V2", "3");
	r13.set("V3", "B");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	//Create second table
	b->add_column("D1", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("D1");
	b->set_key(k2);
	Record r21;
	r21.set("D1", "ABCD");
	Record r22;
	r22.set("D1", "EFGH");
	Record r23;
	r23.set("D1", "IJKL");
	Record r24;
	r24.set("D1", "MNOP");
	Record r25;
	r25.set("D1", "QRST");
	Record r26;
	r26.set("D1", "UVWY");
	Record r27;
	r27.set("D1", "Z123");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	b->insert(r24);
	b->insert(r25);
	b->insert(r26);
	b->insert(r27);
	Table c = a->cross_join(*b);
	BOOST_CHECK(c.size() == 21);
}

BOOST_AUTO_TEST_CASE(naturaljoins_except_test)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("second");
	a->set_key(k);
	Record r11, r12, r13, r14, r21, r22, r23;
	r11.set("key", "6");
	r11.set("second", "a");
	r12.set("key", "7");
	r12.set("second", "b");
	r13.set("key", "8");
	r13.set("second", "c");
	r14.set("key", "9");
	r14.set("second", "e");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	a->insert(r14);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("third");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "2");
	r22.set("third", "dfg");
	r23.set("key", "5");
	r23.set("third", "adc");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	BOOST_CHECK_THROW(a->natural_join(*b), InvalidOperationError);
}
BOOST_AUTO_TEST_CASE(naturaljoins_test0)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r13, r14, r21, r22, r23;
	r11.set("key", "6");
	r11.set("second", "a");
	r12.set("key", "7");
	r12.set("second", "b");
	r13.set("key", "8");
	r13.set("second", "c");
	r14.set("key", "9");
	r14.set("second", "e");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	a->insert(r14);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "2");
	r22.set("third", "dfg");
	r23.set("key", "5");
	r23.set("third", "adc");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 0);
	/*
		key		second				key		third
		6		a					1		dcb
		7		b					2		dfg
		8		c					5		adc
		9		e
	*/
}

BOOST_AUTO_TEST_CASE(naturaljoins_test1)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r21, r22, r23;
	r11.set("key", "1");
	r11.set("second", "a");
	r12.set("key", "2");
	r12.set("second", "b");
	a->insert(r11);
	a->insert(r12);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "3");
	r22.set("third", "dfg");
	r23.set("key", "5");
	r23.set("third", "adc");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 1);
	/*
		key		second				key		third
		1		a					1		dcb
		2		b					3		dfg
									5		adc

					key		second		third
					1		a			dcb
	*/
}

BOOST_AUTO_TEST_CASE(naturaljoins_test2)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r21, r22, r23;
	r11.set("key", "1");
	r11.set("second", "a");
	r12.set("key", "2");
	r12.set("second", "b");
	a->insert(r11);
	a->insert(r12);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "2");
	r22.set("third", "dfg");
	r23.set("key", "5");
	r23.set("third", "adc");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 2);
	/*
		key		second				key		third
		1		a					1		dcb
		2		b					2		dfg
									5		adc

					key		second		third
					1		a			dcb
					2		b			dfg
	*/
}

BOOST_AUTO_TEST_CASE(naturaljoins_test3)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r13, r14, r21, r22, r23;
	r11.set("key", "1");
	r11.set("second", "a");
	r12.set("key", "2");
	r12.set("second", "b");
	r13.set("key", "4");
	r13.set("second", "c");
	r14.set("key", "3");
	r14.set("second", "e");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	a->insert(r14);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "2");
	r22.set("third", "dfg");
	r23.set("key", "5");
	r23.set("third", "adc");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 2);
	/*
		key		second				key		third
		1		a					1		dcb
		2		b					2		dfg
		4		c					5		adc
		5		e

					key		second		third
					1		a			dcb
					2		b			dfg
					5		e			adc
	*/
}

BOOST_AUTO_TEST_CASE(naturaljoins_test4)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r13, r14, r15, r16, r17, r18, r19, r21, r22, r23, r24, r25;
	r11.set("key", "1");
	r11.set("second", "a");
	r12.set("key", "2");
	r12.set("second", "b");
	r13.set("key", "4");
	r13.set("second", "c");
	r14.set("key", "3");
	r14.set("second", "e");
	r15.set("key", "9");
	r15.set("second", "r");
	r16.set("key", "5");
	r16.set("second", "e");
	r17.set("key", "6");
	r17.set("second", "a");
	r18.set("key", "7");
	r18.set("second", "er");
	r19.set("key", "8");
	r19.set("second", "qe");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	a->insert(r14);
	a->insert(r15);
	a->insert(r16);
	a->insert(r17);
	a->insert(r18);
	a->insert(r19);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "20");
	r22.set("third", "dfg");
	r23.set("key", "50");
	r23.set("third", "adc");
	r24.set("key", "42");
	r24.set("third", "dee");
	r25.set("key", "41");
	r25.set("third", "aeeeee");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	b->insert(r24);
	b->insert(r25);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 1);
	/*
		key		second				key		third
		1		a					1		dcb
		2		b					20		dfg
		4		c					50		adc
		3		e					42		dee
		9		r					41		aeeeee
		5		e
		6		a
		7		er
		8		qe

					key		second		third
					1		a			dcb
	*/
}

BOOST_AUTO_TEST_CASE(naturaljoins_test5)
{
	Table* a = new Table();
	Table* b = new Table();
	//Build table a
	a->add_column("key", Table::integer);
	a->add_column("second", Table::varchar);
	std::vector<std::string> k;
	k.push_back("key");
	a->set_key(k);
	Record r11, r12, r13, r14, r15, r21, r22, r23, r24, r25;
	r11.set("key", "1");
	r11.set("second", "a");
	r12.set("key", "2");
	r12.set("second", "b");
	r13.set("key", "4");
	r13.set("second", "c");
	r14.set("key", "3");
	r14.set("second", "e");
	r15.set("key", "9");
	r15.set("second", "r");
	a->insert(r11);
	a->insert(r12);
	a->insert(r13);
	a->insert(r14);
	a->insert(r15);
	//Build table b
	b->add_column("key", Table::integer);
	b->add_column("third", Table::varchar);
	std::vector<std::string> k2;
	k2.push_back("key");
	b->set_key(k2);
	r21.set("key", "1");
	r21.set("third", "dcb");
	r22.set("key", "2");
	r22.set("third", "dfg");
	r23.set("key", "4");
	r23.set("third", "adc");
	r24.set("key", "3");
	r24.set("third", "dee");
	r25.set("key", "9");
	r25.set("third", "aeeeee");
	b->insert(r21);
	b->insert(r22);
	b->insert(r23);
	b->insert(r24);
	b->insert(r25);
	Table c = a->natural_join(*b);
	BOOST_CHECK(c.size() == 5);
	/*
		key		second				key		third
		1		a					1		dcb
		2		b					2		dfg
		4		c					4		adc
		3		e					3		dee
		9		r					9		aeeeee

					key		second		third
					1		a			dcb
					2		b			dfg
					4		c			adc
					3		e			dee
					9		r			aeeeee
	*/
}
BOOST_AUTO_TEST_SUITE_END()
