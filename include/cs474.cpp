#include "cs474.h"
// cs474-lib.cpp : Defines the functions for the static library.
//

// TODO: This is an example of a library function

#define SWAP(a,b) do { auto tmp = a; a = b; b = tmp; } while (0)

namespace cs474 {
	void fcns474lib()
	{

	}
	std::vector<uint8_t> normr(const std::vector<float>& data)
	{
		// First pass: Identify min and max values in the output_data
		auto min_val = *std::min_element(data.begin(), data.end());
		auto max_val = *std::max_element(data.begin(), data.end());

		std::vector<uint8_t> out_data(data.size());
		for (int i = 0; i < data.size(); ++i) {
			out_data[i] = ((data[i] - min_val) * 255) / (max_val - min_val);
		}

		return out_data;
	}

	std::optional<std::vector<uint8_t>> subSample(const std::vector<uint8_t>& rawData, int factor, int width, int height)
	{
		if (width != height)
		{
			return std::nullopt;
		}
		if ((width / factor) % 2 != 0)
		{
			return std::nullopt;
		}
		int newWidth = width / factor;
		int newHeight = height / factor;
		std::vector<uint8_t> subsampleData(newWidth * newHeight);

		for (int y = 0; y < newHeight; ++y)
		{
			for (int x = 0; x < newWidth; ++x)
			{
				int subIndex = (y * factor * width) + (x * factor);
				subsampleData[(y * newWidth) + x] = rawData[subIndex];
			}
		}
		return { subsampleData };
	}

