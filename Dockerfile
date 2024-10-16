# Use an official Windows Server Core image
FROM mcr.microsoft.com/windows/servercore:ltsc2022

# Set the working directory
WORKDIR /app

# Install Chocolatey (package manager for Windows)
RUN @powershell -NoProfile -ExecutionPolicy unrestricted -Command "(iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))) >$null 2>&1"

# Install MinGW
RUN powershell -NoProfile -ExecutionPolicy Bypass -Command "choco install mingw -y"

# Set environment variables for MinGW
# Install MinGW and update PATH
RUN choco install mingw -y
RUN powershell.exe -Command $path = $env:path + ';C:\ProgramData\mingw64\mingw64\bin'; Set-ItemProperty -Path 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' -Name Path -Value $path

# Install CMake
RUN choco install cmake --pre --installargs 'ADD_CMAKE_TO_PATH=System' -y
RUN refreshenv

# Copy the source code into the container
COPY main.cpp .

# Wait for a bit to ensure MinGW is properly installed

# Build the application using g++
RUN g++ main.cpp -o hello.exe

# Command to run the application
CMD ["hello.exe"]
