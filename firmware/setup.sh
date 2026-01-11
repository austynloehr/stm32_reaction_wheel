#!/bin/sh
set -e

echo "Setting up development environment..."

# 1. Add the compilation target for Cortex-M4F
echo "Adding Rust target: thumbv7em-none-eabihf..."
rustup target add thumbv7em-none-eabihf

# 2. Install flip-link (required linker)
if ! command -v flip-link > /dev/null; then
    echo "Installing flip-link..."
    cargo install flip-link
else
    echo "flip-link is already installed."
fi

# 3. Install probe-rs (required for 'cargo run' to flash/log)
# We skip this in CI to save time, as we only need to build, not run.
if [ -z "$CI" ]; then
    if ! command -v probe-rs > /dev/null; then
        echo "Installing probe-rs..."
        # Install with default features (cli)
        cargo install probe-rs --features cli
    else
        echo "probe-rs is already installed."
    fi
else
    echo "CI detected: Skipping probe-rs installation."
fi

echo "Setup complete! You can now run 'cargo build' or 'cargo run'."
