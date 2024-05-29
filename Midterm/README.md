# Simple HTTP Server in C

## Overview
This is a simple HTTP server written in C that listens on port 8080 and serves a static HTML page to any incoming requests.

## Requirements
- Windows 11
- GCC Compiler

## Compilation
To compile the server, use the following command:

gcc -o httpserver code.c -lws2_32

## Running the Server
To run the server, execute:

.\httpserver

## Testing
Open a web browser and navigate to `http://localhost:8080` to view the response from the server.
