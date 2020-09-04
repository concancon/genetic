{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 1,
			"revision" : 5,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 27.0, 56.0, 748.0, 810.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"id" : "obj-29",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 73.0, 17.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 73.0, 146.0, 115.0, 22.0 ],
					"text" : "send paramNumber"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "int" ],
					"patching_rect" : [ 407.0, 112.0, 29.5, 22.0 ],
					"text" : "t b i"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-36",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 407.0, 246.0, 160.0, 22.0 ],
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-35",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 407.0, 184.0, 31.0, 22.0 ],
					"text" : "sig~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-34",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 407.0, 208.0, 77.0, 22.0 ],
					"text" : "mstosamps~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-33",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 407.0, 153.0, 29.5, 22.0 ],
					"text" : "+ 0."
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 302.0, 17.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "number[3]",
							"parameter_shortname" : "number[3]",
							"parameter_type" : 3
						}

					}
,
					"varname" : "number"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-27",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 1,
					"patching_rect" : [ 228.0, 17.0, 50.0, 22.0 ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "number[4]",
							"parameter_shortname" : "number[4]",
							"parameter_type" : 3
						}

					}
,
					"varname" : "number[1]"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-24",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 302.0, 146.0, 50.0, 22.0 ],
					"text" : "s decay"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 228.0, 146.0, 50.0, 22.0 ],
					"text" : "s attack"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 403.0, 400.0, 78.0, 22.0 ],
					"text" : "soundLength"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-22",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 1.0, 578.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 17.0, 557.0, 48.0, 22.0 ],
					"text" : "del 100"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 54.5, 604.0, 79.0, 22.0 ],
					"text" : "send getNext"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"linecount" : 3,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 1017.0, 419.0, 150.0, 47.0 ],
					"text" : "4 slices for the fitness function. there we build the average fitness"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-14",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 271.0, 855.0, 150.0, 34.0 ],
					"text" : "flocoma.org\n"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"linecount" : 4,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 271.0, 785.0, 153.0, 60.0 ],
					"text" : "what are adequate step sizes for measuring an average fitness of a sound with an envelope?"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"linecount" : 5,
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 342.5, 670.0, 81.5, 76.0 ],
					"text" : "197 146 70 199 33 170 111 155 86 84 77 62 80 83 139 135"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-41",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 250.5, 670.0, 90.0, 22.0 ],
					"text" : "send bestValue"
				}

			}
, 			{
				"box" : 				{
					"fontname" : "Andale Mono",
					"fontsize" : 14.0,
					"id" : "obj-55",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 94.0, 288.0, 236.0, 22.0 ],
					"text" : "just press me"
				}

			}
, 			{
				"box" : 				{
					"bgcolor" : [ 0.443137254901961, 0.607843137254902, 0.380392156862745, 1.0 ],
					"id" : "obj-52",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 17.0, 260.0, 68.0, 68.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-50",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 30.0, 446.0, 33.0, 22.0 ],
					"text" : "s init"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-47",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 17.0, 515.0, 48.0, 22.0 ],
					"text" : "del 50"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "bang" ],
					"patching_rect" : [ 17.0, 395.999997615814209, 32.0, 22.0 ],
					"text" : "t b b"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-45",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 403.0, 364.0, 53.0, 22.0 ],
					"text" : "readDict"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 0,
					"patching_rect" : [ 403.0, 333.0, 107.0, 22.0 ],
					"text" : "spectrumCompare",
					"varname" : "spectrumCompare"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"maxclass" : "newobj",
					"numinlets" : 0,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 204.0, 564.0, 119.0, 22.0 ],
					"text" : "receive fitnessResult"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-49",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 166.0, 670.0, 79.0, 22.0 ],
					"text" : "send popDict"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-18",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 73.0, 557.0, 107.0, 22.0 ],
					"text" : "buildPopulation 16"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "dict.view",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ -1.0, 718.0, 190.0, 160.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 82.0, 446.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 166.0, 511.0, 87.0, 22.0 ],
					"text" : "prepend target"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-20",
					"maxclass" : "toggle",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 166.0, 365.999997615814209, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 166.0, 399.749994397163391, 63.0, 22.0 ],
					"text" : "qmetro 20"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "dictionary", "", "" ],
					"patching_rect" : [ 166.0, 614.0, 70.0, 22.0 ],
					"text" : "min.genetic"
				}

			}
