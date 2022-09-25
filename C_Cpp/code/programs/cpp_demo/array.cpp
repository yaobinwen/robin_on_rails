#include <boost/format.hpp>
#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

TEST(ArrayTest, Array1D)
{
    int arr_1d[2] = { 0, 1 };
    int *parr_1d = arr_1d;

    std::type_info const &ti_arr_1d = typeid(arr_1d);
    // "A2_i" means "Array of 2 elements of int".
    EXPECT_EQ(std::string(ti_arr_1d.name()), "A2_i");

    std::type_info const &ti_parr_1d = typeid(parr_1d);
    // "Pi" means "Pointer to int".
    EXPECT_EQ(std::string(ti_parr_1d.name()), "Pi");
}

TEST(ArrayTest, Array2D)
{
    int arr_2d[2][3] = { 0, 1, 2, 3, 4, 5 };

    std::type_info const &ti_arr_2d = typeid(arr_2d);
    // "A2_A3_i" means "Array of 2 elements, each of which is an array of 3"
    // "elements, each of which is int."
    EXPECT_EQ(std::string(ti_arr_2d.name()), "A2_A3_i");
    // Size of the 2D array is the size of the elements in all dimensions.
    EXPECT_EQ(sizeof(arr_2d), sizeof(int) * 2 * 3);

    int(*parr_2d)[3] = arr_2d;
    std::type_info const &ti_parr_2d = typeid(parr_2d);
    // "PA3_i" means "Pointer to an array of 3 elements which are int".
    EXPECT_EQ(std::string(ti_parr_2d.name()), "PA3_i");
    // parr_2d is a pointer so its size is the size of any kind of pointer.
    EXPECT_EQ(sizeof(parr_2d), sizeof(void *));
    EXPECT_EQ(sizeof(parr_2d), sizeof(char *));
    EXPECT_EQ(sizeof(parr_2d), sizeof(&arr_2d));
}

template<class Ty, size_t ROW, size_t COL>
void
Array1D_to_Array2D(std::vector<Ty> const &arr_1d, Ty (&arr_2d)[ROW][COL])
{
    size_t expected = ROW * COL;
    if (arr_1d.size() < expected)
    {
        boost::format fmt("Expect the 1D array to have %1% element(s) but it "
                          "actually has %2% element(s).");
        throw std::invalid_argument(boost::str(fmt % expected % arr_1d.size()));
    }

    for (size_t i = 0; i < arr_1d.size(); ++i)
    {
        size_t r = i / COL;
        size_t c = i - r * COL;
        arr_2d[r][c] = arr_1d[i];
    }
}

TEST(ArrayTest, array1D_to_array2D)
{
    std::vector<int> v;
    v.reserve(12);
    for (int i = 1; i <= 12; ++i)
    {
        v.push_back(i);
    }

    int arr_2d[3][4] = { 0 };

    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ(arr_2d[i][j], 0);
        }
    }

    Array1D_to_Array2D<int, 3, 4>(v, arr_2d);

    int e = 1;
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            EXPECT_EQ(arr_2d[i][j], e);
            e++;
        }
    }
}
