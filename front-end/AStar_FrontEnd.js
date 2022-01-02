// Helper scripts
    // verifica se um determinado parâmetro é válido
    function VerifyCoordinate(x, y, json_obj)
    {
        if ( !(x >= 0 && x < json_obj.Grid_size_X) )
        {
            return false;
        }

        if ( !(y >= 0 && y < json_obj.Grid_size_Y) )
        {
            return false;
        }
        return true;
    }

    function VerifyIndex(node_index, json_obj)
    {
        if ( !(node_index >= 0 && node_index < (json_obj.Grid_size_X * json_obj.Grid_size_Y)) )
        {
            return false;
        }
        return true;
    }

// Scripts to build JSON command

// Hide or show the barrier input based on the problem type
function SetBarrierInputStyle()
{
    let Problem_type = document.querySelector("input[type='radio'][name='Problem_type']:checked").value;
    if(Problem_type == "Node Map")
    {
        document.querySelectorAll(".Barrier_input").forEach( (barrier_input_element) => {barrier_input_element.style.display = "block"});
    }
    if(Problem_type == "Sliding Puzzle")
    {
        document.querySelectorAll(".Barrier_input").forEach( (barrier_input_element) => {barrier_input_element.style.display = "none"});
    }
}

// Causes exceptions in the first load of the page
//window.onload = () => {SetBarrierInputStyle()}

// Add events to problem type inputs
let Problem_type_selector = document.querySelectorAll("input[type='radio'][name='Problem_type']").forEach((element) =>
{
    element.addEventListener("change", () =>
    {
        SetBarrierInputStyle();
    });
});

function DisplayErrorMessageJsonBuild()
{
    text_string = "";
    text_array = 
    [
        "Um\n",
        "Texto\n",
        "Simples!!!\n"
    ];

    for(let text of text_array)
    {
        text_string += text;
    }

    document.querySelector("#GeneratedJSONOutput").innerText = JSON.stringify(text_string);

}


function CreateDownloadLinkOnId(string, html_id, file_type)
{
    let download_link;
    const blob = new Blob([string], {type : file_type});

    if(document.querySelector(html_id).hasAttribute("href"))
    {
        URL.revokeObjectURL(document.querySelector(html_id).href);
    }
    download_link = URL.createObjectURL(blob);

    document.querySelector(html_id).innerText = "Download here";
    document.querySelector(html_id).href = download_link;
}

// Builds JSON
let build_json_btn = document.querySelector("#BuildJson");
build_json_btn.onclick = () =>
{
    json_command = {};
    let json_build_error;
    // Get all the input values
    json_command["Problem_type"] = document.querySelector("input[type='radio'][name='Problem_type']:checked").value;
    json_command["Grid_size_X"] = parseInt( document.querySelector("#Grid_size_X").value );
    json_command["Grid_size_Y"] = parseInt( document.querySelector("#Grid_size_Y").value );
    json_command["Start"] = document.querySelector("#Start").value.split(",").map(Number);
    json_command["Goal"] = document.querySelector("#Goal").value.split(",").map(Number);

    /*if(json_command["Problem_type"] == "Node Map" && document.querySelector("#Barrier").value != "")
    {
        json_command["Barrier"] = document.querySelector("#Barrier").value.split(";").map( (element) => {return element.split(",").map(Number)} );
    }
    else if(json_command["Problem_type"] == "Node Map" && document.querySelector("#Barrier").value == "")
    {
        json_command["Barrier"] = [];
    }*/

    // Sliding puzzle:
    // - If start or goal size aren't equal to Grid_size_X x Grid_size_Y
    if(json_command["Problem_type"] == "Sliding Puzzle")
    {
        if(json_command["Start"].length != json_command["Grid_size_X"] * json_command["Grid_size_Y"])
        {
            json_build_error = "ERROR: Start use invalid size";
            document.querySelector("#GeneratedJSONOutput").innerHTML = JSON.stringify(json_build_error);
            return;
        }

        if(json_command["Goal"].length != json_command["Grid_size_X"] * json_command["Grid_size_Y"])
        {
            json_build_error = "ERROR: Goal use invalid size";
            document.querySelector("#GeneratedJSONOutput").innerHTML = JSON.stringify(json_build_error);
            return;
        }
    }


    // Node map:
    // - Just X and Y coordinates (size == 2)
    // - start.xy >= 0 (enforced by HTML "min=1")
    if(json_command["Problem_type"] == "Node Map")
    {
        if(json_command["Start"].length != 2 || json_command["Start"].length != 2)
        {
            json_build_error = "ERROR: Start use invalid size";
            document.querySelector("#GeneratedJSONOutput").innerHTML = JSON.stringify(json_build_error);
            return;
        }

        if(json_command["Goal"].length != 2)
        {
            json_build_error = "ERROR: Goal use invalid size";
            document.querySelector("#GeneratedJSONOutput").innerHTML = JSON.stringify(json_build_error);
            return;
        }

        if(document.querySelector("#Barrier").value != "")
        {
            json_command["Barrier"] = document.querySelector("#Barrier").value.split(";").map( (element) => {return element.split(",").map(Number)} );
        }
        else if(document.querySelector("#Barrier").value == "")
        {
            json_command["Barrier"] = [];
        }
    }

    document.querySelector("#GeneratedJSONOutput").innerHTML = JSON.stringify(json_command);

    CreateDownloadLinkOnId(JSON.stringify(json_command), "#DownloadBuiltJSON", "applicatio/json");
}

