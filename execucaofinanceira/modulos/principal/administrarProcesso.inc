<?php

if( $_POST['requisicao'] == 'carregarmunicipio' ){
	$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio where estuf = '{$_POST['estuf']}' order by mundescricao asc";
	$arrMunicipio = $db->carregar($sql);
	$arrMunicipio = $arrMunicipio ? $arrMunicipio : array();
	
	$html = '<select name="muncod" id="muncod" class="CampoEstilo" style="width: auto" title="Município">
				<option value="">Todas as Unidades Federais</option>';
	
	foreach ($arrMunicipio as $v) {
		$html.= '<option value="'.$v['codigo'].'">'.$v['descricao'].'</option>';
	}
	$html.= '</select>'.obrigatorio();
	
	
	echo $html;
	exit();
}

//include APPRAIZ . "execucaofinanceira/classes/processos.class.inc";
$obProcesso = new Processo();

if( $_POST['requisicao'] == 'salvar' ){
	//ver($_POST,d);
	$pronumeroprocesso = str_replace('.', '', $_POST['pronumeroprocesso']);
	$pronumeroprocesso = str_replace('/', '', $pronumeroprocesso);
	$pronumeroprocesso = str_replace('-', '', $pronumeroprocesso);
	
	$procnpj = str_replace('.', '', $_POST['procnpj']);
	$procnpj = str_replace('/', '', $procnpj);
	$procnpj = str_replace('-', '', $procnpj);
	
	$obProcesso->proid 				= $_POST['proid'];
	$obProcesso->pronumeroprocesso 	= $pronumeroprocesso;
	$obProcesso->procnpj 			= $procnpj;
	$obProcesso->proesfera 			= trim($_POST['esfera']);
	$obProcesso->muncod 			= $_POST['muncod'];
	$obProcesso->estuf 				= ($_POST['estuf'] ? $_POST['estuf'] : $_POST['estuf_mun']);
	$obProcesso->tipid 				= $_POST['tipid'];
	$obProcesso->tprid 				= $_POST['tprid'];
	
	$proid = $obProcesso->salvar();
	$_SESSION['execucaofinanceira']['proid'] = ($obProcesso->proid ? $obProcesso->proid : $proid);
	if($obProcesso->commit()){
		echo "<script>
					alert('Operação realizada com sucesso');
					window.location.href = 'execucaofinanceira.php?modulo=principal/administrarProcesso&acao=A';
	          </script>";
		exit();
	} else {
		echo "<script>
				alert('Falha na operação');
			</script>";
	}
}
if( $_GET['processo'] ){
	unset($_SESSION['execucaofinanceira']);
	$_SESSION['execucaofinanceira']['proid'] = $_GET['processo'];
}

if($_SESSION['execucaofinanceira']['proid']){

	$processo = $_SESSION['execucaofinanceira']['proid'];
	$obProcesso->carregarPorId($processo);
	$proid				= $obProcesso->proid;
	$procnpj			= $obProcesso->procnpj;
	$estuf 				= $obProcesso->estuf;
	$muncod 			= $obProcesso->muncod;
	$tipid 				= $obProcesso->tipid;
	$tprid 				= $obProcesso->tprid;
	$esfera				= trim($obProcesso->proesfera);
	$pronumeroprocesso 	= $obProcesso->mascaraglobal($obProcesso->pronumeroprocesso, "#####.######/####-##");
	$procnpj 			= $obProcesso->mascaraglobal($procnpj, "##.###.###/####-##");

}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);

