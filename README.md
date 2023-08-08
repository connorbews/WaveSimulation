# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores the fascinating world of oceanography. Using an oceanographic spectrum, we initiate waves in the spectral domain and efficiently convert them to the spatial domain using the inverse fast Fourier transform.

Leveraging a wave propagation model, our simulation enables realistic interactions and evolutions of the waves within the spectral domain. To bring these dynamic waves to life, we once again employ the inverse fast Fourier transform, smoothly converting the spectral domain information back to the spatial domain.

Through the combined power of OpenGL and C++, this project delivers authentic wave simulation. 

# Setup Instructions

## Linux

To run this project, you will need to have the following dependencies. Please type these commands into your terminal.

```sh
sudo apt-get update
sudo apt-get install cmake pkg-config
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev

sudo apt-get install libglew-dev libglfw3-dev libglm-dev
sudo apt-get install libao-dev libmpg123-dev
```

You will also need the glfw library. Please run the shell script using the command below to install glfw.

```sh
bash install_dependencies.sh
```

To download the FFTW library you will need to follow to installation instructions on the [FFTW Website](https://www.fftw.org/download.html)
## Windows

# Math

## Wave Initialization
## Vertex Normals Calculation
## Wave Propagation Model

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.