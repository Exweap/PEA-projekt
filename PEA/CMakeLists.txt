cmake_minimum_required(VERSION 3.15)
project(PEA)

set(CMAKE_CXX_STANDARD 14)

add_executable(PEA main.cpp Graph.cpp Graph.h Menu.cpp Menu.h BruteForce.cpp Dynamic.cpp SimAnnealing.cpp AnnealingManager.cpp AnnealingManager.h Genetic.cpp GeneticManager.cpp GeneticManager.h)