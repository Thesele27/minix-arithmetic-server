# minix-arithmetic-server
Operating Systems 2 lab assignment: A calculation server for MINIX 3 showcasing system calls, kernel integration, and inter-process communication.



# MINIX Calc Server

[![MINIX](https://img.shields.io/badge/MINIX-3.3.0-blue.svg)](https://www.minix3.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A calculation server for MINIX 3 that demonstrates hybrid user-space and kernel-space operations through inter-process communication (IPC) message passing.

## Overview

This server implements a simple yet powerful example of MINIX architecture:
- **Addition** - Performed directly in user-space
- **Multiplication** - Delegated to kernel-space via custom system call

The project showcases how user-space servers can interact with the kernel while providing services to client processes.

## Repository Structure

```
├── calc.c                 # Main server implementation
├── Documentation.txt      # Server functionality overview  
├── Documentation.docx     # Full lab documentation
└── Assignment 1.pdf       # Original assignment specification
```

## Features

- ✅ **IPC Message Passing** - Uses MINIX's message-based communication
- ✅ **User-space Addition** - Basic arithmetic in server context
- ✅ **Kernel-space Multiplication** - System call delegation to kernel
- ✅ **Error Handling** - Comprehensive status code checking
- ✅ **Logging** - Operation details printed for debugging

## Message Types

| Type | Operation | Location |
|------|-----------|----------|
| `CALC_ADD` | Integer addition | User-space |
| `CALC_MULTIPLY` | Integer multiplication | Kernel-space |

## Quick Start

### Build & Install

```bash
cd /usr/src/releasetools
make services
make hdboot
reboot
```

### Usage Example

```c
message m;
m.m_type = CALC_ADD;
m.m1_i1 = 5;
m.m1_i2 = 3;
sendrec(calc_server_endpoint, &m);
// Result: m.m1_i1 = 8
```

### Client Library Example

```c
#include "calc_lib.h"

int main() {
    int sum = calc_add(5, 3);        // Returns 8 (user-space)
    int product = calc_multiply(9, 2); // Returns 18 (kernel-space)
    return 0;
}
```

## Architecture

```
┌─────────────┐     ┌─────┐     ┌─────────────┐     ┌────────┐
│   User App  │ ──▶ │ lib │ ──▶ │    PM       │ ──▶ │ CALC   │
│             │     │     │     │   Server    │     │ Server │
└─────────────┘     └─────┘     └─────────────┘     └────┬───┘
                                                          │
                                                    ┌─────▼─────┐
                                                    │ Addition  │
                                                    │(user-space│
                                                    └───────────┘
                                                          │
                                                    ┌─────▼─────┐
                                                    │Multiply   │
                                                    │(kernel)   │
                                                    └───────────┘
```

## Message Flow

1. **User Application** → Calls `calc_add()` or `calc_multiply()`
2. **Library Function** → Sends message to PM server
3. **PM Server** → Forwards request to CALC server
4. **CALC Server** → Processes request:
   - Addition: Performs operation directly
   - Multiplication: Makes kernel call `sys_multiply()`
5. **Kernel** → Performs multiplication (if needed)
6. **Result** → Flows back through the chain

## Key Components

| Component | Path | Purpose |
|-----------|------|---------|
| CALC Server | `/usr/src/servers/calc/` | Main server logic |
| System Call | `/usr/src/kernel/system/do_multiply.c` | Kernel multiplication |
| Library | `/usr/src/lib/libc/sys-minix/calc.c` | User API |
| PM Integration | `/usr/src/servers/pm/misc.c` | Request forwarding |

## Error Handling

- Message reception failures are logged and skipped
- System call returns are validated for kernel operations
- Invalid message types return `EINVAL`
- Status codes propagated appropriately

## Documentation

See `Documentation.docx` for complete lab documentation including:
- Detailed implementation steps
- Build system modifications
- Boot image integration
- System call registration

## Requirements

- MINIX 3.3.0 or later
- Root access for kernel rebuild
- Basic understanding of MINIX IPC

## License

This project is for educational purposes as part of CS4421 coursework.

## Acknowledgments

- MINIX 3 Development Team
- Department of Mathematics and Computer Science
- National University of Lesotho
