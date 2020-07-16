autowatch = 1;
var inputObject;
var i  = 0;
var nextPop = "pop_";
var populationSize= 0;

function getNext(){
	
 if(i < populationSize)
 outlet(0, inputObject[nextPop.concat(i++)]);
else post('you already iterated through the whole population');
		
}	
	
function dictionary(id){
	
	var d = new Dict(id);

	
	inputObject = JSON.parse(d.stringify());
	populationSize = Object.keys(inputObject).length 
}


