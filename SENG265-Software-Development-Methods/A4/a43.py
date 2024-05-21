#!/usr/bin/env python
import random
from collections import namedtuple
from typing import IO
print(__doc__)


WidthHeight = namedtuple('WidthHeight', ['width', 'height'])
Colour = namedtuple('Colour', ['red', 'green', 'blue', 'op'])
Center = namedtuple('Center', ['x', 'y'])
RxRy = namedtuple('RxRy', ['rx', 'ry'])


class HtmlComponent():
    """HtmlComponent class contains Html component methods"""

    def header(self, winTitle: str, canvas: tuple) -> str:
        """header method: creates an Html header as specified by a4 instructions and returns it as a string"""
        header: str = "<html>\n"
        header += "<head>\n"
        header += f"   <title>{winTitle}</title>\n"
        header += "</head>\n"
        header += "<body>\n"
        header += f'   <svg width="{canvas[0]}" height="{canvas[1]}">\n'
        return header

    def footer(self) -> str:
        """footer method: creates an html footer as specified by a4 instructions and returns it as a string"""
        footer: str = "   </svg>\n"
        footer += "<body>\n"
        footer += "<html>\n"
        return footer


class HtmlDocument(HtmlComponent):
    """HtmlDocument class contains instructions to create Html file"""

    def __init__(self, file_name: str, winTitle: str) -> None:
        """Html Document initializer: Creates an Html-SVG file"""
        self.__file: IO[str] = open(file_name, "w")
        self.__file.write(f"{self.header(winTitle,[500,300])}")
        SvgCanvas(self.__file)
        self.__file.write(f"{self.footer()}")
        self.__file.close()


class PyArtConfig:
    """PyArtConfig class"""
    CNT: int = 0
    SHA_RANGE: tuple = (0, 2)
    X_RANGE: tuple = (0, 500)
    Y_RANGE: tuple = (0, 300)
    RAD_RANGE: tuple = (0, 100)
    RXY_RANGE: tuple = (10, 30)
    WH_RANGE: tuple = (10, 100)
    RGB_RANGE: tuple = (0, 255)
    OP_RANGE: tuple = (0.0, 1.0)

    def __init__(self) -> None:
        """PyArtConfig initializer - creates random shape variables"""
        self.cnt: int = PyArtConfig.CNT
        self.sha: int = random.randint(
            PyArtConfig.SHA_RANGE[0], PyArtConfig.SHA_RANGE[1])

        if self.sha == 0:
            self.rad: int = random.randint(
                PyArtConfig.RAD_RANGE[0], PyArtConfig.RAD_RANGE[1])
        elif self.sha == 1:
            self.WidthHeight: WidthHeight = WidthHeight(random.randint(PyArtConfig.WH_RANGE[0], PyArtConfig.WH_RANGE[1]),
                                                        random.randint(PyArtConfig.WH_RANGE[0], PyArtConfig.WH_RANGE[1]))
        elif self.sha == 2:
            self.RxRy: RxRy = RxRy(random.randint(PyArtConfig.RXY_RANGE[0], PyArtConfig.RXY_RANGE[1]),
                                   random.randint(PyArtConfig.RXY_RANGE[0], PyArtConfig.RXY_RANGE[1]))

        self.Center: Center = Center(random.randint(PyArtConfig.X_RANGE[0], PyArtConfig.X_RANGE[1]),
                                     random.randint(PyArtConfig.Y_RANGE[0], PyArtConfig.Y_RANGE[1]))

        self.Col: Colour = Colour(random.randint(PyArtConfig.RGB_RANGE[0],  PyArtConfig.RGB_RANGE[1]),
                                  random.randint(
                                      PyArtConfig.RGB_RANGE[0],  PyArtConfig.RGB_RANGE[1]),
                                  random.randint(
                                      PyArtConfig.RGB_RANGE[0],  PyArtConfig.RGB_RANGE[1]),
                                  round(random.uniform(PyArtConfig.OP_RANGE[0], PyArtConfig.OP_RANGE[1]), 1))
        PyArtConfig.CNT += 1


