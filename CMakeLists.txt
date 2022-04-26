cmake_minimum_required(VERSION 3.22)
project(HW11)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OSX_ARCHITECTURES "arm64") #comment this out if ur not using arm64 (m1/apple silicon)

configure_file(five.tsv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(challenge.tsv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(linear.tsv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

#add_executable(HW10_TSP chromosome.cpp chromosome.h cities.cpp cities.h deme.cpp deme.h tsp.cpp)
add_executable(HW11 tsp.cpp chromosome.cpp deme.cpp cities.cpp climb_chromosome.cpp climb_chromosome.h tournament_deme.cpp tournament_deme.h) #i think this is all i need, trimmed down from above

#add_executable(test test.cpp)