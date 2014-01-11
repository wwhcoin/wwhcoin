#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-WWH");
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", false));
    BOOST_CHECK(GetBoolArg("-WWH", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-WWHo"));
    BOOST_CHECK(!GetBoolArg("-WWHo", false));
    BOOST_CHECK(GetBoolArg("-WWHo", true));

    ResetArgs("-WWH=0");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", false));
    BOOST_CHECK(!GetBoolArg("-WWH", true));

    ResetArgs("-WWH=1");
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", false));
    BOOST_CHECK(GetBoolArg("-WWH", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noWWH");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", false));
    BOOST_CHECK(!GetBoolArg("-WWH", true));

    ResetArgs("-noWWH=1");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", false));
    BOOST_CHECK(!GetBoolArg("-WWH", true));

    ResetArgs("-WWH -noWWH");  // -WWH should win
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", false));
    BOOST_CHECK(GetBoolArg("-WWH", true));

    ResetArgs("-WWH=1 -noWWH=1");  // -WWH should win
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", false));
    BOOST_CHECK(GetBoolArg("-WWH", true));

    ResetArgs("-WWH=0 -noWWH=0");  // -WWH should win
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", false));
    BOOST_CHECK(!GetBoolArg("-WWH", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--WWH=1");
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", false));
    BOOST_CHECK(GetBoolArg("-WWH", true));

    ResetArgs("--noWWH=1");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", false));
    BOOST_CHECK(!GetBoolArg("-WWH", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-WWH", "eleven"), "eleven");

    ResetArgs("-WWH -bar");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-WWH", "eleven"), "");

    ResetArgs("-WWH=");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-WWH", "eleven"), "");

    ResetArgs("-WWH=11");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-WWH", "eleven"), "11");

    ResetArgs("-WWH=eleven");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-WWH", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-WWH", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-WWH", 0), 0);

    ResetArgs("-WWH -bar");
    BOOST_CHECK_EQUAL(GetArg("-WWH", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-WWH=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-WWH", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-WWH=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-WWH", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--WWH");
    BOOST_CHECK_EQUAL(GetBoolArg("-WWH"), true);

    ResetArgs("--WWH=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-WWH", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noWWH");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", true));
    BOOST_CHECK(!GetBoolArg("-WWH", false));

    ResetArgs("-noWWH=1");
    BOOST_CHECK(!GetBoolArg("-WWH"));
    BOOST_CHECK(!GetBoolArg("-WWH", true));
    BOOST_CHECK(!GetBoolArg("-WWH", false));

    ResetArgs("-noWWH=0");
    BOOST_CHECK(GetBoolArg("-WWH"));
    BOOST_CHECK(GetBoolArg("-WWH", true));
    BOOST_CHECK(GetBoolArg("-WWH", false));

    ResetArgs("-WWH --noWWH");
    BOOST_CHECK(GetBoolArg("-WWH"));

    ResetArgs("-noWWH -WWH"); // WWH always wins:
    BOOST_CHECK(GetBoolArg("-WWH"));
}

BOOST_AUTO_TEST_SUITE_END()
