#!/bin/bash

cd build

sudo make install

sudo cp ../systemd/reflector.service \
    /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable reflector
sudo systemctl start reflector

echo "Installed"
