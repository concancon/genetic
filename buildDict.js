autowatch =1;

var outputObject = {};
var target= [255, 255, 255];
var d = new Dict;


function anything(){
	
	var msg = messagename; //pop_0, pop_1...
	var args = arrayfromargs(arguments);
	if(args.length == target.length){
	//bewerte args
		var score = 0;
		for(var i = 0; i< args.length; i++){
			var difference= Math.abs(args[i]- target[i] );
			var distance = (255 - difference) /255;
	    	score += (distance);	
	  	}
	
	score =  (score / args.length) * 100.;
	
	if(!outputObject.population) outputObject.population= {};
	outputObject.population[messagename] = score;
	
	
	}
	else{
		post('target and args have different lengths');
	}
	
	
}

function generation(g){
	
	outputObject.generation = g;

}

function bang(){
   d.parse(JSON.stringify(outputObject));
   outlet(0, "dictionary", d.name);	
}

function reset(){
	
	outputObject= {};
 	
}
function setTarget(){
  
	target = arrayfromargs(arguments);
		
}



