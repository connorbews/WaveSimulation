# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores ocean simulation. Using an oceanographic spectrum, inverse fast Fourier transform and power of OpenGL and C++, this project delivers authentic wave simulation. 

https://github.com/connorbews/vertexs/assets/49799089/c912705d-5ac3-43e8-a316-fe532ec3ba5a

# What I learned

- Practice coding in the OpenGL Shading Language
- Wrote compute shaders to achieve faster results, utilizing GPU acceleration.
- Gained a solid understanding of the principles of wave physics such as wave propagation, including wave equations, wave types, and wave behavior.
- Gained an understanding of how light interacts with water and programmed that functioanlity into the fragment shader.
- Gained experience using third party libraries such as GLFW and FFTW3

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

# Math

## Wave Initialization

To initialize the wave positions, I used a function known as a Wave Spectrum. This spectrum can be deconstructed into two constituent equations: the non-directional wave spectrum and the directional wave spectrum. By combining the products of these two spectra, I initialized a 256 x 256 grid. This grid exists within the spectral domain; for its conversion into a height field, the transformation utilized of the fftw library alongside a 2D inverse fast Fourier transform.

### Non-directional Wave Spectra

In this project I used the Pierson-Moskowitz Spectrum:

$$S_{pierson moskowitz}(\omega) = \frac{\alpha g^2}{\omega^5}exp(-\beta(\frac{\omega_0}{\omega})^4)$$

### Directional Wave Spectra

In this project I used the Positive Cosine Squared Directional Spreading Spectrum:

$$D_{cos^2}(\theta) = \begin{cases}
    \frac{2}{\pi}cos^2(\theta) & \text{if } \frac{-\pi}{2} < \theta < \frac{\pi}{2} \\
    0 & \text{otherwise}
\end{cases}$$

## Vertex Normals Calculation

I used the following formula to calculate the slope in the x direction:

$$d_x = \frac{1}{2}[h(i + 1, j) - h(i - 1, j)]$$

I used the following formula to calculate the slope in the y direction:

$$d_y = \frac{1}{2}[h(i, j + 1) - h(i, j - 1)]$$

I can then take the cross product of these vectors is the normal vector. The normal vector normalized results in the following equation:

$$n = \frac{(-d_x, -d_y, 1)}{\sqrt(d_x^2 + d_y^2 + 1)}$$

## Wave Propagation Model

The wave propagation model was defined as:

$$\tilde{h}(k_x, k_y) = \tilde{h}(k_x, k_y) \cdot exp(i\cdot g \sqrt(k_x^2 + k_y^2) \cdot dt)$$

Where $\tilde{h}(k_x, k_y)$ is the 256 x 256 grid in the spectral domain obtained from the oceanographic spectrum

Once again I need to calculate the inverse fast fourier transform in order to convert the spectral domain data into spatial domain data.

I increase the value of "dt", applying the wave propagation model again, and execute the inverse fast Fourier transform once more. This sequence is iteratively executed in a loop, enabling the simulation of the wave's progression over time.

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.
