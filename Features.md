## Features Done

* Automatically include/exclude bluetooth based on target chipset
  * ESP32 is currently the only device with Bluetooth support
  * Hopefully ESP32-C6 will join it soon!
* Soft AP
* Command line interface with commands:
    * scan [ ON | OFF ]
      * Scan APs - Fast (API)
      * Scan APs - Continual (SSID + lastSeen when beacons seen)
      * Commands to select/view/remove APs/STAs in scope
      *  Scan STAs - Only include clients of selected AP(s), or all 
      * Fix bug with hidden SSIDs being included in network scan and getting garbled names
      * Update client count when new STAs are found
      * Format timestamps for display
      * RSSI, channel, secondary channel and WPS now being displayed reliably
    * set/get SSID_LEN_MIN SSID_LEN_MAX channel hopping MAC channel
      * Options to get/set MAC hopping between frames
      * Options to get/set EXPIRY - When set (value in minutes) Gravity will not use packets of the specified age or older.
    * view: view [ SSID | STA ] - List available targets for the included tools. Each element is prefixed by an identifier for use with the *select* command, with selected items also indicated. "MAC" is a composite set of identifiers consisting of selected stations in addition to MACs for selected SSIDs.
      * VIEW AP selectedSTA - View all APs that are associated with the selected STAs
      * VIEW STA selectedAP - View all STAs that are associated with the selected APs
    * select: select ( SSID | STA ) &lt;specifier&gt;+ - Select/deselect targets for the included tools.
    * beacon: beacon [ RICKROLL | RANDOM [ COUNT ] | INFINITE | USER | OFF ]  - target SSIDs must be specified for USER option. No params returns current status of beacon attack.
      * Beacon spam - Rickroll
      * Beacon spam - User-specified SSIDs
      * Beacon spam - Fuzzing (Random strings)
      * Beacon spam - Infinite (Random strings)
      * Beacon spam - Selected APs
        * Current implementation will include STAs who probed but did not connect to a specified AP. I think that's a good thing?
    * probe: probe [ ANY | SSIDS | AP | OFF ] - Send either directed (requesting a specific SSID) or broadcast probe requests continually, to disrupt legitimate users. SSIDs sourced either from user-specified target-ssids or selected APs from scan results.
    * deauth: deauth [ STA | AP | BROADCAST | OFF ] - Send deauthentication packets to broadcast if no parameters provided, to selected STAs if STA is specified, or to STAs who are or have been associated with selected APs if AP is specified. This attack will have much greater success if specific stations are specified, and greater success still if you adopt the MAC of the access point you are attempting to deauthenticate a device from
    * mana: mana ( ( [ VERBOSE ] [ ON | OFF ] ) | AUTH [ NONE | WEP | WPA ] ) - Enable or disable Mana, its
      verbose output, and set the authentication type it indicates. If not specified returns the current status. 
      * Mana attack - Respond to all probes
      * Loud Mana - Respond with SSIDs from all STAs
    * beacon & probe fuzzing - send buffer-overflowed SSIDs (>32 char, > ssid_len) - FUZZ OFF | ( BEACON | REQ | RESP )+ ( OVERFLOW | MALFORMED )
      * SSID longer/shorter than ssid_len
      * SSID matches ssid_len, is greater than 32 (MAX_SSID_LEN)
    * ap-dos
      * Respond to frames to/from a selectedAP with
        * deauth to STA using AP's MAC
        * disassoc to AP using STA's MAC
      * Respond to frames between STAs, where one STA is known to be associated with a selectedAP
        * deauth to both STAs using AP's MAC
    * ap-clone
      * composite attack
      * AP-DOS: Clone AP - Use target's MAC, respond to messages with deauth and APs with disassoc
      * Beacon: Send forged beacon frames
      * Mana (almost): Send forged probe responses
      * (Hopefully the SoftAP will handle everything else once a STA initiates a connection)
      * Ability to select security of broadcast AP - open so you can get connections, or matching the target so you get assoc requests not them?
        * No way to get the target's configured auth type
        * Command line parameter - can specify multiple
    * stalk
      * Homing attack (Focus on RSSI for selected STA(s) or AP)
      * Individual wireless devices selected (selected AP, STA, BT, BTLE, ...)
  * Configurable behaviour on BLE out of memory
    * Truncate all non-selected BT devices and continue
    * Truncate all unnamed BT devices and continue
    * Truncate unnamed and unselected BT devices and continue
    * Truncate the oldest BT devices and continue
    * Truncate lowest RSSI (could even write an incremental cutoff)
    * Halt scanning - esp_ble_gap_stop_scanning() - needs config option
  * Move max RSSI and min AGE into menuconfig and CLI parameters




