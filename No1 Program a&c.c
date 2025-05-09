#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 100

typedef struct {
    double year;
    double percentage;
    double population;
} DataPoint;

// Linear interpolation
double interpolate(double x1, double y1, double x2, double y2, double x) {
    return y1 + ((y2 - y1) / (x2 - x1)) * (x - x1);
}

// Read CSV file into DataPoint array
int read_csv(const char *filename, DataPoint data[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open CSV file");
        return -1;
    }

    char line[256];
    int count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        if (count >= MAX_DATA) break;
        sscanf(line, "%lf,%lf,%lf", &data[count].year, &data[count].percentage, &data[count].population);
        count++;
    }

    fclose(file);
    return count;
}

// Find surrounding known years for interpolation
int find_surrounding(DataPoint data[], int count, double target_year, int *i1, int *i2) {
    for (int i = 0; i < count - 1; i++) {
        if (data[i].year <= target_year && data[i + 1].year >= target_year) {
            *i1 = i;
            *i2 = i + 1;
            return 1;
        }
    }
    return 0;
}

int main() {
    DataPoint data[MAX_DATA];
    int count = read_csv("Data Tugas Pemrograman A.csv", data);
    if (count <= 0) return 1;

    // Years to interpolate
    double years_to_estimate[] = {2005, 2006, 2015, 2016};
    int n_estimates = sizeof(years_to_estimate) / sizeof(double);

    for (int i = 0; i < n_estimates; i++) {
        double year = years_to_estimate[i];
        int i1, i2;
        if (find_surrounding(data, count, year, &i1, &i2)) {
            double pop = interpolate(data[i1].year, data[i1].population, data[i2].year, data[i2].population, year);
            double perc = interpolate(data[i1].year, data[i1].percentage, data[i2].year, data[i2].percentage, year);

            printf("Tahun %.0f:\n", year);
            printf("  - Jumlah Penduduk: %.0f jiwa\n", pop);
            printf("  - Persentase Pengguna Internet: %.2f%%\n\n", perc);
        } else {
            printf("Data tidak cukup untuk menginterpolasi tahun %.0f.\n", year);
        }
    }

    return 0;
}

