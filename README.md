[![LibraryBuild](https://github.com/Duckle29/husb238/actions/workflows/LibraryBuild.yml/badge.svg)](https://github.com/Duckle29/husb238/actions/workflows/LibraryBuild.yml)

This is a small library that lets you easily communicate with and configure a
Hynetek HUSB328 USB-PD negotiation chip.


## Functions:
 **get_capabilities()** queries the HUSB for the USB-PD supplies capabilities. This is called in begin, but if the supported capabilities have somehow been re-negotiated, this can re-fresh that.

 **begin()** initializes the Wire instance, and queries the the HUSB IC for the USB-PD capabilities.

 **set_voltage()** attempts to negotiate the requested voltage from the USB-PD supply.

 **update_pd_status()** Queries the HUSB and updates the internal `status` object.

 **get_pd_voltage()** Calls update_pd_status and returns the currently negotiated voltage.

 **get_pd_current()** Calls update_pd_status and returns the currently negotiated current.


## `capability` struct
The capability struct serves as a way to read the supported currents at the different voltages.
If the current is 0, the voltage isn't supported.

for example:
`capability.ma_5V` might be 5000 mA

This struct is packed, so can be iterated over with pointers. `ma_5V` is the first element in the struct.

## `status` object
The library has a status object.  
It's a 16-bit union providing access either to the entire 16bits through `status.combined`,  
to the `PD_STATUS0` and `PD_STATUS1` bytes,  
or to the individual bit fields as defined in the HUSB datasheet.  
For example `status.VOLTAGE_5V` will be 1 if a legacy apple 5V contract is active (I think)
