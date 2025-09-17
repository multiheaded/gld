#ifndef GRIDLOCK_DEFENDERS_GLD_H
#define GRIDLOCK_DEFENDERS_GLD_H

#include <Eigen/Core>


namespace gld {
    using Vector2u = Eigen::Matrix<std::size_t, 2, 1>;
    using Vector2f = Eigen::Matrix<float, 2, 1>;
    using Vector2i = Eigen::Matrix<int, 2, 1>;

    struct UnsignedRect {
        Vector2u position;
        Vector2u size;
    };
}

#endif //GRIDLOCK_DEFENDERS_GLD_H