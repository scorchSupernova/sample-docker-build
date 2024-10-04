# escape=`

FROM saidursajol/my-base-windows-cpp-image:latest

RUN msbuild -version

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
RUN msbuild SimpleManager.sln /p:Configuration=Release /p:VcpkgEnableManifest=true

# Copy required DLLs to the release directory
RUN powershell -Command `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\librdkafka.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libcrypto-3-x64.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\libssl-3-x64.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zlib1.dll' -Destination 'Release64\' -Force; `
    Copy-Item 'packages\librdkafka.redist.2.2.0\runtimes\win-x64\native\zstd.dll' -Destination 'Release64\' -Force"

# Copy config.json to the release directory
WORKDIR ../
WORKDIR /deploy/Release64/
RUN copy C:\deploy\config.json C:\deploy\Release64\

# Set the default command to cmd.exe
CMD ["cmd.exe"]
