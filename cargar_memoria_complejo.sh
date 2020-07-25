#!/bin/sh
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Pikachu 9 3 #19b
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Squirtle 9 3 #20b

./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 10 OK
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 11 FAIL

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Bulbasaur 1 7 #21
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Charmander 1 7 #22

./GameBoy/Debug/GameBoy BROKER GET_POKEMON Pichu #9
./GameBoy/Debug/GameBoy BROKER GET_POKEMON Raichu #10
