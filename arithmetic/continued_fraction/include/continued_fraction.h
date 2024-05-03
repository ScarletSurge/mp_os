#ifndef MP_OS_CONTINUED_FRACTION_H
#define MP_OS_CONTINUED_FRACTION_H

#include <vector>

#include <big_integer.h>
#include <fraction.h>

class continued_fraction final
{

private:

    continued_fraction() = default;

public:

    static std::vector<big_integer> to_continued_fraction_representation(
        fraction const &value);

    static fraction from_continued_fraction_representation(
        std::vector<big_integer> const &continued_fraction_representation);

    static std::vector<fraction> to_convergents_series(
        fraction const &value);

    static std::vector<fraction> to_convergents_series(
        std::vector<big_integer> const &continued_fraction_representation);

    static std::vector<bool> to_Stern_Brokot_tree_path(
        fraction const &value);

    static fraction from_Stern_Brokot_tree_path(
        std::vector<bool> const &path);

    static std::vector<bool> to_Calkin_Wilf_tree_path(
        fraction const &value);

    static fraction from_Calkin_Wilf_tree_path(
        std::vector<bool> const &path);

};

#endif //MP_OS_CONTINUED_FRACTION_H