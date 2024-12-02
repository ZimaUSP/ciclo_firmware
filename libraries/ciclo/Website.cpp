#include "Website.hpp"

String WEBSITE::websiteResistivo(){
    //html
String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
    "<meta charset='UTF-8'>\n"  
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"<title>Document</title>\n"
"</head>\n"
"<body>\n"
    "<nav>\n"
        "<ul>\n"
            "<li class='liRes'><a style='color: #E0B700;' href='/Resistivo/sessions'>Resistivo</a></li>\n"
            "<li class='liNor'><a style='color:#0031A3;' href='/Normal/sessions'>Normal</a></li>\n"
            "<li class='liPas'><a style='color: #00B800;' href='/Passivo/sessions'>Passivo</a></li>\n"
        "</ul>\n"
    "</nav>\n"
    "<div id='ContainerBotoes'>\n"

    "</div>\n"
"</body>\n"
"</html>\n"



"<style>\n"

"body{\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    background-color: #FFF7D6;\n"
"}\n"

"nav ul{\n"
"    list-style-type: none;\n"
"    display: grid;\n"
"    grid-template-columns: repeat(3, 1fr);\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    height: 60px;\n"
"}\n"

"nav ul li{\n"
"    text-align: center;\n"
"    justify-content: center;\n"
"    display: flex;\n"
"    align-items: center;\n"
"}\n"

".liRes{\n"
"    background-color: #FFF7D6;\n"
"    border: 1px solid black;\n"
"    border-bottom: none;\n"
"    border-left: none;\n"
"    text-decoration: underline;\n"
"    text-decoration-color: #E0B700;\n"
"}\n"

".liNor{\n"
"    background-color: #EBF1FF;\n"
"    border: 1px solid black;\n"
"}\n"

".liPas{\n"
"    background-color: #EBFFEB;\n"
"    border: 1px solid black;\n"
"}\n"

"nav ul li a{\n"
"    text-decoration: none;\n"
"    font-size: 22px;\n"
"}\n"

"div{\n"
"    display: flex;\n"
"    justify-content: space-between;\n"
"    flex-direction: row;\n"
"    width: 350px;\n"
"    margin-left: auto;\n"
"    margin-right: auto;\n"
"    margin-top: 50px;\n"
"    gap: 12px;\n"
"    flex-wrap: wrap;\n"
"}\n"

"div a{\n"
"    flex: 0 0 40%;\n"
"}\n"

"div a button{\n"
"    height: 50px;\n"
"    border-radius: 14px;\n"
"    cursor: pointer;\n"
"    width: 100px;\n"
"    background-color: white;\n"
"    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;\n"
"    border: 1px solid black;\n"
"}\n"

"div a button:hover{\n"
"    border: 1.5px solid black;\n"
"}\n"

"</style>\n"

"<script>\n"

"const url = 'http://localhost:3000/';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessionsRes;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + index;\n" 
"        Botao.innerHTML = 'Sessao ' + index;\n"
"        a.appendChild(Botao);\n"
"        ContainerBotoes.appendChild(a);\n"
"    }\n"
"})\n"
".catch(error =>{\n"
"    console.log('Erro ao buscar os dados:',error);\n"
"});\n"



"</script>\n";


return html;


}

