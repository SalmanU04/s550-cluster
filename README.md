#S550 Mustang Digital Cluster Interface

##Overview

This project focuses on reverse-engineering and interfacing a 2015 Ford Mustang (S550) digital instrument cluster using a microcontroller-based system. The goal is to simulate vehicle CAN messages, bring the cluster to life outside the car, and ultimately integrate it with a real-time driving simulator.

The system allows the cluster to boot, display gauges, and respond to manually injected signals from a host computer. Ongoing work aims to transition from manual input to UDP-based real-time data streaming from a simulator environment.

##Features

*Successful standalone S550 cluster startup and operation
*Manual control of cluster signals via host computer
*CAN message injection and frequency analysis
*Embedded firmware written in C
*Modular design for future simulator integration
*Designed for real-time automotive systems research
