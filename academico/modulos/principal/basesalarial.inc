<?php

if ( $_REQUEST['insereBaseMensal'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	insereBaseMensal($_REQUEST);
	exit;
}

$array = academico_pega_orgao_permitido();

if(count($array) == 2 ){
	$visualiza = 2;
	$tabela = 0;
	$cols = "colspan='2'";
	$cols1 = "colspan='6'";
}else{
	$visualiza = 1;
	$tabela = $array[0]['id'];
	$cols = "";
	$cols1 = "colspan='5'";
}

function insereBaseMensal($request){
	global $db;
	
	$bslid = array();
	$bslValor = array();
	$valor1 = array();
	
	$valor = array();
	$valor = explode('|', $request['valor']);
	
	$i = 0;
	$y = 1;

	foreach ($valor as $key => $value) {
		
		if($key % 2){
			if($y == 1){
				$codigo = current($valor);				
				$y = 2;
			}
			$valor1[$i] = $valor[$key];
			$i++;
		}else{
			if($i == 5){
				$i = 0;
				$codigo = $valor[$key];
			}
		}
		
		if($i == 5){
			$sql = "UPDATE 
					  academico.basesalarial  
					SET
					  bslvlr2008 = '{$valor1[0]}',
					  bslvlr2009 = '{$valor1[1]}',
					  bslvlr2010 = '{$valor1[2]}',
					  bslvlr2011 = '{$valor1[3]}',
					  bslvlr2012 = '{$valor1[4]}'
					 
					WHERE 
					  bslid = {$codigo}";
			$db->executar($sql);
		}
	}
	
	$res = $db->commit();
	if($res == "1"){
		echo $res;
	}else{
		echo "0";
	}
}

include APPRAIZ."includes/cabecalho.inc";
print "<br>";

$sql = "SELECT 
		  b.bslid,
		  b.clsid,
		  b.orgid,
		  b.bslvlr2008,
		  b.bslvlr2009,
		  b.bslvlr2010,
		  b.bslvlr2011,
		  b.bslvlr2012,
		  c.clsdsc
		FROM 
		  academico.basesalarial b, academico.classes c 
		WHERE b.clsid = c.clsid
		order by b.clsid, b.orgid";

$dados = $db->carregar($sql);

$arDados = array();

foreach ($dados as $key => $value){
	if($value['orgid'] == 1){
		$bslid 		= $value['bslid'];
		$clsid 		= $value['clsid'];
		$orgid 		= $value['orgid'];
		$bslvlr2008 = $value['bslvlr2008'];
		$bslvlr2009 = $value['bslvlr2009'];
		$bslvlr2010 = $value['bslvlr2010'];
		$bslvlr2011 = $value['bslvlr2011'];
		$bslvlr2012 = $value['bslvlr2012'];
		$clsdsc 	= $value['clsdsc'];
	}else{
		$array = Array("bslidS"    	 => $bslid,
					   "clsidS"    	 => $clsid,
					   "orgidS"   	 => $orgid,
		 			   "bslvlr2008S" => $bslvlr2008,
					   "bslvlr2009S" => $bslvlr2009,
					   "bslvlr2010S" => $bslvlr2010,
					   "bslvlr2011S" => $bslvlr2011,
					   "bslvlr2012S" => $bslvlr2012,
					   "clsdscS"	 => $clsdsc,
					   "bslidP"    	 => $value['bslid'],
					   "clsidP"    	 => $value['clsid'],
					   "orgidP"   	 => $value['orgid'],
		 			   "bslvlr2008P" => $value['bslvlr2008'],
					   "bslvlr2009P" => $value['bslvlr2009'],
					   "bslvlr2010P" => $value['bslvlr2010'],
					   "bslvlr2011P" => $value['bslvlr2011'],
					   "bslvlr2012P" => $value['bslvlr2012'],
					   "clsdscP"	 => $value['clsdsc']
					);
		
		array_push($arDados, $array);		
	}
}

monta_titulo( SIGLA_SISTEMA. ' - Tabelas de Apoio', 'Identificador da Base Salarial' );
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

<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmBaseSalarial" name="frmBaseSalarial" action="" method="post" enctype="multipart/form-data" >

<table id="tblbase" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>
	
	<table id="tblbaseSalarial" class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="title" width="20%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>&nbsp;</strong></td>
			<td <?=$cols1; ?> class="title" width="40%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Educação Superior</strong></td>
			<td <?=$cols1; ?> class="title" width="40%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Educação Profissional</strong></td>
		</tr>
		<tr>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Classes</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2008</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2009</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2010</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2011</strong></td>
			<td <?=$cols; ?> class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2012</strong></td>
			<td <?=$cols; ?> class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2008</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2009</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2010</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2011</strong></td>
			<td class="title" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>2012</strong></td>
		</tr>
		<?php
		foreach ($arDados as $key => $value){ 
			
			$nomeCampoS = 'supvalor_'.$value["bslidS"];
			$idCampoS = "id='$nomeCampoS'";			
			$valor2008S = $value["bslvlr2008S"];
			$valor2009S = $value["bslvlr2009S"];
			$valor2010S = $value["bslvlr2010S"];
			$valor2011S = $value["bslvlr2011S"];
			$valor2012S = $value["bslvlr2012S"];
			
			$nomeCampoP = 'eduvalor_'.$value["bslidP"];
			$idCampoP = "id='$nomeCampoP'";			
			$valor2008P = $value["bslvlr2008P"];
			$valor2009P = $value["bslvlr2009P"];
			$valor2010P = $value["bslvlr2010P"];
			$valor2011P = $value["bslvlr2011P"];
			$valor2012P = $value["bslvlr2012P"];	
			
			$key % 2 ? $cor = "#fcfcfc" : $cor = ""; ?>
			<tr id="tr_<?=$value['bslidS'];?>" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td style="text-align: center;"><?=$value['clsdscS'] ?></td> 
				<?php
				if($tabela == 0){ ?>
				
					<!--  educação superior -->
					
					<td style="text-align: right;">
						<input type="hidden" value="<?=$value["bslidS"]; ?>" name="bslidS_<?=$value["bslidS"]; ?>" id="bslidS_<?=$value["bslidS"]; ?>">
						
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2008S, 'formataValor(this);'); ?>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2009S, 'formataValor(this);'); ?>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2010S, 'formataValor(this);'); ?>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2011S, 'formataValor(this);'); ?>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2012S, 'formataValor(this);'); ?>
					</td>
					<td>&nbsp;</td>
					
					<!--  educação profissional -->
					<td>&nbsp;</td>
					<td style="text-align: right;">
						<input type="hidden" value="<?=$value["bslidP"]; ?>" name="bslidP_<?=$value["bslidP"]; ?>" id="bslidP_<?=$value["bslidP"]; ?>">
						
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2008P, 'formataValor(this);'); ?> </td>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2009P, 'formataValor(this);'); ?> </td>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2010P, 'formataValor(this);'); ?> </td>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2011P, 'formataValor(this);'); ?> </td>
					<td style="text-align: right;">
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2012P, 'formataValor(this);'); ?> </td>	
				<?
				}else{
					//educação superior
					if($tabela == 1){ ?>
						<td style="text-align: right;">
							<input type="hidden" value="<?=$value["bslidS"]; ?>" name="bslidS_<?=$value["bslidS"]; ?>" id="bslidS_<?=$value["bslidS"]; ?>">							
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2008S, 'formataValor(this);'); ?>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2009S, 'formataValor(this);'); ?>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2010S, 'formataValor(this);'); ?>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2011S, 'formataValor(this);'); ?>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 13, 11, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2012S, 'formataValor(this);'); ?>
						</td>	
					<?php
					}else{ ?>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2008S'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2009S'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2010S'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2011S'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2012S'] ?></td>
					<?php
					}
					
					//educação profissional
					if($tabela == 2){ ?>
						<td style="text-align: right;">
							<input type="hidden" value="<?=$value["bslidP"]; ?>" name="bslidP_<?=$value["bslidP"]; ?>" id="bslidP_<?=$value["bslidP"]; ?>">							
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2008P, 'formataValor(this);'); ?> </td>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2009P, 'formataValor(this);'); ?> </td>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2010P, 'formataValor(this);'); ?> </td>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2011P, 'formataValor(this);'); ?> </td>
						<td style="text-align: right;">
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 13, 11, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valor2012P, 'formataValor(this);'); ?> </td>	
					<?php
					}else{ ?>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2008P'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2009P'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2010P'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2011P'] ?></td>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['bslvlr2012P'] ?></td>
					<?php
					}
				} ?>
			</tr>
		<?php
		}
		?>
	</table>
	</td>
