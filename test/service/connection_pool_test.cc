#include "gtest/gtest.h"
#include "lib/service/connection_pool.h"


TEST(ConnectionPool, test_submitConnection) {
    ConnectionPool connectionPool;
    EXPECT_EQ(connectionPool.submitConnection("123"),0);
    EXPECT_EQ(connectionPool.submitConnection("123"),1);
    EXPECT_EQ(connectionPool.submitConnection("1234"),0);
}

TEST(ConnectionPool, test_removeConnection) {
    ConnectionPool connectionPool;
    int fd = connectionPool.submitConnection("123");
    connectionPool.removeConnection("123",fd);
    EXPECT_FALSE(connectionPool.isAlived("123",fd));
}

TEST(ConnectionPool, test_isAlived) {
    ConnectionPool connectionPool;
    int fd = connectionPool.submitConnection("123");
    EXPECT_TRUE(connectionPool.isAlived("123",fd));
    connectionPool.removeConnection("123",fd);
    EXPECT_FALSE(connectionPool.isAlived("123",fd));
}