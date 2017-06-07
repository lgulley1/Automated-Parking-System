//
//  garage.cpp
//  APS
//
//  Created by Luke Gulley on 12/10/16.
//  Copyright © 2016 lgulley1. All rights reserved.
//

#include "garage.hpp"

#define RESET   "\033[0m"
#define BLACK   "\033[1m\033[30m"      /* Bold Black */
#define RED     "\033[1m\033[31m"      /* Bold Red */
#define GREEN   "\033[1m\033[32m"      /* Bold Green */
#define YELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BLUE    "\033[1m\033[34m"      /* Bold Blue */
#define MAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define CYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define WHITE   "\033[1m\033[37m"      /* Bold White */

void getColor(std::string color) {
    if(color == "black") { std::cout << BLACK; }
    if(color == "red") { std::cout << RED; }
    if(color == "green") { std::cout << GREEN; }
    if(color == "yellow") { std::cout << YELLOW; }
    if(color == "blue") { std::cout << BLUE; }
    if(color == "magenta") { std::cout << MAGENTA; }
    if(color == "cyan") { std::cout << CYAN; }
    if(color == "white") { std::cout << WHITE; }
}

void Kiosk::acceptCash(Customer &c) {
    if(c.getCash() < calculatePrice(c)) {
        std::cout << "not enough cash, please choose another payment option\n";
    }
    
    else {
        c.deductCash(calculatePrice(c));
        printReceipt(c);
    }
}

void Kiosk::acceptCard(Customer &c) {
    if(c.getAccount().getBalance() < calculatePrice(c)) {
        std::cout << "not enough money, please choose another payment option\n";
    }
    
    else {
        c.getAccount().deductMoney(calculatePrice(c));
        printReceipt(c);
    }
}

void Generator::deductPower() {
    int i = 0;
    char quit = 'x';
    while(i < 100 && quit == 'q') {
        std::this_thread::yield();
        if(status == false) { break; }
        currPower--;
        usleep(1000 * 1000);
        std::cin >> quit;
        usleep(1000 * 1000);
        i++;
    }
    if(i == 100) { std::cout << "generator out of power"; return; }
}


void Garage::setLotUnvisited() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < SIZE; k++) {
                Spaces[i][j][k].setVisited(false);
            }
        }
    }
}


//should check if lot is full first
void Garage::addCar(Vehicle c) {
    
    setLotUnvisited();
    
    //moves elevator to first floor to retrieve car
    elevator.decreaseFloor(elevator.getFloor());
    
    //checks if lot is full and finds next empty floor, moving elevator to that position
    if(isLotFull()) { std::cout << "sorry, lot is full, try again later"; return; }
    
    else {
        elevator.increaseFloor(findNextEmptyFloor());
    }
    
    Spaces[elevator.getFloor()][SIZE/2][SIZE/2].setStatus(true);
    Spaces[elevator.getFloor()][SIZE/2][SIZE/2].setCar(c);
    Spaces[elevator.getFloor()][SIZE/2][SIZE/2].setVisited(true);
    int carX = SIZE/2;
    int carY = SIZE/2;
    placeCar(carX, carY);
}

