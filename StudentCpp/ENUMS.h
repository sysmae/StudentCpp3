// enums.h
#ifndef ENUMS_H
#define ENUMS_H

enum GradingMethod {
    METHOD_1, // A: 상위 35%, B: 상위 70%
    METHOD_2  // A: 상위 50%, B: 상위 90%
};

// 학기를 나타내는 enum 정의
enum Term {
    FIRST_TERM = 1,  // Spring
    SECOND_TERM = 2  // Fall
};

#endif // ENUMS_H
