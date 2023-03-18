# airsoft_bomb
A DIY airsoft prop bomb based on arduino/esp

# Game mode design

This is as rapid draft of game mode using various inputs

## Commun bomb setup
Set time before explosion

## Wire mode defuse
### Bomb setup
Each of X wires connected to the bomb are given a function from:
- set off the bomb (1 wire only)
- time penalty ((X/2)-1 of the wires)
- defuse the bomb (X/2)

Individual wire function can be set randomly or manualy.

### Bomb defuse
Disconnect one wire at a time until defuse is complete

## Keypad code defuse
### Bomb setup
A code is randomly generated or manually set by user
### Bomb defuse
Correct code must be provided to defuse, few failed attempt should be graced. (randomly?)

## Inside game challenge
### Bomb setup
### Bomb defuse
