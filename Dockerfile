# escape=`

FROM saidursajol/my-base-windows-cpp-image:1.0

# Verify MSBuild installation
RUN msbuild -version

# Install NuGet command line
RUN choco install nuget.commandline -y

# Create deploy directory and copy project files
RUN mkdir deploy
COPY . /deploy/
WORKDIR /deploy/

# List files to verify the solution file is copied
RUN dir

# Install the librdkafka.redist package
WORKDIR /deploy/packages/
RUN nuget locals all -clear
RUN nuget install librdkafka.redist -Version 2.2.0

# Go back to deploy directory for building the project
WORKDIR /deploy/

# Build the project from the correct directory
RUN msbuild SimpleManager.sln /p:Configuration=Release /p:VcpkgEnableManifest=true

RUN dir 

RUN dir Release64

# Copy required DLLs to the release directory
RUN powershell -Command `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\librdkafka.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libcrypto-3-x64.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libssl-3-x64.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zlib1.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zstd.dll' -Destination 'Release64\' -Force"

# Copy config.json to the release directory
RUN copy config.json Release64\

# Set the default command to cmd.exe
CMD ["cmd.exe"]