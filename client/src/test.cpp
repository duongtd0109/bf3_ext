#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "../Client/include/io.h"

using namespace std;

typedef const char* Strings;

Strings str_0[4] = {"Phat_vippro123","Phung@321", "Bf3@ext*@",
                "Phat@vippro123"};

Strings str_1[6] = {"123456789abc", "ABC12345678979879879877987987",
                "Bf3@", "Aa+++++++", "         ", "^C"};
Strings str_username = {"tanphat"};
Strings str_pass_1 = {"tanphat1"};
Strings str_pass_2 = {"binhbuon"};

int getlenString(const char *str) {
    int len=-1;
    while (str[++len]) {
        // do nothing
    }
    return len;
}

TEST(Client_test, valid_password_test) {
    for (int i = 0; i < 4; i++)
        ASSERT_EQ(checkIfStrongPasswd(str_0[i], getlenString(str_0[i])), true);
}

TEST(Client_test, invalid_password_test) {
    for (int i = 0; i < 6; i++) {
        ASSERT_EQ(checkIfStrongPasswd(str_1[i], getlenString(str_1[i])), false);
    } 
}

TEST(Client_test, valid_signup_test) {
    EXPECT_NE(checkIfContainUsrnm(str_pass_2, str_username, getlenString(str_username)), true);
    EXPECT_EQ(checkIfContainUsrnm(str_pass_1, str_username, getlenString(str_username)), true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
