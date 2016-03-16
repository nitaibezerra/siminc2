<?php

include APPRAIZ . "includes/classes/Modelo.class.inc";
include APPRAIZ . "includes/classes/view/Lista.class.inc";
include APPRAIZ . "includes/classes/html/TElement.class.inc";
include APPRAIZ . "includes/classes/html/TTableRow.class.inc";
include APPRAIZ . "includes/classes/html/TTableCell.class.inc";
include APPRAIZ . "includes/classes/html/TTable.class.inc";
include APPRAIZ . "includes/classes/PaginacaoAjax.class.inc";
include APPRAIZ . "profeinep/classes/controle/Numeracao.class.inc";

if($_GET['nudidDel']){
	if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO))){
		$sql = "select tpdid from profeinep.numeracaodocumento where nudstatus = 'A' and nudid = ".$_GET['nudidDel'];
		$tpdid = $db->pegaUm($sql);
		$sql = "select count(1) from profeinep.anexos where anxstatus = 'A' and tpdid = {$tpdid} and nudid = ".$_GET['nudidDel'];
	    if($db->pegaUm($sql)){
	    	echo"<script>alert('Já existe um arquivo com este número, exclua primeiro o documento.'); window.location.href = 'profeinep.php?modulo=principal/geraNumeracao&acao=A';</script>";
	    	exit();	
	    }
	    $sql = "UPDATE profeinep.numeracaodocumento SET nudstatus = 'I' where nudid = ".$_GET['nudidDel'];
	    $db->executar($sql);
	    $db->commit();
	    echo"<script>window.location.href = 'profeinep.php?modulo=principal/geraNumeracao&acao=A';</script>";
	    exit();
	}
}

if($_POST['requisicao'] == 'pesquisar'){
	Numeracao::lista(array("filtro" => $_POST, 'nrRegPorPagina' => 200 ));
	die;
}

if($_POST['requisicao'] == 'gerarNumero'){
	
	$prcnumsidoc = trim($_POST['prcnumsidoc']);
	
	# verifica se o número do sidoc existe
	if($prcnumsidoc){
//		$sql = "SELECT count(1) FROM profeinep.processoprofeinep WHERE prcstatus = 'A' AND prcnumsidoc = '$prcnumsidoc'";
//		$boExistePrcnumsidoc = $db->pegaUm($sql);
		# pegamos o prcid
		$sql = "SELECT prcid as numero FROM profeinep.processoprofeinep WHERE prcstatus = 'A' AND prcnumsidoc = '$prcnumsidoc' ";
		$prcid = $db->pegaUm($sql);
		
		if(!$prcid){
			die('semNumeroSidoc');
		}
		
		if($_POST['tpdid']){
			
			# prepara para o número gerado com o tipo do documento
			$sql = "SELECT coalesce(max(nudnumero) + 1,1) as numero FROM profeinep.numeracaodocumento WHERE tpdid = {$_POST['tpdid']} AND nudano = ".date('Y')." ";
			$numeroGerado = $db->pegaUm($sql);
			if($numeroGerado){
				$sql = "INSERT INTO profeinep.numeracaodocumento (tpdid, nudnumero, nudusucpf, nuddatageracao, nudstatus, nudano, prcid) 
				        VALUES ({$_POST['tpdid']}, '$numeroGerado', '{$_SESSION['usucpf']}', '".date('Y-m-d')."', 'A',  '".date('Y')."', $prcid)";
				$db->executar($sql);
				if($db->commit()){
					Numeracao::lista(array('nrRegPorPagina' => 200 ));
					die;
				}
			}		
		}
	}
	
	die;
}

if($_REQUEST['requisicao'] == 'buscanumero'){
	$sql = "select prcnumsidoc from profeinep.processoprofeinep"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($key), $q) !== false) {
			$prcnumsidoc = trim($value['prcnumsidoc']);
			echo "{$prcnumsidoc}|{$prcnumsidoc}\n";			
		}
	}
	
	die;

}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// carregando o menu
if($_SESSION['profeinep_var']['prcid']!=''){
	echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']),$_SERVER['REQUEST_URI']);
}else{
	montaAbaInicio("profeinep.php?modulo=principal/geraNumeracao&acao=A");
}