// -------------------------------------------------------------------------- //

// Scripts to read JSON response and display it

// Creates a table to display the information inside of the json file
function DisplayStats(json_result_string)
{
    let json_result_obj = JSON.parse(json_result_string);

    let stats_html_table = "<table>";

    for (key in json_result_obj)
    {
        if(key == "Best_path" || key == "Barrier")
        {
            let array_size = json_result_obj[key].length;
            if(array_size > 100)
            {
                stats_html_table += `<tr> <td>${ key }</td> <td>[${ json_result_obj[key].length } items]</td> </tr>`;
            }

            else
            {
                stats_html_table += `<tr> <td>${ key }</td> <td>${ JSON.stringify(json_result_obj[key]) }</td> </tr>`;
            }
        }

        else
        {
            stats_html_table += `<tr> <td>${ key }</td> <td>${ JSON.stringify(json_result_obj[key]) }</td> </tr>`;
        }
    }

    stats_html_table += "</table>"

    document.querySelector("#Stats").innerHTML = stats_html_table;
}

function DisplayGraphicalRepresentation(json_result_obj)
{
    // Size of each cell is calculated by the total size in a given axis
    // The idea is to create 2 boxes
    //  - One invisible/abstract that uses all the space available (total_content_box)
    //  - Another one that lives inside of the previous box and displays the actual content (effective_content_box)
    // Coordinates are calculated using the relative position between the inner and outer boxes

    let svg_image = "";
    let svg_content_cell = [];

    let image_width = parseInt(document.querySelector("#ResultSVG").getAttribute("width"));
    let image_height = parseInt(document.querySelector("#ResultSVG").getAttribute("height"));
    svg_image = `<svg width="${image_width}" height="${image_height}" id="ResultSVG" xmlns="http://www.w3.org/2000/svg">`;
    svg_image += `<rect x="0" y="0" width="100%" height="100%" fill="rgb(200,200,200)"/>`; // background color

    let margin_ratio = 0.5; // Controls how much of the space goes for the effective content box
    let default_color = "rgb(255,255,255)"; // Default color for nodes

    let total_content_box_width = image_width/json_result_obj["Grid_size_X"];
    let total_content_box_height = image_height/json_result_obj["Grid_size_Y"];

    let effective_content_box_width = total_content_box_width * margin_ratio;
    let effective_content_box_height = total_content_box_height * margin_ratio;

    for(let coord_y = 0; coord_y < json_result_obj["Grid_size_Y"]; ++coord_y)
    {
        for(let coord_x = 0; coord_x < json_result_obj["Grid_size_X"]; ++coord_x)
        {
            let content_box_coord_x = total_content_box_width * coord_x;
            let content_box_coord_y = total_content_box_height * coord_y;

            let relative_pos_x = (total_content_box_width - effective_content_box_width)/2; // "/2" Center de content
            let effective_content_box_coord_x = relative_pos_x + content_box_coord_x;

            let relative_pos_y = (total_content_box_height - effective_content_box_height)/2; // "/2" Center de content
            let effective_content_box_coord_y = relative_pos_y + content_box_coord_y;

            svg_content_cell.push([]);
            svg_content_cell[coord_x].push(`<rect id="svg_content_cell_${coord_x}_${coord_y}" x="${effective_content_box_coord_x}" y="${effective_content_box_coord_y}" width="${effective_content_box_width}" height="${effective_content_box_height}" fill="${default_color}" rx="15"/>`);
        }
    }

    // Paints start and goal nodes
    let start_color = "rgb(255, 255, 0)";
    let goal_color = "rgb(200,0,0)";
    document.querySelector("#LegendStartColor").style.backgroundColor = start_color;
    document.querySelector("#LegendGoalColor").style.backgroundColor = goal_color;
    svg_content_cell[ json_result_obj["Start"][0] ][ json_result_obj["Start"][1] ] = svg_content_cell[ json_result_obj["Start"][0] ][ json_result_obj["Start"][1] ].replace(`fill="${default_color}"`, `fill="${start_color}"`);
    svg_content_cell[ json_result_obj["Goal"][0] ][ json_result_obj["Goal"][1] ] = svg_content_cell[ json_result_obj["Goal"][0] ][ json_result_obj["Goal"][1] ].replace(`fill="${default_color}"`, `fill="${goal_color}"`);
    
    // Best path
    for(let coord_pair of json_result_obj["Best_path"])
    {
        let best_path_color = "rgb(0,200,0)";
        document.querySelector("#LegendBestPathColor").style.backgroundColor = best_path_color;
        svg_content_cell[ coord_pair[0] ][ coord_pair[1] ] = svg_content_cell[ coord_pair[0] ][ coord_pair[1] ].replace(`fill="${default_color}"`, `fill="${best_path_color}"`);
    }

    // Barrier nodes
    for(let coord_pair of json_result_obj["Barrier"])
    {
        let barrier_effect = "rgb(0,0,0)";
        document.querySelector("#LegendBarrierColor").style.backgroundColor = barrier_effect;
        if( VerifyCoordinate(coord_pair[0], coord_pair[1], json_result_obj) )
        {
            svg_content_cell[ coord_pair[0] ][ coord_pair[1] ] = svg_content_cell[ coord_pair[0] ][ coord_pair[1] ].replace(`fill="${default_color}"`, `fill="${barrier_effect}"`);
        }
        
    }

    // Assemble the hole string
    for(let row of svg_content_cell)
    {
        for(let string of row)
        {
            svg_image += string;
        }
    }
    svg_image += `</svg>`;

    // Create a download link
    document.querySelector("#ResultSVG").innerHTML = svg_image;
    CreateDownloadLinkOnId(svg_image, "#DownloadSVG", "application/svg")
}

