```markdown
# Overall Functionality

The program implements a simple calculation server that:

- Runs as a user-space service in MINIX
- Handles inter-process communication (IPC) via message passing
- Provides addition and multiplication services
- Demonstrates both user-space and kernel-space operations

---

## Key Components

### 1. Main Program Flow

```
main() → calc_init() → calc_main_loop()
```

- Initializes the server
- Enters an infinite message handling loop

### 2. Initialization (calc_init)

- Prints startup message
- Initializes the service with `rs_init()` (remote service initialization)
- Announces service availability using `chardriver_announce()`

### 3. Message Processing Loop (calc_main_loop)

- Continuously receives messages using `sef_receive(ANY, m)`
- Dispatches messages based on `m_type` field
- Sends replies back to the originating process

### 4. Supported Operations

#### Addition (calc_handle_add)

| Aspect | Description |
|--------|-------------|
| Operation | Integer addition |
| Location | User-space calculation |
| Input | Two integers (`m1_i1` and `m1_i2`) |
| Output | Sum returned in `m1_i1` |
| Logging | Prints operation details |

#### Multiplication (calc_handle_multiply)

| Aspect | Description |
|--------|-------------|
| Operation | Integer multiplication |
| Location | Kernel-space via system call `sys_multiply()` |
| Input | Two integers (`m1_i1` and `m1_i2`) |
| Output | Product returned in `m1_i1` |
| Error Handling | Checks system call return code |
| Logging | Prints operation details with "(via kernel)" notation |

### 5. Message Types

| Type | Description |
|------|-------------|
| `CALC_ADD` | Request for addition operation |
| `CALC_MULTIPLY` | Request for multiplication operation |
| Other types | Return `EINVAL` (Invalid argument error) |

---

## Architecture Patterns Demonstrated

### Client-Server Model

- Server waits for requests from any client (`ANY` endpoint)
- Processes requests and sends replies back to `m_source`

### Hybrid Computation

- **Addition**: Performed in user-space (demonstrating simple operations)
- **Multiplication**: Delegated to kernel-space (demonstrating system calls)

### Error Handling

- Checks message reception errors
- Validates system call returns for kernel operations
- Returns appropriate status codes in `m_type` field

---

## Expected Message Structure

### Request Message

```c
m.m_type = CALC_ADD or CALC_MULTIPLY
m.m1_i1 = first operand
m.m1_i2 = second operand
```

### Response Message

```c
m.m_type = OK (success) or error code
m.m1_i1 = result value
```

---

## Usage Example

Client processes would send messages like:

```c
message m;
m.m_type = CALC_ADD;
m.m1_i1 = 5;
m.m1_i2 = 3;
sendrec(calc_server_endpoint, &m);
// Result in m.m1_i1 = 8
```
