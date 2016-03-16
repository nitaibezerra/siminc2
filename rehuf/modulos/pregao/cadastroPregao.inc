<?php
global $db;

if ( $_REQUEST['inserePregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	inserePregao( $_REQUEST );
	exit;
}

if ( $_REQUEST['atualizaPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaPregao( $_REQUEST );
	exit;
}

if ( $_REQUEST['pesquisaItemGrupoPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	pesquisaItemGrupoPregao( $_REQUEST['preid'] );
	exit;
}

if ( $_REQUEST['excluiItemGrupoPregao'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	excluiItemGrupoPregao( $_REQUEST['igpid'] );
	exit;
}

if($_REQUEST['preid']){
	$preid = $_REQUEST['preid'];
	$_SESSION['rehuf']['preid'] = $preid;
}else{
	$preid = $_SESSION['rehuf']['preid'];
}

if($preid){	
	$sql = "SELECT 
			  preid,
			  precodigo,
			  preobjeto,
			  predatainicialpregao,
			  predatafinalpregao,
			  predatainicialpreenchimento,
			  predatafinalpreenchimento
			FROM 
			  rehuf.pregao
			WHERE preid = '{$preid}'";
			
	$arDados = $db->pegaLinha($sql);
	
	if($arDados){
		$preid						 = $arDados['preid'];
		$precodigo 					 = $arDados['precodigo'];
		$preobjeto 					 = $arDados['preobjeto'];
		$predatainicialpregao 		 = $arDados['predatainicialpregao'];
		$predatafinalpregao 		 = $arDados['predatafinalpregao'];
		$predatainicialpreenchimento = $arDados['predatainicialpreenchimento'];
		$predatafinalpreenchimento 	 = $arDados['predatafinalpreenchimento'];
		
		if($predatainicialpregao){
			$predatainicialpregao = formata_data($predatainicialpregao);
		}		
		if($predatafinalpregao){
			$predatafinalpregao = formata_data($predatafinalpregao);
		}		
		if($predatainicialpreenchimento){
			$predatainicialpreenchimento = formata_data($predatainicialpreenchimento);
		}
		if($predatafinalpreenchimento){
			$predatafinalpreenchimento = formata_data($predatafinalpreenchimento);
		}
	}
}else{
	$preid						 = "";
	$precodigo 					 = "";
	$preobjeto 					 = "";
	$predatainicialpregao 		 = "";
	$predatafinalpregao 		 = "";
	$predatainicialpreenchimento = "";
	$predatafinalpreenchimento 	 = "";
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Cadastro de Pregão', '' );
?>

<style>
@CHARSET "ISO-8859-1";

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 55%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
<script	src="/includes/calendario.js"></script>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="preid" id="preid" value="<?=$preid; ?>"> 

<table id="tblCadItensPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Código Pregão:</td>
		<td><?=campo_texto( 'precodigo', 'N', 'S', '', 40, 100, '', '','','','','id="precodigo"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Objeto do Pregão:</td>
		<td><?=campo_texto( 'preobjeto', 'S', 'S', '', 40, 250, '', '','','','','id="preobjeto"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período de Preenchimento:</td>
		<td><?=campo_data('predatainicialpreenchimento', 'N','S','','','','');?> até
						<?=campo_data('predatafinalpreenchimento', 'S','S','','','',''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Vigência do Pregão:</td>
		<td><?=campo_data('predatainicialpregao', 'N','S','','','',''); ?> até
						<?=campo_data('predatafinalpregao', 'N','S','','','',''); ?></td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" onclick="validaFormPregao();">
		<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Voltar();"></th>
	</tr>
</table>
<!-- <input type="text" name="teste" id="teste1" value="" onKeyUp="this.value=formataPrecoPregao(this.value);"> -->
</form>

<div id="addItem">
	<? if($preid){ pesquisaItemGrupoPregao($preid);} ?>
	
</div>

<div id="erro"></div>
<script type="text/javascript">
 function excluiItemGrupoPregao(igpid){
	if(confirm("Tem certeza que deseja excluir este registro?")){
		var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&excluiItemGrupoPregao=true&igpid='+igpid,
					asynchronous: false,
					onComplete: function(res){
						if(res.responseText == 1){
							alert('Operação realizada com sucesso!');
							pesquisaItemGrupoPregao();
						}else{
							alert('Operação não realizada!');
						}
					}						
				});
	}
}

function AddItem(){
	window.open('rehuf.php?modulo=pregao/popupItemGrupoPregao&acao=A&preid='+$('preid').value,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=1000,height=500');
}

function pesquisaItemGrupoPregao(){
	var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&pesquisaItemGrupoPregao=true&preid='+$('preid').value,
					asynchronous: false,
					onComplete: function(res){
						//alert(res.responseText);
						$('addItem').innerHTML = res.responseText; 
					}						
				});
}

function validaFormPregao(){
	if($('preobjeto').value == ""){
		alert('O campo Objeto do Pregão é de preenchimento obrigatório!');
		$('preobjeto').focus();
		return false;
	}else if( ($('predatainicialpreenchimento').value == "") || ($('predatafinalpreenchimento').value == "") ){
		alert('O campo Período de Preenchimento é de preenchimento obrigatório!');
		if( ($('predatainicialpreenchimento').value == "") ){
			$('predatainicialpreenchimento').focus();
			return false;
		}
		if( ($('predatafinalpreenchimento').value == "") ){
			$('predatafinalpreenchimento').focus();
			return false;
		}
		return false;
	}else if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value == "") || ($('predatainicialpregao').value == "") && ($('predatafinalpregao').value != "")){
		alert('O campo Vigência do Pregão é de preenchimento obrigatório!');
		if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value == "") ){
			$('predatafinalpregao').focus();
		}else{
			$('predatainicialpregao').focus();
		}
			return false;
	}else if( !validaDataMaior($('predatainicialpreenchimento'), $('predatafinalpreenchimento')) ){
		alert("A preenchimento inicial não pode ser maior que o preenchimento final!");
		$('predatainicialpreenchimento').focus();
		return false;
	}else{
		if( ($('predatainicialpregao').value != "") && ($('predatafinalpregao').value != "") ){
			if( !validaDataMaior($('predatainicialpregao'), $('predatafinalpregao') ) ){
				alert("A data de vigência inicial não pode ser maior que a vigência final!");
				$('predatainicialpregao').focus();
				return false;
			}else if(!validaData($('predatainicialpregao') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('predatainicialpregao').focus();
				return false;
			}else if(!validaData($('predatafinalpregao') ) ) {
				alert('Data Vigência esta no formato incorreto');
				$('predatafinalpregao').focus();
				return false;
			}
		}
		
		if( ($('predatainicialpreenchimento').value != "") && ($('predatafinalpreenchimento').value != "") ){
			if(!validaData($('predatainicialpreenchimento') ) ) {
				alert('Data do período de preenchimento esta no formato incorreto');
				$('predatainicialpreenchimento').focus();
				return false;
			}else if(!validaData($('predatafinalpreenchimento') ) ) {
				alert('Data do período de preenchimento esta no formato incorreto');
				$('predatafinalpreenchimento').focus();
				return false;
			}
		}
		inserePregao();
	}
	
}
function inserePregao(){
	$('loader-container').show();
	
	if($('preid').value == ""){
		var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&inserePregao=true&precodigo='+$('precodigo').value+
												'&preobjeto='+$('preobjeto').value+
												'&predatainicialpreenchimento='+$('predatainicialpreenchimento').value+
												'&predatafinalpreenchimento='+$('predatafinalpreenchimento').value+
												'&predatainicialpregao='+$('predatainicialpregao').value+
												'&predatafinalpregao='+$('predatafinalpregao').value,
					asynchronous: false,
					onComplete: function(res){

						if( Number(res.responseText) ){
							alert("Operação realizada com sucesso!");
							$('preid').value = res.responseText;
							pesquisaItemGrupoPregao();
						}else{
							if(res.responseText.length < 200){
								alert(res.responseText);
							}else{
								alert('Operação não realizada, entre em contato com administrador do sistema!');
							}	
						}
					}						
				});
	}else{
		var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroPregao&acao=A',{
					method:		'post',
					parameters: '&atualizaPregao=true&preid='+$('preid').value+
												'&precodigo='+$('precodigo').value+
												'&preobjeto='+$('preobjeto').value+
												'&predatainicialpreenchimento='+$('predatainicialpreenchimento').value+
												'&predatafinalpreenchimento='+$('predatafinalpreenchimento').value+
												'&predatainicialpregao='+$('predatainicialpregao').value+
												'&predatafinalpregao='+$('predatafinalpregao').value,
					asynchronous: false,
					onComplete: function(res){

						if( res.responseText == 1 ){
							alert("Operação realizada com sucesso!");
						}else{
							if(res.responseText.length < 200){
								alert(res.responseText);
							}else{
								alert('Operação não realizada, entre em contato com administrador do sistema!');
							}	
						}
					}						
				});
	}
	$('loader-container').hide();
}
function Voltar(){
	window.location.href = 'rehuf.php?modulo=pregao/listaPregao&acao=A';
}

function validaDecimal(obj, qtdeCasasDecimais) {
	var texto = obj.value;
	//var stringRegExp = "^([0-9]*)$|^([0-9]*,[0-9]{1,"+ qtdeCasasDecimais +"})$";
	var stringRegExp = /^((\d+|\d{1,3}(\.\d{3})+)(\,\d{1,"+ qtdeCasasDecimais +"})?|\,\d{1,"+ qtdeCasasDecimais +"})$/
	var reg = new RegExp(stringRegExp);
    
	if ( !reg.test(texto) ){	  
    	alert('Formato inválido');
		obj.focus();
		return false;	
	}  
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
	