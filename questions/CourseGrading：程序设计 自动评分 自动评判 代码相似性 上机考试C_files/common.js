function spread(divObject,imageobject){  
 
   if(document.getElementById(divObject).style.display == 'inline'){
   		
		document.getElementById(divObject).style.display='none';
		document.getElementById(imageobject).src="/images/expandall.gif";
		
   }else{   		
		document.getElementById(divObject).style.display='inline';
		document.getElementById(imageobject).src="/images/collapseall.gif";
   }
}

function displayObject(divObject){  
 
   if(document.getElementById(divObject).style.display == 'inline'){
   		
		document.getElementById(divObject).style.display='none';		
		
   }else{   		
		document.getElementById(divObject).style.display='inline';		
   }
}


function displayObjects(divObject1, divObject2){  
 
   if(document.getElementById(divObject1).style.display == 'inline'){
   		
		document.getElementById(divObject1).style.display='none';
		document.getElementById(divObject2).style.display='none';			
		
   }else{   		
		document.getElementById(divObject1).style.display='inline';	
		document.getElementById(divObject2).style.display='inline';		
   }
}

function displayExclusiveObjects(divObject1, divObject2){  
 
   if(document.getElementById(divObject1).style.display == 'inline'){
   		
		document.getElementById(divObject1).style.display='none';
		document.getElementById(divObject2).style.display='inline';			
		
   }else{   		
		document.getElementById(divObject1).style.display='inline';	
		document.getElementById(divObject2).style.display='none';		
   }
}

function showDetail(object,imageobject,omit){  
 
   if(document.getElementById(object).style.height == 'auto'){
   		document.getElementById(object).style.height='80 px';
		document.getElementById(omit).style.display='inline';
		document.getElementById(imageobject).src="/images/expandall.gif";
		
   }else{
   		document.getElementById(object).style.height='auto';
		document.getElementById(omit).style.display='none';
		document.getElementById(imageobject).src="/images/collapseall.gif";
   }
}

function isIeLessThan9() {
    var div = document.createElement("div");
    div.innerHTML = "<!--[if lt IE 9]><i></i><![endif]-->";
    var lessThan9 = (div.getElementsByTagName("i").length == 1);
    
    //if (lessThan9) {
      //  alert("WE DON'T LIKE YOUR BROWSER");
    //}
    return lessThan9;
}

// JavaScript Document
function isEmail(s)
{
     var r,re;
     re = "^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)$";
     r = s.match(re);
     return (r == s) ? true : false;
}

function isAlbum(s)
{
     var r,re;
     re = "^http://([\w-]+\.)+[\w-]+(/[\w-./?%&=]*)?s$";
     r = s.match(re);
     return (r == s) ? true : false;
}



function LTrim(str) 

{ 

var whitespace = new String(" \t\n\r"); 

var s = new String(str); 

if (whitespace.indexOf(s.charAt(0)) != -1) 

{ 

var j=0, i = s.length; 

while (j < i && whitespace.indexOf(s.charAt(j)) != -1) 

{ 

j++; 

} 

s = s.substring(j, i); 

} 

return s; 

} 



function RTrim(str) 

{ 

var whitespace = new String(" \t\n\r"); 

var s = new String(str); 

if (whitespace.indexOf(s.charAt(s.length-1)) != -1) 

{ 

var i = s.length - 1; 

while (i >= 0 && whitespace.indexOf(s.charAt(i)) != -1) 

{ 

i--; 

} 

s = s.substring(0, i+1); 

} 

return s; 

} 



function Trim(str) 

{ 

return RTrim(LTrim(str)); 

} 



function XMLEncode(str) 

{ 

str=Trim(str); 

str=str.replace("&","&amp;"); 

str=str.replace("<","&lt;"); 

str=str.replace(">","&gt;"); 

str=str.replace("'","&apos;"); 

str=str.replace("\"","&quot;"); 

return str; 

} 

function CGHasExtension(filename, exts) {
    return (new RegExp('(' + exts.join('|').replace(/\./g, '\\.') + ')$')).test(filename);
}

function IsEmpty(obj) 

{ 

    obj=document.getElementsByName(obj).item(0); 

    if(Trim(obj.value)=="") 
    {
        if(obj.disabled==false && obj.readOnly==false) 
        {
            obj.focus(); 
        } 
    } 
} 

function IsInt(objStr,sign,zero) 
{ 

    var reg; 

    var bolzero; 

    if(Trim(objStr)=="") 
    {
        return false; 
    } else { 
        objStr=objStr.toString(); 
    } 

    if((sign==null)||(Trim(sign)=="")) 
    { 
        sign="+-"; 
    } 

    if((zero==null)||(Trim(zero)=="")) 
    { 
        bolzero=false; 
    } else { 

        zero=zero.toString(); 

        if(zero=="0") 
        {
            bolzero=true; 

        }
    } 

    switch(sign) 
    { 

    case "+-": 

        reg=/(^-?|^\+?)\d+$/; 

        break; 

    case "+": 

        if(!bolzero) 
        { 

            reg=/^\+?[0-9]*[1-9][0-9]*$/; 

        }  else { 

            reg=/^\+?[0-9]*[0-9][0-9]*$/; 

        } 

        break; 

    case "-": 

        if(!bolzero) 
        { 

            reg=/^-[0-9]*[1-9][0-9]*$/; 

        } else { 

            reg=/^-[0-9]*[0-9][0-9]*$/; 

        } 

        break; 

    default: 

        return false; 

        break; 
    } 

    var r=objStr.match(reg); 

    if(r==null) 
    { 
        return false; 
    } else { 

        return true; 

    } 

} 



function IsFloat(objStr,sign,zero) 
{ 
    var reg; 

    var bolzero; 

    if(Trim(objStr)=="") 
    { 
        return false; 
    } else { 
        objStr=objStr.toString(); 
    } 

    if((sign==null)||(Trim(sign)=="")) 
    {
        sign="+-"; 
    } 

    if((zero==null)||(Trim(zero)=="")) 
    { 

        bolzero=false; 
    } else { 

        zero=zero.toString(); 

        if(zero=="0") 
        { 
            bolzero=true; 
        } 
    } 

    switch(sign) 
    { 
    case "+-": 

        reg=/^((-?|\+?)\d+)(\.\d+)?$/; 
        break; 

    case "+": 

        if(!bolzero) 
        { 
            reg=/^\+?(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$/; 
        } else { 
            reg=/^\+?\d+(\.\d+)?$/; 
        } 

        break; 

    case "-": 

        if(!bolzero) 
        { 
            reg=/^-(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$/; 
        } else { 
            reg=/^((-\d+(\.\d+)?)|(0+(\.0+)?))$/; 
        } 
        break; 

    default: 

        return false; 

        break; 
    } 

    var r=objStr.match(reg); 

    if(r==null) 
    { 
        return false; 
    } else { 
        return true; 
    } 

} 
