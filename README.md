# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores ocean simulation. Using an oceanographic spectrum, Inverse Discrete Fourier Transform (IDFT) and power of OpenGL and C++, this project delivers authentic wave simulation. 

https://youtu.be/CXsKYo364Cg

# Setup Instructions

## Linux

To run this project, you will need to download the latest zipped folder from Releases on GitHub, cd into the folder and run the following command.

```
bash run.sh
```

If you still do not see a window like the one seen in the video above, please run the following commands:
more
```
sudo apt install xorg-dev mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

# What I learned

- Learned how to write vertex, fragment and compute shaders using OpenGL's Shading Language.
- Wrote compute shaders to take advantage of GPU architecture to achieve faster results.
- Enhanced my understanding of the IDFT by writing a 2D IDFT in a compute shader.
- Learned how to debug code run on the GPU using Intel's Graphics Performance Analyzer.
- Gained a solid understanding of the principles of wave physics such as wave propagation, including wave equations, wave types, and wave behavior.
- I developed a GLTF file importer that utilizes the [nlohmann JSON library](https://github.com/nlohmann/json) to parse GLTF files, extracting and populating essential vertex attributes like position, normals, and texture coordinates.
- Gained experience using third party libraries such as GLFW and FFTW3
- Learned how to build a self-contained executable using CMake that seamlessly integrates multiple third-party libraries.
- Successfully automated the release process of the executable on GitHub using GitHub Actions to ensure easy distribution and accessibility for users.
<!--- Learned how to profile C++ applications -->

# Math

## Wave Initialization

To initialize the wave positions, I used a function known as a Wave Spectrum. This spectrum can be deconstructed into two constituent equations: the non-directional wave spectrum and the directional wave spectrum. By combining the products of these two spectra, I initialized a 256 x 256 grid. This grid exists within the spectral domain; for its conversion into a height field, the transformation utilized the fftw library alongside a 2D IDFT.

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

Once again I need to calculate the IDFT in order to convert the spectral domain data into spatial domain data.

I increase the value of "dt", applying the wave propagation model again, and execute the IDFT once more. This sequence is iteratively executed in a loop, enabling the simulation of the wave's progression over time.

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.
