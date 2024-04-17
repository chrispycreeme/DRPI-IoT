document.getElementById("zoomIn-button").addEventListener("click", function() {
    console.log("Zoom In button clicked");
    zoom(1.1); // Increase scale by 10%
});
document.getElementById("zoomOut-button").addEventListener("click", function() {
    zoom(0.9); // Decrease scale by 10%
});

function zoom(scale) {
    var map = document.querySelector(".map");
    var currentWidth = map.offsetWidth;
    var currentHeight = map.offsetHeight;
    var newWidth = currentWidth * scale;
    var newHeight = currentHeight * scale;
    var deltaWidth = newWidth - currentWidth;
    var deltaHeight = newHeight - currentHeight;

    // Calculate new width and height
    map.style.width = newWidth + "px";
    map.style.height = newHeight + "px";

    // Calculate new position to keep the center fixed
    var offsetX = (deltaWidth / 2);
    var offsetY = (deltaHeight / 2);

    // Calculate new top and left positions
    var newTop = map.offsetTop - offsetY;
    var newLeft = map.offsetLeft - offsetX;

    // Apply new top and left positions
    map.style.top = newTop + "px";
    map.style.left = newLeft + "px";
}