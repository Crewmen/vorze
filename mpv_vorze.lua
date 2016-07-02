require('mpv_bindings');
print("Success!");

mpv_init_vorze();

active = false;
paused = false;

function findLast(haystack, needle)
    local i=haystack:match(".*"..needle.."()")
    if i==nil then return nil else return i-1 end
end

function update_vorze()
   if active and not paused then
      local curr_time_into = mp.get_property("time-pos","");
      update_vorze_to_TS(curr_time_into);
   end
end

function load_csv_to_vorze()
   local path = mp.get_property("path","");
   print("Unedited path is " .. path);
   local last_period_index = findLast(path,"%.");
   print(last_period_index);
   local extensionless_path = string.sub(path,1,last_period_index);
   local csv_path = extensionless_path.."csv";
   print("csv Path is " .. csv_path );  

   active = true
   mpv_load_file(csv_path);
end

function on_pause_change(name, value)
   paused = value;
   if (value == true and active) then
      mpv_stop_vorze_motion();
   end
end

function shutdown_handler()
   mpv_close_vorze();
end


-- TODO: Close the vorze, and only init once a file is loaded

mp.add_periodic_timer(.05,update_vorze);

mp.register_event("file-loaded", load_csv_to_vorze);

mp.register_event("shutdown", shutdown_handler);

mp.observe_property("pause","bool",on_pause_change)
