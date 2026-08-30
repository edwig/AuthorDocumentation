// AuthorDocumentation HTML
// Version 1.0
//
// Popup Support for Documentation in modern browsers
// Browsers must have at least DIV/IFRAME support

var arrayPopupURL 	   = new Array();
var arrayAbsPopupURL = new Array();
var arrayDirty 	   = new Array();
var gBsClientWidth	= 640;
var gBsClientHeight   = 480;
var gstrPopupID            = 'ADHPopup';
var gstrPopupShadowID = 'ADHPopupShadow';
var gstrPopupTopicID     = 'ADHPopupTopic';
var gstrPopupIFrameID  = 'ADHPopupIFrame';
var gstrPopupIFrameName = 'ADHPopupIFrameName';
var gbPopupTimeoutExpired = false;
var gBsStyVisHide	= null;

function getPopupID(nIndex)
{
	return gstrPopupID + nIndex;
}

function getPopupShadowID(nIndex)
{
	return gstrPopupShadowID + nIndex;
}

function getPopupTopicID(nIndex)
{
	return gstrPopupTopicID + nIndex;
}

function getPopupIFrameID(nIndex)
{
	return gstrPopupIFrameID + nIndex;
}

function getPopupIFrameName(nIndex)
{
	return gstrPopupIFrameName + nIndex;
}

function getPopupIFrame(nIndex)
{
	return eval("document.frames['" + getPopupIFrameName(nIndex) + "']");
}

function setAbsPopupURL(nIndex, strURL)
{
	arrayAbsPopupURL[nIndex] = strURL;
}

function getPopupURL(nIndex)
{
	if (nIndex == -1 || arrayPopupURL.length <= nIndex) 
	{
		return null;
	}
	else 
	{
		return arrayPopupURL[nIndex];
	}
}

function getPopupDivStyle(nIndex)
{
	return eval("document.all['" + getPopupID(nIndex) + "']").style;
}

function setDirty()
{
	var i = 0;
	for (i = 0; i < arrayPopupURL.length; i ++ )
	{
		arrayDirty[i] = true;
	}
}

function IsDirty(nIndex)
{
	if (nIndex == -1)
	{
		return true;
	}
	else 
		if (arrayDirty.length > nIndex) 
		{
			return arrayDirty[nIndex];
		}
		else
		{
			return true;
		}
	}
}

//Get client size info
function GetClientSize()
{
	gBsClientWidth = document.body.clientWidth;
	gBsClientHeight = document.body.clientHeight;
}

function	ADHSize(x, y)
{
	this.x = x;
	this.y = y;
}

function findDiv(strURL)
{
	var i = 0;
	for (i = 0; i < arrayPopupURL.length; i ++ ) 
	{
		if (arrayPopupURL[i] == strURL) 
		{
			return i;
		}
	}
	return -1;
}

function addDiv(strURL)
{
	var i = 0; 
	for (i = 0; i < arrayPopupURL.length; i ++) 
	{
		if (arrayPopupURL[i] == null) 
		{
			arrayPopupURL[i] = strURL;
			return i;
		}
	}	
	arrayPopupURL[i] = strURL;
	arrayDirty[i] = true;
	return i;
}


var gnToken = -1;
function takeToken()
{
	gnToken ++;
	if (gnToken > 10000) gnToken = 0;
	return gnToken;
}

function IsValidToken(nToken)
{
	return (gnToken == nToken);
}

function CreateDiv(nIndex)
{
	// DO NOT SET Width and height for the div, otherwize it will make IE4 popup do not work when view the topic alone.
	var strPopupDiv = "<DIV ID='" + getPopupID(nIndex)        + "' STYLE='position:absolute; top:-100; left:0; z-index:600; visibility:hidden;'>";
	strPopupDiv += "<DIV ID='" + getPopupShadowID(nIndex) + "' STYLE=\"position:absolute;top:0; left:0;  background-color:#C0C0C0;\"></DIV>";
	strPopupDiv += "<DIV ID='" + getPopupTopicID(nIndex)     + "' STYLE=\"position:absolute;top:0; left:0;  background-color:#FFFFFF;border:1px #000000 outset;\">";
	strPopupDiv += "<IFRAME title=\"Popup Window\" ID='" + getPopupIFrameID(nIndex) + "' name='" + 
                                        getPopupIFrameName(nIndex) + "' src = '" + getPopupURL(nIndex) + "' frameborder=0 scrolling=auto></IFRAME>";
	strPopupDiv += "</DIV></DIV>";

	var objBody = document.all.tags("BODY")[0];
	if( typeof(objBody) != "object" )
	{
		return;
	}
	objBody.insertAdjacentHTML("beforeEnd", strPopupDiv);
}

