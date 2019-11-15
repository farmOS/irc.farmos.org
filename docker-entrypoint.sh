#!/bin/bash
set -e

# Start cron service.
service cron start

# Execute the arguments passed into this script.
echo "Attempting: $@"
exec "$@"

