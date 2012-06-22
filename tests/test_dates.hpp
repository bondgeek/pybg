/*
 *  test_dates.hpp
 *  bondgeek
 *
 *  Created by BART MOSLEY on 6/3/12.
 *  Copyright 2012 BG Research LLC. All rights reserved.
 *
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <bg/bondgeek.hpp>

using namespace QuantLib;
using namespace bondgeek;
using namespace boost::unit_test;

#if defined(QL_ENABLE_SESSIONS)
namespace QuantLib {
    Integer sessionId() { return 0; }
}
#endif

struct QDateTests {
	Date date0, date1, date2, date3, date4 ;
	
	QDateTests() : 
	date0(QDate(45000)), // March 15th, 2023
	date1(QDate(Date(7, September, 1980))),
	date2(QDate(19800907)),
	date3(QDate("19600809")),
	date4(QDate("12-13-1976"))
	{
		BOOST_TEST_MESSAGE("\nQDate tests");
	}
};

BOOST_FIXTURE_TEST_SUITE( DateTest, QDateTests )

BOOST_AUTO_TEST_CASE( quantlib_date )
{			
	BOOST_TEST_MESSAGE(">>date: " << date1);
	BOOST_CHECK( date1 == Date(7, September, 1980) );
	BOOST_CHECK_EQUAL( date1.year(), 1980 );
	BOOST_CHECK_EQUAL( date1.month(), September);
	BOOST_CHECK_EQUAL( date1.dayOfMonth(), 7 );
	
}

BOOST_AUTO_TEST_CASE( number_based_dates )
{
	BOOST_CHECK_EQUAL(date0, Date(15, March, 2023));
	BOOST_CHECK_EQUAL(date2, Date(7, September, 1980));
}

BOOST_AUTO_TEST_CASE( string_dates )
{
	BOOST_CHECK_EQUAL( date4.year(), 1976 );
	BOOST_CHECK_EQUAL( date4.month(), December);
	BOOST_CHECK_EQUAL( date4.dayOfMonth(), 13 );
	
	BOOST_CHECK_EQUAL( date3, Date(9, August, 1960) );
}

BOOST_AUTO_TEST_SUITE_END()