function CreatePopupDiv(strURL)
{
	var nIndex = findDiv(strURL);
	if (nIndex == -1 ) 
	{
		nIndex = addDiv(strURL);
		CreateDiv(nIndex);
	}
	else {
		if (IsDirty(nIndex)) 
		{
			if("object" == typeof(getPopupIFrame(nIndex).document))
			{
				getPopupIFrame(nIndex).document.location.href = strURL;
			}
		}
	}
	return nIndex;
}

function ADH_Popup(strURL, width, height)
{
	var re = new RegExp("'", 'g');
	strURL = strURL.replace(re, "%27");

	var cuswidth  = 0;
	var cusheight = 0;
	if ("undefined" != typeof(width) && "undefined" != typeof(height)) 
	{
		cuswidth = width;
		cusheight= height;
	}
	var nToken = takeToken(); // take  token first.
	var nIndex = CreatePopupDiv(strURL);
	window.gbPopupTimeoutExpired = false;
	var ntWidth = gBsClientWidth;
	var ntHeight = gBsClientHeight;
	GetClientSize();
	if (ntWidth != gBsClientWidth || ntHeight != gBsClientHeight) 
	{
		setDirty();
	}
	if (IsDirty(nIndex)) 
	{
		setTimeout("Popup_AfterLoad(" + nIndex + "," + nToken + "," + cuswidth + "," + cusheight + ")", 100);
	}
	else 
	{
		MoveDivAndShow(nIndex ,nToken, cuswidth, cusheight);
	}
}

// Legacy call (just to be sure)
function BSSCPopup(strURL, width, height)
{
	ADH_Popup(strULR,width,height);
}

function Popup_AfterLoad(nIndex, nToken, cuswidth, cusheight)
{	
	if (typeof(window.getPopupIFrame(nIndex).document) == "unknown") 
	{
		// Not a modern IFRAME supporting browser
		//_BSSCPopup2(getPopupURL(nIndex), cuswidth, cusheight);
		return;
	}
	if (!IsValidToken(nToken)) 
	{
		return;
	}
	if ((window.getPopupIFrame(nIndex).document.readyState == "complete") &&
		(window.getPopupIFrame(nIndex).document.body != null)) 
	{
		if (window.getPopupIFrame(nIndex).document.location.href.indexOf("about:blank") != -1) 
		{
			// add this check. IE will use about:blank" as the default vaule for Iframe.
			window.getPopupIFrame(nIndex).document.location = getPopupURL(nIndex);
			setTimeout("Popup_AfterLoad(" + nIndex + "," + nToken + "," + cuswidth + "," + cusheight + ")", 200);
		}
		else
		{
			setAbsPopupURL(nIndex, window.getPopupIFrame(nIndex).document.location.href); // change URL to abs url.
			Popup_ResizeAfterLoad(nIndex, nToken, cuswidth, cusheight);
		}
	} 
	else 
	{
		setTimeout("Popup_AfterLoad(" + nIndex + "," + nToken + "," + cuswidth + "," + cusheight + ")", 200);
	}
}

function MoveDivAndShow(nIndex, nToken, cuswidth, cusheight)
{
	if (window.getPopupIFrame(nIndex).document.location.href != getAbsPopupURL(nIndex)) 
	{ // if redirect, reload again.
		window.getPopupIFrame(nIndex).document.location = getPopupURL(nIndex);
		setTimeout("Popup_AfterLoad(" + nIndex + "," + nToken + "," + cuswidth + "," + cusheight + ")", 200);
		return;
	}

	// Determine the position of the window
	var nClickX = window.gnPopupClickX;
	var nClickY = window.gnPopupClickY;
	var nTop = 0;
	var nLeft = 0;

	var nWidth = window.getPopupDivStyle(nIndex).pixelWidth;
	var nHeight = window.getPopupDivStyle(nIndex).pixelHeight;

	if (nClickY + nHeight + 20 < gBsClientHeight + document.body.scrollTop) 
	{
		nTop = nClickY + 10;
	} 
	else 
	{
		nTop = (document.body.scrollTop + gBsClientHeight) - nHeight - 20;
	}
	if (nClickX + nWidth < gBsClientWidth + document.body.scrollLeft) 
	{
		nLeft = nClickX;
	}
	else 
	{
		nLeft = (document.body.scrollLeft + gBsClientWidth) - nWidth - 8;
	}
	
	if (nTop < document.body.scrollTop ) nTop  = document.body.scrollTop + 1;
	if (nLeft< document.body.scrollLeft) nLeft = document.body.scrollLeft + 1;


	window.getPopupDivStyle(nIndex).left = nLeft;
	window.getPopupDivStyle(nIndex).top = nTop;

	window.getPopupShadowStyle(nIndex).left = 4;
	window.getPopupShadowStyle(nIndex).top = 4;

	BSSCPopup_Timeout(nIndex , nToken );
	
	return;
}

