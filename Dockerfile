# escape=`

FROM saidursajol/my-base-windows-cpp-image:latest

# Copy vcpkg.json and install dependencies using vcpkg manifest mode
COPY install_package.cpp  ./
COPY generate_package_json.cpp  ./
RUN g++ install_package.cpp -o install-package
RUN .\install-package.exe
# COPY vcpkg.json C:\vcpkg.json
RUN .\vcpkg\vcpkg install --triplet x64-windows --feature-flags=manifests

# Update PATH environment variable for Visual Studio and MSBuild tools
RUN refreshenv

# Create deploy directory and copy project files
RUN mkdir deploy
COPY . /deploy/
WORKDIR /deploy/
WORKDIR ../
WORKDIR /deploy/packages/
RUN nuget install librdkafka.redist -Version 2.2.0
WORKDIR ../../

# Integrate vcpkg with MSBuild
RUN .\vcpkg\vcpkg integrate install

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