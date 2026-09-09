#include "gamma_peak_detector.h"
#include "Isotope_table.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* ---------------- Internal Helper Functions ---------------- */

// Computes a simple moving average (SMA) on the input data.
static void compute_sma(const float data[GPD_max_data_size], float sma[GPD_max_data_size], int n, int window_size) {
    int half = window_size / 2;
    for (int i = 0; i < n; i++) {
        int start = i - half;
        int end = i + half;
        if (start < 0) start = 0;
        if (end >= n) end = n - 1;
        float sum = 0.0f;
        int count = 0;
        for (int j = start; j <= end; j++) {
            sum += data[j];
            count++;
        }
        sma[i] = sum / count;
    }
}

// Computes a dynamic Gaussian correlation for the data.
// For each index i (starting at 1), a Gaussian kernel is constructed with a width scaling as sqrt(i).
static void dynamic_gaussian_correlation(const float data[GPD_max_data_size], float corr[GPD_max_data_size], int n, float sigma) {
    corr[0] = 0.0f;
    for (int i = 1; i < n; i++) {
        float std_dev = sqrtf((float)i);
        int kernel_range = (int)(sigma * std_dev);
        if (kernel_range < 1) kernel_range = 1;
        int kernel_size = 2 * kernel_range + 1;
        float *kernel = malloc(kernel_size * sizeof(float));
        if (!kernel) exit(1);
        float sum_kernel = 0.0f;
        for (int k = -kernel_range; k <= kernel_range; k++) {
            float value = expf(- (k * k) / (2.0f * std_dev * std_dev));
            kernel[k + kernel_range] = value;
            sum_kernel += value;
        }
        float kernel_mean = sum_kernel / kernel_size;
        float *kernel_dev = malloc(kernel_size * sizeof(float));
        if (!kernel_dev) { free(kernel); exit(1); }
        float norm_factor = 0.0f;
        for (int k = 0; k < kernel_size; k++) {
            kernel_dev[k] = kernel[k] - kernel_mean;
            norm_factor += kernel_dev[k] * kernel_dev[k];
        }
        for (int k = 0; k < kernel_size; k++) {
            if (norm_factor != 0.0f)
                kernel_dev[k] /= norm_factor;
        }
        int start_index = i - kernel_range;
        int end_index = i + kernel_range;
        if (start_index < 0) start_index = 0;
        if (end_index >= n) end_index = n - 1;
        float dot = 0.0f;
        for (int j = start_index; j <= end_index; j++) {
            int k_idx = j - (i - kernel_range);
            if (k_idx >= 0 && k_idx < kernel_size)
                dot += data[j] * kernel_dev[k_idx];
        }
        corr[i] = (dot > 0.0f) ? dot : 0.0f;
        free(kernel);
        free(kernel_dev);
    }
}

#ifdef USE_dynamic_gaussian_correlation_explained
static void matched_filter(const float data[GPD_max_data_size], float corr[GPD_max_data_size], int n, float sigma) {
    corr[0] = 0.0f;
    for (int i = 1; i < n; i++) {
    	//step 1 - determine the size of the kernel.
        float std_dev = sqrtf((float)i);
        int kernel_range = (int)(sigma * std_dev);
        if (kernel_range < 1) kernel_range = 1;
        int kernel_size = 2 * kernel_range + 1;

        //step 2 - allocate memory for the kernel.
        float *kernel = malloc(kernel_size * sizeof(float));
        if (!kernel) exit(1);

        //step 3 - compute the kernel values. which is a bell curve with gaussian distribution.
        float sum_kernel = 0.0f;
        for (int k = -kernel_range; k <= kernel_range; k++) {
            float value = expf(- (k * k) / (2.0f * std_dev * std_dev));
            kernel[k + kernel_range] = value;
            sum_kernel += value;
        }
        float kernel_mean = sum_kernel / kernel_size;

        //step 4 - Allocate memory for the kernel deviation.
        float *kernel_dev = malloc(kernel_size * sizeof(float));
        if (!kernel_dev) { free(kernel); exit(1); }

        //step 5 - Subtract the mean from the kernel values to center the kernel around 0.
        float norm_factor = 0.0f;
        for (int k = 0; k < kernel_size; k++) {
            kernel_dev[k] = kernel[k] - kernel_mean;
            norm_factor += kernel_dev[k] * kernel_dev[k];
        }

        //step 6 - Normalize the kernel
        for (int k = 0; k < kernel_size; k++) {
            if (norm_factor != 0.0f)
                kernel_dev[k] /= norm_factor;
        }

        //step 7 - find the start and end index for the correlation.
        int start_index = i - kernel_range;
        int end_index = i + kernel_range;
        if (start_index < 0) start_index = 0;
        if (end_index >= n) end_index = n - 1;

        //step 8 - Compute the dot product of the data points and the kernel deviation.
        float dot = 0.0f;
        for (int j = start_index; j <= end_index; j++) {
            int k_idx = j - (i - kernel_range);
            if (k_idx >= 0 && k_idx < kernel_size)
                dot += data[j] * kernel_dev[k_idx];
        }
        corr[i] = (dot > 0.0f) ? dot : 0.0f;
        free(kernel);
        free(kernel_dev);
    }
}

