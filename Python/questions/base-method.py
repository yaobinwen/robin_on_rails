#!/usr/bin/python3

class Parent(object):
    def show_name(self):
        print("Parent")

class Child(Parent):
    def show_name(self):
        print("Child")


c = Child()
c.show_name()   # What is the output and why?