	std::optional<std::vector<uint8_t>> scale(const std::vector<uint8_t>& subsampleData, int factor, int width, int height)
	{
		if (width != height)
		{
			return std::nullopt;
		}
		if ((width / factor) % 2 != 0)
		{
			return std::nullopt;
		}

		int scaledWidth = width * factor;
		int scaledHeight = height * factor;

		std::vector<uint8_t> scaledData(scaledWidth * scaledHeight);

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				uint8_t value = subsampleData[y * width + x];
				for (int sy = 0; sy < factor; ++sy) {
					for (int sx = 0; sx < factor; ++sx) {
						scaledData[(y * factor + sy) * scaledWidth + (x * factor + sx)] = value;
					}
				}
			}
		}

		return { scaledData };
	}

	std::optional<std::vector<uint8_t>> quantize(const std::vector<uint8_t>& data, uint8_t levels, int width, int height) {
		if (width != height)
		{
			return std::nullopt;
		}

		std::vector<uint8_t> quantized(width * height);
		int bin_size = UINT8_MAX / levels;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int bin = data[(y * width) + x] / bin_size;
				if (bin == levels) bin -= 1; // force into lower bin 
				quantized[(y * width) + x] = bin * bin_size;
			}
		}

		return { quantized };
	}

	std::vector<float> hist(const std::vector<uint8_t>& data) {
		std::vector<float> hist(UINT8_MAX + 1, 0);
		for (const auto& point : data) hist[point]++;
		return hist;
	}

	// https://math.stackexchange.com/a/2021283
	std::vector<float> cdf(const std::vector<float>& data) {
		std::vector<float> _cdf(data.size());
		std::partial_sum(data.begin(), data.end(), _cdf.begin());
		return _cdf;
	}

	std::vector<uint8_t> norm(const std::vector<float>& data, int total_pixels) {
		auto firstNonZeroIter = std::find_if(data.begin(), data.end(), [](float c) { return c > 0; });
		float minCdfValue = (firstNonZeroIter != data.end()) ? *firstNonZeroIter : 0;

		std::vector<uint8_t> norm(UINT8_MAX + 1);
		for (int i = 0; i < UINT8_MAX + 1; ++i) {
			norm[i] = static_cast<uint8_t>((data[i] - minCdfValue) * UINT8_MAX / (total_pixels - minCdfValue));
		}
		
		return norm;
	}

	std::vector<uint8_t> hist_eq(const std::vector<uint8_t>& data, int width, int height) {
		auto _hist = hist(data);
		auto _cdf = cdf(_hist);
		auto _norm = norm(_cdf, width * height);

		std::vector<uint8_t> equalized(width * height);
		for (int i = 0; i < width * height; ++i) {
			equalized[i] = _norm[data[i]];
		}

		return equalized;
	}

	std::optional<std::vector<uint8_t>> hist_spec(const std::vector<uint8_t>& srcData, const std::vector<uint8_t>& specData, int width, int height) {
		// Step 1: Equalize the histogram of the source image
		auto srcEqualized = hist_eq(srcData, width, height);
		auto specifiedHist = hist(specData);

		// Step 2: Equalize the specified histogram to obtain the mapping
		std::vector<float> specCdf = cdf(specifiedHist);
		std::vector<uint8_t> specNorm = norm(specCdf, std::accumulate(specifiedHist.begin(), specifiedHist.end(), 0));

		// Step 3: Apply the mapping to the uniformly equalized source image
		std::vector<uint8_t> specifiedData(width * height);
		for (int i = 0; i < width * height; ++i) {
			uint8_t srcValue = srcEqualized[i];
			uint8_t mappedValue = 0;
			size_t minDiff = SIZE_MAX;  // Initialize to maximum possible value
        
			// Search for the closest value in the specified histogram
			for (size_t j = 0; j <= UINT8_MAX; ++j) {
				size_t diff = std::abs(static_cast<int>(srcValue) - static_cast<int>(specNorm[j]));
				if (diff < minDiff) {
					minDiff = diff;
					mappedValue = static_cast<uint8_t>(j);
				}
			}
        
			specifiedData[i] = mappedValue;
		}

		return { specifiedData };
	}
	float computeNCC(const std::vector<uint8_t>& image_patch,
		const std::vector<uint8_t>& mask)
	{
		float mean_image = 0.0f;
		float mean_mask = 0.0f;

		for (size_t i = 0; i < image_patch.size(); ++i) {
			mean_image += image_patch[i];
			mean_mask += mask[i];
		}

		mean_image /= image_patch.size();
		mean_mask /= mask.size();

		float numerator = 0.0f;
		float denominator_image = 0.0f;
		float denominator_mask = 0.0f;

		for (size_t i = 0; i < image_patch.size(); ++i) {
			float diff_image = image_patch[i] - mean_image;
			float diff_mask = mask[i] - mean_mask;

			numerator += diff_image * diff_mask;
			denominator_image += diff_image * diff_image;
			denominator_mask += diff_mask * diff_mask;
		}

		return numerator / std::sqrt(denominator_image * denominator_mask);
	}
	std::vector<float> spatial_filtering(const std::vector<uint8_t>& image_data, int image_width, int image_height, const std::vector<uint8_t>& mask_data, int mask_width, int mask_height)
	{

		std::vector<float> similarity(image_width * image_height, 0.0f);

		for (int i = 0; i < image_height; ++i) {
			for (int j = 0; j < image_width; ++j) {
				std::vector<uint8_t> image_patch;

				for (int mi = 0; mi < mask_height; ++mi) {
					for (int mj = 0; mj < mask_width; ++mj) {
						int image_i = i + mi - mask_height / 2;
						int image_j = j + mj - mask_width / 2;
						
						if (image_i >= 0 && image_i < image_height && image_j >= 0 && image_j < image_width) {
							image_patch.push_back(image_data[image_i * image_width + image_j]);
						}
						else {
							image_patch.push_back(0);
						}
					}
				}
				similarity[i * image_width + j] = computeNCC(image_patch, mask_data);
			}
		}
		return similarity;
	}

	std::optional<std::vector<uint8_t>> smoothImage(const std::vector<uint8_t>& srcData, int width, int height, int filterSize, const std::vector<double>& filterMask) {

		std::vector<uint8_t> smoothedData(width * height);
		int offset = filterSize / 2;  // Offset to center the filter mask on a pixel

		//Apply filter to each pixel
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				double sum = 0.0;
				double weightSum = 0.0;

				//Apply the filter mask to neighboring pixels
				for (int j = -offset; j <= offset; ++j) {
					for (int i = -offset; i <= offset; ++i) {
						int newY = y + j;
						int newX = x + i;

						//Skip pixels outside the image boundaries
						if (newX < 0 || newX >= width || newY < 0 || newY >= height) {
							continue;
						}
						double weight = filterMask[(j + offset) * filterSize + (i + offset)];

						sum += srcData[newY * width + newX] * weight;
						weightSum += weight;
					}
				}
				//Normalize and assign the new pixel value
				smoothedData[y * width + x] = static_cast<uint8_t>(std::round(sum / weightSum));
			}
		}
		return { smoothedData };
	}

	std::optional<std::vector<uint8_t>> addNoise(const std::vector<uint8_t>& data, int width, int height, float percentage) {
		std::vector<uint8_t> corrupted = data;
		int total_pixels = width * height;
		int num_corrupted = static_cast<int>(percentage / 100.0 * total_pixels);

		std::srand(std::time(nullptr));

		for (int i = 0; i < num_corrupted; ++i) {
			int rand_index = std::rand() % total_pixels;
			uint8_t salt_or_pepper = (std::rand() % 2 == 0) ? 0 : 255;
			corrupted[rand_index] = salt_or_pepper;
		}

		return { corrupted };
	}

	std::optional<std::vector<uint8_t>> medianFilter(const std::vector<uint8_t>& data, int width, int height, int filterSize) {

		std::vector<uint8_t> filtered(data.size());
		int offset = filterSize / 2;

		// Iterate over each pixel in the image
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				std::vector<uint8_t> neighbors;

				// Collect neighbors
				for (int x = -offset; x <= offset; ++x) {
					for (int y = -offset; y <= offset; ++y) {
						int ni = i + x;
						int nj = j + y;

						// Check boundary conditions
						if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
							neighbors.push_back(data[ni * width + nj]);
						}
					}
				}

				// Find the median value among the neighbors
				std::sort(neighbors.begin(), neighbors.end());
				filtered[i * width + j] = neighbors[neighbors.size() / 2];
			}
		}

		return { filtered };
	}

	std::optional<std::vector<uint8_t>> unsharpAndBoostFilter(const std::vector<uint8_t>& originaldata, const std::vector<uint8_t>& blurredData, int width, int height, int kValue)
	{
		if (blurredData.size() != originaldata.size())
			return std::nullopt;

		std::vector<uint8_t> result(originaldata.size());

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int maskVal = originaldata[i * width + j] - blurredData[i * width + j];
				int newVal = originaldata[i * width + j] + static_cast<int>(kValue * maskVal);

				newVal = std::max(0, std::min(newVal, 255));
				result[i * width + j] = static_cast<uint8_t>(newVal);
			}
		}
		return { result };
	}
	std::vector<float> convolve(const std::vector<uint8_t>& data, int image_width, int image_height, const std::vector<int>& mask, int mask_width, int mask_height)
	{
		std::vector<float> convolution(image_width * image_height, 0.0f);

		for (int i = 0; i < image_height; ++i) {
			for (int j = 0; j < image_width; ++j) {
				std::vector<float> image_patch;

				for (int mi = 0; mi < mask_height; ++mi) {
					for (int mj = 0; mj < mask_width; ++mj) {
						int image_i = i + mi - mask_height / 2;
						int image_j = j + mj - mask_width / 2;

						if (image_i >= 0 && image_i < image_height && image_j >= 0 && image_j < image_width) {
							image_patch.push_back(data[image_i * image_width + image_j] * mask[mi * mask_width + mj]);
						}
						else {
							image_patch.push_back(0);
						}
					}
				}
				convolution[i * image_width + j] = std::accumulate(image_patch.begin(), image_patch.end(), 0);
			}
		}
		return convolution;
	}
	std::vector<float> convolve(const std::vector<float>& data, int image_width, int image_height, const std::vector<int>& mask, int mask_width, int mask_height)
	{
		std::vector<float> convolution(image_width * image_height, 0.0f);

		for (int i = 0; i < image_height; ++i) {
			for (int j = 0; j < image_width; ++j) {
				std::vector<float> image_patch;

				for (int mi = 0; mi < mask_height; ++mi) {
					for (int mj = 0; mj < mask_width; ++mj) {
						int image_i = i + mi - mask_height / 2;
						int image_j = j + mj - mask_width / 2;

						if (image_i >= 0 && image_i < image_height && image_j >= 0 && image_j < image_width) {
							image_patch.push_back(data[image_i * image_width + image_j] * mask[mi * mask_width + mj]);
						}
						else {
							image_patch.push_back(0);
						}
					}
				}
				convolution[i * image_width + j] = std::accumulate(image_patch.begin(), image_patch.end(), 0);
			}
		}
		return convolution;
	}
	std::vector<float> gradient_magnitude(const std::vector<float>& dataX, const std::vector<float>& dataY, int image_width, int image_height)
	{
		std::vector<float> magnitude(image_width * image_height);

		for (int i = 0; i < image_width * image_height; ++i) {
			magnitude[i] = std::sqrt(dataX[i] * dataX[i] + dataY[i] * dataY[i]);
		}

		return magnitude;
	}

	std::vector<uint8_t> sharpen(const std::vector<uint8_t>& data, const std::vector<uint8_t>& edges, int image_width, int image_height)
	{
		std::vector<uint8_t> sharpened(image_width * image_height);

		for (int i = 0; i < image_width * image_height; ++i) {
			uint8_t val = data[i] - edges[i];
			sharpened[i] = std::clamp(val, (uint8_t)0, (uint8_t)255);
		}

		return sharpened;
	}

	std::vector<uint8_t> clamp_overflow_to_uint8_max(std::vector<float>& data)
	{
		std::vector<uint8_t> result(data.size());
		std::transform(data.begin(), data.end(), result.begin(), [](float val) -> uint8_t {
			return static_cast<uint8_t>(std::min(255.0f, val));
		});
		return result;
	}
	std::vector<uint8_t> to_uint8_max(std::vector<float>& data)
	{
		std::vector<uint8_t> result(data.size());
		std::transform(data.begin(), data.end(), result.begin(), [](float val) -> uint8_t {
			return static_cast<uint8_t>(std::clamp(val, 0.0f, 255.0f));
			});
		return result;
	}
	std::vector<uint8_t> visualize_partials(const std::vector<float>& dataX)
	{
		int max_dx = *std::max_element(dataX.begin(), dataX.end());
		int min_dx = *std::min_element(dataX.begin(), dataX.end());

		std::vector<uint8_t> rgba;

		for (size_t i = 0; i < dataX.size(); ++i) {
			// Convert Gx and Gy to [0, 255]
			uint8_t normalizedDx = static_cast<uint8_t>(255.0 * (dataX[i] - min_dx) / (max_dx - min_dx));

			// Gradient magnitude for alpha
			uint8_t alpha = static_cast<uint8_t>(255.0 * std::sqrt(dataX[i] * dataX[i]) / std::sqrt(max_dx * max_dx));
				
			rgba.push_back(255);
			rgba.push_back(255);
			rgba.push_back(255);
			rgba.push_back(normalizedDx);
		}

		return rgba;
	}
	std::vector<uint8_t> flip(const std::vector<uint8_t>& dataX)
	{
		std::vector<uint8_t> result(dataX.size());
		std::transform(dataX.begin(), dataX.end(), result.begin(), [](uint8_t val) -> uint8_t {
			return std::clamp(255 - val, 0 ,255);
			});
		return result;
	}
	std::vector<uint8_t> threshold(const std::vector<uint8_t>& data, int thresholdValue, bool above = true) {
		std::vector<uint8_t> output(data.size(), 0);  // initialize to black

		// Compute gradient magnitudes
		for (size_t i = 0; i < data.size(); ++i) {
			if (((above) ? (data[i] > thresholdValue) : (data[i] < thresholdValue))) {
				output[i] = 255;  // set to white if above threshold
			}
		}

		return output;
	}
	std::vector<uint8_t> threshold_range(const std::vector<uint8_t>& data, int min, int max, bool flip = false)
	{
		std::vector<uint8_t> output(data.size(), 0);  // initialize to black

		// Compute gradient magnitudes
		for (size_t i = 0; i < data.size(); ++i) {
			if ((data[i] >= min) && (data[i] <= max)) {
				output[i] = (flip) ? 255 : 0;  // set to white if above threshold
			}
			else {
				output[i] = (flip) ? 0 : 255;
			}
		}

		return output;
	}

	void fft1D(float data[], unsigned long nn, int isign) {
		unsigned long n, mmax, m, j, istep, i;
		double wtemp, wr, wpr, wpi, wi, theta;
		float tempr, tempi;

		n = nn << 1; // n is the total size of the data array.
		j = 0; // Start j from 0 for zero-based indexing.
		// Bit-reversal section
		for (i = 0; i < n; i += 2) { // Start i from 0 for zero-based indexing.
			if (j > i) {
				SWAP(data[j], data[i]);
				SWAP(data[j + 1], data[i + 1]);
			}
			m = n >> 1;
			while (m >= 2 && j >= m) { // Change condition to j >= m to include m.
				j -= m;
				m >>= 1;
			}
			j += m;
		}

		// Danielson-Lanczos section
		mmax = 2;
		while (n > mmax) {
			istep = mmax << 1;
			theta = isign * (2 * M_PI / mmax);
			wtemp = sin(0.5 * theta);
			wpr = -2.0 * wtemp * wtemp;
			wpi = sin(theta);
			wr = 1.0;
			wi = 0.0;
			for (m = 0; m < mmax; m += 2) { // Start m from 0 for zero-based indexing.
				for (i = m; i < n; i += istep) { // Change condition to i < n to stay within array bounds.
					j = i + mmax;
					tempr = wr * data[j] - wi * data[j + 1];
					tempi = wr * data[j + 1] + wi * data[j];
					data[j] = data[i] - tempr;
					data[j + 1] = data[i + 1] - tempi;
					data[i] += tempr;
					data[i + 1] += tempi;
				}
				wr = (wtemp = wr) * wpr - wi * wpi + wr;
				wi = wi * wpr + wtemp * wpi + wi;
			}
			mmax = istep;
		}
		// Add the scaling step here, after the FFT computation for inverse
		if (isign == -1) { 
			for (i = 0; i < n; i++) {
				data[i] /= nn;
			}
		}
	}
	void fft2D(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, int isign) {
		// Temporary vectors for real and imaginary parts
		std::vector<float> tempReal(2 * width, 0.0);
		std::vector<float> tempImag(2 * height, 0.0);

		// Apply the 1D FFT to each row
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				tempReal[2 * j] = realPart[i * width + j];
				tempReal[2 * j + 1] = imagPart[i * width + j];
			}

			fft1D(tempReal.data(), width, isign);

			for (int j = 0; j < width; j++) {
				realPart[i * width + j] = tempReal[2 * j];
				imagPart[i * width + j] = tempReal[2 * j + 1];
			}
		}

		// Apply the 1D FFT to each column
		for (int j = 0; j < width; j++) {
			for (int i = 0; i < height; i++) {
				tempImag[2 * i] = realPart[i * width + j];
				tempImag[2 * i + 1] = imagPart[i * width + j];
			}

			fft1D(tempImag.data(), height, isign);

			for (int i = 0; i < height; i++) {
				realPart[i * width + j] = tempImag[2 * i];
				imagPart[i * width + j] = tempImag[2 * i + 1];
			}
		}
	}
	void fftShift(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height) {
		int halfWidth = width / 2;
		int halfHeight = height / 2;

		for (int y = 0; y < halfHeight; ++y) {
			for (int x = 0; x < halfWidth; ++x) {
				// Indices for the quadrants
				int indexTL = y * width + x;
				int indexBR = (y + halfHeight) * width + (x + halfWidth);
				int indexTR = y * width + (x + halfWidth);
				int indexBL = (y + halfHeight) * width + x;

				// Swap top-left with bottom-right
				SWAP(realPart[indexTL], realPart[indexBR]);
				SWAP(imagPart[indexTL], imagPart[indexBR]);

				// Swap top-right with bottom-left
				SWAP(realPart[indexTR], realPart[indexBL]);
				SWAP(imagPart[indexTR], imagPart[indexBL]);
			}
		}
	}
	void bandRejectFilter(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, float lowCutOff, float highCutOff) {
		float lowCutOffSq = lowCutOff * lowCutOff;
		float highCutOffSq = highCutOff * highCutOff;

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				float u = (i - height / 2) * (i - height / 2);
				float v = (j - width / 2) * (j - width / 2);
				float dSq = u + v;

				if (dSq >= lowCutOffSq && dSq <= highCutOffSq) {
					realPart[i * width + j] = 0.0; // Apply filter to real part
					imagPart[i * width + j] = 0.0; // Apply filter to imaginary part
				}
			}
		}
	}
	void applyFourNotchFilters(std::vector<float>& realPart, std::vector<float>& imagPart,
		int width, int height,
		int centerX1, int centerY1, int notchWidth1, int notchHeight1,
		int centerX2, int centerY2, int notchWidth2, int notchHeight2,
		int centerX3, int centerY3, int notchWidth3, int notchHeight3,
		int centerX4, int centerY4, int notchWidth4, int notchHeight4) {
		// Helper lambda to apply a single notch
		auto applyNotch = [&](int centerX, int centerY, int notchWidth, int notchHeight) {
			int halfWidth = notchWidth / 2;
			int halfHeight = notchHeight / 2;

			for (int i = centerY - halfHeight; i <= centerY + halfHeight; ++i) {
				for (int j = centerX - halfWidth; j <= centerX + halfWidth; ++j) {
					int idx = (i + height) % height * width + (j + width) % width; // Handle wrapping around edges
					realPart[idx] = 0.0; // Zero out real part
					imagPart[idx] = 0.0; // Zero out imaginary part
				}
			}
		};

		// Apply each notch
		applyNotch(centerX1, centerY1, notchWidth1, notchHeight1);
		applyNotch(centerX2, centerY2, notchWidth2, notchHeight2);
		applyNotch(centerX3, centerY3, notchWidth3, notchHeight3);
		applyNotch(centerX4, centerY4, notchWidth4, notchHeight4);
	}

	void applyFourNotchFiltersNoise(std::vector<float>& realPart, std::vector<float>& imagPart,
		int width, int height,
		int centerX1, int centerY1, int notchWidth1, int notchHeight1,
		int centerX2, int centerY2, int notchWidth2, int notchHeight2,
		int centerX3, int centerY3, int notchWidth3, int notchHeight3,
		int centerX4, int centerY4, int notchWidth4, int notchHeight4) {
		// Helper lambda to check if a point is within a notch
		auto isWithinNotch = [&](int x, int y, int centerX, int centerY, int notchWidth, int notchHeight) {
			int halfWidth = notchWidth / 2;
			int halfHeight = notchHeight / 2;
			return x >= centerX - halfWidth && x <= centerX + halfWidth && y >= centerY - halfHeight && y <= centerY + halfHeight;
		};

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				int idx = i * width + j;
				// Check if the point is outside all four notches
				if (!isWithinNotch(j, i, centerX1, centerY1, notchWidth1, notchHeight1) &&
					!isWithinNotch(j, i, centerX2, centerY2, notchWidth2, notchHeight2) &&
					!isWithinNotch(j, i, centerX3, centerY3, notchWidth3, notchHeight3) &&
					!isWithinNotch(j, i, centerX4, centerY4, notchWidth4, notchHeight4)) {
					realPart[idx] = 0.0; // Zero out real part
					imagPart[idx] = 0.0; // Zero out imaginary part
				}
			}
		}
	}

	void highFrequencyEmphasisFilter(std::vector<float>& realPart, std::vector<float>& imagPart, int width, int height, float D0, float c, float gammaL, float gammaH) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				// Center the frequency domain representation
				float u = i - height / 2.0f;
				float v = j - width / 2.0f;

				// Calculate squared distance from center
				float dSq = u * u + v * v;

				// Compute filter response at each location
				float response = (gammaH - gammaL) * (1 - exp(-c * dSq / (D0 * D0))) + gammaL;

				// Apply the filter to both the real and imaginary parts
				int index = i * width + j;
				realPart[index] *= response;
				imagPart[index] *= response;
			}
		}
	}
	std::vector<float> convolve1DHorizontal(const std::vector<uint8_t>& input, int width, int height, const std::vector<int>& kernel) {
		std::vector<float> output(input.size(), 0);
		int kernelSize = kernel.size();
		int halfKernel = kernelSize / 2;
    
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				float sum = 0.0f;
				for (int k = -halfKernel; k <= halfKernel; k++) {
					int idx = x + k;
					if (idx >= 0 && idx < width) {
						sum += input[y * width + idx] * kernel[halfKernel + k];
					}
				}
				output[y * width + x] = sum;
			}
		}
		return output;
	}

std::vector<float> convolve1DVertical(const std::vector<float>& input, int width, int height, const std::vector<int>& kernel) {
    std::vector<float> output(input.size(), 0);
    int kernelSize = kernel.size();
    int halfKernel = kernelSize / 2;
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float sum = 0.0f;
            for (int k = -halfKernel; k <= halfKernel; k++) {
                int idx = y + k;
                if (idx >= 0 && idx < height) {
                    sum += input[idx * width + x] * kernel[halfKernel + k];
                }
            }
            output[y * width + x] = sum;
        }
    }
    return output;
}

} // namespace cs474
