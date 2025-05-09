#include <stdio.h>
#include <math.h>

#define N 64 // Number of data points (1960 to 2023)

// Data points
int years[N] = {
    1960, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969, 1970, 1971, 1972, 1973, 1974, 1975,
    1976, 1977, 1978, 1979, 1980, 1981, 1982, 1983, 1984, 1985, 1986, 1987, 1988, 1989, 1990, 1991,
    1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2007, 2008, 2009,
    2010, 2011, 2012, 2013, 2014, 2017, 2018, 2019, 2020, 2021, 2022, 2023
};
double percentages[N] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0.001059744, 0.026109477, 0.056623989, 0.194910264, 0.255306646, 0.444415936, 0.925564,
    2.01861, 2.13414, 2.38702, 2.60029, 5.78627, 7.91748, 6.92, 10.92, 12.28, 14.52, 14.94, 17.1432,
    32.3358, 39.9046, 47.6906, 53.7265, 62.1045, 66.4846, 69.2084
};
double populations[N] = {
    88296070, 90791249, 93375850, 96051424, 98833749, 101365130, 103792754, 106526393, 109450006,
    112517639, 115657495, 118833702, 122039841, 125288515, 128555045, 131843848, 135173655,
    138533541, 141953163, 145434834, 148950540, 152485035, 156052152, 159651381, 163251124,
    166776185, 170175065, 173511154, 176855065, 180201630, 183501098, 186778238, 190043744,
    193305168, 196591828, 199888057, 203204348, 206536095, 209826788, 213004668, 216077790,
    219097902, 222088495, 225048008, 227926649, 237062337, 240157903, 243220028, 246305322,
    249470032, 252698525, 255852467, 258877399, 267346658, 269951846, 272489381, 274814866,
    276758053, 278830529, 281190067
};

// Function to calculate polynomial coefficients
void polynomial_fit(int degree, double x[], double y[], int n, double coeff[]) {
    double X[2 * degree + 1];
    for (int i = 0; i <= 2 * degree; i++) {
        X[i] = 0;
        for (int j = 0; j < n; j++) {
            X[i] += pow(x[j], i);
        }
    }

    double B[degree + 1][degree + 2], a[degree + 1];
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            B[i][j] = X[i + j];
        }
        B[i][degree + 1] = 0;
        for (int j = 0; j < n; j++) {
            B[i][degree + 1] += pow(x[j], i) * y[j];
        }
    }

    for (int i = 0; i <= degree; i++) {
        for (int k = i + 1; k <= degree; k++) {
            if (B[i][i] < B[k][i]) {
                for (int j = 0; j <= degree + 1; j++) {
                    double temp = B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = temp;
                }
            }
        }
    }

    for (int i = 0; i <= degree; i++) {
        for (int k = 0; k <= degree; k++) {
            if (k != i) {
                double t = B[k][i] / B[i][i];
                for (int j = 0; j <= degree + 1; j++) {
                    B[k][j] -= t * B[i][j];
                }
            }
        }
    }

    for (int i = 0; i <= degree; i++) {
        a[i] = B[i][degree + 1] / B[i][i];
    }

    for (int i = 0; i <= degree; i++) {
        coeff[i] = a[i];
    }
}

int main() {
    int degree = 3; // Degree of the polynomial
    double coeff_percentages[degree + 1], coeff_populations[degree + 1];

    // Fit polynomial for percentages
    polynomial_fit(degree, years, percentages, N, coeff_percentages);

    // Fit polynomial for populations
    polynomial_fit(degree, years, populations, N, coeff_populations);

    // Print results
    printf("Persamaan Polinomial untuk Persentase Pengguna Internet:\n");
    printf("y = ");
    for (int i = degree; i >= 0; i--) {
        printf("%.6fx^%d ", coeff_percentages[i], i);
        if (i > 0) printf("+ ");
    }
    printf("\n\n");

    printf("Persamaan Polinomial untuk Pertumbuhan Populasi:\n");
    printf("y = ");
    for (int i = degree; i >= 0; i--) {
        printf("%.6fx^%d ", coeff_populations[i], i);
        if (i > 0) printf("+ ");
    }
    printf("\n");

    return 0;
}
