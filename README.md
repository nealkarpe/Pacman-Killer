GRAPHICS ASSIGNMENT 1
======================

## Running Instructions
- mkdir build
- cd build
- cmake ..
- make
- ./game

## Controls
- A: Move left
- D: Move right
- SPACE: Jump

## Objective
- Move around in the world and kill as many enemies as possible

## Scores
- Touch porcupine: -50
- Kill green pacman: +4
- Kill yellow pacman: +3
- Kill red pacman: +2
- Kill blue pacman: +1

## Features
- Acceleration due to gravity considered in all movements
- Good randomisation of enemies
- Player reflects of slopes on balls with correct physics projectile trajectory
- Player bounces off trampoline to a greater height than regular jump
- Zooming and panning
- Magnet appears occasionally and pulls the player with a constant horizontal acceleration (velocity steady increase)
- Water slows down player. Proper fluid dynamics implemented
- Player tends to fall to equillibrium position in the water
- When player is in water, it is unaffected by magnet's gravitational force
- Porupine class available which can initialize a porcupine, given its boundaries of to-and-fro movement

## Window
- Left arrow: pan left
- Right arrow: pan right
- Down arrow: pan down
- Up arrow: pan up
- Scroll up: zoom in
- Scroll down: zoom out
- Q: quit game

## Screenshot
![screenshot1](https://user-images.githubusercontent.com/31779922/36061925-2ab49a9c-0e88-11e8-878d-91967507fa49.png)
