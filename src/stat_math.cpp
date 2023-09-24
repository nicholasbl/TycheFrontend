#include "stat_math.h"

#include "doctest.h"

#include <QDebug>

double compute_std_deviation(std::span<float> data) {
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

TEST_CASE("standard_deviation") {
    std::vector<float> data = { 2, 4, 4, 4, 5, 5, 7, 9 };

    auto res = compute_std_deviation(data);

    CHECK_EQ(res, 2.0);
}

struct MedianResult {
    float            median;
    std::span<float> lower, upper;
};

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

static MedianResult compute_median(std::span<float> sorted_data) {
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

template <class T>
std::vector<T> to_vector(std::span<T> t) {
    return std::vector<T>(t.begin(), t.end());
}

TEST_CASE("median") {
    auto d1  = std::vector<float> { 3, 5, 12 };
    auto r1  = compute_median(d1);
    auto r1l = std::vector<float> { 3 };
    auto r1u = std::vector<float> { 12 };

    REQUIRE_EQ(r1.median, 5);
    REQUIRE(to_vector(r1.lower) == r1l);
    REQUIRE(to_vector(r1.upper) == r1u);

    auto d2  = std::vector<float> { 3,  5,  7,  12, 13, 14, 21, 23,
                                    23, 23, 23, 29, 39, 40, 56 };
    auto r2  = compute_median(d2);
    auto r2l = std::vector<float> { 3, 5, 7, 12, 13, 14, 21 };
    auto r2u = std::vector<float> { 23, 23, 23, 29, 39, 40, 56 };

    REQUIRE_EQ(r2.median, 23);
    REQUIRE(to_vector(r2.lower) == r2l);
    REQUIRE(to_vector(r2.upper) == r2u);

    auto d3  = std::vector<float> { 3,  5,  7,  12, 13, 14, 21,
                                    23, 23, 23, 23, 29, 40, 56 };
    auto r3  = compute_median(d3);
    auto r3l = std::vector<float> { 3, 5, 7, 12, 13, 14, 21 };
    auto r3u = std::vector<float> { 23, 23, 23, 23, 29, 40, 56 };

    REQUIRE_EQ(r3.median, 22);
    REQUIRE(to_vector(r3.lower) == r3l);
    REQUIRE(to_vector(r3.upper) == r3u);
}

BoxplotResult compute_boxplot(std::span<float> sorted_data) {

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

TEST_CASE("box_plot") {
    {
        QVector<float> test_a = { 3.44, 3.52, 3.54, 3.51, 3.40,
                                  3.54, 3.56, 3.48, 3.50 };

        auto res1 = compute_boxplot(test_a);

        REQUIRE(res1.structure.value(0) == doctest::Approx(3.4));
        REQUIRE(res1.structure.value(1) == doctest::Approx(3.48));
        REQUIRE(res1.structure.value(2) == doctest::Approx(3.51));
        REQUIRE(res1.structure.value(3) == doctest::Approx(3.54));
        REQUIRE(res1.structure.value(4) == doctest::Approx(3.56));

        REQUIRE(res1.outliers.isEmpty());
    }

    {
        QVector<float> test_a = { 75, 58, 69, 80, 87, 77, 91, 67 };

        auto res1 = compute_boxplot(test_a);

        REQUIRE(res1.structure.value(0) == doctest::Approx(58));
        REQUIRE(res1.structure.value(1) == doctest::Approx(68.5));
        REQUIRE(res1.structure.value(2) == doctest::Approx(76));
        REQUIRE(res1.structure.value(3) == doctest::Approx(81.75));
        REQUIRE(res1.structure.value(4) == doctest::Approx(91));

        REQUIRE(res1.outliers.isEmpty());
    }

    {
        QVector<float> test_a = { 100, 110, 110, 110, 120, 120,
                                  130, 140, 140, 150, 170, 220 };

        auto res1 = compute_boxplot(test_a);

        REQUIRE(res1.structure.value(0) == doctest::Approx(100));
        REQUIRE(res1.structure.value(1) == doctest::Approx(110));
        REQUIRE(res1.structure.value(2) == doctest::Approx(125));
        REQUIRE(res1.structure.value(3) == doctest::Approx(142.5));
        REQUIRE(res1.structure.value(4) == doctest::Approx(170));

        REQUIRE(res1.outliers == QVector<float> { 220 });
    }
}
