# Build HLSL shaders
add_custom_target(shaders)
find_program(GLSLC glslc)

set(VLSL_SHADER_FILES SimpleVS.vert SimplePS.frag PBR.vert PBR.frag)

foreach(shader ${VLSL_SHADER_FILES})

	set(input_glsl "${CMAKE_CURRENT_SOURCE_DIR}/${shader}")
	set(output_spv "${input_glsl}.spv")
	
	execute_process(COMMAND ${GLSLC} "${input_glsl}" "-o" "${output_spv}")

endforeach(shader)