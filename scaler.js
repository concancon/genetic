autowatch = 1;
inlets = 1;


var numScales= 20;
var theScales = new Array(128);
var theOutlets = new Array(128);
var memory = 0;

function loadbang(){
	scales(numberOfSynthesisParameters);
//this.box.compile();
}
function scales(val){
	
	
	if(arguments.length){
		
		var a = arguments[0];
			// safety check for number of sliders
		if(a<0) a = 0; // too few scales, set to 0
		if(a>128) a = 128; // too many scales, set to 128		
		
		if(memory!= 0){
			post("memory" + memory + "\n");
			for(var i =0; i<memory; i++){
				this.patcher.remove(theScales[i]);
				this.patcher.remove(theOutlets[i]);
			}
		}
			
		// ...in with the new
		numScales = a;
		///if(numScales) ////
		ourself = this.patcher.getnamed("tester");
		for(var k= 0; k < a; k++)
		{
			theScales[k] = this.patcher.newdefault(300+(k*100), 50, "scale", 0.0, 255.0, 0.0, 5000.0);		
			theOutlets[k] = this.patcher.newdefault(300+(k*100), 100,  "outlet");
			if(numScales)
			{
				this.patcher.connect(ourself,k, theScales[k], 0);
				this.patcher.connect(theScales[k], 0, theOutlets[k], 0);
			}
		}	
		post("a: " + a + "\n");		
		memory=a;
			
		
		
	}
	else // complain about arguments
	{
		post("scales message needs arguments");
		post();
	}
		
}