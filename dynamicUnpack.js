autowatch=  1;
inlets = 1;
outlets = 16;
numberOfSynthesisParameters= 16;

function loadbang(){	

 post("memory in dynamic" + g.memory + "\n");
 
 	    createOutlets(numberOfSynthesisParameters);
		
}

function createOutlets(n){
  
  post(n + "\n");
  outlets = n;	
  this.box.compile();		

}

function list(myList){
	
  
   var args = arrayfromargs(arguments); 

	if(args!= numberOfSynthesisParameters){ //only update on change
   
   	 numberOfSynthesisParameters= args;
     if(args.length != g.memory){
	 createOutlets(args.length);

	 }

  	for(var i = 0; i < args.length-1; i++)
		{
		outlet(i, args[i]);
     	}
    }
  
		
}


function msg_int(val){
	
   
   //createOutlets(val);

		
}
