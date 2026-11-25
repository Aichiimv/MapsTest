# MapsTest
Test para API y aplicacion de google maps en C++
cmake -S . -B build
cmake --build build -j4
to compile, this will create a /build directory with the bin files

IF changing just the main: 
cmake --build build -j4
no need to recompile everything again

To run(once you have the /build directory and inside the binary file pi_maps
./build/pi_maps


You need to have a googlemaps API FIRST!! setted as a enviroment variable:

export GOOGLE_MAPS_API_KEY="your_actual_api_key_here"
