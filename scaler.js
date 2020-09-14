autowatch = 1;
inlets = 1;


var numScales= 20;
var theScales = new Array();
var theOutlets = new Array();
g = new Global("memory");

function save(){
	
 embedmessage("setMemory",16);
}

function setMemory(x){
	
		g.memory= x;
}
function loadbang(){
	
	post("number of synth params" + numberOfSynthesisParameters + "\n");
	post("memory" + g.memory + "\n");
	if(g.memory == 0){
	
	 scales(numberOfSynthesisParameters);
	}
	
	
	
}
function scales(val){
	
	
	if(arguments.length){
		
		var a = arguments[0];
			// safety check for number of sliders
		if(a<0) a = 0; // too few scales, set to 0
		if(a>128) a = 128; // too many scales, set to 128		
		
	
			
		// ...in with the new
		numScales = a;
		///if(numScales) ////
		
		for(var k= 0; k < a; k++)
		{
			//this.patcher.newdefault(120, 90, "scale 0 10. 0 20.");
			theScales[k] = this.patcher.newdefault(300+(k*100), 50, "scale 0. 255. 0. 5000.");		
			theOutlets[k] = this.patcher.newdefault(300+(k*100), 100,  "outlet");
			if(numScales)
			{
				this.patcher.connect(this.patcher.getnamed("dynamicUnpack"),k, theScales[k], 0);
				this.patcher.connect(theScales[k], 0, theOutlets[k], 0);
				this.patcher.connect(this.patcher.getnamed("myCycle"), k, theScales[k], 4);
			}
		}	
		post("a: " + a + "\n");		
		g.memory=a;
			
		
		
	}
	else // complain about arguments
	{
		post("scales message needs arguments");
		post();
	}
		
}