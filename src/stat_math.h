#pragma once

#include <QVector>

#include <span>

template <class T>
auto safe_subspan(std::span<T> sp,
                  size_t       offset,
                  size_t       count = std::dynamic_extent) {
    if (offset >= sp.size()) return std::span<T>();

    if (count != std::dynamic_extent) {
        if ((offset + count) >= sp.size()) { return sp.subspan(offset); }
    }

    return sp.subspan(offset, count);
}

struct MedianResult {
    float            median;
    std::span<float> lower, upper;
};

MedianResult compute_median(std::span<float> sorted_data);

struct BoxplotResult {
    QVector<float> structure;
    QVector<float> outliers;
};

struct ViolinResult {
    QVector<float> x, y;
};

double compute_std_deviation(QVector<float> data);

BoxplotResult compute_boxplot(QVector<float> data);

ViolinResult compute_violin(QVector<float> data);
