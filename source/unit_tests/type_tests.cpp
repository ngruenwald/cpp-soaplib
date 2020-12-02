#include "unit_tests.hpp"

#include <soaplib/types/dateTypes.hpp>


TEST_CASE("Timezone to_string", "[all][types]")
{
    {
        auto s = to_string(soaplib::Timezone{true, 0.f});
        REQUIRE(s == "Z");
    }
    {
        auto s = to_string(soaplib::Timezone{false, 1.0});
        REQUIRE(s == "+01:00");
    }
    {
        auto s = to_string(soaplib::Timezone{false, -3.0});
        REQUIRE(s == "-03:00");
    }
    {
        auto s = to_string(soaplib::Timezone{false, 1.5});
        REQUIRE(s == "+01:30");
    }
}

TEST_CASE("Timezone from_string", "[all][types]")
{
    {
        auto tz = soaplib::Timezone{};
        from_string("+23:00", tz);
        REQUIRE(tz.IsUTC == false);
        REQUIRE(tz.Offset == 23.f);
    }
    {
        auto tz = soaplib::Timezone{};
        from_string("-07:00", tz);
        REQUIRE(tz.IsUTC == false);
        REQUIRE(tz.Offset == -7.f);
    }
    {
        auto tz = soaplib::Timezone{};
        from_string("Z", tz);
        REQUIRE(tz.IsUTC == true);
        REQUIRE(tz.Offset == 0.f);
    }
    {
        auto tz = soaplib::Timezone{};
        from_string("+12:30", tz);
        REQUIRE(tz.IsUTC == false);
        REQUIRE(tz.Offset == 12.5f);
    }
}

TEST_CASE("Date to_string", "[all][types]")
{
    {
        auto s = to_string(soaplib::Date{2020, 1, 30});
        REQUIRE(s == "2020-01-30");
    }
    {
        auto s = to_string(soaplib::Date{2020, 12, 1, {true, 0.f}});
        REQUIRE(s == "2020-12-01Z");
    }
    {
        auto s = to_string(soaplib::Date{2020, 12, 1, {false, 3.5f}});
        REQUIRE(s == "2020-12-01+03:30");
    }
}

TEST_CASE("Date from_string", "[all][types]")
{
    {
        auto d = soaplib::Date{};
        from_string("2020-01-30", d);
        REQUIRE(d.Year == 2020);
        REQUIRE(d.Month == 1);
        REQUIRE(d.Day == 30);
        REQUIRE(d.Timezone.has_value() == false);
    }
    {
        auto d = soaplib::Date{};
        from_string("2020-12-01Z", d);
        REQUIRE(d.Year == 2020);
        REQUIRE(d.Month == 12);
        REQUIRE(d.Day == 1);
        REQUIRE(d.Timezone.has_value() == true);
        REQUIRE(d.Timezone->IsUTC == true);
        REQUIRE(d.Timezone->Offset == 0.f);
    }
    {
        auto d = soaplib::Date{};
        from_string("2020-12-01-09:15", d);
        REQUIRE(d.Year == 2020);
        REQUIRE(d.Month == 12);
        REQUIRE(d.Day == 01);
        REQUIRE(d.Timezone.has_value() == true);
        REQUIRE(d.Timezone->IsUTC == false);
        REQUIRE(d.Timezone->Offset == -9.25f);
    }
    {
        auto d = soaplib::Date{};
        from_string("2020-01-30+04:30", d);
        REQUIRE(d.Year == 2020);
        REQUIRE(d.Month == 1);
        REQUIRE(d.Day == 30);
        REQUIRE(d.Timezone.has_value() == true);
        REQUIRE(d.Timezone->IsUTC == false);
        REQUIRE(d.Timezone->Offset == 4.5f);
    }
}

