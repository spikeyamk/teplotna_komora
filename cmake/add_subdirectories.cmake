# Define a function to automatically add subdirectories
function(add_subdirectories base_dir)
    # List all items (files and directories) in the base_dir
    file(GLOB children RELATIVE ${base_dir} ${base_dir}/*)

    # Loop through each item
    foreach(child ${children})
        # Check if the item is a directory
        if(IS_DIRECTORY ${base_dir}/${child})
            # Add the subdirectory
            message(STATUS "Adding subdirectory: ${child}")
            add_subdirectory(${base_dir}/${child})
        endif()
    endforeach()
endfunction()