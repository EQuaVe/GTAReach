# Timed Automata with diagonal constraints and updates

[![Build and test TChecker](https://github.com/ticktac-project/tchecker/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/ticktac-project/tchecker/actions/workflows/build-and-test.yml)
[![License](https://img.shields.io/badge/license-MIT-informational.svg)](https://github.com/ticktac-project/tchecker/blob/master/LICENSE)

This extends the zone based algorithms of classical Timed Automata to Timed Automata with diagonal constraints and updates. This implements the simulation relation described in the following two papers on top of the tool [TChecker](https://github.com/ticktac-project/tchecker):

 - *Fast Algorithms for Handling Diagonal Constraints in Timed Automata* <br>
Paul Gastin, Sayan Mukherjee, B Srivathsan <br>
in the proceedings of CAV 2019, LNCS 11561 [link to paper](https://link.springer.com/chapter/10.1007%2F978-3-030-25540-4_3)
 - *Reachability for Updatable Timed Automata Made Faster and More Effective* <br>
 Paul Gastin, Sayan Mukherjee, B Srivathsan <br>
 in the proceedings of FSTTCSS 2020, LIPIcs 182 [link to paper](https://drops.dagstuhl.de/opus/volltexte/2020/13288/)

This implementation adds the algorithm option `gsim`, which can be selected while running TChecker using the flag `-a`.

---------

# TChecker

## Description

This is TChecker, an *open-source* model-checker for timed systems. TChecker is
written in C++17 and provides a library of classes to describe timed systems,
to compute the corresponding timed automata, to compute its semantics, as well
as symbolic representations and algorithms to check requirements over timed
systems. It also comes with tools to run these algorithms from the command
line.

TChecker originates and is still lead by academic research on the verification
of timed systems. It is intended to serve as a plateform to experiment new data
structures and algorithms for the verification and the synthesis of timed
systems. The goal of the project is to implement state-of-the-art algorithms as
well as benchmarks to evaluate these algorithms, and to share then with the
community.

## Installation

Please, refer to [Installation of TChecker](https://github.com/ticktac-project/tchecker/wiki/Installation-of-TChecker) or to file INSTALL.md in the repository.

## Usage

Please, refer to [Using TChecker](https://github.com/ticktac-project/tchecker/wiki/Using-TChecker) or to file doc/usage.md in the repository.

## Credits

The authors of the project are credited in the file AUTHORS in the repository.

## License

TChecker is published under the MIT license reproduced below (and in the file
LICENSE in the repository).

MIT License

Copyright (c) 2019 Bordeaux INP, CNRS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
