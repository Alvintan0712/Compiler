function cgShowProcessLoading(myframeID) {
    var showmessageFrame = document.getElementById(myframeID);
    showmessageFrame = showmessageFrame.contentWindow || ( showmessageFrame.contentDocument.document || showmessageFrame.contentDocument);

    showmessageFrame.document.open();
    showmessageFrame.document.write("<img src='/images/loading2.gif'>");
    showmessageFrame.document.close();
}

function cgShowProcessCenterLoading(myframeID) {
    var showmessageFrame = document.getElementById(myframeID);
    showmessageFrame = showmessageFrame.contentWindow || ( showmessageFrame.contentDocument.document || showmessageFrame.contentDocument);

    showmessageFrame.document.open();
    showmessageFrame.document.write("<p style='text-align:center;'><img src='/images/loading2.gif'></p>");
    showmessageFrame.document.close();
}


function cgShowLoading(myframeID, tipMsg) {
    var showmessageFrame = document.getElementById(myframeID);
    showmessageFrame = showmessageFrame.contentWindow || ( showmessageFrame.contentDocument.document || showmessageFrame.contentDocument);

    showmessageFrame.document.open();
    showmessageFrame.document.write(tipMsg);
    showmessageFrame.document.close();
}
function cgShowLoadingImg(myframeID, tipMsg) {
    var showmessageFrame = document.getElementById(myframeID);
    showmessageFrame = showmessageFrame.contentWindow || ( showmessageFrame.contentDocument.document || showmessageFrame.contentDocument);

    showmessageFrame.document.open();
    showmessageFrame.document.write("<p>" + tipMsg + "</p><img src='/images/loading2.gif'>");
    showmessageFrame.document.close();
}