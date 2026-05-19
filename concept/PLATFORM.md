## Platform

The platform layer describes how Pearl fits the target operating system and machine architecture.

### What It Covers

- Operating system differences
- CPU architecture differences
- Build and toolchain settings
- Target-specific output rules

### Simple Approach

Keep platform support narrow at first. Start with one working path, then add more only when the project needs them.

### Why It Matters

Different platforms may need different compiler flags, different object file formats, or different runtime details.

### Design Goal

The platform layer should hide target differences from the front end as much as possible.
