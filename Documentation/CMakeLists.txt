function(add_article name)
    add_custom_command(OUTPUT "${name}.html"
                       COMMAND xsltproc --output "${name}.html"
                               ${CMAKE_CURRENT_SOURCE_DIR}/article.xsl
                               "${CMAKE_CURRENT_SOURCE_DIR}/${name}.xml"
                       DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.xml"
                                ${CMAKE_CURRENT_SOURCE_DIR}/article.xsl)
endfunction()

function(add_function_spec name)
    add_custom_command(OUTPUT "${name}.html"
                       COMMAND xsltproc --output "${name}.html"
                               ${CMAKE_CURRENT_SOURCE_DIR}/function-spec.xsl
                               "${CMAKE_CURRENT_SOURCE_DIR}/${name}.xml"
                       DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.xml"
                                ${CMAKE_CURRENT_SOURCE_DIR}/function-spec.xsl)
endfunction()

add_article(lang)
add_function_spec(sum)

add_custom_target(html DEPENDS lang.html sum.html)
