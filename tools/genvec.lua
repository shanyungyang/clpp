--          Copyright Shan-Yung Yang 2010.
-- Distributed under the Boost Software License, Version 1.0.
--    (See accompanying file LICENSE_1_0.txt or copy at
--          http://www.boost.org/LICENSE_1_0.txt)

-- genvec.lua
--
-- This program is written to generate vector.hpp which containts
-- constructors for OpenCL vector types.

TYPE_PREFIX = "cl_" -- OpenCL prefix for vector types

NUMBER = {[1]="one", [2]="two", [4]="four", [8]="eight", [16]="sixteen"}

function GenNameAndTypes(base, dim, arg_dim)
    -- function name: base#
    local name = base .. dim

    -- argument type: cl_base$
    local arg_type = TYPE_PREFIX .. base

    if arg_dim > 1 then
        arg_type = arg_type .. arg_dim
    end

    -- return type: cl_base#
    local ret_type = TYPE_PREFIX .. base .. dim
    return name, arg_type, ret_type
end

function GenVectorAssignment(dim, arg_dim, GetArgName)
    local body = ""
    for i = 0, dim/arg_dim-1 do
        if i%4 == 0 then
            body = body .. "\n   "
        end

        -- r.v$[i] = a#.v$
        body = string.format("%s r.v%d[%d] = %s.v%d;", body, arg_dim, i, GetArgName(i), arg_dim)
    end
    return body .. "\n"
end

function GenScalarAssignment(dim, arg_dim, GetArgName)
    local body = ""
    local k = 0
    for i = 0, dim/arg_dim-1 do
        for j = 0, arg_dim-1 do
            if k%4 == 0 then
                body = body .. "\n   "
            end

            if arg_dim == 1 then
                body = string.format("%s r.s[%d] = %s;", body, i, GetArgName(i))
            else
                body = string.format("%s r.s[%d] = %s.s[%d];", body, k, GetArgName(i), j)
            end

            k = k + 1
        end
    end
    return body .. "\n"
end

function GenComment(base, dim, arg_dim, simple)
    local article = function(i) if i == 8 then return "an" else return "a" end end

    local comment = string.format("/// Construct %s %i-D %s vector", article(dim), dim, base)

    if simple then
        if arg_dim == 1 then
            comment = string.format("%s from a scalar value.\n/// The scalar value will be replicated to all components of the returned vector.", comment)
        else
            comment = string.format("%s from %s %d-D vector.\n/// This %d-D vector will be replicated to all components of the returned vector.", comment, article(arg_dim), arg_dim, arg_dim)
        end
    else
        if arg_dim > 1 then
            comment = string.format("%s from %s %d-D vectors.", comment, NUMBER[dim/arg_dim], arg_dim)
        else
            comment = string.format("%s from %s elements.", comment, NUMBER[dim/arg_dim])
        end
    end
    return comment
end

function GenSimpleCtor(base, dim, arg_dim)
    local name, arg_type, ret_type = GenNameAndTypes(base, dim, arg_dim)
    
    local args = string.format("%s a", arg_type)
    local vsym = "__" .. string.upper(arg_type) .. "__"

    local GetArgName = function(i) return "a" end

    local core = GenScalarAssignment(dim, arg_dim, GetArgName)

    if arg_dim > 1 then
        core = string.format("\n#if defined( %s )%s#else%s#endif // %s\n",
            vsym,
            GenVectorAssignment(dim, arg_dim, GetArgName),
            core,
            vsym )
    end

    local body = string.format("    %s r;%s    return r;\n", ret_type, core)
    return string.format("inline %s %s(%s)\n{\n%s}\n", ret_type, name, args, body)
end

function GenComplexCtor(base, dim, arg_dim)
    local name, arg_type, ret_type = GenNameAndTypes(base, dim, arg_dim)

    local args = ""
    local argc = dim/arg_dim
    for i = 0, argc-1 do
        -- arguments are named by "cl_base si"
        args = string.format("%s%s a%d", args, arg_type, i)
        if i ~= argc-1 then
            args = args .. ", "
        end
    end
    
    local vsym = "__" .. string.upper(arg_type) .. "__"

    local GetArgName = function(i) return "a"..i end

    local core = GenScalarAssignment(dim, arg_dim, GetArgName)

    if arg_dim > 1 then
        core = string.format("\n#if defined( %s )%s#else%s#endif // %s\n",
            vsym,
            GenVectorAssignment(dim, arg_dim, GetArgName),
            core,
            vsym )
    end

    local body = string.format("    %s r;%s    return r;\n", ret_type, core)
    return string.format("inline %s %s(%s)\n{\n%s}\n", ret_type, name, args, body)
end

-- Generate headers
print("//          Copyright Shan-Yung Yang 2010.")
print("// Distributed under the Boost Software License, Version 1.0.")
print("//    (See accompanying file LICENSE_1_0.txt or copy at")
print("//          http://www.boost.org/LICENSE_1_0.txt)\n")

print("#ifndef CLPP_VECTOR_HPP")
print("#define CLPP_VECTOR_HPP\n")

print("/* vector.hpp")
print(" * This file is generated by genvec.lua automatically.")
print(" * DO NOT MODIFY THIS FILE DIRECTLY.")
print(" * Please edit genvec.lua and re-generate this file for modification.")
print(" */\n")

print("#include \"size.hpp\"\n")
print("namespace clpp {\n")

local TYPES = {
    "char", "uchar",
    "short", "ushort",
    "int", "uint",
    "long", "ulong",
    "float", "double"
}

local DIMS = { 1, 2, 4, 8, 16 }

for t, base in ipairs(TYPES) do
    for i, dim in ipairs(DIMS) do
        for j = 1, i-1 do
            print(GenComment(base, dim, DIMS[j], true))
            print(GenSimpleCtor(base, dim, DIMS[j]))

            print(GenComment(base, dim, DIMS[j], false))
            print(GenComplexCtor(base, dim, DIMS[j]))
        end
    end
end

print("} // namespace clpp\n")
print("#endif // CLPP_VECTOR_HPP")
