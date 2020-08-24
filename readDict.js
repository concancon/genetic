autowatch = 1;
outlets = 3;
var inputObject;
var i  = 0;
var nextPop = "pop_";
var populationSize= 0;

function getNext(){

 if(i < populationSize){

 var tag = nextPop.concat(i++);
 outlet(1, inputObject.population[tag]);
 outlet(0, tag);
 popSize();
}
 else post('you already iterated through the whole population\n');
		
}	
	
function dictionary(id){
	post('got another dictionary \n');
	inputObject = {};
	i = 0; //reset i with every new dicte
	var d = new Dict(id);
	inputObject = JSON.parse(d.stringify());//make a new js object from dictionary
	populationSize = Object.keys(inputObject.population).length; 

	post('pop size is ' + populationSize +   '\n');
}

function popSize(){
  outlet(2, Object.keys(inputObject.population).length);	
}

function reset(){
	post('reset\n');
		i = 0;
}

