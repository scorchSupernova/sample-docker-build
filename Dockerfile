# escape=`

FROM saidursajol/my-base-windows-cpp-image:latest


RUN powershell.exe -Command $path = $env:path + ';C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.38.33130\bin\Hostx64\x64;C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin;C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\amd64'; Set-ItemProperty -Path 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' -Name Path -Value $path
RUN refreshenv


# Install NuGet and related packages
RUN choco install nuget.commandline -y

# Create deploy directory and copy project files
RUN mkdir deploy
COPY . /deploy/
WORKDIR /deploy/
WORKDIR ../
WORKDIR /deploy/packages/
RUN nuget install librdkafka.redist -Version 2.2.0
WORKDIR ../../

# Build the project
WORKDIR /deploy/
RUN msbuild SimpleManager.sln /p:Configuration=Release /p:VcpkgEnableManifest=true

# Copy DLLs to the release directory
RUN powershell -Command `
    Copy-Item 'C:\deploy\packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\librdkafka.dll' -Destination 'C:\deploy\Release64\' -Force; `
    Copy-Item 'C:\deploy\packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libcrypto-3-x64.dll' -Destination 'C:\deploy\Release64\' -Force; `
    Copy-Item 'C:\deploy\packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libssl-3-x64.dll' -Destination 'C:\deploy\Release64\' -Force; `
    Copy-Item 'C:\deploy\packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zlib1.dll' -Destination 'C:\deploy\Release64\' -Force; `
    Copy-Item 'C:\deploy\packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zstd.dll' -Destination 'C:\deploy\Release64\' -Force"

# Copy config.json to the release directory
WORKDIR ../
WORKDIR /deploy/Release64/
RUN copy C:\deploy\config.json C:\deploy\Release64\

# Set the default command to cmd.exe
CMD ["cmd.exe"]