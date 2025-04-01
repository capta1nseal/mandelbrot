#ifndef _MANDELBROTGRID2D
#define _MANDELBROTGRID2D

#include <cassert>
#include <utility>
#include <vector>

// grid wrapping std::vector<T> that can be indexed with [x, y] syntax.
template <typename T> class Grid2d {
public:
    Grid2d() {
        m_width = 0;
        m_height = 0;
    }
    Grid2d(std::size_t width, std::size_t height) { resize(width, height); }

    Grid2d& operator=(const Grid2d& other) {
        if (this == &other)
            return *this;

        m_width = other.m_width;
        m_height = other.m_height;

        data = other.data;

        return *this;
    }
    Grid2d& operator=(Grid2d<T>&& other) noexcept {
        if (this == &other)
            return *this;

        data = std::move(other.data);
        m_width = std::exchange(other.m_width, 0ul);
        m_height = std::exchange(other.m_height, 0ul);

        return *this;
    }

    void resize(std::size_t width, std::size_t height) {
        m_width = width;
        m_height = height;
        data.resize(m_width * m_height);
    }
    void assign(std::size_t width, std::size_t height, T value) {
        assert(width <= m_width and height <= m_height);

        for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
                (*this)[x, y] = value;
            }
        }
    }

    T& operator[](std::size_t x, std::size_t y) {
        assert(x < m_width and y < m_height);
        return data[y * m_width + x];
    }

    std::size_t width() const { return m_width; }
    std::size_t height() const { return m_height; }
    std::size_t size() const { return m_width * m_height; }

private:
    std::size_t m_width;
    std::size_t m_height;
    std::vector<T> data;
};

#endif