TEST_CASE("Time to_string", "[all][types]")
{
    {
        auto s = to_string(soaplib::Time{1, 2, 3});
        REQUIRE(s == "01:02:03");
    }
    {
        auto s = to_string(soaplib::Time{1, 2, 3, 45});
        REQUIRE(s == "01:02:03.45");
    }
    {
        auto s = to_string(soaplib::Time{1, 2, 3, 45, {false, 1.5f}});
        REQUIRE(s == "01:02:03.45+01:30");
    }
}

TEST_CASE("Time from_string", "[all][types]")
{
    {
        soaplib::Time t;
        from_string("17:32:18", t);
        REQUIRE(t.Hour == 17);
        REQUIRE(t.Minute == 32);
        REQUIRE(t.Second == 18);
        REQUIRE(t.Fraction == 0);
        REQUIRE(t.Timezone.has_value() == false);
    }
    {
        soaplib::Time t;
        from_string("17:32:18.1234", t);
        REQUIRE(t.Hour == 17);
        REQUIRE(t.Minute == 32);
        REQUIRE(t.Second == 18);
        REQUIRE(t.Fraction == 1234);
        REQUIRE(t.Timezone.has_value() == false);
    }
    {
        soaplib::Time t;
        from_string("17:32:18Z", t);
        REQUIRE(t.Hour == 17);
        REQUIRE(t.Minute == 32);
        REQUIRE(t.Second == 18);
        REQUIRE(t.Fraction == 0);
        REQUIRE(t.Timezone.has_value() == true);
        REQUIRE(t.Timezone->IsUTC == true);
    }
    {
        soaplib::Time t;
        from_string("17:32:18.77+01:00", t);
        REQUIRE(t.Hour == 17);
        REQUIRE(t.Minute == 32);
        REQUIRE(t.Second == 18);
        REQUIRE(t.Fraction == 77);
        REQUIRE(t.Timezone.has_value() == true);
        REQUIRE(t.Timezone->IsUTC == false);
        REQUIRE(t.Timezone->Offset == 1.f);
    }
}

TEST_CASE("DateTime to_string", "[all][types]")
{
    {
        auto s = to_string(soaplib::DateTime{{2021, 1, 1}, {23, 59, 31, 567}, {true, 0.f}});
        REQUIRE(s == "2021-01-01T23:59:31.567Z");
    }
}

TEST_CASE("DateTime from_string", "[all][types]")
{
    {
        soaplib::DateTime dt;
        from_string("1999-12-31", dt);
        REQUIRE(dt.Date.Year == 1999);
        REQUIRE(dt.Date.Month == 12);
        REQUIRE(dt.Date.Day == 31);
        REQUIRE(dt.Date.Timezone.has_value() == false);
        REQUIRE(dt.Time.Hour == 0);
        REQUIRE(dt.Time.Minute == 0);
        REQUIRE(dt.Time.Second == 0);
        REQUIRE(dt.Time.Fraction == 0);
        REQUIRE(dt.Time.Timezone.has_value() == false);
        REQUIRE(dt.Timezone.has_value() == false);
    }
    {
        soaplib::DateTime dt;
        from_string("1999-12-31T14:20:00Z", dt);
        REQUIRE(dt.Date.Year == 1999);
        REQUIRE(dt.Date.Month == 12);
        REQUIRE(dt.Date.Day == 31);
        REQUIRE(dt.Date.Timezone.has_value() == false);
        REQUIRE(dt.Time.Hour == 14);
        REQUIRE(dt.Time.Minute == 20);
        REQUIRE(dt.Time.Second == 0);
        REQUIRE(dt.Time.Fraction == 0);
        REQUIRE(dt.Time.Timezone.has_value() == true);
        REQUIRE(dt.Time.Timezone->IsUTC == true);
        REQUIRE(dt.Timezone.has_value() == true);
        REQUIRE(dt.Timezone->IsUTC == true);
    }
    {
        soaplib::DateTime dt;
        from_string("2019-05-04T11:11:11.111+00:00", dt);
        REQUIRE(dt.Date.Year == 2019);
        REQUIRE(dt.Date.Month == 05);
        REQUIRE(dt.Date.Day == 04);
    //    REQUIRE(dt.Date.Timezone.has_value() == false);   // doesn't do what we want
        REQUIRE(dt.Time.Hour == 11);
        REQUIRE(dt.Time.Minute == 11);
        REQUIRE(dt.Time.Second == 11);
        REQUIRE(dt.Time.Fraction == 111);
    //    REQUIRE(dt.Time.Timezone.has_value() == true);    // doesn't do what we want
        REQUIRE(dt.Time.Timezone->IsUTC == false);
        REQUIRE(dt.Timezone->Offset == 0.f);
    }
}

