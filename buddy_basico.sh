#!/bin/sh
./GameBoy/Debug/Gameboy BROKER CAUGHT_POKEMON 1 OK
./GameBoy/Debug/Gameboy BROKER CAUGHT_POKEMON 2 FAIL

./GameBoy/Debug/Gameboy BROKER NEW_POKEMON Pikachu 2 3 1

./GameBoy/Debug/Gameboy BROKER CATCH_POKEMON Onyx 4 5

./GameBoy/Debug/Gameboy BROKER SUSCRIPTOR NEW_POKEMON 10

./GameBoy/Debug/Gameboy BROKER CATCH_POKEMON Charmander 4 5
