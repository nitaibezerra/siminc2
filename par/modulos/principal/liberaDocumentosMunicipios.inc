<?php

if( $_POST['libera'] ){
	
	$arrMunicipio	= implode( "', '", $_POST['libera'] );
	$arrSelecionado = implode( "', '", $_POST['hid_select'] );
	
	$sql = "UPDATE par.instrumentounidade SET inutermoliberado = false WHERE itrid = 2 and muncod in ('".$arrSelecionado."')";
	$db->executar( $sql );
	
	$sql = "UPDATE par.instrumentounidade SET inutermoliberado = true WHERE muncod in ('".$arrMunicipio."') and itrid = 2";	
	$db->executar( $sql );
	
	if( $db->commit() ){
		$db->sucesso('principal/liberaDocumentosMunicipios');
	} else {
		echo "<script>
				alert('Falha na operação!');
			</script>";
	}
	exit();
}

$_SESSION['par']['arMuncod'] = pegaResponssabilidade( '2' );
if(!$_SESSION['par']['boPerfilSuperUsuario'] && !$_SESSION['par']['boPerfilConsulta']){
	if( (!isset($_SESSION['par']['arMuncod']) && !count($_SESSION['par']['arMuncod'])) && $_SESSION['par']['muncod'] ){
		header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
		exit;
	}
}

include APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

$db->cria_aba( $abacod_tela, $url, $parametros);

monta_titulo( $titulo_modulo, '' );

# Municipio
$_SESSION['par']['itrid'] = 2;

if( is_array($_POST['ptoid']) ){
	$_SESSION['par']['ptoid'] = ($_SESSION['par']['ptoid'] == $_POST['ptoid']) ? $_SESSION['par']['ptoid'] : $_POST['ptoid'];
}

//$_SESSION['par']['muncod'];

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	$obEntidadeParControle = new EntidadeParControle();
	$sql = $obEntidadeParControle->listaMunicipio($_POST);
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("Código","Município", "UF", "Situação");
	$db->sql_to_excel($sql, 'relListaMunPAR', $cabecalho, $formato);
	unset($_SESSION['par']['sql']);
	exit;

}
$exporta = "false";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

$(document).ready( function(){

	$('#formulario').submit(function() { 
		selectAllOptions( document.getElementById( 'grupo' ) );
        selectAllOptions( document.getElementById( 'ideb' ) );
    });
	
    $('a').mouseover(function() {
		selectAllOptions( document.getElementById( 'grupo' ) );
    	selectAllOptions( document.getElementById( 'ideb' ) );
    } );
    
    $('#btnLiberar').click(function(){
		var boSelect = false;
		$('#formlistamunicipio input').each(function(){
			if( $(this).attr('checked') == true ){
				boSelect = true;
			}
		});
		
		if( boSelect == false ){
			alert('Selecione um município!');
			return false;
		}
		
		$('#formlistamunicipio').submit();
		
	});
    
});

function exportarExcel(){
	document.getElementById('exporta').value = "true";
	selectAllOptions( document.getElementById( 'grupo' ) );
	selectAllOptions( document.getElementById( 'ideb' ) );
	document.getElementById('formulario').submit();
}

function submitFormulario(){
	document.getElementById('exporta').value = "false";
	selectAllOptions( document.getElementById( 'grupo' ) );
	selectAllOptions( document.getElementById( 'ideb' ) );
	document.getElementById('formulario').submit();
}
				
function abrePlanoTrabalho(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'}, 
				  {name : 'entidadePar', value : entidadePar}, 
				  {name : 'estuf', value : estuf},
				  {name : 'muncod', value : muncod},
				  {name : 'inuid', value : inuid}
				 );	
	$.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
									var inuid = msg;
									if(inuid > 0){
										return $(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
										//return $(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa');
									}
					  }
		 });

}

function abrePlanoTrabalhoM(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'}, 
				  {name : 'entidadePar', value : entidadePar}, 
				  {name : 'estuf', value : estuf},
				  {name : 'muncod', value : muncod},
				  {name : 'inuid', value : inuid}
				 );	
	$.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
									var inuid = msg;
									if(inuid > 0){
										//return $(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
										return $(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa');
									}
					  }
		 });

}

function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

//$(function() {
    
//});

