#include "parallelConvolution.h"

parallelConvolution::parallelConvolution(Mat src, Mat& dst, Mat kernel)
	:m_src(src),
	m_dst(dst),
	m_kernel(kernel)
{
	
	sz = kernel.rows / 2;
	copyMakeBorder(m_src, m_src, sz, sz, sz, sz, BORDER_REPLICATE);
}


void parallelConvolution::operator()(const Range& range) const{
	for (int r=range.start;r<range.end;r++)
	{
		int i = r / m_src.cols, j = r % m_src.cols;
		double value = 0;
		//kernel
		for (int k=-sz;k<=sz;k++)
		{
			uchar* sptr = (uchar*)(m_src.ptr(i + sz + k));
			for (int l=-sz;l<=sz;l++)
			{
				value += m_kernel.ptr<double>(k + sz)[l + sz] * sptr[j + sz + l];
			}
		}
		m_dst.ptr(i)[j] = saturate_cast<uchar>(value);
	}

}