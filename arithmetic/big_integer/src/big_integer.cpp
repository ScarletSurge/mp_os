#include "../include/big_integer.h"

big_integer &big_integer::trivial_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::trivial_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Karatsuba_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Karatsuba_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier) const
{
    throw not_implemented("big_integer &big_integer::Schonhage_Strassen_multiplication::multiply(big_integer &, big_integer const &)", "your code should be here...");
}

big_integer &big_integer::trivial_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::trivial_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::trivial_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Newton_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Newton_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::divide(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::divide(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::Burnikel_Ziegler_division::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    big_integer::multiplication_rule multiplication_rule) const
{
    throw not_implemented("big_integer &big_integer::Burnikel_Ziegler_division::modulo(big_integer &, big_integer const &, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer::big_integer(
    int const *digits,
    size_t digits_count,
    allocator *allocator)
{
    throw not_implemented("big_integer::big_integer(int *, size_t, allocator *)", "your code should be here...");
}

big_integer::big_integer(
    std::vector<int> const &digits,
    allocator *allocator)
{
    throw not_implemented("big_integer::big_integer(std::vector<int> const &, allocator *)", "your code should be here...");
}

big_integer::big_integer(
    std::string const &value_as_string,
    size_t base,
    allocator *allocator)
{
    throw not_implemented("big_integer::big_integer(std::string const &, size_t, allocator *)", "your code should be here...");
}

big_integer::~big_integer()
{
    throw not_implemented("big_integer::~big_integer()", "your code should be here...");
}

big_integer::big_integer(
    big_integer const &other)
{
    throw not_implemented("big_integer::big_integer(big_integer const &)", "your code should be here...");
}

big_integer &big_integer::operator=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator=(big_integer const &)", "your code should be here...");
}

big_integer::big_integer(
    big_integer &&other) noexcept
{
    throw not_implemented("big_integer::big_integer(big_integer &&other) noexcept", "your code should be here...");
}

big_integer &big_integer::operator=(
    big_integer &&other) noexcept
{
    throw not_implemented("big_integer &big_integer::operator=(big_integer &&) noexcept", "your code should be here...");
}

bool big_integer::operator==(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator==(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator!=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator!=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator<(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator>(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator<=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator<=(big_integer const &) const", "your code should be here...");
}

bool big_integer::operator>=(
    big_integer const &other) const
{
    throw not_implemented("bool big_integer::operator>=(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-() const
{
    throw not_implemented("big_integer big_integer::operator-() const", "your code should be here...");
}

big_integer &big_integer::operator+=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator+=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator+(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator+(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator+(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator-=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator-=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator-(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator-(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator-(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator*=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator*=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator*(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator*(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator*(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator/=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator/=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator/(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator/(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator/(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator%=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator%=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator%(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator%(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator%(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer big_integer::operator~() const
{
    throw not_implemented("big_integer big_integer::operator~() const", "your code should be here...");
}

big_integer &big_integer::operator&=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator&=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator&(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator&(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator&(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator|=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator|=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator|(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator|(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator|(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator^=(
    big_integer const &other)
{
    throw not_implemented("big_integer &big_integer::operator^=(big_integer const &)", "your code should be here...");
}

big_integer big_integer::operator^(
    big_integer const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(big_integer const &) const", "your code should be here...");
}

big_integer big_integer::operator^(
    std::pair<big_integer, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator^(std::pair<big_integer, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator<<=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator<<=(size_t)", "your code should be here...");
}

big_integer big_integer::operator<<(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(size_t) const", "your code should be here...");
}

big_integer big_integer::operator<<(
    std::pair<size_t, allocator *> const &shift) const
{
    throw not_implemented("big_integer big_integer::operator<<(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::operator>>=(
    size_t shift)
{
    throw not_implemented("big_integer &big_integer::operator>>=(size_t)", "your code should be here...");
}

big_integer big_integer::operator>>(
    size_t shift) const
{
    throw not_implemented("big_integer big_integer::operator>>(size_t) const", "your code should be here...");
}

big_integer big_integer::operator>>(
    std::pair<size_t, allocator *> const &other) const
{
    throw not_implemented("big_integer big_integer::operator>>(std::pair<size_t, allocator *> const &) const", "your code should be here...");
}

big_integer &big_integer::multiply(
    big_integer &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::multiply(big_integer &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::multiply(
    big_integer const &first_multiplier,
    big_integer const &second_multiplier,
    allocator *allocator,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::multiply(big_integer const &, big_integer const &, allocator *, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::divide(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::divide(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::divide(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::divide(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer &big_integer::modulo(
    big_integer &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer &big_integer::modulo(big_integer &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

big_integer big_integer::modulo(
    big_integer const &dividend,
    big_integer const &divisor,
    allocator *allocator,
    big_integer::division_rule division_rule,
    big_integer::multiplication_rule multiplication_rule)
{
    throw not_implemented("big_integer big_integer::modulo(big_integer const &, big_integer const &, allocator *, big_integer::division_rule, big_integer::multiplication_rule)", "your code should be here...");
}

std::ostream &operator<<(
    std::ostream &stream,
    big_integer const &value)
{
    throw not_implemented("std::ostream &operator<<(std::ostream &, big_integer const &)", "your code should be here...");
}

std::istream &operator>>(
    std::istream &stream,
    big_integer &value)
{
    throw not_implemented("std::istream &operator>>(std::istream &, big_integer &)", "your code should be here...");
}

[[nodiscard]] allocator *big_integer::get_allocator() const noexcept
{
    throw not_implemented("allocator *big_integer::get_allocator() const noexcept", "your code should be here...");
}