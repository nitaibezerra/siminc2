<?php
global $db;

if ( $_REQUEST['insereTabelaApoio'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	insereTabelaApoio($_REQUEST);
	exit;
}

$array = academico_pega_orgao_permitido();

if(count($array) == 2 ){
	$visualiza = 2;
	$tabela = 0;
}else{
	$visualiza = 1;
	$tabela = $array[0]['id'];
}
		   
function insereTabelaApoio($request){
	global $db;
	
	$impid = array();
	$impvalor = array();
	
	$impid = explode('|', $request['impid']);
	$impvalor = explode('|', $request['impvalor']);
	
	$i = 0;

	foreach ($impid as $key => $value) {
		
		foreach ($impvalor as $key1 => $value1) {
			if($key == $key1){
				
				$sql = "UPDATE
						    academico.indiceimpacto
						SET
						    impvalor = {$impvalor[$key1]}
						WHERE
						    impid = {$impid[$key]}";

				$db->executar($sql);
				break;
			}	
		}
	}
	$res = $db->commit();
	if($res == "1"){
		echo $res;
	}else{
		echo "0";
	}
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$arDados = array();

//educação Superior
$sql = "SELECT
		    impid,
		    orgid,
		    impsequencia,
		    impmes,
		    impvalor
		FROM
		    academico.indiceimpacto
		order by impsequencia, orgid";

$dados = $db->carregar($sql);

foreach ($dados as $key => $value){
	if($value['orgid'] == 1){
		$impid = $value['impid'];
		$orgid = $value['orgid'];
		$impmes = $value['impmes'];
		$impvalor = $value['impvalor'];
	}else{
		$array = Array("impidS"    => $impid,
					   "orgidS"    => $orgid,
					   "impmesS"   => $impmes,
		 			   "impvalorS" => $impvalor,
					   "impidP"    => $value['impid'],
					   "orgidP"    => $value['orgid'],
					   "impmesP"   => $value['impmes'],
		 			   "impvalorP" => $value['impvalor']);
		
		array_push($arDados, $array);		
	}
}

monta_titulo( SIGLA_SISTEMA. ' - Tabelas de Apoio', 'Índice de Impacto Financeiro' );

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
<script language="JavaScript" src="../includes/wz_tooltip.js"></script>
<body>

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmTabelaApoio" name="frmTabelaApoio" action="" method="post" enctype="multipart/form-data" >

<table id="tblApoio" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td>

	<table id="tblApoio" class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 500px;">
		<tr>
			<td class="title" width="20%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Mês</strong></td>
			<td class="title" width="40%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Educação Superior</strong></td>
			<td class="title" width="40%" bgcolor="e9e9e9" align="center" valign="top" onmouseout="this.bgColor='e9e9e9';" onmouseover="this.bgColor='#c0c0c0';" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); height: 25px;"><strong>Educação Profissional</strong></td>
		</tr><?php
		foreach ($arDados as $key => $value){
			
			$nomeCampoS = 'supvalor_'.$value["impidS"];
			$idCampoS = "id='$nomeCampoS'";			
			$valorS = $value["impvalorS"];
			
			$nomeCampoP = 'eduvalor_'.$value["impidP"];
			$idCampoP = "id='$nomeCampoP'";			
			$valorP = $value["impvalorP"];			
			
			$key % 2 ? $cor = "#fcfcfc" : $cor = ""; ?>
			<tr id="tr_<?=$value['impidS'];?>" bgcolor="<?=$cor ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td style="text-align: center;"><?=$value['impmesS'] ?></td>
				<?php
				
				if($tabela == 0){ ?>
					<td style="text-align: right;">
						<input type="hidden" value="<?=$value["impidS"]; ?>" name="impid_<?=$value["impidS"]; ?>" id="impid_<?=$value["impidS"]; ?>">
						
						<?= campo_texto( $nomeCampoS, 'N', 'S', '', 25, 17, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valorS, 'formataValor(this);'); ?>
					</td>
					<td style="text-align: right;">
						<input type="hidden" value="<?=$value["impidP"]; ?>" name="impid_<?=$value["impidP"]; ?>" id="impid_<?=$value["impidP"]; ?>">
						
						<?= campo_texto( $nomeCampoP, 'N', 'S', '', 25, 17, '', '',' right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valorP, 'formataValor(this);'); ?>
					</td>	
				<?
				}else{				
					//educação superior
					if($tabela == 1){ ?>
						<td style="text-align: right;">
							<input type="hidden" value="<?=$value["impidS"]; ?>" name="impid_<?=$value["impidS"]; ?>" id="impid_<?=$value["impidS"]; ?>">
							
							<?= campo_texto( $nomeCampoS, 'N', 'S', '', 28, 17, '', '','right','','',$idCampoS." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valorS, 'formataValor(this);'); ?>
						</td>	
					<?php
					}else{ ?>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['impvalorS'] ?></td>	
					<?php
					}
					?>
					
					<?php
					//educação profissional
					if($tabela == 2){ ?>
						<td style="text-align: right;">
							<input type="hidden" value="<?=$value["impidP"]; ?>" name="impid_<?=$value["impidP"]; ?>" id="impid_<?=$value["impidP"]; ?>">
							
							<?= campo_texto( $nomeCampoP, 'N', 'S', '', 28, 17, '', '','right','','',$idCampoP." onkeypress='return formataDecimal(event);'", 'verificaOcorrenciaPonto(this);', $valorP, 'formataValor(this);'); ?>
						</td>	
					<?php
					}else{ ?>
						<td align="right" title="Valor" style="color: rgb(0, 102, 204);"><?=$value['impvalorP'] ?></td>	
					<?php
					}
				}
				?>
			</tr>
		<?php
		}
		?>
	</table>
	</td>
</tr>
<tr>
	<th colspan="2" style="text-align: left;">
			<input type="button" name="btnSalvar" value="Salvar" onclick="insereTabelaApoio();"> &nbsp; 
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
	var f = frmTabelaApoio;
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

function insereTabelaApoio(){
	if(validaFormulario()){
		$('loader-container').show();
		
		var f = frmTabelaApoio;
		var valorH = "";
		var valor = "";
		
		for(i = 0; i < f.length; i++){
			if(f.elements[i].type == "hidden"){
				if( f.elements[i].id.indexOf("hid") == -1 ){
					if(!f.elements[i].disabled ){
						if(valorH == ""){
							valorH = f.elements[i].value;
						}else{
							valorH = valorH + "|" + f.elements[i].value;
						}
					}
				}
			}
			
			if(f.elements[i].type == "text"){
				if(!f.elements[i].disabled ){
					if(valor == ""){
						valor = f.elements[i].value.replace(",", ".");
					}else{
						valor = valor + "|" + f.elements[i].value.replace(",", ".");
					}
				}
			}
		}
		var myAjax = new Ajax.Request('academico.php?modulo=principal/tabela_apoio&acao=A',{
					method:		'post',
					parameters: '&insereTabelaApoio=true&impid='+valorH+
													   '&impvalor='+valor,
					asynchronous: false,
					onComplete: function(res){
 
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
		if(valor.value.length > 6){
			valor.value = valor.value.substr(0, 6);
		}
	}
}

function verificaOcorrenciaPonto(valor){
	if( valor.value.indexOf('.') != valor.value.lastIndexOf('.') ){
		valor.value = valor.value.substr(0, valor.value.lastIndexOf('.') ); 		
	}
	
	if(valor.value.length == 7){
		if(valor.value.indexOf('.') == -1){
			valor.value = valor.value.substr(0, 6) + '.' + valor.value.substr(6, 7);
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