Ecole
=====

.. image:: https://circleci.com/gh/ds4dm/ecole.svg?style=svg
   :target: https://circleci.com/gh/ds4dm/ecole
   :alt: CircleCI


Ecole (pronounced [ekɔl]) stands for *Extensible Combinatorial Optimization Learning
Environments* and aims to expose a number of control problems arising in combinatorial
optimization solvers as Markov
Decision Processes (*i.e.*, Reinforcement Learning environments).
Rather than trying to predict solutions to combinatorial optimization problems directly, the
philosophy behind Ecole is to work
in cooperation with a state-of-the-art Mixed Integer Linear Programming solver
that acts as a controllable algorithm.

The underlying solver used is `SCIP <https://scip.zib.de/>`_, and the user facing API is
meant to mimic the `OpenAi Gym <https://gym.openai.com/>`_ API (as much as possible).


User Documentation
------------------
Please refer to the `documentation <https://doc.ecole.ai>`_ for tutorials, examples, and
installation instructions.


Developer Notes
---------------
.. used to include portion of this file in the documentation
.. SOURCE_INSTALL_START
Build dependencies
^^^^^^^^^^^^^^^^^^
Conda
~~~~~
All dependencies required for building can be resolved using a
`conda <https://docs.conda.io/en/latest/>`_ environment.
Install everything in a development (named ``ecole``) environment using

.. code-block:: bash

   conda env create -n ecole -f conda-dev.yml

For the following, the ``ecole`` environment always needs to be activated

.. code-block:: bash

   conda activate ecole
   conda config --append channels conda-forge
   conda config --set channel_priority strict

*Note: this environment contains tools to build ecole and scip, format code, test,
generate documentation etc. These are more than the dependencies to only use Ecole.*

CMake
~~~~~
`CMake <https://cmake.org/>`_ is a meta-build tool, configuring other build tools
(*e.g.* Make) or IDE's.
The whole build of Ecole can be done with CMake.
A one-time configuration is necessary for CMake to find dependencies, detect system
information, *etc*.
This is the time to pass optional build options, such as the build type and compiler
choice. For instance ``-D CMAKE_BUILD_TYPE=Debug`` can be added to compile with debug
information.
Using ``cmake``, we recommend building out of source using ``cmake -B build/`` to
configure, and ``cmake --build build/`` to compile.
CMake is made available in the ``ecole`` environment created earlier.


SCIP runtime dependency
^^^^^^^^^^^^^^^^^^^^^^^
Head to the `download page <https://scip.zib.de/index.php#download>`_, select version
7.0.0 and download the ``scipoptsuite-7.0.0.tgz`` file.
Extract it using

.. code-block:: bash

   tar -xz -f scipoptsuite-7.0.0.tgz

Create a local conda package for SCIP using the recipe provided in this repository

.. code-block:: bash

   export CONDA_BLD_PATH="${CONDA_PREFIX}/conda-bld"
   SCIP_DIR="/path/to/scipoptsuite-7.0.0" conda build -c conda-forge .circleci/recipe/scipoptsuite-7.0.0

Instal the SCIP package in the ``ecole`` environment

.. code-block:: bash

   conda install -c "file://${CONDA_BLD_PATH}" -c conda-forge scipoptsuite


Building
^^^^^^^^
Building Ecole is very similar to building SCIP, as they both use CMake.
In the Ecole source repository, configure using

.. code-block:: bash

   cmake -B build -D ECOLE_DEVELOPER=ON

The definition ``-D ECOLE_DEVELOPER`` changes the defaults settings (such as the build
type, static analysis, *etc.*) for added convenience and ensuring high quality
contributions.
Only the default settings are changed.
This mode does not override any explicitly setting.

Then, build Ecole with

.. code-block:: bash

   cmake --build build/

The Python package can be installed from the build directory

.. code-block:: bash

   python -m pip install -I build/python

Compiler issues
~~~~~~~~~~~~~~~
If you encounter problem with your compiler (because it is too old for instance),
you can use the ones from ananconda.

.. code-block:: bash

   conda install gxx_linux-64  # Linux

And start again the configuring of Ecole.

.. code-block:: bash

   rm -r build/ && cmake -B build -D ECOLE_DEVELOPER=ON

.. SOURCE_INSTALL_END


Running the tests
^^^^^^^^^^^^^^^^^
C++ tests
~~~~~~~~~
The C++ tests are build with `Catch2 <https://github.com/catchorg/Catch2>`_.
It produces a single standalone executable under ``build/libecole/tests/test-libecole``.
For test options, consult

.. code-block:: bash

   build/libecole/tests/test-libecole --help

Python tests
~~~~~~~~~~~~
Python tests are build with `PyTest <https://docs.pytest.org/en/latest/>`_.
As any Python project, running the tests require that Python finds the correct Ecole
package.
It is therefore **not recommended** to manually install Ecole between every tests, as
it leaves room for running tests with an out of date installation.
The CMake configuration creates a Python virtual environment and install the Ecole
Python package inside automatically.
To run the tests (or another operation with Python Ecole), use the virtual
environemnt

.. code-block:: bash

   ./build/venv/bin/python -m pytest python/tests/


Generating the documentation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The documentation is a CMake target that is not build by default.
When using ``-D ECOLE DEVELOPER=ON``, the documentation can be generated by providing
the target to the CMake build command:

.. code-block:: bash

   cmake --build build --target ecole-sphinx

The HTML files generated are located under ``build/doc/sphinx/html``.
In particular, ``build/doc/sphinx/html/index.html`` can be opened in your browser to
visualize the documentation.


Coding standards
^^^^^^^^^^^^^^^^
The quality and conventions of the code are enforced automatically with various tools
detailed here.

C++ toolset
~~~~~~~~~~~
Visual layout of the code is formated using
`clang-format <https://clang.llvm.org/docs/ClangFormat.html>`_.
All files can be formatted at once using

.. code-block:: bash

   find libecole python -name '*.[hc]pp' -exec clang-format --style=file -i {} \;

Additional coding best practices are enforced through
`clang-tidy <https://clang.llvm.org/extra/clang-tidy/>`_.
``clang-tidy`` is run automatically in CMake when using ``-D ECOLE_DEVELOPER=ON`` to
ensure that the guidelines are respected.
The tool also has the ability to fix (some) errors automatically, but this is not done
in CMake (which does not modify the source code).
To run, the tool needs access to a *compilation database*.
The database is also created automatically when using ``-D ECOLE_DEVELOPER=ON``, but the
file need to be accessible at the root directory of the project.
One can create a symbolic link (assuming the build dircetory is named ``build``) using

.. code-block:: bash

   ln -s build/compile_commands.json

Then, ``clang-tidy`` can be run on all files for fixing with

.. code-block:: bash

   find libecole python -name '*.[hc]pp' -exec clang-tidy --fix --fix-errors {} \;

Both these tools are available in the conda environment.
They also integrate seamlessly with most editors and IDE to avoid running these
commands manually.

Python toolset
~~~~~~~~~~~~~~
Python code is formatted using `Black <https://black.readthedocs.io>`_.
``black`` is available in the conda environment, and all files can be formatted using

.. code-block:: bash

   python -m black python/