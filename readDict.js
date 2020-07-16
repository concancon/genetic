autowatch = 1;
outlets = 3;
var inputObject;
var i  = 0;
var nextPop = "pop_";
var populationSize= 0;

function getNext(){
	
 if(i < populationSize){
 var tag = nextPop.concat(i++);
 outlet(0, tag);
 outlet(1, inputObject.population[tag]);
 popSize();
}
 else post('you already iterated through the whole population');
		
}	
	
function dictionary(id){
	inputObject = {};
	i = 0; //reset i with every new dicte
	var d = new Dict(id);
	inputObject = JSON.parse(d.stringify());
	populationSize = Object.keys(inputObject.population).length; 
}

function popSize(){
  outlet(2, Object.keys(inputObject.population).length);	
}


