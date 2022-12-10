import os
import ycm_core

flags = [ '-std=c++17', '-I..', '-I.', '-x', 'c++' ]
SOURCE_EXTENSIONS = [ '.cpp', '.cc', '.c', '.h' ]

def Settings( **kwargs ):
    return { 'flags': flags }
