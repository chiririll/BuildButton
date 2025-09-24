#include <unity.h>
#include <ArgsParser.h>

#include "Actions/CurlAction.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_empty_string()
{
    auto parser = ArgsParser();
    parser.parse("");
}

void test_curl_args()
{
    auto parser = ArgsParser();
    parser.parse("curl -X POST");
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();
    UNITY_END();
}
