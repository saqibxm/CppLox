//
// Created by F20242661308 on 11/22/2024.
//


#include "Callable.hpp"
#include "LoxFunction.hpp"

#include <thread>

#ifndef SLEEP_H
#define SLEEP_H

namespace lox
{
    struct Sleep final : public NativeFunction
    {
        Sleep() : NativeFunction("sleep") {}
        unsigned arity() const noexcept override { return 1; }
        Object call(Interpreter&, const ArgumentList &args) override
        {
            std::this_thread::sleep_for(std::chrono::duration<Operand>(args.front().get_number()));
            return {};
        }
    };
}

#endif //SLEEP_H
