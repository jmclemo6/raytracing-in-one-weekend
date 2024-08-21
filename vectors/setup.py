from setuptools import Extension, setup

setup(
        ext_modules=[
            Extension(
                name="vectors",
                sources = ["vectorsmodule.c"]
                ),
            ]
        )
