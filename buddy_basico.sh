#!/bin/sh
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 1 OK
./GameBoy/Debug/GameBoy BROKER CAUGHT_POKEMON 2 FAIL

./GameBoy/Debug/GameBoy BROKER NEW_POKEMON Pikachu 2 3 1

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Onyx 4 5

./GameBoy/Debug/GameBoy BROKER SUSCRIPTOR NEW_POKEMON 10

./GameBoy/Debug/GameBoy BROKER CATCH_POKEMON Charmander 4 5
