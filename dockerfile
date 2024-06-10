# Start with a gcc image that will be used for both building and running the application
FROM gcc:latest

# Install CMake and other necessary build tools
RUN apt-get update && \
    apt-get install -y cmake qt6-base-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory for building the application
WORKDIR /build

# Copy the source code and CMakeLists.txt into the container
COPY . .

# Build the application
RUN cmake . && make

# Explicitly create the /app directory
RUN mkdir -p /app

# Move the built application to the /app directory
RUN mv myserver /app/myserver && \
    rm -rf /build

# Set the working directory to /app, where the application binary is located
WORKDIR /app
EXPOSE 8083

# Command to run the application
CMD ["./myserver"]