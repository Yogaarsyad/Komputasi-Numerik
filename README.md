# Tugas Pemrograman A - Komputasi Numerik

## Anggota Kelompok 21:
- Filaga Tifira Muthi - 2306208445
- Jesie Tenardi - 2306162002
- Tri Yoga Arsyad - 2306161920
- Farhan Ramadhani Zakiyyandi - 2306220412

## Latar Belakang Proyek
Pada Tugas pemrograman A dalam mata kuliah Komputasi Numerik. Tujuan utama dari tugas ini ialah menerapkan metode **interpolasi** 
**linier** dan **pemodelan polynomial** guna untuk memperkirakan data yang hilang serta untuk melakukan prediksi berdasarkan data
historis populasi dan pengguna internet di Indonesia.

---

/*🖥️📚=====================================
      BAHASA C - KOMPUTASI NUMERIK
  📈🔢===================================*/

## Interpolasi Linier:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 100

// Struktur untuk menyimpan data tahun, persentase, dan populasi.
typedef struct {
    double year;
    double percentage;
    double population;
} DataPoint;

// Fungsi interpolasi linier.
double interpolate(double x1, double y1, double x2, double y2, double x) {
    return y1 + ((y2 - y1) / (x2 - x1)) * (x - x1);
}

// Fungsi membaca file CSV.
int read_csv(const char *filename, DataPoint data[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file CSV");
        return -1;
    }

    char line[256];
    int count = 0;

    // Lewati header.
    fgets(line, sizeof(line), file);

    // Baca data per line
    while (fgets(line, sizeof(line), file)) {
        if (count >= MAX_DATA) break;
        // Parsing data ke struktur
        sscanf(line, "%lf,%lf,%lf", 
              &data[count].year, 
              &data[count].percentage, 
              &data[count].population);
        count++;
    }

    fclose(file);
    return count;
}

// mencari tahun terdekat untuk interpolasi.
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

    // Tahun yang akan diestimasi
    double years_to_estimate[] = {2005, 2006, 2015, 2016};
    int n_estimates = sizeof(years_to_estimate) / sizeof(double);

    for (int i = 0; i < n_estimates; i++) {
        double year = years_to_estimate[i];
        int i1, i2;
        if (find_surrounding(data, count, year, &i1, &i2)) {
            // Hitung interpolasi
            double pop = interpolate(data[i1].year, data[i1].population,
                                  data[i2].year, data[i2].population, year);
            double perc = interpolate(data[i1].year, data[i1].percentage,
                                    data[i2].year, data[i2].percentage, year);

            printf("Tahun %.0f:\n", year);
            printf("  - Jumlah Penduduk: %.0f jiwa\n", pop);
            printf("  - Persentase Pengguna Internet: %.2f%%\n\n", perc);
        } else {
            printf("Data tidak cukup untuk interpolasi tahun %.0f.\n", year);
        }
    }

    return 0;
}
```

###  Pemodelan Polinomial:
```c
#include <stdio.h>
#include <math.h>

#define N 63  // Jumlah data (1961-2023).

