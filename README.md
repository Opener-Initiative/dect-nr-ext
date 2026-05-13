# DECT-2020 NR ns-3 Implementation
# Installation
## Download ns-3
This link can be used to download the source code and the `ns3` executable:  
https://www.nsnam.org/releases/
## Clone the project
Clone the repository to the `contrib` directory inside ns-3
```bash
git clone https://github.com/seelennebel/dect-nr-ext.git
```
Go into `dect-nr-ext` directory:
```bash
cd dect-nr-ext
```
Change the branch to `exe`
```bash
git checkout exe
```
## Configure and compile
Run these commands in the ns-3 root directory (where `ns3` executable is located) to configure ns-3 to run examples (simulation scripts) and tests:
```bash
./ns3 configure --enable-examples --enable-tests
```
```bash
./ns3 build dect-nr-ext
```
# Running simulation scripts
1. Start by moving into the root directory of `dect-nr-ext`
2. Use this command to run any simulation script from the project's `examples` directory:
```bash
../../ns3 run examples/<example script to run>
```
# Running tests
Python is required to run the ns-3 test.py program
1. Start by moving into the root directory of ns-3 (where the `test.py` program is located)
2. Run the command:
```bash
python test.py --suite="dect*"
````
The argument to the `--suite` option can be any full name of the test, or, as shown in the example above, it can also be a regex.
# To synchronize local project source code with GitHub
1. Start by navigating to contrib/dect-nr-ext directory
2. Use this command to synchronize local project source code with new commit on GitHub:
```bash
git pull
```
3. Reconfigure and recompile NS3 (this action is needed every time the source code is changed)
# Visualization
If this command is executed:
```bash
../../ns3 run examples/dect2020ext.cc
```
The script will create `dect2020ext_topology.py` and put it into the root directory of the project. Run these commands from `dect-nr-ext` root to create a Python virtual environment, download dependencies, and view the visualization:
```bash
python -m venv my-venv
```
If you are on MacOs/Linux:
```bash
source my-venv/bin/activate
```
If you are on Windows:
```bash
./my-venv/Scripts/activate
```
Install dependencies:
```bash
pip install matplotlib networkx
```
Run the visualization script:
```bash
python dect2020ext_topology.py
```
# Reading logs from simulations
If this command is executed:
```bash
../../ns3 run examples/dect2020ext.cc
```
The `dect2020ext.cc` simulation script will output all logs into the terminal. The logs can be also redirected into a file:
```bash
../../ns3 run examples/dect2020ext.cc 2> dect2020ext.log
```
This command will output all logs into a single file `dect2020ext.log` located in the root directory of `dect-nr-ext`.  
Logs can be separated based on indentity of models:
```bash
python splitlogs.py dect2020ext.log
```
After those steps, all logs can be visible inside the `logs` directory of `dect-nr-ext`. The logs are separated by layer and node ID.