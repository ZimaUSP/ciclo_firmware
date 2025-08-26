#include "Website.hpp"

String WEBSITE::websiteChart(){
	String html = 
"<!DOCTYPE html>\n"
"<html lang='en'>\n"
"<head>\n"
"    <meta charset='UTF-8'>\n"
"    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
"    <title>Gráfico</title>\n"
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

String WEBSITE::websiteGRAFICOS(){
    
    String html = R"DELIM(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Document</title>
    <style>
        body {
            margin: 100px;
        }
    </style>
</head>
<body>

    <script>
        let params =  new URLSearchParams(document.location.search);
        let id = params.get('id');
        let modo = params.get('modo')
        console.log('id: ' + id);
    </script>

    <h1 id='titulo'></h1>

    <div>
        <canvas id='myChart' style='width: 700px; display: block; height: 400px'></canvas>
    </div>
    
    <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>

    <button id='grafico'> Download grafico </button>
    <button id='csv'> Download csv </button>

    <script>
        const ctx = document.getElementById('myChart');

        const url = '/data/' + modo + '/sessions';
        let titulo_cont = 'Gráfico de torque x tempo da Sessão ' + id
        document.getElementById('titulo').innerText = titulo_cont

        const options = {
            method: 'GET'
        };

        let chart; // Variável para armazenar a instância do gráfico 
        fetch(url + '?id=' + id, options)
            .then(response => response.json())
            .then(response => {
                console.log('fetch do Data');
                console.log(response);
                let tempo = response.tempo;
                let torque = response.torque;
                chart = createChart(tempo, torque); // Atribuindo o gráfico à variável
            });

        function createChart(tempo, torque) {
            const data = {
                labels: tempo,
                datasets: [{
                    label: 'Teste',
                    data: torque,
                    fill: false,
                    borderColor: 'rgb(75, 192, 192)',
                    tension: 0.1
                }]
            };
            const config = {
                type: 'line',
                data: data,
            };

            return new Chart(ctx, config); // Retorna a instância do gráfico
        }

        function downloadGrafico() {
            if (chart) {
                const imageUrl = chart.toBase64Image();
                const link = document.createElement('a');
                link.href = imageUrl;
                link.download = 'chart.png';
                link.click();
            } else {
                alert('Chart not loaded yet.');
            }
        };

        function download_csv() {
            const hiddenElement = document.createElement('a');
            hiddenElement.href = '/data/' + modo + '/sessions/csv?id=' + id;
            hiddenElement.download = 'sessao_' + modo + '_' + id + '.csv';
            hiddenElement.click();
        }

        document.getElementById('grafico').addEventListener('click', downloadGrafico);
        document.getElementById('csv').addEventListener('click', download_csv);
    </script>
</body>
</html>
)DELIM";

    return html;
}

String WEBSITE::websiteResistivo(){
    //html
String html = R"DELIM(
<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Document</title>
</head>
<body>
    <nav>
        <ul>
            <li class='liRes'><a style='color: #E0B700;' href='/resistivo/sessions'>Resistivo</a></li>
            <li class='liNor'><a style='color:#0031A3;' href='/normal/sessions'>Normal</a></li>
            <li class='liPas'><a style='color: #00B800;' href='/passivo/sessions'>Passivo</a></li>
        </ul>
    </nav>
    <div id='ContainerBotoes'>

    </div>
</body>
</html>

<style>

body{
    padding: 0;
    margin: 0;
    background-color: #FFF7D6;
}

nav ul{
    list-style-type: none;
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    padding: 0;
    margin: 0;
    height: 60px;
}

nav ul li{
    text-align: center;
    justify-content: center;
    display: flex;
    align-items: center;
}

.liRes{
    background-color: #FFF7D6;
    border: 1px solid black;
    border-bottom: none;
    border-left: none;
    text-decoration: underline;
    text-decoration-color: #E0B700;
}

.liNor{
    background-color: #EBF1FF;
    border: 1px solid black;
}

.liPas{
    background-color: #EBFFEB;
    border: 1px solid black;
}

nav ul li a{
    text-decoration: none;
    font-size: 22px;
}

div{
    display: flex;
    justify-content: space-between;
    flex-direction: row;
    width: 350px;
    margin-left: auto;
    margin-right: auto;
    margin-top: 50px;
    gap: 12px;
    flex-wrap: wrap;
}

div a{
    flex: 0 0 40%;
}

div a button{
    height: 50px;
    border-radius: 14px;
    cursor: pointer;
    width: 100px;
    background-color: white;
    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
    border: 1px solid black;
}

div a button:hover{
    border: 1.5px solid black;
}

</style>

<script>

const url = '/number/resistivo/sessions';
fetch(url).then(response =>{
    return response.json();
})
.then(data => {
    nbotoes = data.sessions;
    const a = document.createElement('a');
    const Botao = document.createElement('button');
    const ContainerBotoes = document.getElementById('ContainerBotoes');
    for (let index = 1; index < nbotoes; index++) {
        const a = document.createElement('a');
        const Botao = document.createElement('button');
        a.href = '/graficos' + '?id=' + (index-1) + '&' + 'modo=resistivo';
        Botao.innerHTML = 'Sessao ' + index;
        a.appendChild(Botao);
        ContainerBotoes.appendChild(a);
    }
})
.catch(error =>{
    console.log('Erro ao buscar os dados:',error);
});

</script>
)DELIM";


return html;


}