function Popup_ResizeAfterLoad(nIndex, nToken, cuswidth, cusheight)
{
	if (window.gbPopupTimeoutExpired) 
	{
		return;
	}
	if (!IsValidToken(nToken)) 
	{
		return;
	}
	getPopupDivStyle(nIndex).visibility = gBsStyVisHide;

	GetClientSize();
	
	var size = new ADHSize(0, 0);

	if (cuswidth <= 0 || cusheight <= 0)
	{
		GetContentSize(window.getPopupIFrame(nIndex), size);
	}
	else 
	{
		size.x = cuswidth;
		size.y = cusheight;
	}

	// Determine the width and height for the window
	var nWidth = size.x;
	var nHeight = size.y;

	// for small popup size, we should allow any size.
	// The popup size should be ok if bigger than 0
	if (nWidth < 0 || nHeight < 0) 
	{
		return; 	// there must be something terribly wrong.		
	}
	window.getPopupDivStyle(nIndex).pixelWidth = nWidth;
	window.getPopupDivStyle(nIndex).pixelHeight = nHeight;
	window.getPopupShadowStyle(nIndex).pixelWidth = nWidth + 2;
	window.getPopupShadowStyle(nIndex).pixelHeight = nHeight + 2;
	window.getPopupTopicStyle(nIndex).pixelWidth = nWidth + 2;
	window.getPopupTopicStyle(nIndex).pixelHeight = nHeight + 2;
	window.getPopupIFrameStyle(nIndex).pixelWidth = nWidth;
	window.getPopupIFrameStyle(nIndex).pixelHeight = nHeight;
	window.getPopupIFrameStyle(nIndex).top = 0;
	window.getPopupIFrameStyle(nIndex).left = 0;
	
	var strURL = getPopupURL(nIndex);
	if (strURL.indexOf("#") != -1)
	{
		getPopupIFrame(nIndex).location.href = strURL;  // reload again, this will fix the bookmark misunderstand in IE5.
	}
	MoveDivAndShow(nIndex, nToken, cuswidth, cusheight);
}

