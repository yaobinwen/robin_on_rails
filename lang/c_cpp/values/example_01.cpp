#include <iostream>
#include <string>

class Intvec
{
public:
    explicit Intvec(std::string const &name, size_t num = 0)
      : m_name(name), m_size(num), m_data(new int[m_size])
    {
        log("constructor");
    }

    ~Intvec()
    {
        log("destructor");
        if (m_data)
        {
            delete[] m_data;
            m_data = 0;
        }
    }

    Intvec(std::string const &name, const Intvec &other)
      : m_name(name), m_size(other.m_size), m_data(new int[m_size])
    {
        log("copy constructor");
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    Intvec &
    operator=(const Intvec &other)
    {
        log("copy assignment operator");
        Intvec tmp("tmp", other);
        std::swap(m_size, tmp.m_size);
        std::swap(m_data, tmp.m_data);
        return *this;
    }

protected:
    void
    log(const char *msg)
    {
        std::cout << "[" << m_name << "] " << msg << std::endl;
    }

    std::string m_name;
    size_t m_size;
    int *m_data;
};

Intvec
getIntvec(std::string const &name, size_t num)
{
    return Intvec(name, num);
}

class MoveableIntvec : public Intvec
{
public:
    using Intvec::Intvec;

    MoveableIntvec &
    operator=(MoveableIntvec &&other)
    {
        log("move assignment operator");
        std::swap(m_size, other.m_size);
        std::swap(m_data, other.m_data);
        return *this;
    }
};

MoveableIntvec &&
getMoveableIntvec(std::string const &name, size_t num)
{
    // Causes "stack smashing".
    MoveableIntvec tmp(name, num);
    return std::move(tmp);
}

int
main(int argc, char *argv[])
{
    {
        Intvec v1("v1", 20);
        Intvec v2("v2");

        std::cout << "assigning lvalue ..." << std::endl;
        v2 = v1;
        std::cout << "ended assigning lvalue." << std::endl;
    }

    std::cout << std::string(30, '-') << std::endl;

    {
        Intvec v2("v2");

        std::cout << "assigning rvalue ..." << std::endl;
        v2 = Intvec("rvalue_obj", 20);
        std::cout << "ended assigning rvalue." << std::endl;
    }

    std::cout << std::string(30, '-') << std::endl;

    {
        Intvec v3("v3");

        std::cout << "assigning rvalue ..." << std::endl;
        v3 = getIntvec("rvalue_obj", 30);
        std::cout << "ended assigning rvalue." << std::endl;
    }

    std::cout << std::string(30, '-') << std::endl;

    {
        MoveableIntvec v2("v2");

        std::cout << "assigning rvalue ..." << std::endl;
        v2 = MoveableIntvec("rvalue_obj", 20);
        std::cout << "ended assigning rvalue." << std::endl;
    }

    std::cout << std::string(30, '-') << std::endl;

    {
        MoveableIntvec v5("v5");

        std::cout << "assigning rvalue ..." << std::endl;
        v5 = getMoveableIntvec("rvalue_obj", 50);
        std::cout << "ended assigning rvalue." << std::endl;
    }

    return 0;
}
