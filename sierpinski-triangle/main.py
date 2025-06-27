from PIL import Image, ImageDraw
from fractions import Fraction

SIZE = 2001


def create_sierpinski_triangle(i):
    if i == 0:
        return [[(Fraction(0), Fraction(1)), (Fraction(1, 2), Fraction(0)), (Fraction(1), Fraction(1))]]

    previous_triangles = create_sierpinski_triangle(i - 1)
    previous_triangles = [[(coord[0] / 2, coord[1] / 2) for coord in triangle] for triangle in previous_triangles]

    triangles = []

    for offset in [(Fraction(1, 4), Fraction(0)), (Fraction(0), Fraction(1, 2)), (Fraction(1, 2), Fraction(1, 2))]:
        previous_triangles_copy = previous_triangles.copy()

        for triangle in previous_triangles_copy:
            triangles.append([(coord[0] + offset[0], coord[1] + offset[1]) for coord in triangle])

    return triangles


def main():
    image = Image.new("RGBA", (SIZE, SIZE), (255, 255, 255, 255))
    draw = ImageDraw.Draw(image)

    triangles = create_sierpinski_triangle(5)

    for triangle in triangles:
        scaled_triangle = [(int(coord[0] * SIZE), int(coord[1] * SIZE)) for coord in triangle]
        draw.polygon(scaled_triangle, fill=(255, 0, 0, 255), outline=(255, 0, 0, 255))

    image.save("triangle.png")


if __name__ == "__main__":
    main()
