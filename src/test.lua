-- Load the module. This results in a call to luaopen_mymodule().
local m = require("mymodule")

-- Create a new object instance
m1 = m.new("module1")

-- Print the object (results in a call to __tostring).
print(m1)

-- Call instance method and print the result.
print(m1:getName())
