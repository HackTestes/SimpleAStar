let path = "";
let map_string = "";
path = process.argv[2];
let my_map = [];
let my_barrier_indexes = [];

console.log(path);

const fs = require('fs')

try
{
    const data = fs.readFileSync(path, 'utf8')
    map_string = data;
}
catch (err)
{
    console.error(err)
}

// Put every letter in an addressable array
my_map = map_string.split("\n");
for (let i = 0; i < my_map.length; ++i)
{
    my_map[i] = my_map[i].split("");
}

// Read the barrier
for (let line = 0; line < my_map.length; ++line)
{
    for (let letter = 0; letter < my_map[line].length; ++letter)
        if (my_map[line][letter] == "@")
        {
            my_barrier_indexes.push([line, letter]);
        }
}

for (let i = 0; i < my_barrier_indexes.length; ++i)
{
    console.log("[" + my_barrier_indexes[i][0] + ", " + my_barrier_indexes[i][1] + "],");
}