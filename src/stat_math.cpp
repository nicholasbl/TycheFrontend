#include "stat_math.h"

#include <QDebug>

#include <random>

double compute_std_deviation(QVector<float> data) {
    double sum = 0.0;
    for (auto d : data) {
        sum += d;
    }

    double average = sum / data.size();

    double sq_err = 0.0;

    for (auto d : data) {
        double v = (d - average);
        sq_err += v * v;
    }
    double variance = sq_err / data.size();

    return sqrt(variance);
}

static float compute_quartile(std::span<float> sorted_data, float quartile) {
    quartile              = std::clamp(quartile, 0.0f, 1.0f);
    auto   n              = sorted_data.size();
    auto   floating_index = (n - 1) * quartile;
    size_t lower          = std::floor(floating_index);
    size_t upper          = std::ceil(floating_index);
    auto   delta          = floating_index - lower;
    auto   lower_val      = sorted_data[lower];
    auto   upper_val      = sorted_data[upper];

    return (1.0 - delta) * lower_val + delta * upper_val;
}

MedianResult compute_median(std::span<float> sorted_data) {
    if (sorted_data.size() < 2) return {};

    bool is_odd = sorted_data.size() % 2 != 0;

    if (is_odd) {
        int index = std::floor(sorted_data.size() / 2.0);

        return {
            .median = sorted_data[index],
            .lower  = safe_subspan(sorted_data, 0, index),
            .upper  = safe_subspan(sorted_data, index + 1),
        };
    }

    int index = (sorted_data.size() / 2) - 1;

    return {
        .median = (sorted_data[index] + sorted_data[index + 1]) / 2.0f,
        .lower  = safe_subspan(sorted_data, 0, index + 1),
        .upper  = safe_subspan(sorted_data, index + 1),
    };
}

BoxplotResult compute_boxplot(QVector<float> sorted_data) {

    if (sorted_data.empty()) return {};

    std::sort(sorted_data.begin(), sorted_data.end());

    auto Q1 = compute_quartile(sorted_data, 0.25f);
    auto Q2 = compute_quartile(sorted_data, 0.5f);
    auto Q3 = compute_quartile(sorted_data, 0.75f);

    auto IOR = Q3 - Q1;

    auto lower_lim = Q1 - 1.5 * IOR;
    auto upper_lim = Q3 + 1.5 * IOR;

    // find lowest value that is not lower than the limit

    float lowest_valid  = sorted_data.back();
    float highest_valid = sorted_data.front();

    QVector<float> outliers;

    for (auto iter = sorted_data.rbegin(); iter != sorted_data.rend(); iter++) {
        if (*iter >= lower_lim) {
            lowest_valid = *iter;
        } else {
            outliers << *iter;
        }
    }

    for (auto e : sorted_data) {
        if (e <= upper_lim) {
            highest_valid = e;
        } else {
            outliers << e;
        }
    }


    return {
        .structure = QVector<float>()
                     << lowest_valid << Q1 << Q2 << Q3 << highest_valid,
        .outliers = outliers,
    };
}


// =============================================================================

const double EULER = std::exp(1.0);

inline float normal_kernel(float x) {
    return pow(EULER, -(x * x) / 2.0) / sqrt(2 * M_PI);
}

static float
compute_kde(std::span<float> sorted_data, float x, float bandwidth) {
    float sum = 0;
    for (auto v : sorted_data) {
        sum += normal_kernel((x - v) / bandwidth);
    }

    return sum / (sorted_data.size() * bandwidth);
}

ViolinResult compute_violin(QVector<float> data) {
    std::sort(data.begin(), data.end());
    // auto compute bandwidth

    auto std_dev = compute_std_deviation(data);

    float bw = 1.06 * std_dev * std::pow(data.size(), -1.0 / 5.0);

    QVector<float> ret;

    ret.reserve(data.size());

    for (auto v : data) {
        ret << compute_kde(data, v, bw);
    }

    return {
        .x = QVector<float>(data.begin(), data.end()),
        .y = ret,
    };
}
