try{
    xmlhttp = new XMLHttpRequest();
}catch(ee){
    try{
        xmlhttp = new ActiveXObject("Msxml2.XMLHTTP");
    }catch(e){
        try{
            xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
        }catch(E){
            xmlhttp = false;
        }
    }
}

function MouseClick(objeto){
	if (objeto.type == "select-one" || objeto.type == "text" || objeto.type == "password"){
		objeto.className = 'clsMouseFocus';
	}else if(objeto.type == "textarea"){
		objeto.className = 'txareaclsMouseFocus';
	}
}

function MouseBlur( objeto )
{
	if ( objeto.type == "select-one" || objeto.type == "text" || objeto.type == "textarea" || objeto.type == "password" )
	{
		if ( objeto.type == "textarea")
		{
			objeto.className = 'txareanormal';
		}
		else
		{
			objeto.className = 'normal';
		}
	}
}

function alteraConceito(mod){
    var c=document.getElementById("Conceito")
    if(mod == 'M') {
	    c.value = 3;
	}
    else {
	    c.value = 4;
	}
}

function doBusca(tuf){
    //limpa o select
    var c=document.getElementById("municipio");
    while (c.options.length>0) c.options[0]=null;
    c.options[0]=new Option(" -- Aguarde ... -- "," -- Aguarde ... -- ")

    //Monta a url com a uf
    xmlhttp.open("GET", "index.php?view=municipios&uf="+tuf,true);

    xmlhttp.onreadystatechange=function() {
        if (xmlhttp.readyState==4){
            //limpa o select
            var c=document.getElementById("municipio")
            while(c.options.length>0)c.options[0]=null
			var texto = xmlhttp.responseText;
			var aCidades = eval( '(' + texto + ')' );
            //popula o select com a lista de cidades obtida
		    c.options[0]=new Option("Selecione...","Selecione...")
		    for(var Cidade in aCidades) {
		        var Codigo = aCidades[Cidade];
                c.options[c.options.length]=new Option(Cidade,Codigo)
            }
        }
    }

    xmlhttp.send(null)
}

/*
   Sistema Sistema Simec
   Setor responsável: SPO/MEC
   Analista: Cristiano Cabral
   Programador: Cristiano Cabral (e-mail: cristiano.cabral@gmail.com)
   Módulo: valida.js
   Finalidade: Funções de validação em Javascript
   Data de criação: 03/08/2005

objetivo: mascarar de acordo com a mascara passada
caracteres: # - caracter a ser mascarado
           | - separador de mascaras
modos (exemplos):
mascara simples: "###-####"	                 mascara utilizando a mascara passada
mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
mascara dinâmica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja dinâmica ex: ###.###.###.###,##
utilizar no onkeyup do objeto
ex: onkeyup="this.value = mascaraglobal('#####-###',this.value);"
tratar o maxlength do objeto na página (a função não trata isso)
*/

//tirar os espaços das extremidades do valor passado (utilizada pela mascaraglobal)
function trim(valor){
        for (i=0;i<valor.length;i++){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(i);
                        break;
                }
                if (i == valor.length-1){
                        valor = "";
                }
        }
        for (i=valor.length-1;i>=0;i--){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(0,i+1);
                        break;
                }
        }
        return valor;
}

function mascaraglobal(mascara, valor){

        var mascara_utilizar;
        var mascara_limpa;
        var temp;
        var i;
        var j;
        var caracter;
        var separador;
        var dif;
        var validar;
        var mult;
        var ret;
        var tam;
        var tvalor;
        var valorm;
        var masct;
        tvalor = "";
        ret = "";
        caracter = "#";
        separador = "|";
        mascara_utilizar = "";
        valor = trim(valor);
        if (valor == "")return valor;
        temp = mascara.split(separador);
        dif = 1000;

        valorm = valor;
        //tirando mascara do valor já existente
        for (i=0;i<valor.length;i++){
                if (!isNaN(valor.substr(i,1))){
                        tvalor = tvalor + valor.substr(i,1);
                }
        }
        valor = tvalor;

        //formatar mascara dinamica
        for (i = 0; i<temp.length;i++){
                mult = "";
                validar = 0;
                for (j=0;j<temp[i].length;j++){
                        if (temp[i].substr(j,1) == "]"){
                                temp[i] = temp[i].substr(j+1);
                                break;
                        }
                        if (validar == 1)mult = mult + temp[i].substr(j,1);
                        if (temp[i].substr(j,1) == "[")validar = 1;
                }
                for (j=0;j<valor.length;j++){
                        temp[i] = mult + temp[i];
                }
        }


        //verificar qual mascara utilizar
        if (temp.length == 1){
                mascara_utilizar = temp[0];
                mascara_limpa = "";
                for (j=0;j<mascara_utilizar.length;j++){
                        if (mascara_utilizar.substr(j,1) == caracter){
                                mascara_limpa = mascara_limpa + caracter;
                        }
                }
                tam = mascara_limpa.length;
        }else{
                //limpar caracteres diferente do caracter da máscara
                for (i=0;i<temp.length;i++){
                        mascara_limpa = "";
                        for (j=0;j<temp[i].length;j++){
                                if (temp[i].substr(j,1) == caracter){
                                        mascara_limpa = mascara_limpa + caracter;
                                }
                        }

                        if (valor.length > mascara_limpa.length){
                                if (dif > (valor.length - mascara_limpa.length)){
                                        dif = valor.length - mascara_limpa.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else if (valor.length < mascara_limpa.length){
                                if (dif > (mascara_limpa.length - valor.length)){
                                        dif = mascara_limpa.length - valor.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else{
                                mascara_utilizar = temp[i];
                                tam = mascara_limpa.length;
                                break;
                        }
                }
        }

        //validar tamanho da mascara de acordo com o tamanho do valor
        if (valor.length > tam){
                valor = valor.substr(0,tam);
        }else if (valor.length < tam){
                masct = "";
                j = valor.length;
                for (i = mascara_utilizar.length-1;i>=0;i--){
                        if (j == 0) break;
                        if (mascara_utilizar.substr(i,1) == caracter){
                                j--;
                        }
                        masct = mascara_utilizar.substr(i,1) + masct;
                }
                mascara_utilizar = masct;
        }

        //mascarar
        j = mascara_utilizar.length -1;
        for (i = valor.length - 1;i>=0;i--){
                if (mascara_utilizar.substr(j,1) != caracter){
                        ret = mascara_utilizar.substr(j,1) + ret;
                        j--;
                }
                ret = valor.substr(i,1) + ret;
                j--;
        }
        return ret;
}
