#pragma once
#include "test.h"
#include "down.h"

TEST(AddTestSuit, AddTest)
{
    down::Request req = {web::http::methods::GET, U("https://postman-echo.com/get?a=b")};
    auto rst = down::Reslove(&req);
    // EXPECT_EQ(add(1, 1), 2);
    std::wcout << rst.Name;
    EXPECT_TRUE(1);
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}