TEST_CASE("Duration to_string", "[all][types]")
{
    {
        auto s = to_string(soaplib::Duration{1, 2, 3, 4, 5, 6});
        REQUIRE(s == "P1Y2M3DT4H5M6S");
    }
    {
        auto s = to_string(soaplib::Duration{1, 0, 0, 0, 0, 0});
        REQUIRE(s == "P1Y");
    }
    {
        auto s = to_string(soaplib::Duration{0, 1, 0, 0, 0, 0});
        REQUIRE(s == "P1M");
    }
    {
        auto s = to_string(soaplib::Duration{0, 0, 1, 0, 0, 0});
        REQUIRE(s == "P1D");
    }
    {
        auto s = to_string(soaplib::Duration{0, 0, 0, 1, 0, 0});
        REQUIRE(s == "PT1H");
    }
    {
        auto s = to_string(soaplib::Duration{0, 0, 0, 0, 1, 0});
        REQUIRE(s == "PT1M");
    }
    {
        auto s = to_string(soaplib::Duration{1, 0, 0, 0, 0, 1});
        REQUIRE(s == "P1YT1S");
    }
    {
        auto s = to_string(soaplib::Duration{0, 0, 5, 0, 0, 2, true});
        REQUIRE(s == "-P5DT2S");
    }
}

TEST_CASE("Duration from_string", "[all][types]")
{
    {
        soaplib::Duration du;
        from_string("P", du);
        REQUIRE(du.Years == 0);
        REQUIRE(du.Months == 0);
        REQUIRE(du.Days == 0);
        REQUIRE(du.Hours == 0);
        REQUIRE(du.Minutes == 0);
        REQUIRE(du.Seconds == 0);
        REQUIRE(du.IsNegative == false);
    }
    {
        soaplib::Duration du;
        from_string("1T5S", du);
        REQUIRE(du.Years == 0);
        REQUIRE(du.Months == 0);
        REQUIRE(du.Days == 0);
        REQUIRE(du.Hours == 0);
        REQUIRE(du.Minutes == 0);
        REQUIRE(du.Seconds == 0);
        REQUIRE(du.IsNegative == false);
    }
    {
        soaplib::Duration du;
        from_string("P1Y2M3DT4H5M6S", du);
        REQUIRE(du.Years == 1);
        REQUIRE(du.Months == 2);
        REQUIRE(du.Days == 3);
        REQUIRE(du.Hours == 4);
        REQUIRE(du.Minutes == 5);
        REQUIRE(du.Seconds == 6);
        REQUIRE(du.IsNegative == false);
    }
    {
        soaplib::Duration du;
        from_string("P1D", du);
        REQUIRE(du.Years == 0);
        REQUIRE(du.Months == 0);
        REQUIRE(du.Days == 1);
        REQUIRE(du.Hours == 0);
        REQUIRE(du.Minutes == 0);
        REQUIRE(du.Seconds == 0);
        REQUIRE(du.IsNegative == false);
    }
    {
        soaplib::Duration du;
        from_string("-PT23M", du);
        REQUIRE(du.Years == 0);
        REQUIRE(du.Months == 0);
        REQUIRE(du.Days == 0);
        REQUIRE(du.Hours == 0);
        REQUIRE(du.Minutes == 23);
        REQUIRE(du.Seconds == 0);
        REQUIRE(du.IsNegative == true);
    }
}

