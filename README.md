# Watch Simulation Here

<div align="center">
    <a href="https://youtu.be/CXsKYo364Cg">
        <img src="https://img.youtube.com/vi/CXsKYo364Cg/0.jpg" alt="Watch this video">
    </a>
</div>

# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores ocean simulation. Using an oceanographic spectrum, Inverse Discrete Fourier Transform (IDFT) and power of OpenGL and C++, this project delivers authentic wave simulation. 

# Getting Started

## Downloading Release

## Compiling from Source

Start by cloning the repository with ```git clone https://github.com/connorbews/WaveSimulation.git```

### Windows

Visual Studio 2019 is recommended. I have not tried compiling this project with Visual Studio 2022 but will in the future.

#### Steps
1. Navigate to the WaveSimulation folder, the WaveSimulation folder should look like this: ![initial_state](https://github.com/user-attachments/assets/0d63f715-f858-4c56-a91a-2337783a3228)

2. In the WaveSimulation directory, run ```setup_windows.bat```.
3. The command above should generate a ```WaveSimulation.sln``` in the WaveSimulation directory, double click the ```WaveSimulation.sln``` file to open it.
4. In the solution explorer, right click on the ```WaveSim``` and click "Set as Startup Project". ![StartupProject](https://github.com/user-attachments/assets/5fa77b2b-a9ec-47de-8d64-a931e69d6301)
5. Click "Local Windows Debugger" to start the program.
6. You should now see the wave simulation.

### Linux

To run this project, you will need to download the latest zipped folder from Releases on GitHub, cd into the folder and run the following command.

```
bash run.sh
```

If you still do not see a window like the one seen in the video above, please run the following commands:
more
```
sudo apt install xorg-dev mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.
