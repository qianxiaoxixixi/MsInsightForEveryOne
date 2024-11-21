//
// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
//
#ifndef PROFILER_SERVER_NUMBERSAFEUTIL_H
#define PROFILER_SERVER_NUMBERSAFEUTIL_H

#include <iostream>
#include <type_traits>
#include <limits>
#include <bitset>
#include <utility>
#include <unordered_map>
#include <cmath>

namespace NumberSafe {
template<typename T>
struct BaseType {
    using type = typename std::remove_cv<typename std::remove_reference_t<T>>::type;
};

template<typename T>
using BaseType_t = typename BaseType<T>::type;

/**
 * @brief 通过位运算比较的s中是否有超出T类型内存返回的值
 */
template<typename S, typename T>
bool bitUpper(S &&s)
{
    static_assert(std::is_integral_v<S> && std::is_integral_v<T>, "Only used on two int");
    if constexpr (sizeof(S) <= sizeof(T)) return false;
    using SType = BaseType_t<S>;
    using DType = BaseType_t<T>;
    DType mask = 0;
    SType res = s ^ mask;
    return res != 0;
}

/**
 * @brief 检查符号位是否为0
 */
template<typename T>
bool IsSignBitZero(T &&a)
{
    using BaseType = BaseType_t<T>;
    BaseType mask = static_cast<BaseType>(1) << (sizeof(BaseType) * 8 - 1);
    mask = a & mask;
    return mask == 0;
}

/**
 * @brief 检查S类型的参数能否被安全转换为T类型, 安全指不发生溢出，回绕，其它未定义行为，精度损失不在此列
 */
template<typename F, typename T>
bool IsSafeCast(const F &s)
{
    using S = BaseType_t<F>;
    auto src = static_cast<S>(s);
    (void) src; // 去除编译告警
    using D = BaseType_t<T>;
    if constexpr (std::is_same_v<S, D>) {
        return true;
    } else if constexpr (std::is_unsigned_v<S> && std::is_unsigned_v<D>) {
        if (sizeof(S) < sizeof(D)) return true;
        return !bitUpper<S, D>(std::forward<S>(src));
    } else if constexpr ((std::is_integral_v<S> && std::is_unsigned_v<D>)
                         || (std::is_unsigned_v<S> && std::is_integral_v<D>)) {
        if (!IsSignBitZero<S>(std::forward<S>(src))) return false;
        if (sizeof(S) < sizeof(D)) return true;
        return !bitUpper<S, D>(std::forward<S>(src));
    } else if constexpr (std::is_floating_point_v<S> && std::is_floating_point_v<D>) {
        if (std::is_same_v<S, D>) return true;
        if (std::numeric_limits<S>::digits < std::numeric_limits<D>::digits
            && std::numeric_limits<S>::max_exponent < std::numeric_limits<D>::max_exponent)
            return true;
        if (sizeof(S) > sizeof(D)) return (s > (std::numeric_limits<D>::max()) || s < std::numeric_limits<D>::min());
    } else if constexpr (std::is_integral_v<S> && std::is_floating_point_v<D>) {
        if (static_cast<D>(src) >= -std::numeric_limits<D>::max()
            && static_cast<D>(src) <= std::numeric_limits<D>::max())
            return true;
        return false;
    }
    return std::is_convertible_v<S, D>;
}

/**
 * @brief: 翻转符号位
 */
template<typename T>
auto Flip(T &&a)
{
    using BType = BaseType_t<T>;
    static_assert(std::is_integral_v<BType>, "Only integral type can flip");
    static_assert(!std::is_unsigned_v<BType>, "Unsigned type can't flip");
    BType num = a;
    if (IsSignBitZero(std::forward<BType>(a))) {
        num = ~num + 1;
    } else {
        num = ~(num - 1);
    }
    return num;
}

template<typename T, typename F>
struct OpInter {
    using addType = decltype(std::declval<T>() + std::declval<F>());
    using mulType = decltype(std::declval<T>() * std::declval<F>());
    using divType = decltype(std::declval<T>() / std::declval<F>());
};

template<typename T, typename F>
using OpInterAddType = typename OpInter<T, F>::addType;

template<typename T, typename F>
using OpInterMulType = typename OpInter<T, F>::mulType;

template<typename T, typename F>
using OpInterDivType = typename OpInter<T, F>::divType;

/**
 * @brief 加法
 * @attention 0 作为异常时的返回值
 */
template<typename T, typename F>
static inline auto Add(T &&a, F &&b)
{
    using LType = BaseType_t<T>;
    using RType = BaseType_t<F>;
    using InterType = OpInterAddType<LType, RType>;
    // 如果有一个操作数不能安全转换为InterType，则认为失败
    if (!IsSafeCast<LType, InterType>(a) || !IsSafeCast<RType, InterType>(b)) {
        return static_cast<InterType>(0);
    }
    auto l = static_cast<InterType>(a);
    auto r = static_cast<InterType>(b);
    if (r > 0 && l > std::numeric_limits<InterType>::max() - r) {
        return static_cast<InterType>(0);
    }
    InterType temp = l + r;
    return temp;
}

template<typename T, typename F>
static inline auto Sub(T &&a, F &&b)
{
    using LType = BaseType_t<T>;
    using RType = BaseType_t<F>;
    using InterType = OpInterAddType<LType, RType>;
    auto Lop = static_cast<InterType>(a);
    auto Rop = static_cast<InterType>(b);
    if (!IsSafeCast<LType, InterType>(a) || !IsSafeCast<RType, InterType>(b)) {
        return static_cast<InterType>(0);
    }
    if (std::is_unsigned_v<InterType>) {
        if (Lop < Rop) {
            return static_cast<InterType>(0);
        }
        return Lop - Rop;
    }
    Rop = Flip(Rop);
    return Add(Lop, Rop);
}

template<typename T, typename F>
static inline auto Division(T &&a, F &&b)
{
    using LType = BaseType_t<T>;
    using RType = BaseType_t<F>;
    using InterType = OpInterMulType<LType, RType>;
    if (b == 0) {
        return static_cast<InterType>(0);
    }
    if (std::is_floating_point_v<InterType> && std::fabs(b) < std::numeric_limits<InterType>::epsilon()) {
        return static_cast<InterType>(0);
    }
    if (!IsSafeCast<LType, InterType>(a) || !IsSafeCast<RType, InterType>(b)) {
        return static_cast<InterType>(0);
    }
    // overflow check
    auto Lop = static_cast<InterType>(a);
    auto Rop = static_cast<InterType>(b);
    if (std::is_integral_v<InterType> && !std::is_unsigned_v<InterType>) {
        if (Lop == std::numeric_limits<InterType>::min() && Rop == -1) {
            return static_cast<InterType>(0);
        }
    }
    InterType tmp = static_cast<InterType>(a) / static_cast<InterType>(b);
    if (std::is_floating_point_v<InterType> && (std::isinf(tmp) | std::isnan(tmp))) {
        return static_cast<InterType>(0);
    }
    return tmp;
}

template<typename T, typename F>
static inline auto Muls(T &&a, F &&b)
{
    using LType = BaseType_t<T>;
    using RType = BaseType_t<F>;
    using InterType = OpInterMulType<LType, RType>;
    if (a == 0 || b == 0) {
        return static_cast<InterType>(0);
    }
    if (!IsSafeCast<LType, InterType>(a) || !IsSafeCast<RType, InterType>(b)) {
        return static_cast<InterType>(0);
    }
    auto Lop = static_cast<InterType>(a);
    auto Rop = static_cast<InterType>(b);
    if (a > 0 && b > 0 && a > std::numeric_limits<InterType>::max() / b) {
        return static_cast<InterType >(0);
    } else if (a < 0 && b < 0 && a < std::numeric_limits<InterType>::max() / b) {
        return static_cast<InterType>(0);
    } else if (a < std::numeric_limits<InterType>::min() / b) {
        return static_cast<InterType>(0);
    }
    InterType tmp = Lop * Rop;
    if (std::is_floating_point_v<InterType> && std::isinf(tmp)) {
        return static_cast<InterType>(0);
    }
    if (b > 1 && tmp < static_cast<InterType>(a)) {
        return static_cast<InterType>(0);
    }
    return tmp;
}
}
#endif // PROFILER_SERVER_NUMBERSAFEUTIL_H
