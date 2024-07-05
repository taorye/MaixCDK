#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "stdlib.h"
#include <time.h>
#include "ui_utils.h"
#include "math.h"

static float xyz_table[] = {
    0.000000,  0.030353,  0.060705,  0.091058,  0.121411,  0.151763,  0.182116,  0.212469,
    0.242822,  0.273174,  0.303527,  0.334654,  0.367651,  0.402472,  0.439144,  0.477695,
    0.518152,  0.560539,  0.604883,  0.651209,  0.699541,  0.749903,  0.802319,  0.856813,
    0.913406,  0.972122,  1.032982,  1.096009,  1.161225,  1.228649,  1.298303,  1.370208,
    1.444384,  1.520851,  1.599629,  1.680738,  1.764195,  1.850022,  1.938236,  2.028856,
    2.121901,  2.217388,  2.315337,  2.415763,  2.518686,  2.624122,  2.732089,  2.842604,
    2.955683,  3.071344,  3.189603,  3.310477,  3.433981,  3.560131,  3.688945,  3.820437,
    3.954624,  4.091520,  4.231141,  4.373503,  4.518620,  4.666509,  4.817182,  4.970657,
    5.126946,  5.286065,  5.448028,  5.612849,  5.780543,  5.951124,  6.124605,  6.301002,
    6.480327,  6.662594,  6.847817,  7.036010,  7.227185,  7.421357,  7.618538,  7.818742,
    8.021982,  8.228271,  8.437621,  8.650046,  8.865559,  9.084171,  9.305896,  9.530747,
    9.758735,  9.989873, 10.224173, 10.461648, 10.702310, 10.946171, 11.193243, 11.443537,
    11.697067, 11.953843, 12.213877, 12.477182, 12.743768, 13.013648, 13.286832, 13.563333,
    13.843162, 14.126329, 14.412847, 14.702727, 14.995979, 15.292615, 15.592646, 15.896084,
    16.202938, 16.513219, 16.826940, 17.144110, 17.464740, 17.788842, 18.116424, 18.447499,
    18.782077, 19.120168, 19.461783, 19.806932, 20.155625, 20.507874, 20.863687, 21.223076,
    21.586050, 21.952620, 22.322796, 22.696587, 23.074005, 23.455058, 23.839757, 24.228112,
    24.620133, 25.015828, 25.415209, 25.818285, 26.225066, 26.635560, 27.049779, 27.467731,
    27.889426, 28.314874, 28.744084, 29.177065, 29.613827, 30.054379, 30.498731, 30.946892,
    31.398871, 31.854678, 32.314321, 32.777810, 33.245154, 33.716362, 34.191442, 34.670406,
    35.153260, 35.640014, 36.130678, 36.625260, 37.123768, 37.626212, 38.132601, 38.642943,
    39.157248, 39.675523, 40.197778, 40.724021, 41.254261, 41.788507, 42.326767, 42.869050,
    43.415364, 43.965717, 44.520119, 45.078578, 45.641102, 46.207700, 46.778380, 47.353150,
    47.932018, 48.514994, 49.102085, 49.693300, 50.288646, 50.888132, 51.491767, 52.099557,
    52.711513, 53.327640, 53.947949, 54.572446, 55.201140, 55.834039, 56.471151, 57.112483,
    57.758044, 58.407842, 59.061884, 59.720179, 60.382734, 61.049557, 61.720656, 62.396039,
    63.075714, 63.759687, 64.447968, 65.140564, 65.837482, 66.538730, 67.244316, 67.954247,
    68.668531, 69.387176, 70.110189, 70.837578, 71.569350, 72.305513, 73.046074, 73.791041,
    74.540421, 75.294222, 76.052450, 76.815115, 77.582222, 78.353779, 79.129794, 79.910274,
    80.695226, 81.484657, 82.278575, 83.076988, 83.879901, 84.687323, 85.499261, 86.315721,
    87.136712, 87.962240, 88.792312, 89.626935, 90.466117, 91.309865, 92.158186, 93.011086,
    93.868573, 94.730654, 95.597335, 96.468625, 97.344529, 98.225055, 99.110210, 100.000000
};

void ui_utils_rgb_to_lab(uint8_t rgb_values[3], int8_t lab[3])
{
    double r_lin = xyz_table[rgb_values[0]];
    double g_lin = xyz_table[rgb_values[1]];
    double b_lin = xyz_table[rgb_values[2]];

    double x = ((r_lin * 0.4124) + (g_lin * 0.3576) + (b_lin * 0.1805)) * (1.0 / 095.047);
    double y = ((r_lin * 0.2126) + (g_lin * 0.7152) + (b_lin * 0.0722)) * (1.0 / 100.000);
    double z = ((r_lin * 0.0193) + (g_lin * 0.1192) + (b_lin * 0.9505)) * (1.0 / 108.883);

    if (x > 0.008856)
        x = pow(x, 1.0 / 3.0);
    else
        x = (x * 7.787037) + 0.137931;

    if (y > 0.008856)
        y = pow(y, 1.0 / 3.0);
    else
        y = (y * 7.787037) + 0.137931;

    if (z > 0.008856)
        z = pow(z, 1.0 / 3.0);
    else
        z = (z * 7.787037) + 0.137931;

    int l, a, b;
    l = (int)(floor(116.0 * y) - 16.0);
    a = (int)(floor(500.0 * (x - y)));
    b = (int)(floor(200.0 * (y - z)));

    if (l < 0)
        l = 0;
    else if (l > 100)
        l = 100;

    if (a < -128)
        a = -128;
    else if (a > 127)
        a = 127;

    if (b < -128)
        b = -128;
    else if (b > 127)
        b = 127;

    lab[0] = l;
    lab[1] = a;
    lab[2] = b;
}