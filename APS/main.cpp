//
//  main.cpp
//  APS
//
//  Created by Luke Gulley on 12/10/16.
//  Copyright © 2016 lgulley1. All rights reserved.
//

#include <iostream>
#include <thread>
#include "garage.hpp"

//start generator
void startGenerator(Garage &Lot) {
    Lot.setGeneratorOn();
    int i = 0;
    while(i < 100) {
        std::this_thread::yield();
        if(Lot.getGenerator().getStatus() == false) { break; }
        Lot.decreaseGenerator();
        usleep(1000 * 1000);
        i++;
    }
    if(i == 100) { std::cout << "\ngenerator out of power\n"; return; }
}


int main() {
    
    Garage Lot;

    int option = -1;
    char option2;
    
    int count = 0;
    
    while(option != 0) {
        
        //ends if lot is out of power
        if(Lot.getGenerator().getPower() == 0) { break; }
        
        //options
        std::cout << "0. exit\n1. add customer\n2. add car\n3. remove car from lot\n4. print lot\n5. rebootSystem\n6. car details\n7. pay for parking\n8. report crime\n9. start power generators\n10. stop power generators\n11. remove car from exit bay\n12. see generator power\n";
        std::string name;
        std::string address, license_no, color, model, license_plate;
        std::string txtFile;
        int cash;
        long phone;
        
        std::string curr_cust = "cust_" + std::to_string(count);
        
        std::cout << "select an option: ";
        std::cin >> option;
        
        switch(option) {
            case 0: break;
                //create customer and car/add to database
            case 1: {
                std::cout << "add by txt (t) or by command line (c)?: ";
                std::cin >> option2;
                
                if(option2 == 't') {
                    //add by txt
                    std::cout << "please enter txt path (input/customers.txt): ";
                    std::cin >> txtFile;
                    Lot.addCustomerByText(count, txtFile);
                    std::cout << "\n";
                }
                
                else {
                    //get customer information
                    std::cout << "name: "; std::cin.ignore(); std::getline (std::cin, name);
                    std::cout << "phone: "; std::cin >> phone;
                    std::cout << "address: "; std::cin.ignore(); std::getline (std::cin, address);
                    std::cout << "cash: "; std::cin >> cash;
                    std::cout << "license_no: "; std::cin.ignore(); std::getline (std::cin, license_no);
                    //get vehicle information
                    std::cout << "Vehicle Info\n";
                    std::cout << "color: "; std::getline (std::cin, color);
                    std::cout << "model: "; std::getline (std::cin, model);
                    std::cout << "license_plate: "; std::cin >> license_plate;
                    Vehicle x(color, model, license_plate);
                    Customer curr_cust(name, phone, address, cash, license_no, (Vehicle(color, model, license_plate)));
                    std::cout << "\ncustomer_no = " << count << "\n";
                    Lot.addCustToDatabase(curr_cust, count);
                    count++;
                    std::cout << "\n";
                }
                break;
            }
                //add car to lot by license_plate number
            case 2: {
                std::cout << "add by txt (t) or by command line (c)?: ";
                std::cin >> option2;
                
                if(option2 == 't') {
                    //add by txt
                    std::cout << "please enter txt path (input/add_cars.txt): ";
                    std::cin >> txtFile;
                    Lot.addCarByText(txtFile);
                }
                
                else {
                    Lot.getDisplayMessage();
                    std::string license_no;
                    std::cout << "enter license plate no: ";
                    std::cin >> license_no;
                    int bay_no = Lot.findEmptyBay();
                    Lot.addCarToBay(Lot.findCarByLicensePlate(license_no)); std::cout << "\ncar in bay " << bay_no;
                    Lot.bayToElevator(Lot.getVehicleBay(bay_no)); std::cout << "\ncar moved from bay to elevator\n";
                    Lot.addCar(Lot.findCarByLicensePlate(license_no));
                    std::cout << "car moved to lot\n";
                    std::cout << "\n";
                }
                break;
            }
                //remove car from lot by license_plate number
            case 3: {
                std::cout << "remove by txt (t) or by command line (c)?: ";
                std::cin >> option2;
                
                if(option2 == 't') {
                    //add by txt
                    std::cout << "please enter txt path (input/remove_cars.txt): ";
                    std::cin >> txtFile;
                    Lot.removeByText(txtFile);
                }
                
                else {
                    std::string license_no;
                    std::cout << "enter license plate no: ";
                    std::cin >> license_no;
                    if(Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)].hasPaid()) {
                        Lot.removeCar(Lot.findCarByLicensePlate(license_no));
                        std::cout << "\ncar in exit bay, use command (11) to remove car from bay\n";
                    }
                    else {
                        std::cout << "\nsorry, please pay before removing vehicle.\n";
                    }
                    std::cout << "\n";
                }
                break;
            }
                //print lot
            case 4: {
                std::cout << "\n";
                Lot.printLot();
                std::cout << "\n";
                break;
            }
                //reboot system
            case 5: {
                Lot.getMechanic().rebootSystem();
                break;
            }
                //output car data by license_plate number
            case 6: {
                std::string license_no;
                std::cout << "enter license plate no: ";
                std::cin >> license_no;
                Lot.outputCarData(license_no);
                break;
            }
                //accept payment
            case 7: {
                std::cout << "pay by txt (t) or by command line (c)?: ";
                std::cin >> option2;
                
                if(option2 == 't') {
                    //add by txt
                    std::cout << "please enter txt path (input/pay_cars.txt): ";
                    std::cin >> txtFile;
                    Lot.payByText(txtFile);
                }
                
                else {
                    
                    std::string payment_method;
                    std::string license_no;
                    std::cout << "enter license plate no: ";
                    std::cin >> license_no;
                    Lot.getKiosk().scanTicket(Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)]);
                    
                    std::cout << "\n" << Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)].getIdentity().getName() <<", your total is $" << Lot.getKiosk().calculatePrice(Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)]) << "\n";
                    
                    std::cout << "how would you like to pay (acc, cash)?: ";
                    std::cin >> payment_method;
                    Lot.getKiosk().prepareTicket(payment_method, Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)]);
                    
                    if(payment_method == "cash" ) {
                        Lot.getKiosk().acceptCash(Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)]);
                    }
                    else if(payment_method == "acc")
                    {
                        Lot.getKiosk().acceptCard(Lot.getDatabase()[Lot.findCustomerIDByLicense(license_no)]);
                    }
                    else {
                        std::cout << "error, please try again";
                        
                    }
                }
                break;
            }
                //alert mechanic of crime
            case 8: {
                Lot.alertMechanic();
                break;
            }
                //start power generators
            case 9: {
                std::cout << "\n";
                std::thread Gen(startGenerator, std::ref(Lot));
                std::this_thread::yield();
                usleep(1000 * 1000);
                Gen.detach();
                std::cout << "\n";
                break;
            }
                //turn off power generators
            case 10: {
                std::cout << "\n";
                Lot.setGeneratorOff();
                std::cout << "\n";
                break;
            }
                //remove car from exit bay
            case 11: {
                Lot.emptyExitBay();
                std::cout <<"\ncar removed from exit bay\n\n";
                break;
            }
            case 12: {
                std::cout << "\ncurr power = " << Lot.getMechanic().getGeneratorPower(Lot.getGenerator()) << "\n\n";
                break;
            }
        }
    }
    return 0;
}
