tiny-risc-cpu

A small RISC-style CPU core I’m building from scratch.
This is still a work in progress — I’ll be adding more instructions, pipeline stages, and a few security-focused experiments as the project grows. Keep an eye on the repo for updates.

Overview

The goal of this project is to understand CPU design at a low level: how instructions move through the stages, how the ALU behaves, and how control logic ties everything together.
It’s intentionally minimal, so each part is easy to read and reason about.

Current Features :
Basic instruction fetch and decode
Simple ALU with core arithmetic/logic ops
Register file
Memory access path
Testbench setup for quick iteration


Coming soon :
Branch/jump logic
5-stage pipeline
Forwarding + hazard handling
Small assembler
Security experiments around privilege checks