class Shape():
    """Shape super class"""

    def __init__(self, shape: tuple, Col: tuple) -> None:
        """initializes shared shape parameters"""
        self.Center: Center = Center(shape[0], shape[1])
        self.Col: Colour = Colour(Col[0], Col[1], Col[2], Col[3])


class RandomShape:
    """RandomShape class"""
    @staticmethod
    def new_shape(ArtConfig: PyArtConfig) -> Shape:
        """new_shape method: Creates a random shape from ArtConfig parameters an returns the shape"""
        if ArtConfig.sha == 0:
            return CircleShape((ArtConfig.Center.x, ArtConfig.Center.y, ArtConfig.rad),
                               (ArtConfig.Col.red, ArtConfig.Col.green, ArtConfig.Col.blue, ArtConfig.Col.op))
        if ArtConfig.sha == 1:
            return RectangleShape((ArtConfig.Center.x, ArtConfig.Center.y, ArtConfig.WidthHeight.width, ArtConfig.WidthHeight.height),
                                  (ArtConfig.Col.red, ArtConfig.Col.green, ArtConfig.Col.blue, ArtConfig.Col.op))
        if ArtConfig.sha == 2:
            return EllipseShape((ArtConfig.Center.x, ArtConfig.Center.y, ArtConfig.RxRy.rx, ArtConfig.RxRy.ry),
                                (ArtConfig.Col.red, ArtConfig.Col.green, ArtConfig.Col.blue, ArtConfig.Col.op))


class CircleShape(Shape):
    """CircleShape class"""

    def __init__(self, shape: tuple, Col: tuple) -> None:
        """CircleShape initializer"""
        super().__init__(shape, Col)
        self.rad: int = shape[2]


class RectangleShape(Shape):
    """RectangleShape class"""

    def __init__(self, shape: tuple, Col: tuple) -> None:
        """RectangleShape initializer"""
        super().__init__(shape, Col)
        self.WidthHeight: WidthHeight = WidthHeight(shape[2], shape[3])


class EllipseShape(Shape):
    """EllipseShape class"""

    def __init__(self, shape: tuple, Col: tuple) -> None:
        """EllipseShape initializer"""
        super().__init__(shape, Col)
        self.RxRy: RxRy = RxRy(shape[2], shape[3])


class SvgCanvas():
    """Svg class contains the art generating portion of the file"""

    def __init__(self, file: IO[str]) -> None:
        self.genart(file)

    def genart(self, file: IO[str]) -> None:
        """genart method: generates and draws Shape onto Html file"""
        for i in range(1000):
            shape: Shape = RandomShape.new_shape(PyArtConfig())
            self.draw(shape, file)

    def draw(self, shape: Shape, file: IO[str]) -> None:
        """draw method: draws onto Html file"""
        if isinstance(shape, CircleShape):
            line1: str = f'<circle cx="{shape.Center.x}" cy="{shape.Center.y}" r="{shape.rad}" '
            line2: str = f'fill="rgb({shape.Col.red}, {shape.Col.green}, {shape.Col.blue})" fill-opacity="{shape.Col.op}"></circle>'
        elif isinstance(shape, RectangleShape):
            line1: str = f'<rect x="{shape.Center.x}" y="{shape.Center.y}" width="{shape.WidthHeight.width}" height="{shape.WidthHeight.height}" '
            line2: str = f'fill="rgb({shape.Col.red}, {shape.Col.green}, {shape.Col.blue})" fill-opacity="{shape.Col.op}"></rect>'
        elif isinstance(shape, EllipseShape):
            line1: str = f'<ellipse cx="{shape.Center.x}" cy="{shape.Center.y}" rx="{shape.RxRy.rx}" ry="{shape.RxRy.ry}" '
            line2: str = f'fill="rgb({shape.Col.red}, {shape.Col.green}, {shape.Col.blue})" fill-opacity="{shape.Col.op}"></ellipse>'
        file.write(f"      {line1+line2}\n")


def main() -> None:
    """Main method"""
    HtmlDocument("a431.html", "MyArt")
    HtmlDocument("a432.html", "MyArt")
    HtmlDocument("a433.html", "MyArt")


if __name__ == "__main__":
    main()
