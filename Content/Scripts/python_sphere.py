import unreal_engine as ue
from unreal_engine.classes import PyProcMeshManager
from unreal_engine import FVector, FRotator
import numpy

class PythonSphere:

    # this is called on game start
    def begin_play(self):
        ue.log('Begin Play on Hero class')
        world = ue.get_editor_world()
        manager = world.actor_spawn(PyProcMeshManager, FVector(0, 0, 0), FRotator(0, 0, 0))
        print(manager)
        manager.begin_play()
        print("shit")
    # this is called at every 'tick'    
    def tick(self, delta_time):
        # get current location
        location = self.uobject.get_actor_location()
        # increase Z honouring delta_time
        location.z += numpy.random.rand(10)[0] / 3
        # set new location
        self.uobject.set_actor_location(location)
