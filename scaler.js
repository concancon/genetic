autowatch = 1;
inlets = 1;


var numScales= 20;
var theScales = new Array(128);
var theOutlets = new Array(128);
var myCycle;
var g= new Global("memory");
function loadbang(){
	post("NUMBER OF PARAMS" + g.numberOfParams + "\n");
	scales(g.numberOfParams);
//this.box.compile();
}

function list(val){
	
	var listLength = arguments.length;
	scales(listLength);
	
}
	
function regen(val){
	removeScales();
	g.numberOfParams = 0;
	scales(val);
	
		
				
}	

function removeScales(){
	
		if(g.numberOfParams != 0){
			post("number of params" + g.numberOfParams + "\n");
			for(var i =0; i< g.numberOfParams; i++){
				this.patcher.remove(this.patcher.getnamed("scale" + i));
				this.patcher.remove(this.patcher.getnamed("outlet" + i));
				
			}
			this.patcher.remove(this.patcher.getnamed("myCycle"));
		}
}

function scales(val){
	
	

		
     	if(val!= g.numberOfParams){
			// safety check for number of sliders
		if(val<0) val = 0; // too few scales, set to 0
		if(val>128) val = 128; // too many scales, set to 128		
	
		
		removeScales();
		// ...in with the new
		numScales = val;
		///if(numScales) ////
		ourself = this.patcher.getnamed("dynamicUnpack");	
		myCycle = this.patcher.newdefault(930, -330, "cycle", val);
		myCycle.varname= "myCycle"; 
		for(var k= 0; k < val; k++)
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

		post("a: " + val + "\n");		
		g.numberOfParams=val;
	}	
	
}