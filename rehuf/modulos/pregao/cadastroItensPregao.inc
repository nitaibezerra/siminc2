<?php

if ( $_REQUEST['pesquisaItem'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	$reg = pesquisaItem( $_REQUEST['itecatmat'],$_REQUEST['iteapresentacao']);
	echo (($reg)?"TRUE":"FALSE");
	exit;
}

if ( $_REQUEST['insereItens'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	insereItensPregao( $_REQUEST['itecatmat'], $_REQUEST['itedescricao'], $_REQUEST['iteapresentacao'], $_REQUEST['gruid'], $_REQUEST['itlid'], $_REQUEST['itecodsus']);
	exit;
}

if ( $_REQUEST['atulizaItens'] ) {
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaItensPregao($_REQUEST['itgid'], $_REQUEST['iteid'], $_REQUEST['itecatmat'], $_REQUEST['itedescricao'], $_REQUEST['iteapresentacao'], $_REQUEST['gruid'], $_REQUEST['itlid'], $_REQUEST['itecodsus']);
	exit;
}

global $db;

if($_REQUEST['iteid']){
	$sql = "SELECT 
			  iteid,
			  itecatmat,
			  itedescricao,
			  iteapresentacao,
			  itlid,
			  itecodsus
			FROM 
			  rehuf.item
			WHERE iteid = {$_REQUEST['iteid']}";
	$registro = $db->pegaLinha($sql);

	$iteid 			 = $registro['iteid'];
	$itecatmat 		 = $registro['itecatmat'];
	$itedescricao 	 = $registro['itedescricao'];
	$iteapresentacao = $registro['iteapresentacao'];
	$itlid			 = $registro['itlid'];
	$itecodsus		 = $registro['itecodsus'];
	
	/*$sql = "select ig.itgid from rehuf.itemgrupo ig
				where ig.iteid = $iteid";*/
	
	$sql = " select ig.gruid from rehuf.itemgrupo ig
				where ig.iteid = $iteid
				 AND itgstatus = 'A'";
				
	$arGrupo = $db->carregar($sql);
	$itgid = "";
	$i = 0;

	if($arGrupo){
		foreach ($arGrupo as $key => $value) {
			if($itgid == ""){
				$itgid = $value['gruid'];
			}else{
				$itgid = $itgid.",".$value['gruid'];
			}
			$i++; 	
		}
	}

	$sql = "SELECT 
			  ig.gruid as codigo,
			  g.grunome as descricao
			FROM 
			  rehuf.itemgrupo ig inner join rehuf.grupoitens g
			  on (ig.gruid = g.gruid)
			WHERE iteid = $iteid
			  AND ig.itgstatus = 'A'";
	
	$gruid = $db->carregar($sql);
}else{
	$iteid 			 = "";
	$itecatmat 		 = "";
	$itedescricao 	 = "";
	$iteapresentacao = "";
}

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Cadastro de Itens', '' );
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
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="frmCadItensPregao" name="frmCadItensPregao" action="" method="post" enctype="multipart/form-data" >

<input type="hidden" name="iteid" id="iteid" value="<?=$iteid ?>">
<input type="hidden" name="itgid" id="itgid" value="<?=$itgid ?>">

<table id="tblCadItensPregao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:20%;">CATMAT:</td>
		<td colspan="2"><?=campo_texto( 'itecatmat', 'S', 'S', '', 40, 10, '[#]', '','','','','id="itecatmat"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição:</td>
		<td colspan="2"><?=campo_textarea('itedescricao','S', 'S', '', 80, 05, 1000,'');?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Apresentação:</td>
		<td colspan="2"><?=campo_texto( 'iteapresentacao', 'S', 'S', '', 40, 100, '', '','','','','id="iteapresentacao"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Lote:</td>
		<td colspan="2"><? $sql = "SELECT itlid as codigo, itlabrev as descricao FROM rehuf.itemlote WHERE itlstatus='A'"; 
						   $db->monta_combo("itlid", $sql, "S", "Selecione...", '', '', '', '250', 'N', 'itlid'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Código SUS:</td>
		<td colspan="2"><?=campo_texto( 'itecodsus', 'S', 'S', '', 20, 30, '##.##.##.###-#', '','','','','id="itecodsus"'); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Grupo:</td>
		<td><?
			$sql = "SELECT	gruid as codigo, gruid ||' - '|| grunome as descricao
						FROM rehuf.grupoitens
					WHERE grustatus = 'A'";
			combo_popup('gruid', $sql, 'Orgão', "400x400", 0, array(), "", (($gruid[0])?"S":"S"), true, false, 5, 400);
		?><img src="../imagens/obrig.gif" border="0">
		</td>
	</tr>
	<tr>
		<th align="center" colspan="2"><input type="button" value="Salvar" name="btnSalvar" onclick="validaCadItens();">
		<input type="button" value="Cancelar" name="btnCancelar" onclick="Voltar();"></th>
	</tr>
</table>
</form>
<div id="erro"></div>
<script type="text/javascript">
function validaCadItens(){
	var opt = "";
	
	if($('itecatmat').value == ""){
		alert("O campo CATMAT é de preenchimento obrigatório!");
		$('itecatmat').focus();
		return false;
	}else if($('itedescricao').value == ""){
		alert("O campo Descrição é de preenchimento obrigatório!");
		$('itedescricao').focus();
		return false;
	}else if($('iteapresentacao').value == ""){
		alert("O campo Apresentação é de preenchimento obrigatório!");
		$('iteapresentacao').focus();
		return false;
	}else{
		
		for (var i=0; i< $('gruid').length; i++) {
			if(opt == ""){
				opt = $('gruid').options[i].value;
			}else{
				opt = opt + "|" + $('gruid').options[i].value;
			}
		}
		if(opt == ""){
			alert("Selecione pelo menos um grupo!");
			$('gruid').focus();
			return false;
		}else{
			arr = opt.split("|");
			
			$('loader-container').show();
			
			var solicitarconfirmacao=0;
			
			if($('iteid').value == ""){
			
				var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroItensPregao&acao=A',{
											method:		'post',
											parameters: '&pesquisaItem=true&itecatmat='+$('itecatmat').value+
														'&iteapresentacao='+$('iteapresentacao').value,
											asynchronous: false,
											onComplete: function(res){
												if(res.responseText=="TRUE") {
													solicitarconfirmacao=1;												
												}

											}						
										});
										
				if(solicitarconfirmacao==1) {
					if(!confirm("Já existe um item com esse catmat e apresentação. Deseja continuar?")) {
						$('loader-container').hide();
						return false;
					}
				}

				var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroItensPregao&acao=A',{
											method:		'post',
											parameters: '&insereItens=true&itecatmat='+$('itecatmat').value+
																		 '&'+$('itedescricao').serialize(true)+
																		 '&iteapresentacao='+$('iteapresentacao').value+
																		 '&gruid='+arr+'&itlid='+$('itlid').value+'&itecodsus='+$('itecodsus').value,
											asynchronous: false,
											onComplete: function(res){
												
												if( res.responseText == 1 ){
													alert("Operação realizada com sucesso!");
													Voltar();
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
				var myAjax = new Ajax.Request('rehuf.php?modulo=pregao/cadastroItensPregao&acao=A',{
											method:		'post',
											parameters: '&atulizaItens=true&itgid='+$('itgid').value+
																		 '&iteid='+$('iteid').value+
																		 '&itecatmat='+$('itecatmat').value+
																		 '&'+$('itedescricao').serialize(true)+
																		 '&iteapresentacao='+$('iteapresentacao').value+
																		 '&gruid='+arr+'&itlid='+$('itlid').value+'&itecodsus='+$('itecodsus').value,
											asynchronous: false,
											onComplete: function(res){

												if( Number(res.responseText)){
													alert("Operação realizada com sucesso!");
													Voltar();
												}else{
													alert('Operação não realizada, entre em contato com administrador do sistema!');	
												}
											}						
										});
			}
		$('loader-container').hide();
		}
	}
}

function Voltar(){
	window.location.href = 'rehuf.php?modulo=pregao/listaItemPregao&acao=A';
}

document.observe("dom:loaded", function() {
	document.getElementById('itedescricao').style.textTransform='uppercase';
	document.getElementById('iteapresentacao').style.textTransform='uppercase'; 
});
</script>
