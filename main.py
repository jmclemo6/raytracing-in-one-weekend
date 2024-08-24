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
    # main()
    u = vectors.vec3(1, 2, 3)
    v = vectors.vec3(2, 3, 4);

    print(f"Cross: {vectors.cross(u, v)}")
    print(f"Dot: {vectors.dot(u, v)}")
    print(f"Add: {u + v}")
    print(f"Subtract: {u - v}")
    print(f"Multiply: {u * v}")
    print(f"Multiply w/ Scalar: {u * 2}")
    print(f"Normalize: {u.get_normalized()}")
    v.normalize()
    print(f"Normalize in place: {v}")
    print(f"Negate: {-u}")
