require ("Dlltry")

zc = 
{
	configTest = 
  {
		sub = {A = 0, B = 1},
		str = "I'm string!",
		num = 4,
	},

	debug = false,
}

pcall(loadfile('luaConfig.lua'))

function write_table_to_file(prefix, tbl, luaConfig)
   luaConfig = luaConfig or nil
   local do_close = false
   if luaConfig == nil then
      luaConfig = io.open('luaConfig.lua', 'w')
      do_close = true
   end
   for key,val in pairs(tbl) do
      if 'table' == type(val) then
        write_table_to_file(prefix .. '.' .. tostring(key), val, luaConfig)
      else
         if 'string' == type(val) then
            val = '"' .. val .. '"'
         end
         msg = string.format('%s = %s', prefix .. '.' .. tostring(key), tostring(val) .. '\n')
         luaConfig:write(msg)
      end
   end
   if do_close then
      luaConfig:close()
      luaConfig = nil
      print(luaConfig)
   end
end

write_table_to_file('zc', zc)

num = 0
function main()
   num = num + 1
   sum = Dlltry.Add_in_Lua(0, num)
   print (sum)
end

