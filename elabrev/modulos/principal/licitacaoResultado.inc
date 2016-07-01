<?php

if(!$_SESSION['elabrev_var']['lbrid']) {
	
	echo "<script>
			alert('Problemas nas variaveis enviadas');
			window.location='elabrev.php?modulo=principal/liberacaoorcamentaria&acao=A';
		  </script>";
	exit;
	
}

require_once APPRAIZ . 'includes/classes/entidades.class.inc';
include_once APPRAIZ . 'includes/workflow.php';

define(FUN_EMPRESA_LICITACAO_CDO , 102);

//recupera o entid
$sql = "SELECT entid, lilnulicitacao, lilnucontrato, lilvalortotal  FROM elabrev.liberacaolicitacao where lilstatus='A' and lbrid = ".$_SESSION['elabrev_var']['lbrid'];
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);


if($_REQUEST['opt'] == 'salvarRegistro') {

	if($entid){
		//desativa funid da antiga empresa
		$sql = "UPDATE entidade.funcaoentidade SET fuestatus='I' WHERE funid = ".FUN_EMPRESA_LICITACAO_CDO." and entid = $entid";
		$db->executar($sql);
	}
	
	//insere novo coordenador
	$entidade = new Entidades();
	$entidade->carregarEntidade($_REQUEST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();

	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='elabrev.php?modulo=principal/licitacaoResultado&acao=A';
		  </script>";
	exit;
}

if($_REQUEST['opt'] == 'salvarLicitacao') {

	if($entid){
		$sql = "DELETE FROM elabrev.liberacaolicitacao WHERE lbrid = ".$_SESSION['elabrev_var']['lbrid'];
		$db->executar($sql);
	}
	
	$valortotal = $_REQUEST['lilvalortotal'];
	if($valortotal){
		$valortotal = str_replace(",",".", str_replace(".","", $valortotal));
	}
	else{
		$valortotal = 'null';
	}
	
	
	$sql = "INSERT INTO elabrev.liberacaolicitacao(lbrid, entid, lilvalortotal, lilnulicitacao, lilnucontrato, lildata, lilstatus)
		    VALUES (".$_SESSION['elabrev_var']['lbrid'].", 
		    		".($entid ? $entid : 'null').", 
		    		".($valortotal).",
		    		".($_REQUEST['lilnulicitacao'] ? "'".$_REQUEST['lilnulicitacao']."'" : 'null').", 
		    		".($_REQUEST['lilnucontrato'] ? "'".$_REQUEST['lilnucontrato']."'" : 'null').", 
		    		now(), 
		            'A')";
	$db->executar($sql);
	$db->commit();
	
		
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='elabrev.php?modulo=principal/licitacaoResultado&acao=A';
		  </script>";
	exit;
}


if($_REQUEST['opt'] == 'salvarCredito') {

	$vlcredito = $_REQUEST['vlcredito'];
	if($vlcredito){
		$vlcredito = str_replace(",",".", str_replace(".","", $vlcredito));
	}
	else{
		$vlcredito = 'null';
	}
	
	
	$sql = "INSERT INTO elabrev.liberacaocredito(lbrid, lbcvalor, lbcdata, lbcstatus)
		    VALUES (".$_SESSION['elabrev_var']['lbrid'].", 
		    		".($vlcredito).",
		    		now(), 
		            'A')";
	$db->executar($sql);
	$db->commit();
	
		
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='elabrev.php?modulo=principal/licitacaoResultado&acao=A';
		  </script>";
	exit;
}


if($_REQUEST['excluircred']) {

	$sql = "UPDATE elabrev.liberacaocredito SET lbcstatus='I' WHERE lbcid = ".$_REQUEST['excluircred'];
	$db->executar($sql);
	$db->commit();
	
		
	echo "<script>
			alert('Dados excluidos com sucesso');
			window.location='elabrev.php?modulo=principal/licitacaoResultado&acao=A';
		  </script>";
	exit;
}



require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
echo montarAbasArray(menuAbasLiberacao(), $_SERVER['REQUEST_URI']);

cabecalhoLiberacao($_SESSION['elabrev_var']['lbrid']);
	
//$titulo = "Licitação / Resultado";
//$subtitulo = "Cadastro de Empresa / Licitação / Contrato";
//echo monta_titulo($titulo, $subtitulo);
//echo cabecalhoMaisEducacao();
	


/*
 * CÓDIGO DO NOVO COMPONENTE 
 */
$entidade = new Entidades();

if($entid){
	$entidade->carregarPorEntid($entid);
}

echo '<div id="abas" >
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu">Dados da Empresa</li>
			</ul>
		</div>
		<!-- Dados Empresa -->
		<div id="aba_dadosempresa" class="conteudoAba">';

echo "<table border=0 cellpading=0 cellspacing=0 width='100%'>
		<tr><td width='90%'>";

echo $entidade->formEntidade("elabrev.php?modulo=principal/licitacaoResultado&acao=A&opt=salvarRegistro",
							 array("funid" => FUN_EMPRESA_LICITACAO_CDO, "entidassociado" => $entid),
							 array("enderecos"=>array(1))
							 );
 
							 echo "</td>";
echo "<td width='10%' valign=top>";	

//$estadoAtual = pegarEstadoAtual( $_SESSION['elabrev_var']['lbrid'] );
//$dados_wf = array('adpid' => $_SESSION['par']['adpid']);
//wf_desenhaBarraNavegacao($docid, $dados_wf);

$docid = criarDocumento( $_SESSION['elabrev_var']['lbrid'] );
wf_desenhaBarraNavegacao( $docid , array( 'lbrid' => $_SESSION['elabrev_var']['lbrid'], 'url' => $_SESSION['favurl'] ) );

echo "</td></tr></table>";

echo "</div></div>";
							 
?>

<div id="abas" >
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100px">Dados da Licitação</li>
			</ul>
		</div>
		<!-- Dados Licitação -->
		<div id="aba_dadoslicitacao" class="conteudoAba">
			<form name="formLicitacao" id="formLicitacao" method="post" action="elabrev.php?modulo=principal/licitacaoResultado&acao=A&opt=salvarLicitacao">
			
			<table width="15%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td align="left" width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<?php 
								$sql = "select coalesce(sum(lcdvalor),0) as total from elabrev.liberacaocdo where lcdstatus = 'A' and lbrid = ".$_SESSION['elabrev_var']['lbrid'];
								$totalCdo = $db->pegaUm($sql);
							?>
							<strong>Valor Total CDO R$:<div id="totalCdo" style="float:right"><?=number_format($totalCdo,2,",",".");?></div></strong>
						</td>
					</tr>
				</thead>
			</table>
			
			<br>
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td width="40%" class="SubTituloDireita">Número da Licitação:</td>
					<td>
						<?$lilnulicitacao = ($lilnulicitacao ? $lilnulicitacao : $_REQUEST['lilnulicitacao']);?>
						<?=campo_texto('lilnulicitacao', 'S', 'S', 'Número da Licitação', 30, 30, '', '','','','','id="lilnulicitacao"'); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Número do Contrato:</td>
					<td>
						<?$lilnucontrato = ($lilnucontrato ? $lilnucontrato : $_REQUEST['lilnucontrato']);?>
						<?=campo_texto('lilnucontrato', 'S', 'S', 'Número do Contrato', 30, 30, '', '','','','','id="lilnucontrato"'); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Valor Total Licitação:</td>
					<td>
						<?
						$lilvalortotal = ($lilvalortotal ? $lilvalortotal : $_REQUEST['lilvalortotal']);
						if($lilvalortotal) $lilvalortotal = number_format($lilvalortotal,2,",",".");
						?>
						
						<?=campo_texto('lilvalortotal', 'S', 'S', 'Valor Total', 15, 13, '[###.]###,##', '','','','','id="lilvalortotal"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloCentro" colspan="2" >
						<input type="submit" id="btngravarlicitacao" value="Gravar">
					</td>
				</tr>
			</table>
			</form>
		</div>
</div>

<div id="abas" >
		<div class="abaMenu">
			<ul id="listaAbas">
				<li class="abaItemMenu" style="width: 100px">Dados de Crédito</li>
			</ul>
		</div>
		<!-- Dados Crédito -->
		<div id="aba_dadoscredito" class="conteudoAba">
			<form name="formCredito" id="formCredito" method="post" action="elabrev.php?modulo=principal/licitacaoResultado&acao=A&opt=salvarCredito">
			
			<table width="15%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>
						<td align="left" width="90" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
							<?php 
								$sql = "select coalesce(sum(lbcvalor),0) as total from elabrev.liberacaocredito where lbcstatus = 'A' and lbrid = ".$_SESSION['elabrev_var']['lbrid'];
								$totalCredito = $db->pegaUm($sql);
							?>
							<strong>Valor Total Crédito R$:<div id="totalCredito" style="float:right"><?=number_format($totalCredito,2,",",".");?></div></strong>
						</td>
					</tr>
				</thead>
			</table>
			
			<br>
			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
				<tr>
					<td width="40%" class="SubTituloDireita">Valor do Crédito:</td>
					<td>
						<?=campo_texto('vlcredito', 'S', 'S', 'Valor do Crédito', 15, 13, '[###.]###,##', '','','','','id="vlcredito"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value);"); ?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloCentro" colspan="2" >
						<input type="submit" id="btngravarcredito" value="Gravar">
					</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<table width="50%" align="center">
							<tr>
								<td>
									<?php 
									$sql = "select '<center><a href=\'javascript:excluirCredito('||lbcid||');\'><img border=0 src=\'../imagens/excluir.gif\'></center>' as acao,
													lbcvalor,
													'<center>'||to_char(lbcdata::date, 'DD/MM/YYYY')||'</center>' as lbcdata
											from elabrev.liberacaocredito
											where lbrid = ".$_SESSION['elabrev_var']['lbrid']."
											and lbcstatus = 'A'
											order by lbcdata
											";
									//dbg($sql,1);
									$cabecalho = array("Ação","Valor", "Data");
									$db->monta_lista_simples( $sql, $cabecalho, 100, 1, 'N', '', '');
									?>									
								</td>
							</tr>
						</table>
					</td>
				</tr>
				
			</table>
			</form>
		</div>
</div>



<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){

	/*
	var cpLicitacao 	 = jQuery('#campoLicitacao').html();
	var cpContrato 	 	 = jQuery('#campoContrato').html();

	//jQuery('#tr_entnumcpfcnpj').find('td:first').html('CNPJ :');

	jQuery('#tr_endmapa').after('<tr id="tr_tit_licitacao"><td class="SubTituloCentro" colspan="2">DADOS LICITAÇÃO / CONTRATO</td></tr>');
	jQuery('#tr_tit_licitacao').after('<tr id="tr_licitacao"><td class="SubTituloDireita" width="30%">Número da Licitação :</td><td>'+cpLicitacao+'</td></tr>');
	jQuery('#tr_licitacao').after('<tr id="tr_contrato"><td class="SubTituloDireita" width="30%">Número do Contrato :</td><td>'+cpContrato+'</td></tr>');
	*/
	
	jQuery('#tr_entnuninsest').hide();
	jQuery('#tr_entemail').hide();
	jQuery('#tr_entsig').hide();
	jQuery('#tr_entnumdddcomercial').hide();
	jQuery('#tr_entnumdddfax').hide();
	jQuery('#tr_endereco').hide();
	jQuery('#tr_endcep').hide();
	jQuery('#tr_endlog').hide();
	jQuery('#tr_endcom').hide();
	jQuery('#tr_endnum').hide();
	jQuery('#tr_endbai').hide();
	jQuery('#tr_estuf').hide();
	jQuery('#tr_mundescricao').hide();
	jQuery('#tr_latitude').hide();
	jQuery('#tr_longitude').hide();
	jQuery('#tr_endmapa').hide();
	jQuery('#tr_funid').hide();
	jQuery('#tr_entcodent').hide();
	jQuery('#tr_entunicod').hide();
	jQuery('#tr_entungcod').hide();
	jQuery('#tr_njuid').hide();
	jQuery('#tr_tpctgid').hide();
	jQuery('#tr_tpcid').hide();
	jQuery('#tr_tplid').hide();
	jQuery('#tr_tpsid').hide();
	
	jQuery('#tr_entobs').hide();
	jQuery('#tr_funcoescadastradas').hide();
});

$('frmEntidade').onsubmit  = function(e) {

	if (trim($F('entnumcpfcnpj')) == '') {
		alert('O campo CNPJ é obrigatório.');
    	return false;
	}
	else{
		var cnpj  = trim($F('entnumcpfcnpj'));
		cnpj = cnpj.replace(".","");
		cnpj = cnpj.replace(".","");
		cnpj = cnpj.replace("/","");
		cnpj = cnpj.replace("-","");

		if(!validarCnpj(cnpj)){
			alert('O campo CNPJ é inválido.');
			return false;
		}

		
	}
	
	if (trim($F('entnome')) == '') {
		alert('O campo Nome é obrigatório.');
		return false;
	}
	/*
	if (trim($F('entrazaosocial')) == '') {
		alert('O campo Razão Social é obrigatório.');
		return false;
	}
	if (trim($F('entdatanasc')) != '') {
		if(!validaData(document.getElementById('entdatanasc'))) {
			alert("Data de nascimento é inválida.");return false;
		}
	}
	if (trim($F('endcep1')) == '') {
		alert('O campo CEP é obrigatório.');
		return false;
	}
	if (trim($F('estuf1')) == '' && trim($F('muncod1')) == '') {
		alert('A UF e Munícipio são obrigatórios. Digite o CEP novamente!');
		return false;
	}
	*/
	/*
	if (trim($F('lilnulicitacao')) == '') {
		alert('O campo Número da Licitação é obrigatório.');
    	return false;
	}
	if (trim($F('lilnucontrato')) == '') {
		alert('O campo Número do Contrato é obrigatório.');
    	return false;
	}
	*/

	
	return true;
}

$('formLicitacao').onsubmit  = function(e) {

	
	if (trim($F('lilnulicitacao')) == '') {
		alert('O campo Número da Licitação é obrigatório.');
    	return false;
	}
	if (trim($F('lilnucontrato')) == '') {
		alert('O campo Número do Contrato é obrigatório.');
    	return false;
	}
	if (trim($F('lilvalortotal')) == '') {
		alert('O campo Valor Total Licitação é obrigatório.');
    	return false;
	}


	var valortotal = "<?=$totalCdo?>";
	var valorlicitacao = document.getElementById( 'lilvalortotal' ).value;
	if(valorlicitacao) {
		valorlicitacao = valorlicitacao.replace(/\./gi,"");
		valorlicitacao = valorlicitacao.replace(/\,/gi,".");
	}
	
	if(valorlicitacao > valortotal) {
		alert('O Valor Total Licitação não pode ultrapassar o Valor Total CDO.');
    	return false;
	}
	
	return true;
	
}


$('formCredito').onsubmit  = function(e) {

	
	if (trim($F('vlcredito')) == '') {
		alert('O campo Valor do Crédito é obrigatório.');
    	return false;
	}

	var valorsomacredito = "<?=$totalCredito?>";

	var valorcredito = document.getElementById( 'vlcredito' ).value;
	if(valorcredito) {
		valorcredito = valorcredito.replace(/\./gi,"");
		valorcredito = valorcredito.replace(/\,/gi,".");

		var valortotalcredito = parseFloat(valorsomacredito) + parseFloat(valorcredito);
	}
	
	var valorlicitacao = document.getElementById( 'lilvalortotal' ).value;
	if(valorlicitacao) {
		valorlicitacao = valorlicitacao.replace(/\./gi,"");
		valorlicitacao = valorlicitacao.replace(/\,/gi,".");
	}
	
	if(valortotalcredito > valorlicitacao) {
		alert('O Valor Total Crédito não pode ultrapassar o Valor Total Licitação.');
    	return false;
	}
	
	return true;
	
}

function excluirCredito(cod){

	if(confirm('Deseja realmente excluir este item?')){
		location.href="elabrev.php?modulo=principal/licitacaoResultado&acao=A&excluircred="+cod;
	}
}


</script>