#include "gtest/gtest.h"

#include <fstream>

//
#include <ytl/utility/alignment.hpp>

TEST( Utility, Padding )
{
    EXPECT_EQ( ytl::padding_size( 3, 20 ), 17 );
    EXPECT_EQ( ytl::padding_size( 17, 4 ), 3 );
}

TEST( Utility, Alignment )
{
    EXPECT_EQ( ytl::aligned_size( 3, 20 ), 20 );
    EXPECT_EQ( ytl::aligned_size( 17, 4 ), 20 );
}



//
#include <ytl/utility/mapping_binary_file.hpp>

//TEST( Utility, Mapping )
//{
//}