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

ViolinResult compute_violin(std::span<float> data) {
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


TEST_CASE("violin") {

    QVector<float> test_a = {
        -21.574079134211384, -20.23093023179554,   -19.32341737967034,
        -18.95137754822678,  -16.390131706117614,  -15.87526554182558,
        -15.657376070444364, -15.088337448609188,  -14.868576626119621,
        -14.54843548063353,  -13.785215270526978,  -13.560630326586518,
        -12.835225891619242, -11.952604510434764,  -11.308386307237765,
        -11.023930545189533, -8.972656869272983,   -8.719046476226351,
        -8.454757314712179,  -7.061319484031828,   -6.979250888209327,
        -6.904560302717435,  -6.762567314066899,   -6.12142305666369,
        -6.111850784118983,  -5.865585023556092,   -4.998643611049759,
        -4.8173960585942135, -4.6736201363332865,  -4.545549903189272,
        -4.227553118075367,  -4.136476920721355,   -4.024220353611152,
        -3.568886466657158,  -3.249935848105202,   -3.195667777961847,
        -2.2945388253859345, -2.0132692789924853,  -1.9575022325583655,
        -1.9002160512372614, -1.8474671682236834,  -1.7644730712404322,
        -1.3876646094326694, -1.3250695287665761,  -1.0728711225399459,
        -0.498142005922114,  -0.46482222407797325, -0.23549747084956563,
        0.32426931653653984, 0.4399650903116882,   0.46809597463884106,
        0.5296055539945308,  1.3079823904908263,   1.475746510658099,
        1.5762044928105206,  2.3658316490010254,   2.374290376759616,
        2.9889021921225254,  3.2464340841591404,   3.440901181085279,
        3.7576798256894497,  3.8674005948892036,   3.8990704298324275,
        4.049136888570326,   4.403781235800331,    4.787383977410958,
        4.983236890515171,   5.280176790780503,    5.639623047278381,
        6.050749769456805,   6.146390609894432,    6.35602335088337,
        6.372052362916039,   7.054439716576475,    8.045717410859057,
        8.162956801930001,   8.176084434590877,    8.258698241073581,
        8.792655717956809,   9.486207313315989,    9.503580985739399,
        9.68801281903266,    10.42663979770332,    10.428223853249868,
        11.122695107893621,  11.413688013946155,   13.335907541091437,
        14.147207173589514,  14.581005434658403,   16.08029458653787,
        16.798699203541698,  16.918762819005813,   17.17620286420839,
        18.153416157249474,  19.733603407379082,   20.253888878850788,
        23.452114705194305,  23.730846451751972,   24.670296835116744,
        30.053374028595535
    };

    auto res1 = compute_violin(test_a);

    std::vector<float> truth = {
        0.0061798839516656635, 0.007879693765850843, 0.009063051674043122,
        0.009548002116935569,  0.012731930619569141, 0.013325030964820836,
        0.013571555095324052,  0.014205355306049916, 0.014447275481743722,
        0.014798229559910286,  0.015637339889988056, 0.01588776445002531,
        0.016720286571841485,  0.017810888574053503, 0.018683589956277055,
        0.01909359947843713,   0.022548665698608517, 0.023034097444083624,
        0.023551270769081176,  0.026410995268076835, 0.02658309798437242,
        0.02673976603759041,   0.02703753072829238,  0.028372570276040524,
        0.028392289328841355,  0.028896347225712334, 0.030600323452243984,
        0.030938169613050875,  0.031200675417929473, 0.031430157705067525,
        0.0319809566584545,    0.03213342527913594,  0.0323179499256138,
        0.0330257589482831,    0.03348023997914685,  0.03355401116035166,
        0.03461947173639698,   0.03488816803245291,  0.03493774558551723,
        0.034987394541601165,  0.03503196321179437,  0.035099855891504994,
        0.03537373500096861,   0.03541377529119206,  0.03555939007753781,
        0.035797966544739816,  0.03580786680048477,  0.03586448433457746,
        0.035919646682508716,  0.035916665689342826, 0.035915209513817214,
        0.035911034300843706,  0.03574347917915116,  0.03568031026645248,
        0.03563803168173536,   0.035193123223946125, 0.03518730384713634,
        0.03470706962657846,   0.034472983496168966, 0.03428372716499082,
        0.0339528850572679,    0.03383188453995784,  0.03379635324668824,
        0.03362432990121583,   0.03319413682864573,  0.03269234951855592,
        0.03242196854707837,   0.03199429482446049,  0.0314489430087062,
        0.03078982152965542,   0.030631321963669092, 0.03027737771704562,
        0.030249952861997016,  0.029037958010212844, 0.027146171964894963,
        0.02691435654994969,   0.026888313247355515, 0.026724039414903127,
        0.02564881007675369,   0.02423040348124745,  0.024194739865449903,
        0.02381615510331619,   0.02230775514085117,  0.02230454638602156,
        0.020916322262451728,  0.020348545283075767, 0.016903790741757498,
        0.015640095873391532,  0.015012771111446896, 0.013082142451199652,
        0.012268047446652854,  0.012137731512938683, 0.011863255619652098,
        0.010874188391720949,  0.00940159084690009,  0.0089402019696334,
        0.006288704607649111,  0.00607329167914858,  0.005369543829991409,
        0.002189155466243922
    };

    for (size_t i = 0; i < truth.size(); i++) {
        REQUIRE_EQ(res1.y.value(i), doctest::Approx(truth.at(i)));
    }
}