// Rebuild svg image after size change
let global_json;
document.querySelector("#svg_height").onchange = () =>
{
    document.querySelector("#ResultSVG").setAttribute("height", document.querySelector("#svg_height").value);
    DisplayGraphicalRepresentation(global_json);
};

document.querySelector("#svg_width").onchange = () =>
{
    document.querySelector("#ResultSVG").setAttribute("width", document.querySelector("#svg_width").value);
    DisplayGraphicalRepresentation(global_json);
};

// Read from uploaded file
function ReadFile(file_list)
{
    const reader = new FileReader();

    //console.log(file_list);
    reader.readAsText(file_list[0]);

    // What to do after the json being completed loaded
    reader.onload = () =>
    {
        //JSON.parse( reader.result );
        DisplayStats(reader.result);
        DisplayGraphicalRepresentation( JSON.parse(reader.result) );
        global_json = JSON.parse(reader.result);
    };
}

let upload_json = document.querySelector("#JsonUpload");
upload_json.onchange = function(event)
{
    ReadFile(event.target.files);
}

// -------------------------------------------------------------------------- //

let all_major_sections =
[
    document.querySelector("#Presentation"),
    document.querySelector("#JsonCreation"),
    document.querySelector("#JsonVisualization"),
    document.querySelector("#About")
];

// Hide all major sections and display the selected one
function SetNewActiveSection(html_target)
{
    for(let element of all_major_sections)
    {
        element.style.display = "none";
    }

    document.querySelector(html_target.getAttribute("href")).style.display = "block";
}

// Add events to all navigation buttons
document.querySelectorAll(".navigation").forEach((element) =>
{
    element.addEventListener("click", (event) =>
    {
        SetNewActiveSection(event.target);
    });
});