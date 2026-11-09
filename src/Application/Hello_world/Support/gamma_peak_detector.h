#ifndef GAMMA_PEAK_DETECTOR_H
#define GAMMA_PEAK_DETECTOR_H

#ifdef __cplusplus
extern "C" {

#endif


/**
 * Structure to hold detected peak parameters.
 * - peak_energy: Energy (in keV, here simply the index).
 * - fwhm: Full width at half maximum.
 * - resolution: Resolution in percent (FWHM/peak_energy * 100).
 * - count_rate: Simple moving average value at the peak.
 * - corr_height: Correlation score at the peak.
 * - isotope_index: Matching isotope index (if no match, set to -1).
 */
typedef struct {
    float peak_energy;
    float fwhm;
    float resolution;
    float count_rate;
    float corr_height;
    int isotope_index;
} PeakResult;

/**
 * Configuration parameters for the gamma detector.
 * - window_size: Window size for the simple moving average.
 * - threshold: Minimum prominence for peak detection.
 * - sigma: Standard deviation for the Gaussian kernel in the correlation.
 * - distance: Minimum distance (in indices) between peaks.
 * - tolerance: Tolerance (in keV) for isotope matching.
 */
typedef struct {
    int window_size;
    float threshold;
    float sigma;
    int distance;
    float tolerance;
} GammaDetectorConfig;

// Maximum number of data points.
enum {GPD_max_data_size = 4096};

/**
 * Structure to hold data needed for plotting the gamma spectrometry analysis.
 * All arrays are of fixed size.
 * - n: Length of the data arrays.
 * - x: X-axis values (indices: 0 to n-1).
 * - sma: Simple moving average computed from the input data.
 * - dynamic_corr: Dynamic Gaussian correlation values.
 * - peak_indices: Indices of detected peaks.
 * - peak_fwhm: FWHM for each detected peak.
 * - num_peaks: Number of peaks detected.
 */

typedef struct {
    int n;
    //float x[MAX_DATA_SIZE];
    float sma[GPD_max_data_size];
    float dynamic_corr[GPD_max_data_size];
    int peak_indices[GPD_max_data_size];
    float peak_fwhm[GPD_max_data_size];
    int num_peaks;
} GammaDetectorPlotData;

/**
 * Performs the full analysis of the gamma spectrometry data.
 *
 * @param data         Input gamma data array (size MAX_DATA_SIZE).
 * @param config       Configuration parameters.
 * @param peak_results Output array of PeakResult (should be at least MAX_DATA_SIZE in length).
 * @param plot_data    Pointer to a GammaDetectorPlotData structure to be filled with plot data.
 * @return             The number of detected peaks, or a negative value on error.
 */
int gamma_detector(const float data[GPD_max_data_size], GammaDetectorConfig config,
                   PeakResult peak_results[GPD_max_data_size], GammaDetectorPlotData *plot_data, float a, float b);

#ifdef __cplusplus
}
#endif

#endif // GAMMA_PEAK_DETECTOR_H