</script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" name="pesquisa" value="1>"/>
					<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
					<div style="float: left;">
						<table border="0" cellpadding="3" cellspacing="0">
							<tr>
								<td valign="bottom">
									Município
									<br/>
									<?php 
										$filtro = simec_htmlentities( $_POST['municipio'] );
										$municipio = $filtro;
										echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
									?>
								</td>
								<td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
									?>
								</td>
							</tr>
							<tr>
								<td valign="bottom"><br/>Grupos de Municípios
									<br/>
							<?php
							// Filtros do relatório
							$stSql = "SELECT
										tpmid AS codigo,
										tpmdsc AS descricao
									  FROM
									  	territorios.tipomunicipio mtq
									  WHERE
									  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)";
							
							if( $_REQUEST['grupo'][0] != '' ){
								$grupos = implode( $_REQUEST['grupo'], "," );
								$sql = "SELECT
										tpmid AS codigo,
										tpmdsc AS descricao
									  FROM
									  	territorios.tipomunicipio mtq
									  WHERE
									  	tpmid IN ({$grupos})";
								$grupo = $db->carregar( $sql );
							}

							combo_popup( "grupo", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400 );
										
							?>
								</td>
							</tr>
								<tr>
										<td colspan="2"> 
										<div style="padding-bottom:10px;padding-top:15px;">	
											<label><input type="checkbox" name="naocomecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['naocomecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Não Iniciou preenchimento</label>
											<br><p></p>
											<label><input type="checkbox" name="comecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['comecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Iniciou preenchimento</label>
											<br><p></p>
											<label><input type="checkbox" name="dadosunidadecheck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['dadosunidadecheck'] == 1 ? 'checked="checked"' : '' ?>/> 100% dados da unidade preenchido</label>
											<br><p></p>
											<label><input type="checkbox" name="questoespontuaischeck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['questoespontuaischeck'] == 1 ? 'checked="checked"' : '' ?>/> 100% questões pontuais respondido / iniciou a pontuação</label>
										</td>
									</tr>
								<tr>
									<td colspan="2">
										Situação<br/>
										<?php
								
										
										$esdid = $_POST["esdid"];
										$sql = "select esdid as codigo, esddsc as descricao from workflow.estadodocumento where tpdid = 44 order by esdordem";
										
										$res = $db->carregar( $sql );
										
										$db->monta_combo( "esdid", $res , 'S', 'Todas as Situações', '', '' );
										?>
									</td>
									<td colspan="1">
									</td>
								</tr>
								<tr>
									<td valign="bottom" colspan="2">
										Com PAR / Sem PAR
										<br/>
										<?php 
											$comsempar = $_POST['comsempar'];
											$arrcomsempar = array(0 => array("codigo"=>"compar","descricao"=>"Com Par"), 
															   1 => array("codigo"=>"sempar","descricao"=>"Sem Par"));
											$db->monta_combo( "comsempar", $arrcomsempar, 'S', 'Selecione', '', '' ); 
										?>
									</td>
								</tr>
							<tr>
								<td>
									<br />
									<style>
										#ideb_, #ideb { width: 300px; }
									</style>
									<table border="0" cellpadding="3" cellspacing="0">
										<tr>
											<td width="342">
												Classificação IDEB
											</td>
											<td>
												Filtrar por:
											</td>
										</tr>
									</table>
									
									<?php
					
									$agrupador = new Agrupador( "formulario" );
									
									$sql = sprintf( "SELECT tpmid, tpmdsc FROM territorios.tipomunicipio WHERE gtmid = 2 AND tpmstatus = 'A'" );
									$tipos = $db->carregar( $sql );
									
									$origem = array();
									$destino = array();
									if ( $tipos ) {
										foreach ( $tipos as $tipo ) {
											if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
												$destino[] = array(
													'codigo'    => $tipo['tpmid'],
													'descricao' => $tipo['tpmdsc']
												);
											} else {
												$origem[] = array(
													'codigo'    => $tipo['tpmid'],
													'descricao' => $tipo['tpmdsc']
												);
											}
										}
									}
									
									$agrupador->setOrigem( "ideb_", null, $origem );
									$agrupador->setDestino( "ideb", null, $destino );
									$agrupador->exibir();
				
									?>
								</div>
								<?php /* ?>
								<label><input type="checkbox" name="adesao_proinf" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['adesao_proinf'] == 1 ? 'checked="checked"' : '' ?>/> Com Adesão do PROINFÂNCIA</label>
								<?php */ ?>								
								</td>
							</tr>
						</table>
					<div style="float: left;">
						<input type="button" name="pesquisar" value="Pesquisar" onclick="submitFormulario();"/>
						<?php 
						$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
						if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)  ){
						?>
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
						<?php } ?>
						<input type="button" name="btnLiberar" id="btnLiberar" value="Liberar Documento" />
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
$obEntidadeParControle = new EntidadeParControle();
$obEntidadeParControle->listaMunicipio($_POST, 'doc');
?>
<div id="divDebug"></div>