from distutils.core import setup, Extension

module1 = Extension('DDSL',
                    sources = ['main.cpp','cnpy.cpp'],
                    include_dirs = ['../src'],
		extra_compile_args=['-std=c++11','-O2'])

setup (name = 'DDSL',
       version = '1.0',
       description = 'Package used for processing image data.',
       ext_modules = [module1])

