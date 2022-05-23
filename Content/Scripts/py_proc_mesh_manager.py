import unreal_engine as ue 
from unreal_engine.classes import Character

class PyProcMeshManager(Character):
	def begin_play(self):
		ue.log('Begin Play on PyProcMesh class')
        
    # this is called at every 'tick'    
	def tick(self, delta_time):
		location = self.uobject.get_actor_location()
		# increase Z honouring delta_time
		location.z += 0.1
		# set new location
		self.uobject.set_actor_location(location)
		
