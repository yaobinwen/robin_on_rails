#include <iostream>
#include <string>

#include <boost/format.hpp>

class A
{
public:
    std::string name;
    int *pvalue;

    A(std::string const &n) : name(n), pvalue(new int(1))
    {
        std::cout << "Constructing " << this->str() << std::endl;
    }

    A(A &a) : name(a.name + "(copy)"), pvalue(new int(*a.pvalue))
    {
        std::cout << "Copying " << this->str() << std::endl;
    }

    A(A &&a) : name(a.name + "(moved)"), pvalue(a.pvalue)
    {
        a.name.clear();
        a.pvalue = nullptr;
        std::cout << "Moving " << this->str() << std::endl;
    }

    ~A()
    {
        std::cout << "Destructing " << this->str() << std::endl;
        delete pvalue;
    }

    std::string
    str() const
    {
        boost::format fmt("<A name=%s; value=%s; addr=%s>");
        std::string value(this->pvalue ? std::to_string(*this->pvalue) : "nullptr");
        return boost::str(fmt % this->name % value % this);
    }
};

A
return_rvalue(bool test)
{
    A a("a"), b("b");
    if (test)
        return a; // 等价于 static_cast<A&&>(a);
    else
        return b; // 等价于 static_cast<A&&>(b);
}

int
main()
{
    A obj = return_rvalue(false);
    std::cout << "obj:" << std::endl;
    std::cout << obj.pvalue << std::endl;
    std::cout << *obj.pvalue << std::endl;
    return 0;
}
