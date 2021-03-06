#!/bin/bash

cd
mkdir Proyectos
cd
mv tp-2020-1c-Postre-de-vainilla-DIA-/ Proyectos/
cd

git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library/
sudo make install
cd

cd Proyectos/tp-2020-1c-Postre-de-vainilla-DIA-
sudo ./buildBroker.sh
sudo ./buildGameCard.sh
sudo ./buildGameBoy.sh
sudo ./buildTeam.sh

cd
mkdir tall-grass
cd tall-grass
mkdir Metadata
cd Metadata
touch Metadata.bin
echo "BLOCK_SIZE=64" | tee -a Metadata.bin
echo "BLOCKS=1024" | tee -a Metadata.bin
echo "MAGIC_NUMBER=TALL_GRASS" | tee -a Metadata.bin

