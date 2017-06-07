//
//  garage.hpp
//  APS
//
//  Created by Luke Gulley on 12/10/16.
//  Copyright © 2016 lgulley1. All rights reserved.
//

#ifndef garage_hpp
#define garage_hpp

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <list>
#include <algorithm>
#include <fstream>

const int SIZE = 5;

class Person {
public:
    std::string getName() { return name; }
    long getPhone() { return phone; }
    std::string getAddress() { return address; }
    void setName(std::string x) { name = x; }
    void setPhone(long x) { phone = x; }
    void setAddress(std::string x) { address = x; }
private:
    std::string name;
    long phone;
    std::string address;
};


class Account {
public:
    Account() { balance = ((rand() % 10)*100); }
    Account(int id, int bal) { account_id = id; balance = bal; }
    void addMoney(int amount) { balance += amount; }
    void deductMoney(int amount) { balance -= amount; }
    int getID() { return account_id; }
    int getBalance() { return balance; }
    void setID(int nid) { account_id = nid; }
    void setBalance(int bal) { balance = bal; }
private:
    int account_id = rand() % 1000000;
    int balance;
};


class Vehicle {
public:
    Vehicle() { color = std::string(); model = std::string(); license_plate = std::string(); }
    Vehicle(std::string nc, std::string nm, std::string nlp) { color = nc; model = nm; license_plate = nlp; }
    std::string getColor() { return color; }
    std::string getModel() { return model; }
    std::string getLicensePlate() {
        return license_plate;
    }
    int getHoursStored() { return std::abs(exitTime - enterTime); }
    bool checkStolen() { return stolen; }
    void setStolen(bool n) { stolen = n; }
    int getEnterTime() { return enterTime; }
    void setExitTime(int x) { exitTime = x; }
private:
    std::string color;
    std::string model;
    std::string license_plate;
    int enterTime;
    int exitTime;
    
    bool stolen;
};


class Ticket {
public:
    int getHoursSpent() { return std::abs(arrival - leave); }
    void setArrival(int currTime) { arrival = currTime; }
    void setLeave(int currTime) { leave = currTime; }
private:
    std::string license_plate_no;
    int arrival = std::rand() % 10;
    int leave;
    
};


class Customer {
public:
    Customer &operator = (const Customer &c);
    Customer() { identity.setName("none"); identity.setAddress("none"); identity.setPhone(0); cash = 0; acc = Account(); }
    Customer(std::string name, long phone, std::string address, int ncash, std::string lic_no, Vehicle ncar);
    Person getIdentity() { return identity; }
    std::string getLicense() { return license_no; }
    Vehicle getCar() { return car; }
    Account getAccount() { return acc; }
    void deductCash(int amount) { cash -= amount;  }
    void payWithAccount(Account acc) { };
    int getCash() { return cash; }
    Ticket getTicket() { return ticket; }
    std::string getReceipt() { return receipt; }
    void setReceipt(std::string &m) { receipt = m; }
    void setCustLeave(int x) { ticket.setLeave(x); }
    void setCash(int ncash) { cash = ncash; }
    Customer(const Customer &rhs);
    void setPaid(bool x) { paid = x; }
    bool hasPaid() { return paid; }
private:
    Ticket ticket;
    Person identity;
    Vehicle car;
    std::string license_no;
    Account acc;
    int cash;
    std::string receipt;
    bool paid = false;
};


class Kiosk {
public:
    double calculatePrice(Customer c) { return c.getTicket().getHoursSpent()*pricePerHour; }
    void acceptCash(Customer&);
    void acceptCard(Customer&);
    void scanTicket(Customer &c) { c.getTicket().setLeave((rand() % 10)+ 1); }  //c.setCustLeave(rand() % 10); }
    void prepareTicket(std::string &method, Customer &c);
    void printReceipt(Customer &c) {
        std::cout << "\n" << c.getIdentity().getName() << c.getReceipt();
        c.setPaid(true);
    }
private:
    const double pricePerHour = 4;
};


class Pallet {
    friend class Mechanic;
private:
    Vehicle car;
    bool visited = false;
    //if car is in spot, status = true
    bool status = false;
public:
    void swap(Pallet &rhs);
    Vehicle getVehicle() { return car; }
    void setVisited(bool x) { visited = x; }
    void setStatus(bool x) { status = x; }
    void setCar(Vehicle c) { car = c; status = true; }
    bool isEmpty() { if(status == false) { return true; } else { return false; } }
    bool isNotVisited() { if(visited == false) { return true; } else { return false; } }
};


class Generator {
    friend class Mechanic;
private:
    int currPower = 100;
    bool status = false;
public:
    void setStatus(bool x) { status = x; }
    int getPower() { return currPower; }
    bool getStatus() { return status; }
    void start() { status = true; deductPower(); }
    void deductPower();
    void stop() { status = false; }
    void subOnePower() { currPower--; }
};


class VoiceoverDisplay {
private:
    int nextEmptyBay = 0;
    std::string message = "proceed to bay " + std::to_string(nextEmptyBay);
public:
    void setMessage(std::string nm) { message = nm; }
    void setEmptyBay(int x) { nextEmptyBay = x; }
    std::string getMessage() { return message; }
};


