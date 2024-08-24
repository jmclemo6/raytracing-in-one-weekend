from setuptools import Extension, setup

setup(
        ext_modules=[
            Extension(
                name="vectors",
                sources = ["source/vectorsmodule.c", "source/vec3.c"]
                ),
            ]
        )
