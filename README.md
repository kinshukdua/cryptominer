# Implementation of an efficient, portable and platform-agnostic cryptocurrency mining algorithm for Internet of Things devices


This repository contains the source code for the barebones implementation of a cryptocurrency miner that can be ported to almost any IoT device. 

The repository is divided into 2 folders- 

1. Implementation - Containing the complete source code that can be modified to run on any platform by using platform specific functions. 

2. Benchmarks - Code for testing the performance (hashrate) of 1) PC, 2) PSP, 3) ESP32 (or Arduino) and replicating the results obtained in the paper. Also contains results of the test run, in the form of text files, plus ipynb to generate the graphs used in the paper.

## Getting Started

### Requirements
* **gcc** 
* **[psptoolchain](https://github.com/pspdev/psptoolchain)** (for testing on PSP)
* **[pspsdk](https://github.com/pspdev/pspsdk)** (for testing on PSP)
* **[PPSSPP](https://github.com/hrydgard/ppsspp)** (for testing on PSP Emulator)
* **Arduino IDE** (for testing on arduino/esp32)

### C libraries required
* (OPTIONAL) **stdint.h** (or change datatypes from `uint32_t` to `unsigned long`.)
* (OPTIONAL) **string.h** (or change `memset` and `memcpy` to platform specific memory managment calls.)


## Running the Implementation
1. Move to the `Implementation` folder.
3. (OPTIONAL) Change the `connect_to_server` and `communicate` functions in `stratum.c` based on your platform.
4. (OPTIONAL) Change `malloc` and `calloc` calls based on the platform.
5. (OPTIONAL) Change `printf` calls based on the platform.
6. (OPTIONAL) Edit the makefile to compile for the specific platform.
7. Run `make`.
8. Run the generated binary.
NOTE: By default for testing purposes `receive_block_data_test` is used to generate a fake block.
Uncomment `get_target` and `receive_block_header` for mining on the actually recieved data.

for UNIX
```
$ ./miner
```
## Reproducing benchmark results

NOTE: Install the specific requirements before proceeding

Move to `Benchmarks` folder
### PC
1. Compile the benchmark binary

    ```
    $ gcc miner.c sha2.c
    ```
    or 
    ```
    $ gcc miner.c sha2_opt.c
    ```
    For running the sha2 or optimized sha2 benchmark respectively.

2. Run the binary
    ```
    $ ./a.out
    ```

3. The results should be available in `results.txt`

### PSP
1. Download and set up the PSP development environment (psptoolchain, pspsdk)
2. Create the EBOOT
    ```
    $ make
    ```
3. (Optional) Sign the EBOOT file using [ebootsigner](https://github.com/pspdev/ebootsigner), or if you have CFW, copy the file directly to the `GAME` directory.
4. Launch it on PSP

### ESP32
1. Download the Arduino IDE 
2. Open the folder as a new project
3. Click Upload
4. Open Tools->Serial Monitor to view the results


## Example 
### ESP32 Implementation

* String.h, calloc/malloc, stdint.h are directly available so we don't need to change them.
* The `connect_to_server` can be changed using:
    ```
    WiFiClient client;
    client.connect(host, port);
    ```

* Similarly `communicate` can be modified using:
    ```
    client.print(); // for sending the data
    client.read(data, 2000); // for receiving data;
    ```

* Finally print statements can be replaced with `Serial.print` and that's all that is required for running a cryptocurrency miner on ESP32.
