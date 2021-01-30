#include "gtest/gtest.h"

#include "plusctx/plusctx.hpp"


TEST(plusctx, test_ctx1) {
    PLUSCTX_CTX("ctx");

    EXPECT_EQ(plusctx_ctx__.name, "ctx");

#ifdef _MSC_VER
    EXPECT_EQ(plusctx_ctx__.file_name, "test\\unit_test\\test\\plusctx-test.cpp");
    EXPECT_EQ(plusctx_ctx__.func_name, "TestBody", );
    EXPECT_EQ(plusctx_ctx__.rich_func_name, "void __thiscall plusctx_test_ctx1_Test::TestBody(void)");
#else
#endif
    EXPECT_EQ(plusctx_ctx__.line_no, "7");
}

TEST(plusctx, test_ctx2) {
    PLUSCTX_CTX("ctx");

    []() -> void {
        PLUSCTX_CTX("ctx2");

        EXPECT_EQ(plusctx_ctx__.name, "ctx2");
        EXPECT_EQ(plusctx_ctx__.func_name, "operator ()");
        EXPECT_EQ(plusctx_ctx__.line_no, "24");
    }();
}

TEST(plusctx, test_stack) {
    PLUSCTX_CTX("ctx");

    const auto stack = plusctx::get_context_stack();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0]->name, "ctx");

    []() -> void {
        PLUSCTX_CTX("ctx2");

        const auto stack = plusctx::get_context_stack();

        EXPECT_EQ(stack.size(), 2);
        EXPECT_EQ(stack[0]->name, "ctx");
        EXPECT_EQ(stack[1]->name, "ctx2");
    }();

    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack[0]->name, "ctx");
}
