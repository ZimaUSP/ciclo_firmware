#include "Website.hpp"

String WEBSITE::websiteChart() {
	String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
"    <meta charset='UTF-8'>\n"
"    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"    <title>Document</title>\n"
"</head>\n"
"<body>\n"
"    <div>\n"
"        <canvas id='myChart' ></canvas>\n"
"    </div>\n"
"    <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>\n"
"    <script>\n"
"        const ctx = document.getElementById('myChart');\n"
"\n"
"        const url = '/data/resistivo/sessions';\n"
"        \n"
"        const options = {\n"
"		    method: 'GET'\n"
"        };\n"
"        fetch(url, options)\n"
"            .then(response => response.json() )\n"
"            .then( response => {\n"
"                console.log('hello');\n"
"                console.log(response);\n"
"                let tempo = response.tempo;\n"
"                let torque = response.torque;\n"
"                createChart(tempo, torque);\n"
"            } );\n"
"\n"
"        function createChart(tempo, torque) {\n"
"            const data = {\n"
"                labels: tempo,\n"
"                datasets: [{\n"
"                    label: 'Teste',\n"
"                    data: torque,\n"
"                    fill: false,\n"
"                    borderColor: 'rgb(75, 192, 192)',\n"
"                    tension: 0.1\n"
"                }]\n"
"            };\n"
"            const config = {\n"
"                type: 'line',\n"
"                data: data,\n"
"            };\n"
"            \n"
"            new Chart(ctx, config);\n"
"        }\n"
"    </script>\n"
"</body>\n"
"</html>\n";

	return html;

}

String WEBSITE::websiteTESTE(){
    String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
"    <meta charset='UTF-8'>\n"
"    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"    <title>Document</title>\n"
"</head>\n"
"<body>\n"
"    <div>\n"
"        <canvas id='myChart'></canvas>\n"
"    </div>\n"
"    <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>\n"
"    <button id='download'>Download</button>\n"
"\n"
"    <script>\n"
"        const ctx = document.getElementById('myChart');\n"
"\n"
"        const url = '/data/resistivo/sessions';\n"
"        \n"
"        const options = {\n"
"            method: 'GET'\n"
"        };\n"
"        \n"
"        let chart; // Variável para armazenar a instância do gráfico\n"
"\n"
"        fetch(url, options)\n"
"            .then(response => response.json())\n"
"            .then(response => {\n"
"                console.log('hello');\n"
"                console.log(response);\n"
"                let tempo = response.tempo;\n"
"                let torque = response.torque;\n"
"                chart = createChart(tempo, torque); // Atribuindo o gráfico à variável\n"
"            });\n"
"\n"
"        function createChart(tempo, torque) {\n"
"            const data = {\n"
"                labels: tempo,\n"
"                datasets: [{\n"
"                    label: 'Teste',\n"
"                    data: torque,\n"
"                    fill: false,\n"
"                    borderColor: 'rgb(75, 192, 192)',\n"
"                    tension: 0.1\n"
"                }]\n"
"            };\n"
"            const config = {\n"
"                type: 'line',\n"
"                data: data,\n"
"            };\n"
"\n"
"            return new Chart(ctx, config); // Retorna a instância do gráfico\n"
"        }\n"
"\n"
"        function download() {\n"
"            if (chart) {\n"
"                const imageUrl = chart.toBase64Image();\n"
"                const link = document.createElement('a');\n"
"                link.href = imageUrl;\n"
"                link.download = 'chart.png';\n"
"                link.click();\n"
"            } else {\n"
"                alert('Chart not loaded yet.');\n"
"            }\n"
"        };\n"
"\n"
"        document.getElementById('download').addEventListener('click', download);\n"
"    </script>\n"
"</body>\n"
"</html>\n";

    return html;
}

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

"const url = '/number/resistivo/sessions';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessions;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + '?id=' + index;\n" 
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

"const url = '/number/normal/sessions';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessions;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + '?id=' + index;\n" 
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

"const url = '/number/passivo/sessions';\n"
"fetch(url).then(response =>{\n"
"    return response.json();\n"
"})\n"
".then(data => {\n"
"    nbotoes = data.sessions;\n"
"    const a = document.createElement('a');\n"
"    const Botao = document.createElement('button');\n"
"    const ContainerBotoes = document.getElementById('ContainerBotoes');\n"
"    for (let index = 1; index <= nbotoes; index++) {\n"
"        const a = document.createElement('a');\n"
"        const Botao = document.createElement('button');\n"
"        a.href = 'Sessao' + '?id=' + index;\n"
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
