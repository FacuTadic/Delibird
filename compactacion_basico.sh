#!/bin/sh
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 1 OK
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 2 FAIL

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Pikachu 2 3
./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Squirtle 5 2

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Onyx 4 5

./GameBoy/Debug/GameBoy BROKER SUSCRIPTOR CAUGHT_POKEMON 10

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Vaporeon 4 5