String WEBSITE::websiteNormal(){
    //html
String html = R"DELIM(
<!DOCTYPE html>
<html lang='en'>
<head>
   <meta charset='UTF-8'>\n
   <meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Document</title>
</head>
<body>
   <nav>
       <ul>
           <li class='liRes'><a style='color: #E0B700;' href='/resistivo/sessions'>Resistivo</a></li>
           <li class='liNor'><a style='color:#0031A3;' href='/normal/sessions'>Normal</a></li>
           <li class='liPas'><a style='color: #00B800;' href='/passivo/sessions'>Passivo</a></li>
       </ul>
   </nav>
   <div id='ContainerBotoes'>
   </div>
</body>
</html>
<style>
body{
    padding: 0;
    margin: 0;
    background-color: #EBF1FF;
}
nav ul{
    list-style-type: none;
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    padding: 0;
    margin: 0;
    height: 60px;
}
nav ul li{
    text-align: center;
    justify-content: center;
    display: flex;
    align-items: center;
}
.liRes{
    background-color: #FFF7D6;
    border: 1px solid black;
}
.liNor{
    background-color: #EBF1FF;
    border: 1px solid black;
    border-bottom: none;
    text-decoration: underline;
    text-decoration-color: #0031A3;
}
.liPas{
    background-color: #EBFFEB;
    border: 1px solid black;
}
nav ul li a{
    text-decoration: none;
    font-size: 22px;
}
div{
    display: flex;
    justify-content: space-between;
    flex-direction: row;
    width: 350px;
    margin-left: auto;
    margin-right: auto;
    margin-top: 50px;
    gap: 12px;
    flex-wrap: wrap;
}
div a{
    flex: 0 0 40%;
}
div a button{
    height: 50px;
    border-radius: 14px;
    cursor: pointer;
    width: 100px;
    background-color: white;
    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
    border: 1px solid black;
}
div a button:hover{
    border: 1.5px solid black;
}
</style>
<script>
const url = '/number/normal/sessions';
fetch(url).then(response =>{
    return response.json();
})
.then(data => {
    nbotoes = data.sessions;
    const a = document.createElement('a');
    const Botao = document.createElement('button');
    const ContainerBotoes = document.getElementById('ContainerBotoes');
    for (let index = 1; index <= nbotoes; index++) {
        const a = document.createElement('a');
        const Botao = document.createElement('button');
        a.href = '/graficos' + '?id=' + (index-1) + '&' + 'modo=normal';
        Botao.innerHTML = 'Sessao ' + index;
        a.appendChild(Botao);
        ContainerBotoes.appendChild(a);
    }
})
.catch(error =>{
    console.log('Erro ao buscar os dados:',error);
});
</script>
)DELIM";

return html;


}

String WEBSITE::websitePassivo(){
    //html
String html = R"DELIM(
<!DOCTYPE html>
<html lang='en'>
<head>
   <meta charset='UTF-8'>
   <meta name='viewport' content='width=device-width, initial-scale=1.0'>
<title>Document</title>
</head>
<body>
   <nav>
       <ul>
           <li class='liRes'><a style='color: #E0B700;' href='/resistivo/sessions'>Resistivo</a></li>
           <li class='liNor'><a style='color:#0031A3;' href='/normal/sessions'>Normal</a></li>
           <li class='liPas'><a style='color: #00B800;' href='/passivo/sessions'>Passivo</a></li>
       </ul>
   </nav>
   <div id='ContainerBotoes'>
   </div>
</body>
</html>
<style>
body{
    padding: 0;
    margin: 0;
    background-color: #EBFFEB;
}
nav ul{
    list-style-type: none;
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    padding: 0;
    margin: 0;
    height: 60px;
}
nav ul li{
    text-align: center;
    justify-content: center;
    display: flex;
    align-items: center;
}
.liRes{
    background-color: #FFF7D6;
    border: 1px solid black;
}
.liNor{
    background-color: #EBF1FF;
    border: 1px solid black;
}
.liPas{
    background-color: #EBFFEB;
    border: 1px solid black;
    border-bottom: none;
    border-right: none;
    text-decoration: underline;
    text-decoration-color: #00B800;
}
nav ul li a{
    text-decoration: none;
    font-size: 22px;
}
div{
    display: flex;
    justify-content: space-between;
    flex-direction: row;
    width: 350px;
    margin-left: auto;
    margin-right: auto;
    margin-top: 50px;
    gap: 12px;
    flex-wrap: wrap;
}
div a{
    flex: 0 0 40%;
}
div a button{
    height: 50px;
    border-radius: 14px;
    cursor: pointer;
    width: 100px;
    background-color: white;
    box-shadow: rgba(0, 0, 0, 0.24) 0px 3px 8px;
    border: 1px solid black;
}
div a button:hover{
    border: 1.5px solid black;
}
</style>
<script>
const url = '/number/passivo/sessions';
fetch(url).then(response =>{
    return response.json();
})
.then(data => {
    nbotoes = data.sessions;
    const a = document.createElement('a');
    const Botao = document.createElement('button');
    const ContainerBotoes = document.getElementById('ContainerBotoes');
    for (let index = 1; index <= nbotoes; index++) {
        const a = document.createElement('a');
        const Botao = document.createElement('button');
        a.href = '/graficos' + '?id=' + (index-1) + '&' + 'modo=passivo';
        Botao.innerHTML = 'Sessao ' + index;
        a.appendChild(Botao);
        ContainerBotoes.appendChild(a);
    }
})
.catch(error =>{
    console.log('Erro ao buscar os dados:',error);
});



</script>
)DELIM";


return html;


}
