local cell = require "cell"
local c = require "cell.c"
local msgpack = require "cell.msgpack"
cell.dispatch {
	id = 100, -- gui msgpack
	dispatch = function(msg,len,...)
		local login =msgpack:unpack_raw(msg,len)
		for k, v in ipairs(login) do  
   		 	print(k,v)  
		end 
		print(login[1])
		print(type(login[1]))
		--print(a.t)
		print(type(tonumber(login[1])))
		--c.post_message(login[1],"test")
		print(c.post_message(tonumber(login[1]),"test"))
		print("=======")
	end
}
function cell.main()
	c.register("cell_login")
end
