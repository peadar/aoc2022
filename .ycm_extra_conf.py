import os
import ycm_core

flags = [ '-std=c++17' ]
SOURCE_EXTENSIONS = [ '.cpp', '.cc', '.c' ]

def Settings( **kwargs ):
    return { 'flags': flags }
