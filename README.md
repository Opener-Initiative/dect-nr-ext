# DECT-2020 NR Extension for ns-3 (`dect-nr-ext`)

This manual explains how the `dect-nr-ext` project is organized, how to set it up, and how to run experiments.

## Scope and assumptions

- This manual is written for users with no prior project knowledge.
- Commands are written to be executed from the ns-3 root directory (the directory that contains the `ns3` script and `test.py`).
- The project is tested and supported on `ns-3.47`.
- Setup instructions include Linux, macOS, and Windows (PowerShell).

## Project organization

`contrib/dect-nr-ext` is organized as follows:

- `model/`: core module implementation (MAC, PHY, scheduler, channel manager, config, statistics, utilities, message definitions).
- `examples/`: runnable experiment entry points.
  - `dect2020ext.cc` (all-purpose simulation file for quick experimentation with the implementation)
  - `association/association-example-1.cc`
  - `spectrum-management/spectrum-management-example-1.cc`
  - `spectrum-management/spectrum-management-example-2.cc`
  - `spectrum-management/spectrum-management-example-3.cc`
- `test/`: module test suites used by ns-3 test runner.
- `CMakeLists.txt`: module build/test wiring.
- `examples/CMakeLists.txt`: example registration for `ns3 run <example-name>`.
- `splitlogs.py`: optional log post-processing utility.
- `launch.sh`: helper script to quickly launch `dect2020ext.cc` and split the produced log file.
- `propagation-delay-calculation.py`: optional propagation-delay helper utility.
- `logs/`: expected output location for split logs.
- `dect2020ext_topology.py`: generated artifact from experiments (do not treat as source code to edit manually).

## Setup

### 1) Get ns-3.47 and prerequisites

Use official ns-3 resources:

- ns-3 releases: [https://www.nsnam.org/releases/](https://www.nsnam.org/releases/)
- ns-3 getting started and prerequisites: [https://www.nsnam.org/docs/tutorial/html/getting-started.html](https://www.nsnam.org/docs/tutorial/html/getting-started.html)

### 2) Clone `dect-nr-ext` into `contrib`

Run from ns-3 root:

```bash
git clone https://github.com/seelennebel/dect-nr-ext.git contrib/dect-nr-ext
```

### 3) Configure and build ns-3 with `dect-nr-ext`

Linux/macOS:

```bash
./ns3 configure --enable-examples --enable-tests
./ns3 build dect-nr-ext
```

Windows (PowerShell):

```powershell
.\ns3 configure --enable-examples --enable-tests
.\ns3 build dect-nr-ext
```

## Run experiments

All commands below are run from ns-3 root.

### Available experiment entry points

- `dect2020ext`
- `association-example-1`
- `spectrum-management-example-1`
- `spectrum-management-example-2`
- `spectrum-management-example-3`

### Base run command

Linux/macOS:

```bash
./ns3 run <example-name>
```

Windows (PowerShell):

```powershell
.\ns3 run <example-name>
```

### Quick launch helper for `dect2020ext.cc`

For quick iteration, `contrib/dect-nr-ext/launch.sh` is provided as a convenience script to launch the `dect2020ext.cc` example and then run log splitting.

Run it from `contrib/dect-nr-ext`:

```bash
cd contrib/dect-nr-ext
bash launch.sh
```

Note: run the script from that directory, because `launch.sh` uses relative paths for both simulation execution and log processing.

### Experiment list and source mapping

- `dect2020ext`
  - Source: `contrib/dect-nr-ext/examples/dect2020ext.cc`
  - Typical use: baseline end-to-end DECT NR+ scenario.
- `association-example-1`
  - Source: `contrib/dect-nr-ext/examples/association/association-example-1.cc`
  - Typical use: association-focused behavior.
- `spectrum-management-example-1`
  - Source: `contrib/dect-nr-ext/examples/spectrum-management/spectrum-management-example-1.cc`
  - Typical use: channel selection experiment with 2 FTs.
- `spectrum-management-example-2`
  - Source: `contrib/dect-nr-ext/examples/spectrum-management/spectrum-management-example-2.cc`
  - Typical use: channel selection experiment with 3 FTs.
- `spectrum-management-example-3`
  - Source: `contrib/dect-nr-ext/examples/spectrum-management/spectrum-management-example-3.cc`
  - Typical use: square-layout channel selection experiment with 4 FTs.

### File-based success checks

After a successful run, check for generated artifacts (no console-pattern matching required):

- `contrib/dect-nr-ext/dect2020ext_topology.py` exists or has been updated.
- If logs are redirected, `contrib/dect-nr-ext/dect2020ext.log` exists.
- If `splitlogs.py` is used, split logs are created under `contrib/dect-nr-ext/logs/`.

## Recommended log workflow (optional)

Recommended for experiment analysis:

1. Run an example and redirect logs to one file.
2. Split the combined log into per-identifier files.

Linux/macOS:

```bash
./ns3 run <example-name> 2> contrib/dect-nr-ext/dect2020ext.log
(cd contrib/dect-nr-ext && python3 splitlogs.py dect2020ext.log)
```

Windows (PowerShell):

```powershell
.\ns3 run <example-name> 2> contrib/dect-nr-ext/dect2020ext.log
Push-Location contrib/dect-nr-ext
py splitlogs.py dect2020ext.log
Pop-Location
```

Warning:

- `splitlogs.py` requires the `contrib/dect-nr-ext/logs/` directory to exist.

## Topology visualization (optional)

`dect2020ext_topology.py` is generated by example runs. To visualize it, install plotting dependencies in a Python virtual environment.

Linux/macOS:

```bash
python3 -m venv contrib/dect-nr-ext/.venv
source contrib/dect-nr-ext/.venv/bin/activate
python -m pip install matplotlib networkx
python contrib/dect-nr-ext/dect2020ext_topology.py
```

Windows (PowerShell):

```powershell
py -m venv contrib/dect-nr-ext/.venv
.\contrib\dect-nr-ext\.venv\Scripts\Activate.ps1
python -m pip install matplotlib networkx
python .\contrib\dect-nr-ext\dect2020ext_topology.py
```

## Propagation delay helper (optional utility)

`contrib/dect-nr-ext/propagation-delay-calculation.py` is a standalone utility script.

Linux/macOS:

```bash
python3 -m pip install scipy
python3 contrib/dect-nr-ext/propagation-delay-calculation.py
```

Windows (PowerShell):

```powershell
py -m pip install scipy
py contrib/dect-nr-ext/propagation-delay-calculation.py
```

## Run tests (short section)

From ns-3 root:

Linux/macOS:

```bash
python3 test.py --suite="dect*"
```

Windows (PowerShell):

```powershell
py test.py --suite="dect*"
```

The `--suite` argument accepts either a full suite name or a regex pattern.

## Update from GitHub and rebuild

If you need to sync local code with new commits:

Linux/macOS:

```bash
git -C contrib/dect-nr-ext pull
./ns3 configure --enable-examples --enable-tests
./ns3 build dect-nr-ext
```

Windows (PowerShell):

```powershell
git -C contrib/dect-nr-ext pull
.\ns3 configure --enable-examples --enable-tests
.\ns3 build dect-nr-ext
```

## Troubleshooting

- Ensure you are in ns-3 root before running commands in this manual.
- If `splitlogs.py` fails, verify `contrib/dect-nr-ext/logs/` exists.
- If topology visualization fails, make sure `matplotlib` and `networkx` are installed in the active environment.
- If propagation utility fails, install `scipy` first.
- If examples are not found, rerun configure/build with `--enable-examples`.
