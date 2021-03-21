#!/bin/bash

make 

scp -p src/test/unit_test/apd-ucm-testpackage-manager   root@192.168.7.2:/home/root



scp -p  src/package_manager   root@192.168.7.2:/home/root
