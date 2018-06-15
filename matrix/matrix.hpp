#ifndef GRAPH_MATRIX_HPP_INCLUDED
#define GRAPH_MATRIX_HPP_INCLUDED

#include <vector>

template <typename T>
class Matrix {
public:
    using size_type = typename std::vector<std::vector<T>>::size_type;
    using reference =
        typename std::vector<T>::reference;
    using const_reference =
        typename std::vector<T>::const_reference;

    size_type rows() const {
        return data.size();
    }

    size_type columns() const {
        if (data.empty())
            return 0;
        else
            return data[0].size();
    }

    void resize(size_type n, size_type m, const T &fill = T()) {
        size_type old_rows = rows(), old_columns = columns();
        if (n != old_rows)
            data.resize(n, std::vector<T>(m, fill));
        if (m != old_columns)
            for (auto &row : data)
                row.resize(m, fill);
    }

    reference operator()(size_type i, size_type j) {
        expandToSave(i, j);
        return data[i][j];
    }

    const_reference operator()(size_type i, size_type j) const {
        return data[i][j]; // no expand
    }

    reference at(size_type i, size_type j) {
        return data.at(i).at(j);
    }

    const_reference at(size_type i, size_type j) const {
        return data.at(i).at(j);
    }

    const std::vector<std::vector<T>> &raw() const {
        return data;
    }

private:
    void expandToSave(size_type i, size_type j, const T &fill = T()) {
        size_type old_rows = rows(), old_columns = columns();
        if (i < old_rows && j < old_columns)
            return;
        if (i >= old_rows)
            data.resize(i + 1, std::vector<T>(old_columns, fill));
        if (j >= old_columns) {
            for (auto &row : data)
                row.resize(j + 1, fill);
        }
    }

    std::vector<std::vector<T>> data;
};

#endif // GRAPH_MATRIX_HPP_INCLUDED
