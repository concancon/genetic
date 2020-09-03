autowatch=  1;
inlets = 1;


function createOutlets(n){
  
  //post(n + "\n");
  outlets = n;	


}

function list(myList){
	
  
   var args = arrayfromargs(arguments); 

   //post(args.length + "\n");
   createOutlets(args.length);
   //compile 
 
   //then send out each value out of each new outlet

   for(var i = 0; i < args.length; i++){
	
		outlet(i, args[i]);
	}

		
}

