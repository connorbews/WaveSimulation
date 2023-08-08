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

In order to initialize the waves position, we use a function called a Wave Spectrum. A wave spectrum is defined as a "function which expresses the time-average energy of a random ocean configuration as a continuous function of angular frequency ω and direction θ" (Horvath, pg 31). The wave spectrum can be broken into a product of two equations the non-directional wave spectra and the directional wave spectra.

### Non-directional Wave Spectra

The non-directional wave spectra is a function that describes the distribution of wave energy as a function of frequency.

In this project I used the Pierson-Moskowitz Spectrum:

$$S_{pierson moskowitz}(\omega) = \frac{\alpha g^2}{\omega^5}exp(-\beta(\frac{\omega_0}{\omega})^4)$$

### Directional Wave Spectra

## Vertex Normals Calculation
## Wave Propagation Model

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.