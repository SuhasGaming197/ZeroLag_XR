#!/bin/bash
set -e

cd /Users/suhaspuligadda/Desktop/ZeroLag_XR
source .venv/bin/activate

echo "Starting ZeroLag XR Testbench..."
python3 testbench.py