* **ONGOING** Receive and parse 802.11 frames

## Features TODO

* Web Server serving a page and various endpoints
    * Since it's more useful for a Flipper Zero implementation, I'll build it with a console API first
    * Once complete can decide whether to go ahead with a web server
* TODO: additional option to show hidden SSIDs
* improvements to stalk
  * Average
  * Median (or trimmed mean)
* CLI commands to analyse captured data - stations/aps(channel), etc
* handshake
* Capture authentication frames for cracking
* Scan 802.15.1 (BLE/BT) devices and types
  * Currently runs discovery-based scanning, building device list
  * Refactor code to retrieve service information from discovered devices
  * Sniff-based scanning
  * BLE
* BLE/BT fuzzer - Attempt to establish a connection with selected/all devices

## Bugs / Todo

* Add SCAN BT SERVICES [selectedBT]
* Add VIEW BT SERVICES [selectedBT]
* Update documentation to include Bluetooth features
  * Have done a little, needs more.
* Rename ATTACK_SCAN_BT_CLASSIC to ATTACK_SCAN_BT_DISCOVERY
* Further testing of VIEW BT SORT *
* Add BT service information
* Add active BT scanning - connections
* Additional command to inspect a device and its services
  * Move services callback handler into its own function
  * Manage a data model for the services rather than displaying them
  * Don't bother doing that until confirming they can provide useful information
* Retrieve services for all devices
* Interpret services based on their UUIDs
* BT discovery Progress indicator - seconds remaining?
* Better formatting of remaining UIs
* Sorting APs not working. Looks like it should :(
* MAC changing problems on ESP32
  * Dropped packets after setting MAC
    * Which bits to re-init?
    * Currently disabled MAC randomisation
  * Fix MAC randomisation for Probe
  * Implement MAC randomisation for beacon
  * Re-implement MAC spoofing for deauth
  * Re-implement MAC spoofing for DOS (Used in 5 places)
* parse additional packet types
* Decode OUI when displaying MACs (in the same way Wireshark does)
* Test using fuzz with multiple packet types
* Add non-broadcast targets to fuzz
* Mana "Scream" - Broadcast known APs in beacon frames
* Better support unicode SSIDs (captured, stored & printed correctly but messes up spacing in AP table - 1 japanese kanji takes 2 bytes.)
* Improve sniff implementation
* Eventually deauth triggers "wifi:max connection, deauth!"
    
## Testing / Packet verification

| Feature              | Broadcast | selectedSTA (1) | selectedSTA (N>1) | target-SSIDs |
|----------------------|-----------|-----------------|-------------------|--------------|
| Beacon - Random MAC  |  Pass     |  N/A            |  N/A              |  Pass        |
| Beacon - Device MAC  |  Pass     |  N/A            |  N/A              |  Pass        |
| Probe - Random MAC   |  Pass     |  N/A            |  N/A              |  Pass        |
| Probe - Device MAC   |  Pass     |  N/A            |  N/A              |  Pass        |
| Deauth - Frame Src   |  Pass     |  Pass   | Pass  |  Pass             |  N/A         |
| Deauth - Device Src  |  Pass     |  Pass   | Pass  |  Pass             |  N/A         |
| Deauth - Spoof Src   |  N/A      |  Pass   | Pass  |  Pass             |  N/A         |
| Mana - Open Auth     |           |                 |                   |              |
| Mana - Open - Loud   |           |                 |                   |              |
| Mana - WEP           |           |                 |                   |              |
| Mana - WPA           |           |                 |                   |              |
|----------------------|-----------|-----------------|-------------------|--------------|


## Packet types

802.11 type/subtypes
0x40 Probe request
0x50 Probe response
0x80 Beacon
0xB4 RTS
0xC4 CTS
0xD4 ACK
0x1A PS-Poll
0x1E CF-End
0x1F CF-End+CF-Ack
0x08 Data
0x88 QoS Data

QoS data & data both have STA and BSSID
DATA:
BSSID 10
STA 4
QoS DATA:
BSSID 10