void Garage::placeCar(int x, int y) {
    //shifts vehicle up
    if(Spaces[elevator.getFloor()][x-1][y].isEmpty() && Spaces[elevator.getFloor()][x-1][y].isNotVisited() && x-1 < SIZE && x-1 > -1 && y < SIZE && y > -1) {
        //makes new spaces visited
        Spaces[elevator.getFloor()][x-1][y].setVisited(true);
        //places car in new space
        Spaces[elevator.getFloor()][x-1][y].setCar(Spaces[elevator.getFloor()][x][y].getVehicle());
        //sets old space to empty
        Spaces[elevator.getFloor()][x][y].setCar(Vehicle());
        Spaces[elevator.getFloor()][x][y].setStatus(false);
        //updates location
        placeCar(x-1, y);
    }
    
    //shifts vehicle left
    if(Spaces[elevator.getFloor()][x][y-1].isEmpty() && Spaces[elevator.getFloor()][x][y-1].isNotVisited() && x < SIZE && x > -1 && y-1 < SIZE && y-1 > -1) {
        //makes new spaces visited
        Spaces[elevator.getFloor()][x][y-1].setVisited(true);
        //places car in new space
        Spaces[elevator.getFloor()][x][y-1].setCar(Spaces[elevator.getFloor()][x][y].getVehicle());
        //sets old space to empty
        Spaces[elevator.getFloor()][x][y].setCar(Vehicle());
        Spaces[elevator.getFloor()][x][y].setStatus(false);
        //updates location
        placeCar(x, y-1);
    }
    
    //shifts vehicle right
    if(Spaces[elevator.getFloor()][x][y+1].isEmpty() && Spaces[elevator.getFloor()][x][y+1].isNotVisited() && x < SIZE && x > -1 && y+1 < SIZE && y+1 > -1) {
        //makes new spaces visited
        Spaces[elevator.getFloor()][x][y+1].setVisited(true);
        //places car in new space
        Spaces[elevator.getFloor()][x][y+1].setCar(Spaces[elevator.getFloor()][x][y].getVehicle());
        //sets old space to empty
        Spaces[elevator.getFloor()][x][y].setCar(Vehicle());
        Spaces[elevator.getFloor()][x][y].setStatus(false);
        //updates location
        placeCar(x, y+1);
    }
    
    //shifts vehicle down
    if(Spaces[elevator.getFloor()][x+1][y].isEmpty() && Spaces[elevator.getFloor()][x+1][y].isNotVisited() && x+1 < SIZE && x+1 > -1 && y < SIZE && y > -1) {
        //makes new spaces visited
        Spaces[elevator.getFloor()][x+1][y].setVisited(true);
        //places car in new space
        Spaces[elevator.getFloor()][x+1][y].setCar(Spaces[elevator.getFloor()][x][y].getVehicle());
        //sets old space to empty
        Spaces[elevator.getFloor()][x][y].setCar(Vehicle());
        Spaces[elevator.getFloor()][x][y].setStatus(false);
        //updates location
        placeCar(x+1, y);
    }
}


void Garage::printLot() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(Spaces[elevator.getFloor()][i][j].isEmpty()) { std::cout << 'x'; }
            else {
                getColor(Spaces[elevator.getFloor()][i][j].getVehicle().getColor());
                std::cout << 'c';
                std::cout << RESET;
            }
        }
        std::cout << "\n";
    }
}


void Pallet::swap(Pallet &rhs) {
    Pallet tmp = *this;
    this->setVisited(rhs.visited);
    this->setCar(rhs.getVehicle());
    this->setStatus(rhs.status);
    this->visited = rhs.visited;
    rhs = tmp;
}


void Garage::swapUp(int startX, int startY, int amount) {
    for(int i = 0; i < amount; i++) {
        Spaces[elevator.getFloor()][startX-i][startY].swap(Spaces[elevator.getFloor()][startX-i-1][startY]);
    }
}

void Garage::swapDown(int startX, int startY, int amount) {
    for(int i = 0; i < amount; i++) {
        Spaces[elevator.getFloor()][startX+i][startY].swap(Spaces[elevator.getFloor()][startX+i+1][startY]);
    }
}

void Garage::swapLeft(int startX, int startY, int amount) {
    for(int i = 0; i < amount; i++) {
        Spaces[elevator.getFloor()][startX][startY-i].swap(Spaces[elevator.getFloor()][startX][startY-i-1]);
    }
}

void Garage::swapRight(int startX, int startY, int amount) {
    for(int i = 0; i < amount; i++) {
        Spaces[elevator.getFloor()][startX][startY+i].swap(Spaces[elevator.getFloor()][startX][startY+i+1]);
    }
}


