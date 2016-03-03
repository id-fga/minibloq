#!/bin/bash

sudo make uninstall ; 
make && 
sudo make install &&
make uninstall_examples &&
make config &&
make examples