static void dynamic_gaussian_correlation_explained(const float data[GPD_max_data_size], float corr[GPD_max_data_size], int n, float sigma) {
    corr[0] = 0.0f;
    for (int i = 1; i < n; i++) {
    	//step 1 - determine the size of the kernel.
        float std_dev = sqrtf((float)i); // This makes the kernel wider for later points in the data. In practice, this means that the correlation will be more sensitive to noise in the earlier data points.
        int kernel_range = (int)(sigma * std_dev); // The kernel range is scaled by the standard deviation, which increases with i. example sigma = 2 will make the kernel range 2*sqrt(ex.100) = 20.
        if (kernel_range < 1) kernel_range = 1; // Ensure the kernel range is at least 1.
        int kernel_size = 2 * kernel_range + 1; // The size of the kernel is determined by the range. *2 for both sides plus 1 for the center.

        //step 2 - allocate memory for the kernel.
        float *kernel = malloc(kernel_size * sizeof(float)); // Allocate memory for the kernel.
        if (!kernel) exit(1); // Check for memory allocation failure.


        // Compute the Gaussian kernel values.
        //kernal is a 1D array that represents the Gaussian function values. wich is used to weight the data points around the current index i.
        //the data looks like a bell curve where the center is the current index i and the width is determined by the std_dev.
        //the kernel values are calculated using the Gaussian function, which is defined as:
        //value = exp(- (k * k) / (2.0f * std_dev * std_dev));
        //where k is the offset from the center of the kernel (i.e., k = -kernel_range to kernel_range).
        //the kernel values are normalized to ensure that the sum of the kernel values is equal to 1.
        //this is done by dividing each kernel value by the sum of all kernel values.
        //this normalization ensures that the correlation value is not biased by the kernel size.
        //the kernel values are then used to compute the correlation value for the current index i.

        // Loop through the kernel range. This leves us with a bell curve shape with the center at 0, the width is determined by the std_dev.
        // highest point is at 0 = 1 and the lowest at the edges.

        //step 3 - compute the kernel values. which is a bell curve with gaussian distribution.
        float sum_kernel = 0.0f; // Initialize the sum of the kernel.
        for (int k = -kernel_range; k <= kernel_range; k++) { // Loop through the kernel range.
            float value = expf(- (k * k) / (2.0f * std_dev * std_dev)); // Gaussian function.
            kernel[k + kernel_range] = value; // Store the value in the kernel array.
            sum_kernel += value; // Sum the kernel values.
        }
        float kernel_mean = sum_kernel / kernel_size; // Calculate the mean of the kernel values.


        //step 4 - Allocate memory for the kernel deviation.
        float *kernel_dev = malloc(kernel_size * sizeof(float)); // Allocate memory for the kernel deviation.
        if (!kernel_dev) { free(kernel); exit(1); } // Check for memory allocation failure.

        //step 5 - Subtract the mean from the kernel values to center the kernel around 0. (kernel array is not used anymore after this point)
        float norm_factor = 0.0f; // Initialize the normalization factor.
        for (int k = 0; k < kernel_size; k++) { // Loop through the kernel size.
            kernel_dev[k] = kernel[k] - kernel_mean; // Center the kernel around 0.
            norm_factor += kernel_dev[k] * kernel_dev[k]; // Compute the norm factor (used in step 6).
        }
        //we now have a gausian distribution with a mean of 0 and a standard deviation of std_dev.

        //step 6 - Normalize the kernel deviation to ensure it has a unit norm. meaning the sum of the squares of the kernel values is equal to 1.
        for (int k = 0; k < kernel_size; k++) { // Normalize the kernel deviation.
            if (norm_factor != 0.0f) // Avoid division by zero.
                kernel_dev[k] /= norm_factor; // Normalize the kernel deviation.
        }
        //we now have a kernel that is centered around 0 and has a standard deviation of std_dev and a norm of 1.

        //step 7 - find the start and end index for the correlation.
        int start_index = i - kernel_range; // Calculate the start index for the correlation.
        int end_index = i + kernel_range; // Calculate the end index for the correlation.
        if (start_index < 0) start_index = 0; // Ensure the start index is not negative.
        if (end_index >= n) end_index = n - 1; // Ensure the end index does not exceed the data size.

        //step 8 - Compute the dot product of the data points and the kernel deviation.
        //this is the correlation value for the current index i.
        //the dot product is computed by multiplying each data point in the range with the corresponding kernel deviation value.
        //this gives us a weighted sum of the data points around the current index i.
        //the dot product is then stored in the correlation array.
        float dot = 0.0f;	 // Initialize the dot product.
        for (int j = start_index; j <= end_index; j++) { // Loop through the data points in the range.
            int k_idx = j - (i - kernel_range); // Calculate the index in the kernel. this is the offset from the center of the kernel.
            if (k_idx >= 0 && k_idx < kernel_size) // Ensure the kernel index is valid. this is the index of the kernel value that corresponds to the data point.
                dot += data[j] * kernel_dev[k_idx]; // Compute the dot product. this is the correlation value for the current index i.
        }
        corr[i] = (dot > 0.0f) ? dot : 0.0f; // Store the correlation value, ensuring it's non-negative.
        free(kernel); // Free the kernel memory.
        free(kernel_dev); // Free the kernel deviation memory.
    }
}
#endif

