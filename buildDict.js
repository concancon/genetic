autowatch =1;

var outputObject = {};
var target= [255, 255, 255];
var d = new Dict;
var popSize = 0;

function anything(){
	
	var key = messagename; //pop_0, pop_1...
	var args = arrayfromargs(arguments); 
	  
	
	
	if(!outputObject.population) outputObject.population= {};
	outputObject.population[messagename]= args[0];
	//is it complete? if so send it out
	
	
	for( var key in  Object.keys(outputObject.population)){
			
			post('\'' + Object.keys(outputObject.population)[key] +'\'\n');
		}
	
	if(Object.keys(outputObject.population).length == popSize) {
	//post('outputting dict\n');
	bang();
	reset();
	}
			
}

function setSize(l){
	
	popSize = l;


}

function generation(g){

	outputObject.generation = g;

}

function bang(){
   //post('bang\n');
   d.parse(JSON.stringify(outputObject));
   outlet(0, "dictionary", d.name);	
}



function reset(){
	
	d = new Dict;
	outputObject= {};
	bang();
	
 	
}




