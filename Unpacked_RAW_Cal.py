#author: Harryu
# this script use to calulate 10 bit raw image with GRBG bayer
# The resolution could be set, the default is (3024,4032)
import numpy as np
import os
from matplotlib import pyplot as plt
import sys


class RawConvert():
    def tobmp(files):
        files = np.fromfile(files, dtype=np.uint16)
        change_files = np.reshape(files, (3024, 4032))
        return change_files

    def meanvalue(A, x1, y1, x2, y2):
        Gr_sum = R_sum = Gb_sum = B_sum = Gr_counter = R_counter = Gb_counter = B_counter = 0
        for i in range(x1, x2):  # 0
            for j in range(y1, y2):  # 0
                if(i % 2) == 0:
                    if(j % 2) == 0:
                        Gr_sum = Gr_sum + A[i, j]
                        Gr_counter += 1
                    if(j % 2) == 1:
                        R_sum = R_sum + A[i, j]
                        R_counter += 1
                if (i % 2) == 1:  # gb
                    if(j % 2) == 0:
                        B_sum = B_sum + A[i, j]
                        B_counter += 1
                    if(j % 2) == 1:
                        Gb_sum = Gb_sum + A[i, j]
                        Gb_counter += 1
        mean_Gr = Gr_sum/(Gr_counter)
        mean_R = R_sum/(R_counter)
        mean_B = B_sum/(B_counter)
        mean_Gb = Gb_sum/(Gb_counter)
        print("mean_Gr=", mean_Gr)
        print("mean_R=", mean_R)
        print("mean_B=", mean_B)
        print("mean_Gb=", mean_Gb)
        # print("mean_B=",mean_B)
path = 'D:\\cpp\\upload_github\\mipi_unpacked_demo.raw'
Bayer_Matrix = RawConvert.tobmp(path)
height = Bayer_Matrix.shape[0]
width = Bayer_Matrix.shape[1]
# cal center
RawConvert.meanvalue(Bayer_Matrix, int(height*(1/3)),
                     int(width*(1/3)), int(height*(2/3)), int(width*(2/3)))
plt.imshow(Bayer_Matrix)
plt.show()
