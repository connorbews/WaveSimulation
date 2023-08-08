# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores the fascinating world of oceanography. Using an oceanographic spectrum, we initiate waves in the spectral domain and efficiently convert them to the spatial domain using the inverse fast Fourier transform.

Leveraging a wave propagation model, our simulation enables realistic interactions and evolutions of the waves within the spectral domain. To bring these dynamic waves to life, we once again employ the inverse fast Fourier transform, smoothly converting the spectral domain information back to the spatial domain.

Through the combined power of OpenGL and C++, this project delivers authentic wave simulation. 

# Setup Instructions

## Linux

```sh
sudo apt-get update
sudo apt-get install cmake pkg-config
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev

sudo apt-get install libglew-dev libglfw3-dev libglm-dev
sudo apt-get install libao-dev libmpg123-dev
```
## Windows

# Math

## Wave Initialization
## Vertex Normals Calculation
## Wave Propagation Model

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.