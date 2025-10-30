# Installation
1. Start by installing NS3. This link can be used to download the source code and the NS3 executable:  
https://www.nsnam.org/releases/

2. Clone the dev branch of the dect-nr repository to the **contrib** directory inside NS3
```bash
git clone https://github.com/seelennebel/dect-nr.git
```
# Configuration and compilation
Run these commands in the NS3 root directory (where ns3 executable is located)
```bash
ns3 configure
```
```bash
ns3 build dect-nr-ext
```
To configure NS3 to run examples (simulation scripts) and tests, use:
```bash
ns3 configure --enable-examples --enable-tests
```
# Running simulation scripts
1. Start by moving into the root directory of NS3 (where ns3 executable is located)
2. To run any simulation script from the project's **examples** directory:
```bash
./ns3 run contrib/dect-nr-ext/examples/<example script to run>
```
# Running tests
Python is required to run the NS3 test.py program.
1. Start by moving into the root directory of NS3(where the **test.py** program is located)
2. Run the command:
```bash
python test.py --suite="dect*"
````
The argument to the **--suite** option can be any full name of the test, or, as shown in the example above, it can also be a regex.
# To synchronize local project source code with GitHub
1. Start by navigating to contrib/dect-nr-ext directory
2. Use this command to synchronize local project source code with new commit on GitHub:
```bash
git pull
```
3. Reconfigure and recompile NS3 (this action is needed every time the source code is changed)





