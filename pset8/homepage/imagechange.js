window.onload = function()
{
    var images = ['flats.jpg', 'lights.jpg'];
    var len = images.length;
    let image = document.getElementById("background");
    let i = 0;
    image.src = images[i];

    document.querySelector("#bb").onclick = function()
    {
        if (i == 0)
        {
            i++;
            image.src = images[i];
        }
        else if(i == len-1)
        {
            i = 0;
            image.src = images[i];
        }
    }
}