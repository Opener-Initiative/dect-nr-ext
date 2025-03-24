# Installation and Simulation

This guide will help to setup the simulation environment to run the simulation scripts from the GitHub repository.

### 1. NS3 installation
For a detailed guide about the process of installing NS3 simulation environment can be found on the official page 
of NS3 project:  
[https://www.nsnam.org/docs/installation/html/](https://www.nsnam.org/docs/installation/html/)

### 2. Installing and running the scripts
Navigate to the scratch directory inside NS3 folder:
```bash
cd /path/to/the/folder/ns3/ns-allinone-*/ns-*/scratch
```
It is recommended to append the PATH variable of your system with the path to the NS3 binary to
make execution of scripts simpler. On Windows, it is possible to easily change the PATH variable via the system's settings. On the Unix based machines (only for the current shell session): 
```bash
export PATH=$PATH:/path/to/the/folder/ns3/ns-allinone-*/ns-*
```

Clone the repository of the project:
```bash
git clone https://github.com/seelennebel/dect_nr
```

```bash
cd dect_nr
```

Finally, run the simulation script:
```bash
ns3 run simulation.cc
```