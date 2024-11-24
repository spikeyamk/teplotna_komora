#pragma once

namespace admin {
    // helper type for the visitor
    template<class... Ts>
    struct overloaded : Ts... { using Ts::operator()...; };
}