class Elevator {
    friend class Mechanic;
private:
    Vehicle car;
    int locX;
    int locY;
    int currFloor = 0;
    bool status;
public:
    void setCar(Vehicle c) { car = c; }
    int getFloor() { return currFloor; }
    int getX() { return locX; }
    int getY() { return locY; }
    void increaseFloor(int x) { currFloor += x; }
    void decreaseFloor(int x) { currFloor -= x; }
    bool checkEmpty() { return status; }
    void setStatus(bool x) { status = x; }
    Vehicle getCar() { return car; }
    void emptyElevator() { car = Vehicle(); status = false; }
};


class Mechanic {
private:
    Person identity;
    bool alert;
public:
    void startGenerator(Generator &g) { g.start(); }
    void stopGenerator(Generator &g) { g.stop(); }
    int getGeneratorPower(Generator g) { return g.getPower(); }
    void setIdentity(std::string name, long phone, std::string address) { identity.setName(name); identity.setPhone(phone); identity.setAddress(address); }
    void rebootSystem() { std::cout << "\nsystem rebooted\n\n"; }
    Person getIdentity() { return identity; }
    void contactPolice() { std::cout << "contacted police"; }
    void contactOwner() { std::cout << "contacted owner"; }
    void setAlert(bool x) { std::cout << "mechanic contacted"; alert = x; }
};


class SecurityCamera {
private:
public:
    void record() { std::cout << "security cameras: recording started\n\n"; }
    bool detectMotion() { std::cout << "\nmotion detected, contacting mechanic || "; return true; }
    void alertStaff() { std::cout << "mechanic alerted, police called"; contactPolice(); }
    void contactPolice() { std::cout << "\npolice contacted\n\n"; }
};


class SensorsLasers {
private:
    bool empty = false;
public:
    void setVehicleEmpty(bool x) { empty = x; }
    bool checkVehicleEmpty() { return empty; }
    void takePhotos() { std::cout << "\nphotos taken\n"; }
    bool checkStolen(Vehicle x) { return x.checkStolen(); }
};


class Bay {
private:
    int locX;
    int locY;
    std::string direction;
    bool status;
    Pallet p;
    SensorsLasers sens_lasers;
public:
    SensorsLasers getSensLasers() { return sens_lasers; }
    void setX(int nx) { locX = nx; }
    void setY(int ny) { locY = ny; }
    int getX() { return locX; }
    int getY() { return locY; }
    bool getStatus() { return status; }
    Pallet getPallet() { return p; }
    void setPallet(Vehicle c, bool pStatus, std::string dir) { p.setCar(c); p.setStatus(pStatus); direction = dir; }
    void emptyBay() { p.setCar(Vehicle());
        p.setStatus(false); }
    void setDirection(std::string ndir) { direction = ndir; }
    void driveAway() { p.setCar(Vehicle()); p.setStatus(false); direction = ""; }
};


class Garage {
public:
    Garage() { camera.record(); for(int i = 0; i < 10000; i++) { CustomerDatabase[i] = Customer(); mechanic.setIdentity("John", 7404229319, "102 Main Street"); } }
    void removeCar(Vehicle);
    void addCar(Vehicle);
    void placeCar(int, int);
    void setLotUnvisited();
    void printLot();
    void swapUp(int startX, int startY, int amount);
    void swapDown(int startX, int startY, int amount);
    void swapLeft(int startX, int startY, int amount);
    void swapRight(int startX, int startY, int amount);
    int findCarFloor(Vehicle c);
    bool isFloorFull();
    bool isLotFull();
    Vehicle findCustomerCar(int);
    void addCustToDatabase(Customer c, int);
    void elevatorToBay(Vehicle c, std::string);
    void outputCarData(std::string);
    Kiosk getKiosk() { return paymentTerminal; }
    Vehicle findCarByLicensePlate(std::string license_no);
    Mechanic getMechanic() { return mechanic; }
    void alertMechanic() { if(camera.detectMotion()) { mechanic.setAlert(true); camera.contactPolice(); } }
    Generator getGenerator() { return backupGenerator; }
    Bay getExitBay() { return exitBay; }
    int findNextEmptyFloor();
    std::string getOwnerName(std::string license_no);
    int findEmptyBay();
    std::string getDisplayMessage() { return vod.getMessage(); }
    void setVehicleBays() { for(int i = 0; i < 10; i++) { vehicleBays[i].setX(0); vehicleBays[i].setY(i); } }
    void addCarToBay(Vehicle c);
    void bayToElevator(Bay b);
    VoiceoverDisplay getVOD() { return vod; }
    Bay getVehicleBay(int x) { return vehicleBays[x]; }
    int findCustomerIDByLicense(std::string lic_no);
    Customer* getDatabase() { return dataArray; }
    void setGeneratorOn() { mechanic.startGenerator(backupGenerator); }
    void decreaseGenerator() { backupGenerator.subOnePower(); }
    void setGeneratorOff() { mechanic.stopGenerator(backupGenerator); }
    void emptyExitBay() { exitBay.emptyBay(); }
    void addCustomerByText(int&, std::string);
    void addCarByText(std::string);
    void payByText(std::string);
    void removeByText(std::string);
    void removeCar(std::string direction, Vehicle c);
private:
    VoiceoverDisplay vod;
    Mechanic mechanic;
    Customer CustomerDatabase[10000];
    Customer *dataArray = CustomerDatabase;
    Pallet Spaces[SIZE][SIZE][SIZE];
    Elevator elevator;
    Generator backupGenerator;
    Kiosk paymentTerminal;
    SecurityCamera camera;
    Bay vehicleBays[10];
    Bay exitBay;
};

#endif /* garage_hpp */
