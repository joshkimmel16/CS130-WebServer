var viewer;
var memeData = {};
var img;
var topCaption;
var bottomCaption;
var id;

//main page ready event handler
$(document).ready(function(e) {
   
    viewer = $(this).find(".viewer");
    console.log(viewer);
    
    img = viewer.find("#memeImage");
    topCaption = viewer.find("#topCaption");
    bottomCaption = viewer.find("#bottomCaption");
    
    viewer.on("dataReady", populateData);
    
    //parse URL query string for ID
    id = parseUrl();
    
    //GET meme data based on ID
    getMemeData("/memes", id);
    
});

//parse URL to retrieve id from query string
function parseUrl () {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get('id');
}

//retrieve meme data from the server
function getMemeData (uri, id) {
    $.get((uri + "/" + id), function(data) {
        memeData = data;
        viewer.trigger("dataReady");
    });
}

//populate meme data on the form
function populateData () {
    img.attr("src", ("/image/memes/" + memeData.memeSelect));
    topCaption.text(memeData.topCaption);
    bottomCaption.text(memeData.bottomCaption);
}