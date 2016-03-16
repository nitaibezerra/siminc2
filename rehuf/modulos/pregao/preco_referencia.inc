<?php

include_once APPRAIZ.'www/rehuf/_componentes.php';

if ( $_REQUEST['atualizaPrecoReferencia'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaPrecoReferencia( $_POST['valor'] );
	exit;
}
if ( $_REQUEST['carregaDados'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	relat_preco_referencia_pregao( $_SESSION['rehuf']['preid'] );
	exit;
}
if ( $_REQUEST['carregaDetalhesPrecoReferencia'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	carregaDetalhesPrecoReferencia( $_REQUEST );
	exit;
}

if(!$_SESSION['rehuf']['preid']) {
	
	die("<script>
			alert('Pregão não selecionado. Acesse novamente');
			window.location='rehuf.php?modulo=inicio&acao=C';
		 </script>");
	
}


include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Preços de Referência', '' );

$preid = $_SESSION['rehuf']['preid'];
echo rehuf_cabecalho_pregao( $preid );
?>
<html>
<body>
<div id="lista"><? relat_preco_referencia_pregao( $preid ); ?></div>
<div id="erro"></div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="./js/rehuf.js"></script>
<script type="text/javascript">
	function limparDados(){
		document.formulario.reset();		
	}
	
	function detalhesPrecoReferencia(obj, iteid) {
		var tabela = obj.parentNode.parentNode.parentNode;
		var linha  = obj.parentNode.parentNode;
		if(obj.title=="mais") {
			var nlinha = tabela.insertRow(linha.rowIndex);
			var col0   = nlinha.insertCell(0);
			col0.colSpan=12;
			col0.id="filho"+linha.rowIndex;
			col0.innerHTML="Carregando...";
			ajaxatualizar('&carregaDetalhesPrecoReferencia=true&iteid='+iteid,'filho'+linha.rowIndex);
			obj.title="menos";
			obj.src="../imagens/menos.gif";
		} else {
			tabela.deleteRow((document.getElementById('filho'+linha.rowIndex).parentNode.rowIndex-1));
			obj.title="mais";
			obj.src="../imagens/mais.gif";
		}
	}
	
	function carregaDados(){
		var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/preco_referencia&acao=A',{
				method:		'post',
				parameters: '&carregaDados=true',
				asynchronous: false,
				onComplete: function(res){
					$('lista').innerHTML = res.responseText;
				}						
			});
	}
	
	function atualizaPrecoReferencia(){
		var ippid = "";
		var igpprecoreferencia = "";
		var valor = "";
		
		 for(i=0; i<formulario.length; i++){
 			
			if (formulario.elements[i].id.indexOf('igpprecoreferencia') == 0 ){
	 			igpprecoreferencia =  formulario.elements[i].value.replace('.','').replace(',','.');
	 		}
	 		if (formulario.elements[i].id.indexOf('ippid') == 0){
	 			if(!valor){
	 				valor = igpprecoreferencia + '_' + formulario.elements[i].value;
	 			}else{
	 				valor = valor + '|' + igpprecoreferencia + '_' + formulario.elements[i].value;
	 			}
	 		}
	 	}
	 	
		var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/preco_referencia&acao=A',{
				method:		'post',
				parameters: '&atualizaPrecoReferencia=true&valor='+valor,
				asynchronous: false,
				onComplete: function(res){
					if(res.responseText == 1){
						alert('Operação realizada com sucesso!');
						carregaDados();
					} else{
						alert('Operação não realizada!');
					}
					//$('erro').innerHTML = res.responseText;
				}						
			});
	}
	function formataPrecoPregao(valor){
		
		var mascaraFinal = '###.###.##0,0000';
		var mascara = mascaraFinal.replace(/0/gi, '#'); 
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
        caracterObrigatorio = "0";
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
        valor = new Number(valor);
        valor = new String(valor);
        
        while(valor.length < 5)
        {
        	valor = "0"+valor;
        }
        
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
        
        //alert(ret);
        var retornoFinal = "";
        var diferenca = mascaraFinal.length - ret.length;
        var limiteObrigatorio = mascaraFinal.indexOf('0');
        for (i = mascaraFinal.length - 1;i>=0;i--){
        	/*alert(i);
        	alert(mascaraFinal.substr(i,1));
        	
        	alert(i-diferenca);
        	alert(ret.substr(i-diferenca,1));
        	*/
        	
        	var caracterFinal = ""; 
	        	
        	if(i-diferenca >= 0){
	        	caracterFinal = ret.substr(i-diferenca,1);
	        }
	        else if (mascaraFinal.substr(i,1) == caracterObrigatorio){ 
        		caracterFinal = 0;
        	}
			
			retornoFinal = caracterFinal + retornoFinal;
        	
        	//alert('Final: '+retornoFinal);
        }
        
        return retornoFinal;
	}
</script>
</body>
</html>