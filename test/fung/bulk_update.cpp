#include <fung/fung.hh>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::DoubleEq;

namespace
{
    auto generateTestFunction()
    {
        using namespace FunG;
        auto f = ( variable< 0 >( 1. ) + variable< 1 >( 2. ) ) * pow< 2 >( variable< 2 >( 3. ) );
        return f;
    }
} // namespace

TEST( BulkUpdateTest, D0 )
{
    using namespace FunG;
    auto f = generateTestFunction();
    EXPECT_THAT( f(), DoubleEq( 27 ) );
    f.bulk_update( IndexedType< double, 0 >{ 2.0 }, IndexedType< double, 1 >{ 3.0 },
                   IndexedType< double, 2 >{ 1.0 } );
    EXPECT_THAT( f(), DoubleEq( 5 ) );
}
