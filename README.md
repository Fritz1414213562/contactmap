# contactmap
Calculate contact map between two index-specified regions from dcd trajectory data.
## Usage
```
/path-to-contactmap/bin/contactmap --ifname [path-to-input-trajectory ...] --group1 [path-to-group1-index-file] --group2 [path-to-group2-index-file] --ofname [path-to-output] --cutoff [cutoff length to assume a contact (10.0 by default)] --start [frame to start calculation (0 by default)] --end [frame to end calculation (-1 by default)] --skip [frame to skip calculation (1 by default)] --mask [number of neighbor beads to ignore contacts (5 by default)]
```
