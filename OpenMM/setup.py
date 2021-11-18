from setuptools import setup

setup(
    name='ace_openmm',
    version='0.1',
    packages=['ace_openmm'],
    zip_safe=False,
    package_data={'ace_openmm': ['ace_c.so']}
)