String WEBSITE::websiteNormal(){
    //html
String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
    "<meta charset='UTF-8'>\n"  
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"<title>Document</title>\n"
"</head>\n"
"<body>\n"
    "<nav>\n"
        "<ul>\n"
            "<li class='liRes'><a style='color: #E0B700;' href='/Resistivo/sessions'>Resistivo</a></li>\n"
            "<li class='liNor'><a style='color:#0031A3;' href='/Normal/sessions'>Normal</a></li>\n"
            "<li class='liPas'><a style='color: #00B800;' href='/Passivo/sessions'>Passivo</a></li>\n"
        "</ul>\n"
    "</nav>\n"
    "<div id='ContainerBotoes'>\n"

    "</div>\n"
"</body>\n"
"</html>\n"



"<style>\n"

"body{\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    background-color: #EBF1FF;\n"
"}\n"

"nav ul{\n"
"    list-style-type: none;\n"
"    display: grid;\n"
"    grid-template-columns: repeat(3, 1fr);\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    height: 60px;\n"
"}\n"

"nav ul li{\n"
"    text-align: center;\n"
"    justify-content: center;\n"
"    display: flex;\n"
"    align-items: center;\n"
"}\n"

".liRes{\n"
"    background-color: #FFF7D6;\n"
"    border: 1px solid black;\n"
"}\n"

".liNor{\n"
"    background-color: #EBF1FF;\n"
"    border: 1px solid black;\n"
"    border-bottom: none;\n"
"    text-decoration: underline;\n"
"    text-decoration-color: #0031A3;\n"
"}\n"

".liPas{\n"
"    background-color: #EBFFEB;\n"
"    border: 1px solid black;\n"
"}\n"

"nav ul li a{\n"
"    text-decoration: none;\n"
"    font-size: 22px;\n"
"}\n"

"div{\n"
"    display: flex;\n"
"    justify-content: space-between;\n"
"    flex-direction: row;\n"
"    width: 350px;\n"
"    margin-left: auto;\n"
"    margin-right: auto;\n"
"    margin-top: 50px;\n"
"    gap: 12px;\n"
"    flex-wrap: wrap;\n"
"}\n"

"div a{\n"
"    flex: 0 0 40%;\n"
"}\n"

"div a button{\n"
"    height: 50px;\n"
"    border-radius: 14px;\n"
"    cursor: pointer;\n"
"    width: 100px;\n"
"    background-color: white;\n"
"    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;\n"
"    border: 1px solid black;\n"
"}\n"

"div a button:hover{\n"
"    border: 1.5px solid black;\n"
"}\n"

"</style>\n"

"<script>\n"

"const url = 'http://localhost:3000/';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessionsRes;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + index;\n" 
"        Botao.innerHTML = 'Sessao ' + index;\n"
"        a.appendChild(Botao);\n"
"        ContainerBotoes.appendChild(a);\n"
"    }\n"
"})\n"
".catch(error =>{\n"
"    console.log('Erro ao buscar os dados:',error);\n"
"});\n"



"</script>\n";


return html;


}

String WEBSITE::websitePassivo(){
    //html
String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
    "<meta charset='UTF-8'>\n"  
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"<title>Document</title>\n"
"</head>\n"
"<body>\n"
    "<nav>\n"
        "<ul>\n"
            "<li class='liRes'><a style='color: #E0B700;' href='/Resistivo/sessions'>Resistivo</a></li>\n"
            "<li class='liNor'><a style='color:#0031A3;' href='/Normal/sessions'>Normal</a></li>\n"
            "<li class='liPas'><a style='color: #00B800;' href='/Passivo/sessions'>Passivo</a></li>\n"
        "</ul>\n"
    "</nav>\n"
    "<div id='ContainerBotoes'>\n"

    "</div>\n"
"</body>\n"
"</html>\n"



"<style>\n"

"body{\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    background-color: #EBFFEB;\n"
"}\n"

"nav ul{\n"
"    list-style-type: none;\n"
"    display: grid;\n"
"    grid-template-columns: repeat(3, 1fr);\n"
"    padding: 0;\n"
"    margin: 0;\n"
"    height: 60px;\n"
"}\n"

"nav ul li{\n"
"    text-align: center;\n"
"    justify-content: center;\n"
"    display: flex;\n"
"    align-items: center;\n"
"}\n"

".liRes{\n"
"    background-color: #FFF7D6;\n"
"    border: 1px solid black;\n"
"}\n"

".liNor{\n"
"    background-color: #EBF1FF;\n"
"    border: 1px solid black;\n"
"}\n"

".liPas{\n"
"    background-color: #EBFFEB;\n"
"    border: 1px solid black;\n"
"    border-bottom: none;\n"
"    border-right: none;\n"
"    text-decoration: underline;\n"
"    text-decoration-color: #00B800;\n"
"}\n"

"nav ul li a{\n"
"    text-decoration: none;\n"
"    font-size: 22px;\n"
"}\n"

"div{\n"
"    display: flex;\n"
"    justify-content: space-between;\n"
"    flex-direction: row;\n"
"    width: 350px;\n"
"    margin-left: auto;\n"
"    margin-right: auto;\n"
"    margin-top: 50px;\n"
"    gap: 12px;\n"
"    flex-wrap: wrap;\n"
"}\n"

"div a{\n"
"    flex: 0 0 40%;\n"
"}\n"

"div a button{\n"
"    height: 50px;\n"
"    border-radius: 14px;\n"
"    cursor: pointer;\n"
"    width: 100px;\n"
"    background-color: white;\n"
"    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;\n"
"    border: 1px solid black;\n"
"}\n"

"div a button:hover{\n"
"    border: 1.5px solid black;\n"
"}\n"

"</style>\n"

"<script>\n"

"const url = 'http://localhost:3000/';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessionsRes;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + index;\n" 
"        Botao.innerHTML = 'Sessao ' + index;\n"
"        a.appendChild(Botao);\n"
"        ContainerBotoes.appendChild(a);\n"
"    }\n"
"})\n"
".catch(error =>{\n"
"    console.log('Erro ao buscar os dados:',error);\n"
"});\n"



"</script>\n";


return html;


}