void Garage::removeCar(Vehicle c) {
    //gets exit direction
    std::string dir;
    std::cout << "choose direction(N,S,E,W): "; std:: cin >> dir;
    
    int carFloor = findCarFloor(c);
    
    //places elevator on correct floor
    for(int i = 0; i < SIZE; i++) {
        if(elevator.getFloor() == carFloor) { break; }
        if(elevator.getFloor() > carFloor) {
            elevator.decreaseFloor(1);
        }
        else if(elevator.getFloor() < carFloor) {
            elevator.increaseFloor(1);
        }
    }
    
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapUp(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapLeft((SIZE/2 -i), SIZE/2, i);
            swapDown(SIZE/2-i, SIZE/2-i, i);
            swapRight((SIZE/2), SIZE/2-i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator\n"; elevatorToBay(c, dir); return; }
    
    //searches top right
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapUp(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapRight((SIZE/2 -i), SIZE/2, i);
            swapDown(SIZE/2-i, SIZE/2+i, i);
            swapLeft((SIZE/2), SIZE/2+i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator\n"; elevatorToBay(c, dir); return; }
    
    //searches bottom left
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapDown(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapLeft((SIZE/2 +i), SIZE/2, i);
            swapUp(SIZE/2+i, SIZE/2-i, i);
            swapRight((SIZE/2), SIZE/2-i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator\n"; elevatorToBay(c, dir); return; }
    
    //searches bottom right
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapDown(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapRight((SIZE/2 +i), SIZE/2, i);
            swapUp(SIZE/2+i, SIZE/2+i, i);
            swapLeft((SIZE/2), SIZE/2+i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator\n"; elevatorToBay(c, dir); return; }
}


void Garage::removeCar(std::string direction, Vehicle c) {
    int carFloor = findCarFloor(c);
    
    //places elevator on correct floor
    for(int i = 0; i < SIZE; i++) {
        if(elevator.getFloor() == carFloor) { break; }
        if(elevator.getFloor() > carFloor) {
            elevator.decreaseFloor(1);
        }
        else if(elevator.getFloor() < carFloor) {
            elevator.increaseFloor(1);
        }
    }
    
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapUp(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapLeft((SIZE/2 -i), SIZE/2, i);
            swapDown(SIZE/2-i, SIZE/2-i, i);
            swapRight((SIZE/2), SIZE/2-i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator, removed from exit bay\n"; elevatorToBay(c, direction); exitBay.driveAway(); return; }
    
    //searches top right
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapUp(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapRight((SIZE/2 -i), SIZE/2, i);
            swapDown(SIZE/2-i, SIZE/2+i, i);
            swapLeft((SIZE/2), SIZE/2+i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator, removed from exit bay\n"; elevatorToBay(c, direction); exitBay.driveAway(); return; }
    
    //searches bottom left
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapDown(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapLeft((SIZE/2 +i), SIZE/2, i);
            swapUp(SIZE/2+i, SIZE/2-i, i);
            swapRight((SIZE/2), SIZE/2-i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator, removed from exit bay\n"; elevatorToBay(c, direction); exitBay.driveAway(); return; }
    
    //searches bottom right
    for(int i = 1; i < SIZE/2 + 1; i++) {
        for(int k = 0; k < ((i*i)+1); k++) {
            swapDown(SIZE/2, SIZE/2, i);
            if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
            swapRight((SIZE/2 +i), SIZE/2, i);
            swapUp(SIZE/2+i, SIZE/2+i, i);
            swapLeft((SIZE/2), SIZE/2+i, i);
        }
        if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { break; }
    }
    
    if(c.getLicensePlate() == Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle().getLicensePlate()) { std::cout << "car on elevator, removed from exit bay\n"; elevatorToBay(c, direction); exitBay.driveAway(); return; }
}


int Garage::findCarFloor(Vehicle c) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < SIZE; k++) {
                if(Spaces[i][j][k].getVehicle().getLicensePlate() == c.getLicensePlate()) { return i; }
            }
        }
    }
    return -1;
}


bool Garage::isFloorFull() {
    int count = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(Spaces[elevator.getFloor()][i][j].isEmpty()) { count++; }
        }
    }
    if(count > 1) { return false; }
    else {
        return true;
    }
}


bool Garage::isLotFull() {
    int count = 0;
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < SIZE; k++) {
                if(Spaces[i][j][k].isEmpty()) { count++; }
            }
        }
    }
    if(count > SIZE) { return false; }
    else {
        return true;
    }
}

void Garage::addCustToDatabase(Customer c, int id) {
    CustomerDatabase[id] = c;
}

Vehicle Garage::findCustomerCar(int customer_id) {
    return CustomerDatabase[customer_id].getCar();
}

void Garage::elevatorToBay(Vehicle c, std::string dir) {
    
    int currFloor = elevator.getFloor();
    
    //places car on elevator
    elevator.setCar(Spaces[elevator.getFloor()][SIZE/2][SIZE/2].getVehicle());
    
    //returns elevator to main floor
    elevator.decreaseFloor(elevator.getFloor());
    
    //empties car memory from floor
    Spaces[currFloor][SIZE/2][SIZE/2].setCar(Vehicle());
    Spaces[currFloor][SIZE/2][SIZE/2].setStatus(false);
    
    //puts car in exit bay
    exitBay.setPallet(elevator.getCar(), true, dir);
    
    //empties elevator
    elevator.emptyElevator();
}


void Garage::outputCarData(std::string license_plate_num) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            for(int k = 0; k < SIZE; k++) {
                if(Spaces[i][j][k].getVehicle().getLicensePlate() == license_plate_num) {
                    std::cout << "\n";
                    std::cout << "owner name: "  << getOwnerName(Spaces[i][j][k].getVehicle().getLicensePlate()) << "\n";
                    std::cout << "floor = " << i << "\n";
                    std::cout << "x = " << j << "\n";
                    std::cout << "y = " << k << "\n\n";
                    std::cout << "color: " << Spaces[i][j][k].getVehicle().getColor() << "\n";
                    std::cout << "model: " << Spaces[i][j][k].getVehicle().getModel() << "\n";
                    std::cout << "license plate number: " << Spaces[i][j][k].getVehicle().getLicensePlate() << "\n";
                    std::cout << "enter time: " << Spaces[i][j][k].getVehicle().getEnterTime() << "\n";
                    std::cout << "stolen?: "; if(Spaces[i][j][k].getVehicle().checkStolen()) { std::cout << "yes"; } else { std::cout << "no"; } std::cout << "\n";
                    std::cout << "\n";
                    
                }
            }
        }
    }
}

std::string Garage::getOwnerName(std::string license_no) {
    for(int i = 0; i < 10000; i++) {
        if(CustomerDatabase[i].getCar().getLicensePlate() == license_no) {
            return CustomerDatabase[i].getIdentity().getName();
        }
    }
    return "none";
}


Vehicle Garage::findCarByLicensePlate(std::string license_no) {
    for(int i = 0; i < 10000; i++) {
        if(CustomerDatabase[i].getCar().getLicensePlate() == license_no) {
            return CustomerDatabase[i].getCar();
        }
    }
    return Vehicle();
}


Customer & Customer::operator=(const Customer &rhs) {
    if(this == &rhs) { return *this; }
    identity = rhs.identity;
    ticket = rhs.ticket;
    car = rhs.car;
    license_no = rhs.license_no;
    acc = rhs.acc;
    cash = rhs.cash;
    receipt = rhs.receipt;
    acc = rhs.acc;
    return *this;
}



Customer::Customer(const Customer &rhs) {
    identity = rhs.identity;
    ticket = rhs.ticket;
    car = rhs.car;
    license_no = rhs.license_no;
    acc = rhs.acc;
    cash = rhs.cash;
    receipt = rhs.receipt;
    acc = rhs.acc;
}


int Garage::findNextEmptyFloor() {
    int prevFloor = elevator.getFloor();
    elevator.decreaseFloor(elevator.getFloor());
    
    for(int i = 0; i < SIZE; i++) {
        if(!isFloorFull()) {
            //returns elevator to prev location
            elevator.decreaseFloor(elevator.getFloor());
            elevator.increaseFloor(prevFloor);
            return i;
        }
        else {
            elevator.increaseFloor(1);
        }
    }
    
    //if all floors are full
    elevator.decreaseFloor(elevator.getFloor());
    elevator.increaseFloor(prevFloor);
    return -1;
}


Customer::Customer(std::string name, long phone, std::string address, int ncash, std::string lic_no, Vehicle ncar) {
    identity.setName(name);
    identity.setPhone(phone);
    identity.setAddress(address);
    cash = ncash;
    acc = Account();
    car = ncar;
    license_no = lic_no;
}


int Garage::findEmptyBay() {
    for(int i = 0; i < 10; i++) {
        if(!vehicleBays[i].getStatus()) {
            return i;
        }
    }
    return -1;
}


void Garage::bayToElevator(Bay b) {
    elevator.decreaseFloor(elevator.getFloor());
    elevator.setCar(b.getPallet().getVehicle());
    b = Bay();
}


void Kiosk::prepareTicket(std::string &method, Customer &c) {
    std::string m = "\npayment method: " + method + "\nyour total was " + std::to_string(calculatePrice(c)) + "\npaid\n\n";
    c.setReceipt(m);
}


void Garage::addCarToBay(Vehicle c)
{
    int filled_bay = findEmptyBay();
    if(findEmptyBay() == -1)
    {
        vod.setMessage("no bay empty, please wait");
        return;
    }
    vehicleBays[findEmptyBay()].setPallet(c, true, "north");
    
    vehicleBays[filled_bay].getSensLasers().takePhotos();
    std::cout << "car added\n";
    
    if(vehicleBays[filled_bay].getSensLasers().checkStolen(vehicleBays[filled_bay].getPallet().getVehicle())) {
        vod.setMessage("\nvehicle stolen, contact police\n");
        vod.getMessage();
    }
    else {
        vod.setMessage("\nvehicle not stolen\n");
        vod.getMessage();
    }
    
    if(vehicleBays[filled_bay].getSensLasers().checkVehicleEmpty()) {
        vod.setMessage("\nvehicle: empty\n");
        vod.getMessage();
    }
    
    else {
        vod.setMessage("\nvehicle: not empty\nplease exit the vehicle\n");
        vod.getMessage();
        vehicleBays[filled_bay].getSensLasers().setVehicleEmpty(true);
        vod.setMessage("\nvehicle now empty, ready for transport\n");
        vod.getMessage();
    }
    //resets voiceover message for next customer
    vod = VoiceoverDisplay();
}


int Garage::findCustomerIDByLicense(std::string lic_no) {
    for(int i = 0; i < 10000; i++) {
        if(CustomerDatabase[i].getCar().getLicensePlate() == lic_no) {
            return i;
        }
    }
    return  -1;
}


void Garage::addCustomerByText(int &currCount, std::string input) {
    int numOfParameters = 8;
    std::string paramArray[8];
    std::string param;
    std::ifstream in;
    in.open(input);
    char curr = '\0';
    while(!in.eof()) {
        //get's all 6 parameters
        for(int i = 0; i < numOfParameters; i++) {
            //in >> curr;
            in.get(curr);
            while(curr != ':' || '\n') {
                if(curr == ':' || curr == '\n') { break; }
                param += curr;
                in.get(curr);
                //in >> curr;
            }
            paramArray[i] = param;
            //resets param
            param = std::string();
        }
        //creates customer
        if(in.eof()) { break; }
        CustomerDatabase[currCount] = Customer(paramArray[0], std::stol(paramArray[1]), paramArray[2], std::stoi(paramArray[3]), paramArray[4], (Vehicle(paramArray[5], paramArray[6], paramArray[7])));
        //increments customer id number
        currCount++;
    }
}


void Garage::addCarByText(std::string input) {
    std::string param;
    std::ifstream in;
    in.open(input);
    int bay_no;
    while(!in.eof()) {
        in >> param;
        if(in.eof()) { break; }
        getDisplayMessage();
        bay_no = findEmptyBay();
        addCarToBay(CustomerDatabase[findCustomerIDByLicense(param)].getCar());
        bayToElevator(getVehicleBay(bay_no));
        addCar(findCarByLicensePlate(param));
        if(in.eof()) { break; }
    }
    std::cout << "\n";
}


void Garage::payByText(std::string txtFileLocation) {
    int numOfParameters = 2;
    char curr;
    std::string param;
    std::ifstream in;
    in.open(txtFileLocation);
    std::string paramArray[2];
    
    
    while(!in.eof()) {
        
        if(in.eof()) { break; }
        
        //get's all 2 parameters
        for(int i = 0; i < numOfParameters; i++) {
            //in >> curr;
            in.get(curr);
            if(in.eof()) { break; }
            while(curr != ':' || '\n') {
                if(curr == ':' || curr == '\n') { break; }
                param += curr;
                in.get(curr);
                //in >> curr;
            }
            paramArray[i] = param;
            //resets param
            param = std::string();
        }
        
        paymentTerminal.scanTicket(CustomerDatabase[findCustomerIDByLicense(paramArray[1])]);
        paymentTerminal.prepareTicket(paramArray[0], CustomerDatabase[findCustomerIDByLicense(paramArray[1])]);
        
        if(paramArray[0] == "cash" ) {
            paymentTerminal.acceptCash(CustomerDatabase[findCustomerIDByLicense(paramArray[1])]);
        }
        else if(paramArray[0] == "acc")
        {
            paymentTerminal.acceptCard(CustomerDatabase[findCustomerIDByLicense(paramArray[1])]);
        }
        else {
            std::cout << "error, please try again\n";
        }
        if(in.eof()) { break; }
    }
}

void Garage::removeByText(std::string txtFile) {
    int numOfParameters = 2;
    char curr;
    std::string param;
    std::ifstream in;
    in.open(txtFile);
    std::string paramArray[2];
    while(!in.eof()) {
        if(in.eof()) { break; }
        
        //get's all 2 parameters
        for(int i = 0; i < numOfParameters; i++) {
            //in >> curr;
            in.get(curr);
            if(in.eof()) { break; }
            while(curr != ':' || '\n') {
                if(curr == ':' || curr == '\n') { break; }
                param += curr;
                in.get(curr);
                //in >> curr;
            }
            paramArray[i] = param;
            //resets param
            param = std::string();
        }
        if(in.eof()) { break; }
        removeCar(paramArray[0], CustomerDatabase[findCustomerIDByLicense(paramArray[1])].getCar());
    }
    std::cout << "\n";
}