$titulo = "Geração de números";
monta_titulo( $titulo, '&nbsp;' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />

<script type="text/javascript">
<!--
$(document).ready(function() {

	$('#prcnumsidoc').autocomplete("profeinep.php?modulo=principal/geraNumeracao&acao=A&requisicao=buscanumero", {
		//delay:10,
	  	//minChars:1,
	  	//matchSubset:1,
	  	//matchContains:1,
	  	cacheLength:10,
		width: 440,
		scrollHeight: 220,
		selectFirst: true,
		autoFill: true
	
	});

	$('#limpar').click(function(){
		$('#tpdid_pesq').val('');
		$('#usucpf').val('');
		$('#dtinicio').val('');
		$('#dtfim').val('');
	});
	
	$('#pesquisar').click(function(){
		$('#pesquisar').attr("disabled",true);
		var data = new Array();
		var nudstatus = 'A';
		if( $('#nudstatusI').attr("checked") ){
			nudstatus = 'I';
		}
		data.push({name : 'requisicao',  		value : 'pesquisar'}, 
				  {name : 'tpdid_pesq',  		value : $('#tpdid_pesq').val()}, 
				  {name : 'nudnumero',   		value : $('#nudnumero').val()}, 
				  {name : 'prcnumsidoc_pesq',   value : $('#prcnumsidoc_pesq').val()}, 
				  {name : 'usucpf',      		value : $('#usucpf').val()}, 
				  {name : 'dtinicio',    		value : $('#dtinicio').val()}, 
				  {name : 'nudstatus',   		value : nudstatus}, 
				  {name : 'dtfim', 		 		value : $('#dtfim').val()} 
				 );	
		$.ajax({
			   type		: "POST",
			   url		: "profeinep.php?modulo=principal/geraNumeracao&acao=A",
			   data		: data,
			   async    : false,
			   success	: function(data){
				   		  		$('#listaNumeracao').html(data);
								$('#pesquisar').attr("disabled",false);
						  }
			 });
		 
		
	});
	
	$('#gerar').click(function(){
		var msg = '';
		
		if($('#tpdid').val() == ''){
			msg += 'É necessário escolher Tipo de Documento\n';
		}
		if($('#prcnumsidoc').val() == ''){
			msg += 'É necessário digitar algum Número do Processo\n';
		}

		if(msg){
			alert(msg);
			return false;
		}
		
		var data = new Array();
			data.push({name : 'requisicao',  value : 'gerarNumero'}, 
					  {name : 'tpdid',       value : $('#tpdid').val()}, 
					  {name : 'prcnumsidoc', value : $('#prcnumsidoc').val()} 
					 );	
			$.ajax({
				   type		: "POST",
				   url		: "profeinep.php?modulo=principal/geraNumeracao&acao=A",
				   data		: data,
				   async    : false,
				   success	: function(data){
				   					//$('#divDebug').html(data);
				   					if(data == 'semNumeroSidoc'){
										alert('Número do Processo inválido.');
					   				} else {
					   					$('#listaNumeracao').html(data);
					   					$('#tpdid').val('');
					   					$('#prcnumsidoc').val('');
						   			}				   					
							  }
				 });
		
	});
	
});

function excluirNudid( nudid ){
	if ( confirm( 'Deseja excluir o Item?' ) ) {
		location.href= 'profeinep.php?modulo=principal/geraNumeracao&acao=A&nudidDel='+nudid;
	}
}

--></script>
<form method="post" name="formulario" id="formulario">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td style="width: 180px">Tipo de documento:</td>
			<td style="width: 280px">Número Sidoc:</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td>
				<?php
					$sql = "SELECT 
							 	tipodoc.tpdid  as codigo,
								tipodoc.tpddsc as descricao  
							FROM
								profeinep.tipodocumento as tipodoc
							WHERE
								tipodoc.tpdstatus = 'A' and tpdgeranumeracao = 'S'
							ORDER BY 
								tipodoc.tpddsc ASC";
					$db->monta_combo('tpdid', $sql, 'S', "Selecione...", '', '', '', '160', 'S', 'tpdid');
				?>
			</td>
			<td>
				<?php
					echo campo_texto( 'prcnumsidoc', 'S', $permissao_formulario, 'Número Processo', 30, 255, '', '','','','','id="prcnumsidoc" autocomplete="off" ');
				?>
			</td>
			<td>
				<input type="button" name="gerar" id="gerar" value="Gerar" />
			</td>
		</tr>
	</table>
</form>
<?php if(possuiPerfil(array(PRF_ADMINISTRADOR,PRF_SUPERUSUARIO))): ?>
<form method="post" name="formPesq" id="formPesq">
	<input type="hidden" name="salvar" value="0"> 
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	    <tr style="background-color: #cccccc">
	        <td align='left' style="vertical-align:top; width:25%; font-size:12px" colspan="2"><strong>Filtro de pesquisa</strong></td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Nome:</td>
	        <td>
	        	<?php
					$sql = "select distinct u.usucpf as codigo, u.usunome as descricao from profeinep.numeracaodocumento nd
							inner join seguranca.usuario u on u.usucpf = nd.nudusucpf
							ORDER BY 
								u.usunome ASC";
					$db->monta_combo('usucpf', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'usucpf');
				?>
	        </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
	        <td>
	        	<?php
					$sql = "SELECT 
							 	tipodoc.tpdid  as codigo,
								tipodoc.tpddsc as descricao  
							FROM
								profeinep.tipodocumento as tipodoc
							WHERE
								tipodoc.tpdstatus = 'A' and tpdgeranumeracao = 'S'
							ORDER BY 
								tipodoc.tpddsc ASC";
					$db->monta_combo('tpdid_pesq', $sql, 'S', "Selecione...", '', '', '', '160', 'N', 'tpdid_pesq');
				?>
	        </td>
	    </tr>
	    <tr>
	    	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número do Documento:</td>
	    	<td>
				<?php
					echo campo_texto( 'nudnumero', 'N', 'S', 'Número Documento', 30, 255, '', '','','','','id="nudnumero" autocomplete="off" ');
				?>
			</td>
	    </tr>
	    <tr>
	    	<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Número do Processo:</td>
	    	<td>
				<?php
					echo campo_texto( 'prcnumsidoc_pesq', 'N', 'S', 'Número Processo', 30, 255, '', '','','','','id="prcnumsidoc_pesq" autocomplete="off" ');
				?>
			</td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Perído:</td>
	        <td>
	        	<?php
					echo campo_data2('dtinicio', 'N', 'S', 'Data', '##/##/####');
					echo "&nbsp;&nbsp;";
					echo campo_data2('dtfim', 'N', 'S', 'Data', '##/##/####'); 
				?>
	        </td>
	    </tr>
	    <tr>
	        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivos Excluidos:</td>
	        <td>
	        	<input type="radio" value="I" id="nudstatusI" name="nudstatus">Excluídos
	        	<br />
	        	<input type="radio" value="A" id="nudstatusA" name="nudstatus" checked="checked">Não Excluídos
	        </td>
	    </tr>
	    <tr style="background-color: #cccccc">
	        <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
	        <td height="30"><input type="button" name="pesquisar" id="pesquisar" value="Pesquisar" />&nbsp;<input type="button" name="limpar" id="limpar" value="Limpar" /></td>
	    </tr> 
	</table>
</form>
<?php endif; ?>
<div id="listaNumeracao"><?php Numeracao::lista(array("filtro" => $_POST,'nrRegPorPagina' => 200 ));?></div>
<div id="divDebug"></div>