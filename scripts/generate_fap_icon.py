# This script generates a 1-bit 10x10px image which could be used as icon
# for flipper zero applications
#
# @Author: qw3rtty
# @Repository: github.com/qw3rtty/flipper-zero-stuff

from PIL import Image
import argparse

# Define the argument for the image name
parser = argparse.ArgumentParser()
parser.add_argument("--name", help="Name of the generated image", default="fap_icon.png")
args = parser.parse_args()

# Define the pattern for the icon
fap_icon_pattern = [
    [1, 1, 1, 0, 1, 0, 0, 1, 1, 1],
    [1, 0, 1, 0, 0, 1, 0, 1, 0, 1],
    [1, 1, 1, 0, 1, 0, 0, 1, 1, 1],
    [0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
    [1, 0, 1, 0, 1, 0, 1, 0, 1, 0],
    [0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
    [0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
    [1, 1, 1, 0, 0, 1, 0, 1, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 0, 1, 0],
    [1, 1, 1, 0, 0, 1, 0, 0, 0, 1]
]

# Create a new image with mode '1' (1-bit pixels, black and white)
img = Image.new('1', (10, 10))

# Load the pixel data
pixels = img.load()

# Set the pixels based on the pattern
for y in range(10):
    for x in range(10):
        pixels[x, y] = fap_icon_pattern[y][x]

# Save the image
img.save(args.name)

