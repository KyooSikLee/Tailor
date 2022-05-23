from unreal_engine.classes import PyFactory

import unreal_engine as ue

class Hello(PyFactory):

    def __init__(self):
        ue.log_error('Hello World, i am a useless factory')
