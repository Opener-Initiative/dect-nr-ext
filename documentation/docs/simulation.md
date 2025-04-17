# Simulation

## Debug NS3 simulation script with [GDB](https://sourceware.org/gdb/)
(to follow the examples, it is recommended to append your system's env PATH variable with the path to the NS3 executable)
```bash
ns3 run --gdb script.cc
```

## Simulation Flow
1. **`ns3::ApplicationContainer::Start()`**  
This method simply iterates through the contained **`Applications`** and calls their **`SetStartTime()`** methods with the provided **`Time`**
2. **`ns3::Application::SetStartTime`**  
Applications start at various times in the simulation scenario. The Start method specifies when the application should be started. The application subclasses should override the private "StartApplication" method defined below, which is called at the time specified, to cause the application to begin. 
3. 