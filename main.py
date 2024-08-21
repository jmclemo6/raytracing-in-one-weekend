import sys
import vectors

def main():
    image_width = 256
    image_height = 256

    print("P3")
    print(f"{image_width} {image_height}")
    print("255")

    for j in range(0, image_height):
        print(f'\rScanlines remaining: {image_height - j}', file=sys.stderr);
        for i in range(0, image_width):
            rgb = vectors.Vec3(i / (image_width - 1), j / (image_height - 1), 0)
            print(rgb.get_as_ppm_color(255))


if __name__ == '__main__':
    main()
