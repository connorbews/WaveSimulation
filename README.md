# Watch Simulation Here

<div align="center">
    <a href="https://youtu.be/CXsKYo364Cg">
        <img src="https://img.youtube.com/vi/CXsKYo364Cg/0.jpg" alt="Watch this video">
    </a>
</div>

# Introduction

Inspired by the methods pioneered by Horvath[1] and Tessendorf[2], this project explores ocean simulation. Using an oceanographic spectrum, Inverse Discrete Fourier Transform (IDFT) and power of OpenGL and C++, this project delivers authentic wave simulation. 

# Getting Started

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

1. Navigate to the WaveSimulation folder, the WaveSimulation folder should look like this: ![Screenshot from 2024-09-04 15-56-17](https://github.com/user-attachments/assets/eabd9151-c17f-4c41-b95d-9d985d6c863a)

2. Run ```chmod +x ./setup_linux.sh```, you will need to run this command in order to give execute permission to ```setup_linux.sh```
3. In the WaveSimulation directory, run ```./setup_linux.sh```.
4. The command above should generate a Makefile in the WaveSimulation directory, run ```make``` to generate an executable.
5. Run the ```./WaveSim``` command to start the program.
6. You should now see the wave simulation.

# References

1. HORVATH, C. 2015. Empirical directional wave spectra for computer graphics
2. TESSENDORF, J. 2001. Simulating ocean water. White paper.