monta_titulo( $titulo_modulo, '' );
if( $_REQUEST['acao'] != 'C' )
$obProcesso->montaCabecalhoProcesso();
//echo '<br>';
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="proid" id="proid" value="<?=$proid; ?>">
	<table align="center" bgcolor="#f5f5f5" width="95%" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td colspan="2" style="height: 05px;"></td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloesquerda" style="text-align: center;"><b>Dados do Processo</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="40%" style="font-weight: bold;">Classificação da Entidade:</td>
			<td >
			<input type="radio" name="esfera" <?=($esfera == 'E' ? 'checked="checked"' : '') ?> id="esfera_e" onclick="selecionaTipo(this.value)" value="E"> Secretaria Estadual<br>			
			<input type="radio" name="esfera" <?=($esfera == 'M' ? 'checked="checked"' : '') ?> id="esfera_m" onclick="selecionaTipo(this.value)" value="M"> Prefeitura Municipal<br>
			<input type="radio" name="esfera" <?=($esfera == 'P' ? 'checked="checked"' : '') ?> id="esfera_p" onclick="selecionaTipo(this.value)" value="P"> Entidade Privada<br>
			<input type="radio" name="esfera" <?=($esfera == 'F' ? 'checked="checked"' : '') ?> id="esfera_f" onclick="selecionaTipo(this.value)" value="F"> Entidade Federal</td>
		</tr>
		<tr id="tr_estuf">
			<td class="subtitulodireita" width="40%" style="font-weight: bold;">Estado:</td>
			<td><?
				//$estuf = $_POST['estuf'];
				$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
				 $db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '', '', '', 'S', 'estuf', '', '', 'Estado' ); ?></td>
		</tr>
		<tr id="tr_estuf_mun">
			<td class="subtitulodireita" width="40%" style="font-weight: bold;">Estado:</td>
			<td><?
				if( $muncod && !$estuf) $estuf = $db->pegaUm("select estuf from territorios.municipio where muncod = '$muncod'");
				$estuf_mun = $estuf;
				$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
				 $db->monta_combo( "estuf_mun", $sql, 'S', 'Todas as Unidades Federais', 'carregaMunicipio', '', '', '', 'S', 'estuf_mun', '', '', 'Estado'); ?></td>
		</tr>
		<tr id="tr_muncod">
			<td class="subtitulodireita" width="40%" style="font-weight: bold;">Município:</td>
			<td><div id="combomunicipio"><?
				if( $estuf_mun != '' ) $filtroMuni = " where estuf = '{$estuf_mun}'";
				
				$sql = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio $filtroMuni order by mundescricao asc";
				 $db->monta_combo( "muncod", $sql, 'S', 'Todas os Municípios', '', '', '', '', 'S', 'muncod', '', '', 'Município' ); ?></div></td>
		</tr>
	</table>
	<div id="div_tabela">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tbody>
			<tr>
				<td class="subtituloDireita"  style="font-weight: bold; width: 20%">Número do Processo:</td>
				<td>
					<? echo campo_texto('pronumeroprocesso', 'S', 'S', 'Número do Processo', 30, 25, '#####.######/####-##', '', '', '', 0, 'id="pronumeroprocesso"'); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita"  style="font-weight: bold;" width="40%">CNPJ da Entidade:</td>
				<td>
					<? echo campo_texto('procnpj', 'S', 'S', 'CNPJ da Entidade', 20, 18, '##.###.###/####-##', '', '', '', 0, 'id="procnpj"', '', '', 'validarCNPJ(this)'); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita"  style="font-weight: bold;">Tipo do Processo:</td>
				<td>
					<? 
					$sql = "SELECT tipid as codigo, tipdescricao as descricao FROM execucaofinanceira.tipoprocesso WHERE tipstatus = 'A'";
					$db->monta_combo( "tipid", $sql, 'S', 'Todas os Tipos do Processo', '', '', '', '', 'S', 'tipid', '', '', 'Tipo do Processo' ); ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloDireita"  style="font-weight: bold;">Tipo de Execução:</td>
				<td>
					<? 
					$sql = "SELECT tprid as codigo, tprdescricao as descricao FROM execucaofinanceira.tipoexecucao WHERE tprstatus = 'A'";
					$db->monta_combo( "tprid", $sql, 'S', 'Todas os Tipos de Execução', '', '', '', '', 'S', 'tprid', '', '', 'Tipo de Execução' ); ?>
				</td>
			</tr>
			<tr>
				<td colspan="2" class="subtituloCentro"  style="font-weight: bold; background-color: #dcdcdc;">
					<input type="button" name="btnSalvar" id="btnSalvar" value="Salvar" <?=verificaPermissaoPerfil(); ?>>
				</td>
			</tr>
		</tbody>
	</table>
	</div>
</form>
<?
	$obProcesso->carregaListaDadosProcesso();
?>
<div id="debug"></div>
<script type="text/javascript">
function validarCNPJ( obj ){

	if( obj.value != '' ){
		var cnpj = replaceAll(obj.value, '.', '');
		cnpj = replaceAll(cnpj, '/', '');
		cnpj = replaceAll(cnpj, '-', '');
		if( !validarCnpj( cnpj  ) ){
			alert( "CNPJ inválido!\nFavor informar um CNPJ válido!" );
			$("#procnpj").val('');
		}
	}	
	
}

$(document).ready(function(){
	$('#tr_estuf_mun').hide();
	$('#tr_estuf').hide();
    $('#botao').hide();
	$('#tr_muncod').hide();
	$('#div_tabela').hide();
	
	if( $('#esfera_e').attr('checked') == true  ){
		selecionaTipo('E');
		$('#div_tabela').show();
	}else if( $('#esfera_m').attr('checked') == true ) {
		selecionaTipo('M');
		$('#tr_muncod').show();
		$('#div_tabela').show();
	}else if( $('#esfera_p').attr('checked') == true ) {
		selecionaTipo('P');
		$('#tr_muncod').show();
		$('#div_tabela').show();
	}else if( $('#esfera_F').attr('checked') == true ) {
		selecionaTipo('F');
		$('#div_tabela').show();
	}

	//if( $('#muncod').val() == '' ) $('#tr_muncod').css('display', 'none');
	//if( $('#estuf').val() == '' ) $('#tr_estuf').css('display', 'none');
	
	$('input[name=btnSalvar]').click(function(){
		var erro = 0;
		var campo = '';
		$("#formulario input, #formulario select").each(function() { 
			if(!this.value){				
				if( $('#tr_'+this.id).css('display') != 'none' && this.type != 'hidden') {
					erro = 1;
					campo = campo + '-->'+this.title+'\n';	
				}
			}
		});
		if(erro == 0){
			$('#requisicao').val('salvar');
			$('#formulario').submit();
		} else {
			alert('O(s campo(s) abaixo são obrigatório seu preenchimento!\n'+campo);
			return false;
		}
	});
});

function carregaMunicipio(estuf){
	if( estuf != '' ){
		$.ajax({
	   		type: "POST",
	   		url: "execucaofinanceira.php?modulo=principal/administrarProcesso&acao=<?=$_REQUEST['acao'] ?>",
	   		data: "requisicao=carregarmunicipio&estuf="+estuf,
	   		async: false,
	   		success: function(msg){
	   			$('#tr_muncod').show();
	   			$('#botao').show();
	   			document.getElementById('combomunicipio').innerHTML = msg;
	   		}
		});
	} else {
		$('#tr_muncod').hide();
	}
}

function selecionaTipo(valor){
	if(valor == 'E' || valor == 'F'){
		$('#tr_estuf').show();
		$('#tr_muncod').hide();
		$('#tr_estuf_mun').hide();
		$('#div_tabela').show();
		$('#estuf_mun').val('');
		$('#muncod').val('');
	} else {
		$('#tr_estuf').hide();
		$('#tr_estuf_mun').show();
		$('#tr_muncod').hide();
		$('#div_tabela').show();
		$('#estuf').val('');
	}
}

</script>