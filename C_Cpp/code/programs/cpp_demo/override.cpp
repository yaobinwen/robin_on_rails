#include <gtest/gtest.h>

class Base
{
public:
    virtual ~Base() {}

    virtual int
    generateNum() const = 0;
};

class Child : public Base
{
public:
    int
    generateNum() const override
    {
        return 10;
    }

    int getNum() const {
        return this->generateNum();
    }
};

class Grandchild : public Child
{
public:
    int
    generateNum() const override
    {
        return -10;
    }
};

TEST(OverrideTest, test)
{
    Grandchild gc;

    EXPECT_EQ(gc.generateNum(), -10);
    EXPECT_EQ(gc.getNum(), -10);
}
