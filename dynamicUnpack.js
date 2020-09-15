autowatch=  1;
inlets = 1;
outlets = 64;


var g = new Global("memory");
g.numberOfParams= 16;


function list(myList){
	
  
   var args = arrayfromargs(arguments); 


   	 if(args.length)g.numberOfParams= args.length;
    
  	for(var i = 0; i < args.length; i++)
		{
		outlet(i, args[i]);
     	}
    
  
		
}


