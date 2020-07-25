#!/bin/sh
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Pikachu 9 3
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Squirtle 9 3

./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 10 OK
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 11 FAIL

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Bulbasaur 1 7
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Charmander 1 7

./GameBoy/Debug/GameBoy BROKER GET_POKEMON Pichu
./GameBoy/Debug/GameBoy BROKER GET_POKEMON Raichu
