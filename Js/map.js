document.getElementById("zoomIn-button").addEventListener("click", function() {
    console.log("Zoom In button clicked");
    zoom(1.1); // Increase scale by 10%
});
document.getElementById("zoomOut-button").addEventListener("click", function() {
    zoom(0.9); // Decrease scale by 10%
});

var originalWidth = null;
var originalHeight = null;

function zoom(scale) {
    var map = document.querySelector(".map");

    // Get the original dimensions if not set
    if (originalWidth === null || originalHeight === null) {
        originalWidth = map.offsetWidth;
        originalHeight = map.offsetHeight;
    }

    var currentWidth = map.offsetWidth;
    var currentHeight = map.offsetHeight;
    var newWidth = currentWidth * scale;
    var newHeight = currentHeight * scale;
    
    // Prevent zooming out if it goes back to original size
    if (scale < 1 && newWidth <= originalWidth && newHeight <= originalHeight) {
        return;
    }

    var deltaWidth = newWidth - currentWidth;
    var deltaHeight = newHeight - currentHeight;

    // Apply CSS transitions for smoother animation
    map.style.transition = "width 0.7s, height 0.7s, top 0.7s, left 0.7s";
    
    // Calculate new width and height
    map.style.width = newWidth + "px";
    map.style.height = newHeight + "px";

    // Calculate new position to keep the center fixed
    var offsetX = deltaWidth / 2;
    var offsetY = deltaHeight / 2;

    // Calculate new top and left positions
    var newTop = map.offsetTop - offsetY;
    var newLeft = map.offsetLeft - offsetX;

    // Apply new top and left positions
    map.style.top = newTop + "px";
    map.style.left = newLeft + "px";

    // Reset transitions after animation completes
    setTimeout(function() {
        map.style.transition = "none";
    }, 500);
}
