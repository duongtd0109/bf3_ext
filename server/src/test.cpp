#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "../include/server.h"

using namespace std;

typedef const char* Strings;

string str_0[4] = {"Phat Tan","Hiep y", "Bf3@ext*@",
                "MinhCo"};
string str_1[4] = {"PhatTan001#","yn111KK^4", "Bf3@111111",
                "MinhCo111**"};

string str_2[4] = {"123456789abc", "ABC12345678979879879877987987",
                "Bf3@", "Aa+++++++"};

Server server;
User *user = new User();
int k = 0;

TEST(Server_test, valid_signUp_test) {
    for (int i = 0; i < 4; i++)
        EXPECT_NE(server.signUp(str_0[i], str_1[i], str_2[i], k), 0);
}

TEST(Server_test, valid_login_test) {
    for (int i = 0; i < 4; i++)
        EXPECT_EQ(server.logIn(str_1[i], str_2[i], user), 0);
}

int main(int argc, char **argv) {
    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
