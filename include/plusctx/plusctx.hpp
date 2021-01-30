// Copyright (C) 2020 kkAyataka
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef PLUSCTX_HPP__
#define PLUSCTX_HPP__

#include <deque>
#include <string>


/** Version number.
 * 0x01020304 -> 1.2.3.4 */
#define PLUSCTX_VERSION 0x01000000

namespace plusctx {
namespace detail {

/** @private */
constexpr bool is_match(const char * s1, const char * s2) {
    for (std::size_t i = 0; s1[i] && s2[i]; ++i) {
        if (s1[i] != s2[i]) {
            return false;
        }
    }

    return true;
}

/** @private */
constexpr const char * shorten_file_name(const char * fileName) {
#ifdef _MSC_VER
    const char sep = '\\';
    const char * src = "src\\";
    const char * inc = "include\\";
    const char * tes = "test\\";
#else
    const char sep = '/';
    const char * src = "src/";
    const char * inc = "include/";
    const char * tes = "test/";
#endif

    for (std::size_t i = 0; fileName[i]; ++i) {
        const auto c = fileName + i;
        if (*c == sep) {
            const auto c1 = c + 1;
            if ((*c1 == src[0] && is_match(c1, src)) ||
                (*c1 == inc[0] && is_match(c1, inc)) ||
                (*c1 == tes[0] && is_match(c1, tes))) {
                return c + 1;
            }
        }
    }

    return fileName;
}

} // namespace plusctx::detail

/**
 * Context class.
 *
 * Context has a holder for a context name, function name, file name, and line number.
 * Usually, use PLUSCTX_CTX macro for creating a context object.
 * Or define a useful new macro.
 */
class Context {
private:
    /** @private */
    inline static std::deque<Context *> & get_context_stack() {
        thread_local std::deque<Context *> stack;
        return stack;
    }

    friend const std::deque<Context *> & get_context_stack();

public:
    Context(
        const std::string name,
        const std::string func_name,
        const std::string rich_func_name,
        const std::string file_name,
        const std::size_t line_no
    ) : name(name),
        func_name(func_name),
        rich_func_name(rich_func_name),
        file_name(file_name),
        line_no(std::to_string(line_no)) {

        get_context_stack().push_back(this);
    }

    virtual ~Context() {
        get_context_stack().pop_back();
    }

    const std::string name;
    const std::string func_name;
    const std::string rich_func_name;
    const std::string file_name;
    const std::string line_no;
};

/**
 * Gets a context stack for each current thread.
 * A context object is managed per thread.
 */
inline const std::deque<Context *> & get_context_stack() {
    return Context::get_context_stack();
}

} // namespace plusctx

#ifdef _MSC_VER
#define PLUSCTX_RICHFUNCNAME __FUNCSIG__
#else
#define PLUSCTX_RICHFUNCNAME __PRETTY_FUNCTION__
#endif

/**
 * Macro for creating context.
 */
#define PLUSCTX_CTX(name) plusctx::Context plusctx_ctx__( \
    name, \
    __func__, \
    PLUSCTX_RICHFUNCNAME, \
    plusctx::detail::shorten_file_name(__FILE__), \
    __LINE__)

#endif // PLUSCTX_HPP__
