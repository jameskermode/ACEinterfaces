from setuptools import setup

setup(
    name='ace',
    version='0.1',
    #py_modules=['ace'],
    packages=['ace'],
    zip_safe=False,
    package_data={'ace': ['ace_c.so']}
)