function GetContentSize(thisWindow, size)
{
	// Resize the width until it is wide enough to handle the content
	// The trick is to start wide and determine when the scrollHeight changes
	// because then we know a scrollbar is necessary. We can then go back
	// to the next widest size (for no scrollbar)

	var ClientRate = gBsClientHeight / gBsClientWidth;

	var GoldenSize = new ADHSize(0,0);
	GoldenSize.x = gBsClientWidth * gBMaxXOfParent;
	GoldenSize.y = gBsClientHeight *gBMaxYOfParent ;

	if (ClientRate > gBRateH_W) 
	{
		GoldenSize.y = GoldenSize.x * gBRateH_W;
	}
	else {
		GoldenSize.x = GoldenSize.y / gBRateH_W;
	}

	// Try to using parent specified max x.
	var x = 0;
	var maxgoldx = GoldenSize.x;
	var maxx = gBsClientWidth * gBMaxXOfParent;
	
	// This double resize causes the document to re-render (and we need it to)
	//if (!gbBsIE5)
	//{
	//	thisWindow.moveTo(10000,10000); // this is used to fix the flash on IE4.
	//}
	thisWindow.resizeTo(1, 1);
	thisWindow.resizeTo(1, 1);
	thisWindow.resizeTo(maxgoldx, thisWindow.document.body.scrollHeight + gBscrollHeight);
	thisWindow.resizeTo(maxgoldx, thisWindow.document.body.scrollHeight + gBscrollHeight);
		
	var miny = thisWindow.document.body.scrollHeight + gBscrollHeight;
	
	if (miny > GoldenSize.y) // the popup does not fix in the parent wanted golden area. so try to expand itself as large as it can
	{
		thisWindow.resizeTo(maxx , thisWindow.document.body.scrollHeight + gBscrollHeight);
		thisWindow.resizeTo(maxx , thisWindow.document.body.scrollHeight + gBscrollHeight);
		
		miny = 	thisWindow.document.body.scrollHeight + gBscrollHeight;
		maxy = gBsClientHeight * gBMaxYOfParent;
		
		if (miny > maxy) { // the popup must have a scroll, OK let it be.
			miny = maxy;
			size.x = maxx;
			size.y = maxy;
			thisWindow.document.body.scroll = 'yes'; // At this time we do want to show scroll any more. so it will looks better a little.
		}
		else { // popup still can fit in the parent area by someway. now we choose the same h/w rate as parent.
			size.y = miny;
			
			//  downsize from maxx , now I try to using binary divide.
			x = maxx;
			deltax = -maxx/2;
			//j = 0;
			while (true) {
				x = x + deltax;
				thisWindow.resizeTo(x, miny);
				thisWindow.resizeTo(x, miny);
				diffy = thisWindow.document.body.scrollHeight + gBscrollHeight - x * ClientRate;
				if (diffy >  gBpermitYDelta ) // it is higher than wanted, so x need to be wide a little bitter
					deltax = Math.abs(deltax) /2;
				else if (diffy <  -gBpermitYDelta) // it is shorter than wanted, so x need to be narrow a little bitter
					deltax = -Math.abs(deltax) /2;
				else 
					// the y is close enough to wanted.
					break;
				if (Math.abs(deltax) < gBpermitXDelta) // the next change is too slight and it can be ignore.
					break;
			}
			size.x = thisWindow.document.body.scrollWidth; //+ gBscrollWidth;
			size.y = thisWindow.document.body.scrollHeight;// + gBscrollHeight;	
			thisWindow.document.body.scroll = 'no';
			
		// Handle absurd cases just in case IE flakes
	//		if (size.y < 100) {
	//			size.y = 100;
	//		}
		}
	}
	else 
	{
		if (thisWindow.document.body.scrollWidth > maxgoldx) 
		{
			size.x = maxx; 
			size.y = miny;	
			thisWindow.document.body.scroll = 'yes';
		}
		else 
		{
			//  downsize from maxgoldx , now I try to using binary divide.
			x = maxgoldx;
			deltax = -maxgoldx/2;
			//i = 0;
			while (true) 
			{
				x = x + deltax;
				thisWindow.resizeTo(x, miny);
				thisWindow.resizeTo(x, miny);
				diffy = thisWindow.document.body.scrollHeight + gBscrollHeight - x * gBRateH_W;
				if (diffy >  gBpermitYDelta ) // it is higher than wanted, so x need to be wide a little bitter
					deltax = Math.abs(deltax) /2;
				else if (diffy <  -gBpermitYDelta) // it is shorter than wanted, so x need to be narrow a little bitter
					deltax = -Math.abs(deltax) /2;
				else 
					// the y is close enough to wanted.
					break;
				if (Math.abs(deltax) < gBpermitXDelta) // the next change is too slight and it can be ignore.
					break;
				//i ++;
				
			}
			//size.x = x - gBscrollWidth;
			size.x = thisWindow.document.body.scrollWidth; //+ gBscrollWidth;
			size.y = thisWindow.document.body.scrollHeight ;
			thisWindow.document.body.scroll = 'no'; // At this time we do not want to show scroll any more. so it will looks better a little.
			thisWindow.resizeTo(size.x, size.y);
			if (thisWindow.document.body.scrollWidth > size.x)
			{
				size.x = thisWindow.document.body.scrollWidth;
			}
			if (thisWindow.document.body.scrollHeight > size.y)
			{
				size.y = thisWindow.document.body.scrollHeight;
			}
		}
	}
	// no reload no scrollbar.
	//size.x = size.x + 16;	 //reserve a width for scrollbar (IE 4.0 only)
	thisWindow.resizeTo(size.x, size.y);
	thisWindow.resizeTo(size.x, size.y);
	return;
}
