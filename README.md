# Traffic-Simulator #

### An OpenGL C++ project which simulate a daily traffic. ###

  - 4 lanes.
  - Add a car by tapping '1' '2' '3' or '4' key. Number represented the lane you want to load it. 
  - Every car has implemented an AI to avoid collision with other cars, also maintain and adjust the speed for keeping a reasonable distance toward the car in front of her. 
  - After a tour (cross entire displayed road) the car will be respawned to remake the tour of the road.

### How to run the game ###

  - Game runs only on Linux (for other OS, you have to adapts it manually)
  - Install freeglut and SOIL:
  - sudo apt-get install freeglut3 freeglut3-dev
  - sudo apt-get install libsoil-dev
  - Change current directory in terminal to "Traffic-Simulator" 
  - Type command "make"
  - Type command "./exe"
  - Have fun!
