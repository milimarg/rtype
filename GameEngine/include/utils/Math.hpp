/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-1-rtype-noe.tritsch
** File description:
** Math.hpp
*/

#ifndef B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_MATH_HPP
    #define B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_MATH_HPP

    #include <cmath>
    #include <cstdlib>
    #include <ctime>
    #include <iostream>
    #include <limits>
    #include <memory>
    #include <thread>
    #include <random>

class Math {
    public:
        static double degreesToRadians(double degrees);
        static double randomDouble();
        static double randomDouble(double min, double max);
        static int randomInt(int min, int max);

        static const double _infinity;
        static const double _pi;
};

#endif /* B_CPP_500_STG_5_1_RTYPE_NOE_TRITSCH_MATH_HPP */
