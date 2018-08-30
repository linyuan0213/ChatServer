all :
	$(MAKE) -C ./Main 
	$(MAKE) -C ./MultServer 
	$(MAKE) -C ./Model
	$(MAKE) -C ./Views 
	$(MAKE) -C ./Control 
	$(MAKE) -C ./Log 
	$(MAKE) -C ./objs
 
 
clean :
	$(MAKE) -C ./Main clean
	$(MAKE) -C ./MultServer clean
	$(MAKE) -C ./Model clean
	$(MAKE) -C ./Views clean
	$(MAKE) -C ./Control clean
	$(MAKE) -C ./Log clean

 
 
.PHONY : all clean
