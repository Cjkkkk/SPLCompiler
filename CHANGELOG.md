# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2019-05-12
### Added
- copy propagation
- const propagation
- dead code removal

### Fixed
- check pull request #11

## [2.0.0] - 2019-04-26
### Added
- AST (record & array not included)
- simple type check
- IR generation
- IR CFG
- IR SSA form
- test framework

## [1.2.0] - 2019-03-22
### Added
- Expression + - * / and or Mod
- id (not symbol table check yet, default value to 1 for debug)
- simple variable assignment

## [1.1.0] - 2019-03-16
### Added
- Debug configuration file for Visual Studio Code
- Reset compiling opimization level

## [1.0.0] - 2019-03-14
### Added
- Clean up the framework of project
- Finish compiler's frontend