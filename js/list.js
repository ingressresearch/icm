function search(list,searchBox) {
	listFixIEPrepare(list);
	list.selectedIndex=-1;
	if (searchBox.value.length < 1) {
		return;
	}
	var lastFound = -1;
	listFixIEStart(list);
	for (i = 0; i < list.length; i++) {
		if (list.options[i].text.toLowerCase().indexOf(searchBox.value.toLowerCase()) >= 0) {
			list.options[i].selected=true;
			lastFound = i;
		}
		else {
			list.options[i].selected=false;
		}
	}
	listFixIEEnd(list,lastFound);
}

function clearSearch(searchBox) {
	if(searchBox.value=="Search:" || searchBox.value=="search title") { 
   		searchBox.value=""; 
   	} 
}

function listFixIEPrepare(list) {
	if (!window.searchTimeoutIdx) {
	        window.searchTimeoutIdx = {};
	}
	if (window.searchTimeoutIdx[list.id]) {
	        clearTimeout(window.searchTimeoutIdx[list.id]);
	        searchTimeoutIdx[list.id] = null;
	}
}

function listFixIEStart(list) {
	list.style.visibility="hidden";
	list.disabled = true;
}

function listFixIEEnd(list,lastFound) {
	list.disabled = false;
	list.style.visibility="visible";
	if (lastFound >= 0) {
	        window.searchTimeoutIdx[list.id] = setTimeout("document.getElementById('"+list.id+"').options["+lastFound+"].selected = true",1);
	} else {
		window.searchTimeoutIdx[list.id] = setTimeout("document.getElementById('"+list.id+"').options[0].selected = true;document.getElementById('"+list.id+"').options[0].selected = false",1);
	}
}

function move(fromName,toName) {
	var fromList = document.getElementById(fromName);
	var toList   = document.getElementById(toName);

  	if(fromList.selectedIndex==-1) return;
   	if(fromList.options[fromList.selectedIndex].value=='-1') return;
  	
  	if(toList.length==1&&toList.options[0].value==-1) {
  		clearList(toList);
  	}
  
	var moveOption = fromList.options[fromList.selectedIndex];

     	var selectedOption = null; 
    
   	var i=0;
	while (i<fromList.length) {
  		if ((!fromList.options[i].selected) || (fromList.options[i].value=='')) {
			i++;
		} else {
			var moveOption = fromList.options[i];
	
	       
			try {
				toList.add(moveOption,selectedOption); // standards compliant; doesn't work in IE
			}
			catch(ex) {
				var tempOption = document.createElement('option');
				tempOption.value = moveOption.value;
				tempOption.text = moveOption.text;
				fromList.options[i] = null;
				toList.add(tempOption); // IE only
				toList.options[toList.options.length-1].selected=true;	
			}
		}
   	}
}

function remove(fromName,toName) {
	var fromList = document.getElementById(fromName);
	var toList   = document.getElementById(toName);

	if (fromList.selectedIndex<0) return;
   	if(fromList.options[fromList.selectedIndex].value=='-1') return;
   	if(fromList.options[fromList.selectedIndex].value=='') return;
  
   	var i=0;
	while (i<fromList.length) {
  		if ((!fromList.options[i].selected) || (fromList.options[i].value=='')) {
			i++;
		} else {
			var moveOption = fromList.options[i];
	
			try {
				toList.add(moveOption,null); // standards compliant; doesn't work in IE
			}
			catch(ex) {
				var tempOption = document.createElement('option');
				tempOption.value = moveOption.value;
				tempOption.text = moveOption.text;
	
				fromList.options[i] = null;
				toList.add(tempOption); // IE only
				toList.options[toList.options.length-1].selected=true;
			}
	  
			/*if(fromList.length==0) {
				var newOption = document.createElement('option');
			newOption.text = 'Select from the list on the left';
				 newOption.value = '-1';
	  
				try {
				fromList.add(newOption, null); // standards compliant; doesn't work in IE
			}
			catch(ex) {
				fromList.add(newOption); // IE only
			}
			}*/
		}
	}
}

function isInList(value, toListCopy) {
	var temp = -1;
   	var i=0;
	while ((i<toListCopy.length) && (temp==-1)) {
  		if (toListCopy[i]==value) {
			temp = i;
		}
		i++;
	}
	
	return temp;
}

function buildList(movedElements, toList, fieldidname, fieldtextname) {
	if (!(movedElements instanceof Array)) {
		movedElements = [movedElements];
	}
	
	var fieldid = eval(fieldidname);
	if (!fieldid) {
		fieldid = fieldIDOrder;
	}
	var fieldtext = eval(fieldtextname);
	if (!fieldtext) {
		fieldtext = fieldTextOrder;
	}
	if (fieldid) {
		// We make a copy of the listbox values first
		var toListValueCopy = Array();
		var toListTextCopy = Array();
		var i = 0;
		while (i<toList.length) {
			toListValueCopy[i] = toList.options[i].value;
			toListTextCopy[i] = toList.options[i].text;
			i++;
		}
		toList.options.length = 0;
		
		var i = 0;
		
		while (i < fieldid.length) {
			if (fieldid[i] == "") {
				// Probably a category so we always copy it.
				var tempOption = document.createElement('option');
				tempOption.value = fieldid[i];
				tempOption.text = fieldtext[i];
				try {
					toList.add(tempOption,null); // standards compliant; doesn't work in IE
				}
				catch(ex) {
					toList.add(tempOption); // IE only
				}
			}
			else {
				// Check if the element exists in the destination listbox or if it's the element we're moving
				var inList = isInList(fieldid[i], toListValueCopy);
				if (inList >= 0) {
					var tempOption = document.createElement('option');
					tempOption.value = toListValueCopy[inList];
					tempOption.text = toListTextCopy[inList];
					try {
						toList.add(tempOption, null);	//Standards compliant; doesn't work in IE
					}
					catch(ex) {
						toList.add(tempOption);	//IE only
					}
				}
				else if (movedElements.length > 0) {
					for (var j = 0; j < movedElements.length; j++) {
						if (fieldid[i] == movedElements[j].value) {
							var movedElement = movedElements.splice(j, 1);
							var tempOption = document.createElement('option');
							tempOption.value = movedElement[0].value;
							tempOption.text = movedElement[0].text;
							try {
								toList.add(tempOption, null);	//Standards compliant; doesn't work in IE
							}
							catch(ex) {
								toList.add(tempOption);	//IE only
							}
							break;
						}
					}
				}
			}
			i++;
		}
	}
}

function clearList(fromName, toName) {
	var fromList = document.getElementById(fromName);
	var toList = document.getElementById(toName);
	selectAllElementsInList(fromList);
	remove(fromName, toName);
	fromList.options.length=0;
}

function selectAllElementsInList(list) {
   	for(i=list.options.length-1;i>=0; i--) {
		list.options[i].selected=true;
	}
}

function unselectAllElementsInList(list) {
   	for(i=list.options.length-1;i>=0; i--) {
		list.options[i].selected=false;
	}
}
