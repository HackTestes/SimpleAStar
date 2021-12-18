// URL: https://developer.mozilla.org/en-US/docs/Web/API/File_System_Access_API

// store a reference to our file handle
// Read from uploaded file
function ReadFile(file_list)
{
    const reader = new FileReader();

    //console.log(file_list);
    reader.readAsText(file_list[0]);

    // What to do after the json being loaded
    reader.onload = () =>
    {
        console.log( reader.result );
        global_json = JSON.parse(reader.result);
        callMain(["-R", reader.result, "-j"]);
    };
}

let upload_json = document.querySelector("#file_tag");
upload_json.onchange = function(event)
{
    ReadFile(event.target.files);
}


document.onclick = () => document.querySelector("#file_tag").click();