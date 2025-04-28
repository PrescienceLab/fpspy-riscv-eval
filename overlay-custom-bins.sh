#!/usr/bin/env bash

BASE_IMG="$1"
MOUNTPOINT='mountpoint'
OVERLAY_ITEMS_DIR='overlay'

if [[ ! -f "$BASE_IMG" ]]; then
	echo "You must provide the image (*.img) to overlay your custom binaries!"
	exit 1
else
	echo "Running with \$BASE_IMG=$BASE_IMG"
fi

sudo mount "$BASE_IMG" "$MOUNTPOINT"

for bin in $OVERLAY_ITEMS_DIR/*; do
	bin=$(basename "$bin")
	echo "Copying $bin to $MOUNTPOINT/$bin"
	sudo cp -r $OVERLAY_ITEMS_DIR/* "$MOUNTPOINT/"
done

sudo sync
sudo umount "$MOUNTPOINT"