// Comparator for sorting PeakResult structures in descending order by correlation height.
static int compare_peak_results(const void *a, const void *b) {
    const PeakResult *pa = (const PeakResult *)a;
    const PeakResult *pb = (const PeakResult *)b;
    if (pa->corr_height < pb->corr_height)
        return 1;
    else if (pa->corr_height > pb->corr_height)
        return -1;
    return 0;
}

// Simple peak detector: finds indices where the correlation signal is a local maximum above the threshold.
// If two peaks are too close (less than 'distance'), only the higher peak is kept.
static int detect_peaks(const float corr[GPD_max_data_size], int n, int peaks[GPD_max_data_size], int distance, float threshold) {
    int count = 0;
    for (int i = 1; i < n - 1; i++) {
        if (corr[i] >= threshold && corr[i] > corr[i - 1] && corr[i] >= corr[i + 1]) {
            if (count > 0 && (i - peaks[count - 1]) < distance) {
                if (corr[i] > corr[peaks[count - 1]])
                    peaks[count - 1] = i;
            } else {
                peaks[count++] = i;
            }
        }
    }
    return count;
}

// Computes the full width at half maximum (FWHM) for a peak using linear interpolation.
static float compute_fwhm(const float corr[GPD_max_data_size], int n, int peak_index) {
    float peak_val = corr[peak_index];
    if (peak_val <= 0.0f)
        return 0.0f;
    float half = peak_val / 2.0f;
    int left = peak_index;
    while (left > 0 && corr[left] > half)
        left--;
    float left_interp = (float)left;
    if (left < peak_index && left + 1 < n) {
        float diff = corr[left + 1] - corr[left];
        if (diff != 0.0f)
            left_interp = left + (half - corr[left]) / diff;
    }
    int right = peak_index;
    while (right < n - 1 && corr[right] > half)
        right++;
    float right_interp = (float)right;
    if (right > peak_index && right - 1 >= 0) {
        float diff = corr[right] - corr[right - 1];
        if (diff != 0.0f)
            right_interp = right - (corr[right] - half) / diff;
    }
    return right_interp - left_interp;
}

/* ---------------- Public API Functions ---------------- */

// gamma_detector: performs the full analysis by computing the SMA, dynamic correlation,
// detecting peaks, computing FWHM and resolution, and finally identifying matching isotopes.
// The peak energy is simply taken as the index (in keV) and the isotope_index field in PeakResult
// is set accordingly.
int gamma_detector(const float data[GPD_max_data_size], GammaDetectorConfig config,
                   PeakResult peak_results[GPD_max_data_size], GammaDetectorPlotData *plot_data, float a, float b) {
    if (!plot_data)
        return -1;
    int n = GPD_max_data_size;
    plot_data->n = n;

    // Compute the simple moving average and dynamic correlation.
    compute_sma(data, plot_data->sma, n, config.window_size);
    dynamic_gaussian_correlation(plot_data->sma, plot_data->dynamic_corr, n, config.sigma);

    // Detect peaks.
    int temp_peaks[GPD_max_data_size];
    int num_peaks = detect_peaks(plot_data->dynamic_corr, n, temp_peaks, config.distance, config.threshold);
    plot_data->num_peaks = num_peaks;
    for (int i = 0; i < num_peaks; i++) {
        plot_data->peak_indices[i] = temp_peaks[i];
        plot_data->peak_fwhm[i] = compute_fwhm(plot_data->dynamic_corr, n, temp_peaks[i]);
    }

    // Fill the peak results.
    int result_count = 0;
    for (; result_count < num_peaks; result_count++) {
        int peak = plot_data->peak_indices[result_count];
        float fwhm = compute_fwhm(plot_data->dynamic_corr, n, peak) * a;
//        float peak_lsb = (float)peak;  // Using the index as the energy (in keV)
        float peak_kev = (float)peak * a + b;  // Convert to keV

        float resolution = (peak_kev != 0.0f) ? (fwhm / peak_kev * 100.0f) : 0.0f;
        float count_rate = plot_data->sma[peak];
        float corr_height = plot_data->dynamic_corr[peak];
        int isotope_index = identify_isotope(peak_kev, config.tolerance);

        peak_results[result_count].peak_energy = peak_kev;
        peak_results[result_count].fwhm = fwhm;  // Convert to keV from index;
        peak_results[result_count].resolution = resolution;
        peak_results[result_count].count_rate = count_rate;
        peak_results[result_count].corr_height = corr_height;
        peak_results[result_count].isotope_index = isotope_index;
    }

    // Sort the peak results by correlation height.
    if (result_count > 1)
        qsort(peak_results, result_count, sizeof(PeakResult), compare_peak_results);
    return result_count;
}
