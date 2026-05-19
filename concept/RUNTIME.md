## Runtime

The runtime is the support code that compiled Pearl programs use while they run.

### Main Job

- Provide basic helper functions
- Support printing
- Support simple program startup and shutdown behavior
- Keep generated code smaller and simpler

### Small Scope

The runtime should stay minimal. It should only include what is needed for the first useful version of the language.

### Example Responsibilities

- Print values
- Handle basic program entry
- Offer a few shared helper routines

### Design Goal

The runtime should be easy to understand, easy to link, and easy to reuse.
