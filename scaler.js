autowatch = 1;
inlets = 1;


var numScales= 20;
var theScales = new Array(128);
var theOutlets = new Array(128);
var myCycle;

function loadbang(){
	post("NUMBER OF PARAMS" + g.numberOfParams + "\n");
	scales(g.numberOfParams);
//this.box.compile();
}

function list(val){
	
	var listLength = arguments.length;
	scales(listLength);
	
}
	


function scales(val){
	
	
	if(arguments.length){
		
		var a = arguments[0];
	if(val!= g.numberOfParams){
			// safety check for number of sliders
		if(a<0) a = 0; // too few scales, set to 0
		if(a>128) a = 128; // too many scales, set to 128		
	
		
		if(g.numberOfParams != 0){
			post("number of params" + g.numberOfParams + "\n");
			for(var i =0; i< g.numberOfParams; i++){
				this.patcher.remove(this.patcher.getnamed("scale" + i));
				this.patcher.remove(this.patcher.getnamed("outlet" + i));
				
			}
			this.patcher.remove(this.patcher.getnamed("myCycle"));
	
		}
		// ...in with the new
		numScales = a;
		///if(numScales) ////
		ourself = this.patcher.getnamed("dynamicUnpack");	
		myCycle = this.patcher.newdefault(930, -330, "cycle", a);
		myCycle.varname= "myCycle"; 
		for(var k= 0; k < a; k++)
		{
			theScales[k] = this.patcher.newdefault(300+(k*100), 50, "scale 0. 255. 0.5000.");		
		    theScales[k].varname= "scale" + k;				   
			theOutlets[k] = this.patcher.newdefault(300+(k*100), 100,  "outlet");
			theOutlets[k].varname= "outlet" + k;	
			patcher.parentpatcher.connect(patcher.parentpatcher.getnamed("parameterInterface"),k, patcher.parentpatcher.getnamed("synthGuess"), k +1);
			

			if(numScales)
			{
				this.patcher.connect(ourself,k, theScales[k], 0);
				this.patcher.connect(theScales[k], 0, theOutlets[k], 0);
				this.patcher.connect(patcher.getnamed("formantColl"), 0, myCycle, 0);			
				this.patcher.connect(myCycle, k, theScales[k], 4);
			
				
			}
		}	

		post("a: " + a + "\n");		
		g.numberOfParams=a;
	}	
	
 }
 else // complain about arguments
 {
   post("scales message needs arguments");
   post();
 }
}