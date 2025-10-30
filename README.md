# Installation
1. Start by installing NS-3. This link can be used to download the source code and the NS-3 executable:  
https://www.nsnam.org/releases/

2. Clone the dev branch of the dect-nr repository to the **contrib** directory inside NS-3
```bash
git clone https://github.com/seelennebel/dect-nr.git
```
# Configuration and compilation
Run these commands in the NS3 root
# Running simulation scripts
1. Start by moving into the root directory of NS-3(where ns3 executable is located)
2. To run any simulation script from the project's **examples** directory:
```bash
./ns3 run contrib/dect-nr-ext/examples/<example script to run>
```
# Running tests
Python is required to run the NS-3 test.py program.
1. Start by moving into the root directory of NS-3(where the **test.py** program is located)
2. Run the command:
```bash
python test.py --suite="dect*"
````
The argument to the **--suite** option can be any full name of the test, or, as shown in the example above, it can also be a regex.