</tr>
<tr>
	<th colspan="2" style="text-align: left;">
			<input type="button" name="btnSalvar" value="Salvar" onclick="insereBaseMensal();"> &nbsp; 
			<input type="button" name="btnCancelar" value="Voltar" onclick="voltar();"> 
	</th>
</tr>
</table>
</form>
<div id="erro"></div>

<script type="text/javascript">

function voltar(){
	history.back(1);
}

function validaFormulario(){
	var f = frmBaseSalarial;
	var hid = "";
	for(i = 0; i < f.length; i++){
		if(f.elements[i].type == "hidden"){
			if( f.elements[i].id.indexOf("hid") == -1 ){
				hid = f.elements[i].id;
			}
		}
		if(f.elements[i].type == "text"){
			if(f.elements[i].value == ""){
				alert("Não é permitido campo valor vazio");
				f.elements[i].focus();
				return false;
			}
		}
	}
	return true;
}

function insereBaseMensal(){
	if(validaFormulario()){
		$('loader-container').show();
		
		var arDados = new Array();
		var f = frmBaseSalarial;
		var valorH = "";
		var valor = "";
		
		for(i = 0; i < f.length; i++){
			if(f.elements[i].type == "hidden"){

				if( f.elements[i].id.indexOf("bslid") == 0 ){
					valorH = f.elements[i].value;
				}
			}
			
			if(f.elements[i].type == "text"){
				if(valor == ""){
					valor = valorH + "|" + f.elements[i].value.replace(",", ".");
				}else{
					valor = valor + "|" + valorH + "|" + f.elements[i].value.replace(",", ".");
				}
			}
		}
		
		var myAjax = new Ajax.Request('academico.php?modulo=principal/basesalarial&acao=A',{
					method:		'post',
					parameters: '&insereBaseMensal=true&valor='+valor,
					asynchronous: false,
					onComplete: function(res){
						$('erro').innerHTML = res.responseText;
						if( res.responseText == 1 ){
							alert("Operação realizada com sucesso!");
						}else{
							alert('Operação não realizada, entre em contato com administrador do sistema!');	
						}
					}						
				});
		$('loader-container').hide();
	}
}

function formataValor(valor){
	if(valor.value.indexOf('.') == -1){
		if(valor.value.length > 8){
			valor.value = valor.value.substr(0, 8);
		}
	}
}

function verificaOcorrenciaPonto(valor){
	if( valor.value.indexOf('.') != valor.value.lastIndexOf('.') ){
		valor.value = valor.value.substr(0, valor.value.lastIndexOf('.') ); 		
	}
	
	if(valor.value.length == 9){
		if(valor.value.indexOf('.') == -1){
			valor.value = valor.value.substr(0, 8) + '.' + valor.value.substr(8, 2);
		}
	}
}

function formataDecimal(e) {	
	if(window.event) {
    	/* Para o IE, 'e.keyCode' ou 'window.event.keyCode' podem ser usados. */
        key = e.keyCode;
    }
    else if(e.which) {
    	/* Netscape */
        key = e.which;
    }
    if( ( (key > 47) && (key < 58) ) || (key == 46) || (key == 8) || (key == 9) ){
		return true;
    }else{
    	return false;
    }

} 
</script>
</body>