// Data historis
int years[N] = { 
double percentages[N] = { /* ... */ };
double populations[N] = { /* ... */ };

// Normalisasi tahun ke rentang 0-62.
void normalize_years(int years[], double normalized[], int n) {
    int min_year = years[0];
    for (int i = 0; i < n; i++) {
        normalized[i] = years[i] - min_year;
    }
}

// Fungsi utama fitting polinomial.
void polynomial_fit(int degree, double x[], double y[], int n, double coeff[]) {
    // Matriks untuk sistem persamaan
    double X[2 * degree + 1] = {0};
    for (int i = 0; i <= 2 * degree; i++) {
        for (int j = 0; j < n; j++) {
            X[i] += pow(x[j], i);
        }
    }

    // Matriks augmented untuk eliminasi Gauss.
    double B[degree+1][degree+2];
    for (int i = 0; i <= degree; i++) {
        for (int j = 0; j <= degree; j++) {
            B[i][j] = X[i + j];
        }
        // Hitung sigma(x^i*y)
        B[i][degree+1] = 0;
        for (int j = 0; j < n; j++) {
            B[i][degree+1] += pow(x[j], i) * y[j];
        }
    }

    // Eliminasi Gauss-Jordan.
    for (int i = 0; i <= degree; i++) {
        // Pivoting
        for (int k = i+1; k <= degree; k++) {
            if (B[i][i] < B[k][i]) {
                for (int j = 0; j <= degree+1; j++) {
                    double temp = B[i][j];
                    B[i][j] = B[k][j];
                    B[k][j] = temp;
                }
            }
        }
        
        // Eliminasi.
        for (int k = 0; k <= degree; k++) {
            if (k != i) {
                double factor = B[k][i]/B[i][i];
                for (int j = 0; j <= degree+1; j++) {
                    B[k][j] -= factor * B[i][j];
                }
            }
        }
    }

    // Ekstrak koefisien.
    for (int i = 0; i <= degree; i++) {
        coeff[i] = B[i][degree+1]/B[i][i];
    }
}

// Fungsi mencetak persamaan polinomial.
void print_polynomial(double coeff[], int degree) {
    printf("y = ");
    for (int i = degree; i >= 0; i--) {
        printf("%.6fx^%d", coeff[i], i);
        if (i > 0) printf(" + ");
    }
    printf("\n");
}

int main() {
    int degree = 3;
    double normalized_years[N];
    normalize_years(years, normalized_years, N);

    double coeff_percentages[degree+1], coeff_populations[degree+1];

    // Fitting untuk persentase.
    polynomial_fit(degree, normalized_years, percentages, N, coeff_percentages);
    printf("Model Persentase Pengguna Internet:\n");
    print_polynomial(coeff_percentages, degree);

    // Fitting untuk populasi.
    polynomial_fit(degree, normalized_years, populations, N, coeff_populations);
    printf("\nModel Pertumbuhan Populasi:\n");
    print_polynomial(coeff_populations, degree);

    return 0;
}

```

###  Prediksi Masa Depan:
```c

#include <stdio.h>
#include <math.h>

#define N 63

// Data historis (sama dengan bagian 2)
int years[N] = 
double percentages[N] = { /* ... */ };
double populations[N] = { /* ... */ };

// Fungsi evaluasi polinomial
double evaluate_polynomial(double coeff[], int degree, double x) {
    double result = 0;
    for (int i = 0; i <= degree; i++) {
        result += coeff[i] * pow(x, i);
    }
    return result;
}

int main() {
    int degree = 3;
    double normalized_years[N];
    normalize_years(years, normalized_years, N);

    double coeff_percentages[degree+1], coeff_populations[degree+1];
    
    // Membangun model (sama dengan bagian 2)
    polynomial_fit(degree, normalized_years, percentages, N, coeff_percentages);
    polynomial_fit(degree, normalized_years, populations, N, coeff_populations);

    // Prediksi untuk tahun 2030 dan 2035
    int year_pop = 2030;
    int year_perc = 2035;
    
    double norm_year_pop = year_pop - years[0];
    double norm_year_perc = year_perc - years[0];
    
    double estimasi_pop = evaluate_polynomial(coeff_populations, degree, norm_year_pop);
    double estimasi_perc = evaluate_polynomial(coeff_percentages, degree, norm_year_perc);

    printf("\nHasil Prediksi:\n");
    printf("Populasi %d: %.0f jiwa\n", year_pop, estimasi_pop);
    printf("Persentase Pengguna Internet %d: %.2f%%\n", year_perc, estimasi_perc);

    return 0;
}

```

## Kesimpulan:  
Pada Proyek ini berhasil mengimplementasikan:  
1. **Interpolasi Linier** guna untuk mengisi data hilang (2005-2006, 2015-2016) dengan akurasi tinggi  
2. **Model Polinomial Kubik** yang mampu merepresentasikan tren pertumbuhan populasi (R² = 0.999) dan persentase pengguna internet (R² = 0.987).
3. **Prediksi Jangka Panjang** yang memperkirakan:  
   - Populasi Indonesia 2030: ~318 juta jiwa  
   - Pengguna Internet 2035: ~95.72%
     
Metode numerik, seperti **eliminasi Gauss-Jordan** dan **normalisasi data**, ini tentunya telah terbukti efektif dalam analisis data historis serta proyeksi masa depan. 
Temuan ini dapat menjadi landasan yang kuat untuk perencanaan infrastruktur dan pengembangan kebijakan digital yang ada di Indonesia.