, 			{
				"box" : 				{
					"attr" : "expFactor",
					"id" : "obj-8",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 417.0, 487.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "mutationIndex",
					"id" : "obj-7",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 417.0, 511.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "mutationRate",
					"id" : "obj-19",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 265.0, 511.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 61.0, 511.0, 103.0, 22.0 ],
					"text" : "getMaxFitness"
				}

			}
, 			{
				"box" : 				{
					"attr" : "maxPopulation",
					"id" : "obj-23",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 264.0, 487.0, 150.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"attr" : "accuracy",
					"id" : "obj-3",
					"maxclass" : "attrui",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 570.0, 511.0, 150.0, 22.0 ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 1 ],
					"midpoints" : [ 226.5, 643.0, 414.5, 643.0 ],
					"source" : [ "obj-1", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-41", 0 ],
					"midpoints" : [ 201.0, 652.5, 260.0, 652.5 ],
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"midpoints" : [ 175.5, 656.5, 8.5, 656.5 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-49", 0 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-50", 0 ],
					"source" : [ "obj-11", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 82.5, 575.0, 123.0, 575.0, 123.0, 600.0, 175.5, 600.0 ],
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 274.5, 552.575163543224335, 175.5, 552.575163543224335 ],
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 273.5, 552.08730161190033, 175.5, 552.08730161190033 ],
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 1 ],
					"source" : [ "obj-25", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"order" : 1,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-33", 0 ],
					"order" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-24", 0 ],
					"order" : 1,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 0 ],
					"order" : 0,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"order" : 1,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"midpoints" : [ 82.5, 335.0, 82.5, 335.0 ],
					"order" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 579.5, 552.0, 175.5, 552.0 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 70.5, 544.228758335113525, 175.5, 544.228758335113525 ],
					"source" : [ "obj-30", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-35", 0 ],
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-34", 0 ],
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 213.5, 599.5, 175.5, 599.5 ],
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"order" : 0,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"order" : 1,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 91.5, 507.5, 175.5, 507.5 ],
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 426.5, 552.614379167556763, 175.5, 552.614379167556763 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"midpoints" : [ 426.5, 551.05820107460022, 175.5, 551.05820107460022 ],
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-21::obj-16" : [ "live.gain~", "live.gain~", 0 ],
			"obj-21::obj-90::obj-11" : [ "number[2]", "number[2]", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-20" : [ "Freq", "Freq", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-22" : [ "TimeMode", "TimeMode", 1 ],
			"obj-21::obj-90::obj-82::obj-15::obj-23" : [ "CV2", "CV2", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-28" : [ "Res", "Res", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-4" : [ "Offset", "Offset", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-54" : [ "CV1", "CV1", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-55" : [ "power", "power", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-63" : [ "CV3", "CV3", 0 ],
			"obj-21::obj-90::obj-82::obj-15::obj-95" : [ "ResCV", "CV", 0 ],
			"obj-21::obj-90::obj-95" : [ "number", "number", 0 ],
			"obj-21::obj-90::obj-96" : [ "number[1]", "number[1]", 0 ],
			"obj-21::obj-97::obj-39" : [ "number[12]", "number[1]", 0 ],
			"obj-21::obj-97::obj-41" : [ "number[11]", "number[2]", 0 ],
			"obj-21::obj-9::obj-26" : [ "number[7]", "number", 0 ],
			"obj-21::obj-9::obj-39" : [ "number[6]", "number[1]", 0 ],
			"obj-21::obj-9::obj-41" : [ "number[5]", "number[2]", 0 ],
			"obj-27" : [ "number[4]", "number[4]", 0 ],
			"obj-28" : [ "number[3]", "number[3]", 0 ],
			"parameterbanks" : 			{

			}
,
			"parameter_overrides" : 			{
				"obj-21::obj-16" : 				{
					"parameter_longname" : "live.gain~"
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "genetic_testMatrix[2].maxsnap",
				"bootpath" : "~/Documents/Max 8/Snapshots",
				"patcherrelativepath" : "../../../../../Snapshots",
				"type" : "mx@s",
				"implicit" : 1
			}
, 			{
				"name" : "spectrumCompare.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "zsa.easy_mfcc~.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/zsa.descriptors/misc",
				"patcherrelativepath" : "../../../../zsa.descriptors/misc",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "zsa.abs_mfcc~.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/zsa.descriptors/misc",
				"patcherrelativepath" : "../../../../zsa.descriptors/misc",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "buildDict.js",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "fcompare~.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "bassoonA3.wav",
				"bootpath" : "~/Documents/Max 8/Packages/dada/media",
				"patcherrelativepath" : "../../../../dada/media",
				"type" : "WAVE",
				"implicit" : 1
			}
, 			{
				"name" : "bp.Ladder.maxpat",
				"bootpath" : "C74:/packages/BEAP/clippings/BEAP/Filter",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "dynamicUnpack.js",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "scaler.js",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "readDict.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "readDict.js",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "soundLength.maxpat",
				"bootpath" : "~/Documents/Max 8/Packages/min-devkit/source/projects/min.genetic",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "min.genetic.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "zsa.mfcc~.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "fiddle~.mxo",
				"type" : "iLaX"
			}
 ],
		"autosave" : 0,
		"snapshot" : 		{
			"filetype" : "C74Snapshot",
			"version" : 2,
			"minorversion" : 0,
			"name" : "snapshotlist",
			"origin" : "jpatcher",
			"type" : "list",
			"subtype" : "Undefined",
			"embed" : 1,
			"snapshot" : 			{
				"valuedictionary" : 				{
					"parameter_values" : 					{
						"CV1" : 0.0,
						"CV1[1]" : 0.0,
						"CV2" : 100.0,
						"CV2[1]" : 100.0,
						"CV3" : 0.0,
						"CV3[1]" : 0.0,
						"Freq" : 6766.246564310111353,
						"Freq[1]" : 6766.246564310111353,
						"Offset" : 0.0,
						"Offset[1]" : 0.0,
						"Res" : 0.0,
						"ResCV" : 100.0,
						"ResCV[1]" : 100.0,
						"Res[1]" : 0.0,
						"live.gain~" : -6.0,
						"power" : 0.0,
						"power[1]" : 0.0,
						"TimeMode" : 1.0,
						"TimeMode[1]" : 1.0,
						"blob" : 						{
							"number" : [ 97 ],
							"number[1]" : [ 10 ],
							"number[2]" : [ 200.0 ],
							"number[3]" : [ 168 ],
							"number[4]" : [ 200 ]
						}

					}

				}

			}
,
			"snapshotlist" : 			{
				"current_snapshot" : 0,
				"entries" : [ 					{
						"filetype" : "C74Snapshot",
						"version" : 2,
						"minorversion" : 0,
						"name" : "genetic_testMatrix[2]",
						"origin" : "genetic_testMatrix",
						"type" : "patcher",
						"subtype" : "Undefined",
						"embed" : 0,
						"snapshot" : 						{
							"valuedictionary" : 							{
								"parameter_values" : 								{
									"CV1" : 0.0,
									"CV1[1]" : 0.0,
									"CV2" : 100.0,
									"CV2[1]" : 100.0,
									"CV3" : 0.0,
									"CV3[1]" : 0.0,
									"Freq" : 6766.246564310111353,
									"Freq[1]" : 6766.246564310111353,
									"Offset" : 0.0,
									"Offset[1]" : 0.0,
									"Res" : 0.0,
									"ResCV" : 100.0,
									"ResCV[1]" : 100.0,
									"Res[1]" : 0.0,
									"live.gain~" : -6.0,
									"power" : 0.0,
									"power[1]" : 0.0,
									"TimeMode" : 1.0,
									"TimeMode[1]" : 1.0,
									"blob" : 									{
										"number" : [ 97 ],
										"number[1]" : [ 10 ],
										"number[2]" : [ 200.0 ],
										"number[3]" : [ 168 ],
										"number[4]" : [ 200 ]
									}

								}

							}

						}
,
						"fileref" : 						{
							"name" : "genetic_testMatrix[2]",
							"filename" : "genetic_testMatrix[2].maxsnap",
							"filepath" : "~/Documents/Max 8/Snapshots",
							"filepos" : -1,
							"snapshotfileid" : "cf23c51a456564072a3fa0b479d60fa1"
						}

					}
 ]
			}

		}

	}

}
