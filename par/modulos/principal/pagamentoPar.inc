<?php

if( $_REQUEST['geraExcelEmpenho'] ){
	$_REQUEST['exportarexcel'] = "Exportar Excel";
}

if($_POST['habilita']){
	header('content-type: text/html; charset=ISO-8859-1');
	atualizaHabilitaPar($_POST['cnpj']);
	exit();
}

if ($_POST['tipodocumento']) {
    $obModelosDocumentosControle = new ModelosDocumentosControle();
    $sqlModelosDoc = $obModelosDocumentosControle->carregarModeloDocumento($_POST['tipodocumento'], TRUE, 'NOT');
    echo $db->monta_combo("mdoid", $sqlModelosDoc, 'S', 'Selecione', 'verificaMoceloDoc', '', '', 400, 'N', 'mdoid', '', $_POST['mdoid']);
    exit();
}

if($_REQUEST['verificaDocumentacao']){
		
	$prpid = $_POST['prpid'];
	$sql = "SELECT
				prptipoexecucao 
			FROM 
				par.processopar 
			WHERE 
				prpid = {$prpid}
			AND
				prpstatus = 'A'	
				";
				
	$prptipoexecucao = $db->pegaUm( $sql );
	
	$erros = array();
	
	if( $prptipoexecucao == 'T' ){
		$sql = "SELECT 
					prpid, sum(qtd1) as cadastradas, sum(qtd2) as validadas
				FROM (
						(
							SELECT 
								prp.prpid, count(prp.dopid) as qtd1, 0 as qtd2
							FROM 
								par.vm_documentopar_ativos prp
							INNER JOIN par.modelosdocumentos md ON md.mdoid = prp.mdoid
							WHERE
								md.tpdcod = 102
							GROUP BY 
								prp.prpid
							
						) UNION ALL (
						
							SELECT 
								prp.prpid, 0 as qtd1, count(prp.dopid) as qtd2
							FROM 
								par.documentopar prp
							INNER JOIN par.modelosdocumentos md ON md.mdoid = prp.mdoid
							WHERE 
								prp.dopusucpfvalidacaogestor IS NOT NULL
								AND md.tpdcod = 102
							GROUP BY
								prp.prpid
						)
				) as f
				WHERE
					prpid = ".$_POST['prpid']."
				GROUP BY
					prpid";
		
		$dado = $db->pegaLinha( $sql );
		
		if( $dado['cadastradas'] != $dado['validadas'] || $dado['cadastradas'] == 0 ){
			$erro = "Pagamento bloqueado para os processos sem\nTermo de Compromisso validado pelo gestor.";
		}
		
	} else {
		$sql = "SELECT 
					prp.dopid
				FROM 
					par.vm_documentopar_ativos prp
				INNER JOIN par.modelosdocumentos md ON md.mdoid = prp.mdoid
				WHERE
					md.tpdcod = 16
					AND prpid = ".$_POST['prpid'];
		
		$dadoConv = $db->pegaUm( $sql );
		
		if( $dadoConv == '' ){
			$erros[] = "Convênio";
		} 

		$sql = "SELECT 
					prp.dopid
				FROM 
					par.vm_documentopar_ativos prp
				INNER JOIN par.modelosdocumentos md ON md.mdoid = prp.mdoid
				WHERE
					md.tpdcod = 101
					AND prpid = ".$_POST['prpid'];
		
		$dadoPar = $db->pegaUm( $sql );
		
		if( $dadoPar == '' ){
			$erros[] = "Parecer";
		} 
		if( is_array( $erros ) ){
			if( $erros[0] ){
				$erro = "Pagamento bloqueado por falta do(s) documento(s) de " . implode(" e ", $erros) . ".";
			}
		}
	}
	
	if( $erro ){
		echo $erro;
	} else {
		echo "valida";
	}
	
	exit;
}
//include '_funcoes_empenho.php';

/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
$tabela_execucao = monta_tabela_execucao( TIPO_PAR );
monta_titulo( $titulo_modulo, $tabela_execucao );
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<link href="../includes/JsLibrary/slider/slider.css" type="text/css" rel="stylesheet"></link>
<script
	language="javascript" type="text/javascript"
	src="../includes/blendtrans.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/_start.js"></script>
<script
	language="javascript" type="text/javascript"
	src="../includes/JsLibrary/slider/slider.js"></script>
<!--JS para o componente de exibir o tolltip de dados do processo-->
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css" />

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
	
	jQuery.noConflict();

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,140);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}
</script>
<script>

jQuery.noConflict();

function verificaDocumentos( prpnumeroprocesso, prpid, tipo, inuid ){
	var myajax = new Ajax.Request('par.php?modulo=principal/pagamentoPar&acao=A', {
		        method:     'post',
		        parameters: '&verificaDocumentacao=true&prpid='+prpid+'&prpnumeroprocesso='+prpnumeroprocesso,
		        asynchronous: false,
		        onComplete: function (res){
		        	//if( res.responseText == 'valida' ){
						window.open("par.php?modulo=principal/solicitacaoPagamentoPar&acao=A&processo="+prpnumeroprocesso+"&prpid="+prpid+"&inuid="+inuid,"Empenho","scrollbars=yes,fullscreen=yes,status=no,toolbar=no,menubar=no,location=no");
					//} else {
					//	alert(res.responseText);
					//}
		        }
		  });
}
function carregarListaEmpenhoPagamentoPar(pro, obj) {
	var tabela = obj.parentNode.parentNode.parentNode.parentNode.parentNode;
	var linha = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex+1);
		var col0 = nlinha.insertCell(0);
		col0.innerHTML = "&nbsp;";
		var col1 = nlinha.insertCell(1);
		col1.id="listapagamentoprocesso_"+pro;
		col1.colSpan=8;
		carregarListaPagamentoEmpenhoPar('', pro);
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex+1);	
	}
}


function submeterTela(valor){
	document.getElementById('exportarexcel').value = valor;
	selectAllOptions( document.formulario.tpdcod ); 
	selectAllOptions( document.formulario.mdoid ); 
	selectAllOptions( document.formulario.ptsid ); 
	document.formulario.submit();
}
/*
function submeterTela2(valor){
	document.getElementById('exportarexcel').value = valor;
	document.getElementById('geraExcelEmpenho').value = valor;
	selectAllOptions( document.formulario.tpdcod ); 
	selectAllOptions( document.formulario.mdoid ); 
	selectAllOptions( document.formulario.ptsid ); 
	document.formulario.submit();
}
*/
jQuery(document).ready(function(){
	if(jQuery('#tpdcod').val() != ''){
		verificaTipoDocumento( jQuery('#tpdcod').val() );
	}
	if( jQuery('#mdoid').val() == 29 ){
		jQuery('#tr_perc_obra').show();
	}
	
	jQuery('[name="contratoVinculado"]').click(function(){
		if( jQuery(this).val() == 1 ){
			jQuery('#div_termo_contrato').css('display', '');
		} else {
			jQuery('#div_termo_contrato').css('display', 'none');
		}
	});
	
	jQuery('[name="contratoNota"]').click(function(){
		if( jQuery(this).val() == 1 ){
			jQuery('#div_termo_nota').css('display', '');
		} else {
			jQuery('#div_termo_nota').css('display', 'none');
		}
	});
	
	if( jQuery('[name="contratoVinculado"]:checked').val() == 1 ){
		jQuery('#div_termo_contrato').css('display', '');
	} else {
		jQuery('#div_termo_contrato').css('display', 'none');
	}
	
	if( jQuery('[name="contratoNota"]:checked').val() == 1 ){
		jQuery('#div_termo_nota').css('display', '');
	} else {
		jQuery('#div_termo_nota').css('display', 'none');
	}
});

function verificaTipoDocumento(tipo){
    if (tipo !== '') {
	jQuery.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/pagamentoPar&acao=A",
   		data: "tipodocumento=" + tipo + '&mdoid=<?=$_REQUEST['mdoid'] ?>',
   		async: false,
   		success: function(msg){
   			jQuery('#tr_modelodoc').show();
			jQuery('#div_modelodoc').html(msg);
   		}
   	});
    } else {
        jQuery('#tr_modelodoc').hide();
    }
}

function verificaMoceloDoc(tipo){
	
	if(tipo == 29){
		jQuery('#tr_perc_obra').show();
		//jQuery('#div_perc_obra').show();
	} else {
		jQuery('#tr_perc_obra').hide();
	}
}

function pesquisaPagamento(){
	selectAllOptions( formulario.grupoMunicipio );
	
	if( document.getElementById('termogerado_1').checked == true || document.getElementById('termogerado_2').checked == true ){
		
		if( jQuery('#tpdcod').val() == '' ){
			alert('Informe um Tipo de Documento!');
			jQuery('#tpdcod').focus();
			return false;
		}
		if( jQuery('#mdoid').val() == '' ){
			alert('Informe um Modelo de Documento!');
			jQuery('#mdoid').focus();
			return false;
		}
	} else {
		if( jQuery('#tpdcod').val() != '' ){
			if( jQuery('#mdoid').val() == '' ){
				alert('Informe um Modelo de Documento!');
				jQuery('#mdoid').focus();
				return false;
			}
		}
	}
	/*if( jQuery('[name="contratoVinculado"]:checked').val() == 1 ){
		jQuery('#div_termo_contrato').css('display', '');
	}*/
	jQuery('#formulario').submit();
}

function verificaHabilita(cnpj){
	
	jQuery.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/pagamentoPar&acao=A",
   		data: "habilita=true&cnpj=" + cnpj,
   		async: false,
   		success: function(msg){
			alert('Dados Atualizado com Sucesso!');
	   		window.location.href = window.location;
   		}
   	});
}

</script>
<style type="text/css">
	
	.form_widget_amount_slider{
		border-top:1px solid #9d9c99;
		border-bottom:1px solid #eee;
		background-color:#f0ede0;
		height:3px;
		position:absolute;
		bottom:0px;
	}

	</style>
<form method="post" name="formulario" id="formulario">
<table id="pesquisas" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td class="textosTabela"></td>
			<td class="camposTabela"></td>
			<td class="textosTabela"></td>
			<td></td>
		</tr>
	</thead>
	<tbody>
	<tr>
		<td class="SubTituloDireita" width="30%">Número de Processo:</td>
		<td colspan="3">
		<?php
			$filtro = simec_htmlentities( $_POST['numeroprocesso'] );
			$numeroprocesso = $filtro;
			echo campo_texto( 'numeroprocesso', 'N', 'S', '', 50, 200, '#####.######/####-##', ''); 
		?>
		</td>
		
	</tr>
	<tr>
		<td class="SubTituloDireita" >Município:</td>
		<td colspan="3">
		<?php 
			$filtro = simec_htmlentities( $_POST['municipio'] );
			$municipio = $filtro;
			echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Forma de Execução:</td>
		<td colspan="3">
		<?php
			$frmid = $_POST['frmid'];
			$sql = "SELECT frmid as codigo , frmdsc as descricao FROM par.formaexecucao WHERE frmid in (6, 13) ORDER BY frmdsc ASC";
			$db->monta_combo( "frmid", $sql, 'S', 'Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo de Subação:</td>
		<td colspan="3">
		<?php
			$ptsid = $_POST['ptsid'];
			$sql = "SELECT pts.ptsid as codigo, pts.ptsdescricao || ' - ' || fe.frmdsc  AS descricao 
					FROM par.propostatiposubacao pts
					INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
					WHERE pts.ptsstatus = 'A' 
					AND fe.frmid in (6, 13)
					ORDER BY pts.ptsdescricao, fe.frmdsc";
			$db->monta_combo( "ptsid", $sql, 'S', 'Tipo de subação - Forma de Execução', '', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Pagamento:</td>
		<td colspan="3">
			<?
			$pago = $_REQUEST['pago'];
			$arrEmpenho = array(
								array('codigo' => 1, 'descricao' => '100% Pago'),
								array('codigo' => 2, 'descricao' => 'Sem Pagamento'),
								array('codigo' => 3, 'descricao' => 'Pagamento Parcial')
							);
			$db->monta_combo( "pago", $arrEmpenho, 'S', 'Selecione', '', '' );
			?>
			<!--  Sim: <input <?php echo $_REQUEST['empenhado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="1" >
			Não: <input <?php echo $_REQUEST['empenhado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhado" value="2" >-->
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita">Esfera:</td>
		<td>
			<?php 
				$esfera = $_POST['esfera'];
				$arrEsfera = array(0 => array("codigo"=>"Municipal","descricao"=>"Municipal"), 1 => array("codigo"=>"Estadual","descricao"=>"Estadual"));
				$db->monta_combo( "esfera", $arrEsfera, 'S', 'Todas as esferas', '', '' ); 
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Ano do Processo:</td>
		<td>
			<?php
			$anoprocesso = $_POST['anoprocesso'];
			$sql = "select distinct  substring(prpnumeroprocesso,12,4) as codigo,  substring(prpnumeroprocesso,12,4) as descricao from par.processopar where substring(prpnumeroprocesso,12,4) <> '' and prpstatus = 'A'";
			$db->monta_combo( "anoprocesso", $sql, 'S', 'Todos', '', '' );
			?>
		</td>
	</tr>		
	<tr>
		<td class="SubTituloDireita">Grupo de Município:</td>
		<td>
			<?php 

			$sql = "SELECT 
							tpmid as codigo, 
							tpmdsc as descricao
						FROM 
							territorios.tipomunicipio
						WHERE
							tpmstatus = 'A' 
							AND gtmid in ( 1,2 )
						ORDER BY
							descricao";
			
			if( $_REQUEST['grupoMunicipio'][0] ){
				$sqlD = "SELECT 
							tpmid as codigo, 
							tpmdsc as descricao
						FROM 
							territorios.tipomunicipio
						WHERE
							tpmstatus = 'A' 
							AND gtmid in ( 1,2 )
							and tpmid in (". implode(",",$_REQUEST['grupoMunicipio']). ")
						ORDER BY
							descricao";
				
				$grupoMunicipio = $db->carregar($sqlD);
			}

			combo_popup('grupoMunicipio', $sql, 'Selecione...', "400x400", 0, array(), '', 'S', false, false, 3 );
						
			?>															
		</td>
	</tr>
	<!-- 
	<tr>
		<td class="SubTituloDireita">Empenho:</td>
		<td>
		
			<input <?php echo $_REQUEST['empenhosolicitado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="1" />Empenho Solicitado
			<input <?php echo $_REQUEST['empenhosolicitado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value="2" />Empenho Não Solicitado
			<input <?php echo empty($_REQUEST['empenhosolicitado']) ? "checked='checked'" : "" ?> type="radio" name="empenhosolicitado" value=""   />Todos
			
		</td>
	</tr>
	 -->			
	<tr>
		<td class="SubTituloDireita" >Tipo Execução:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['tipoExecucao'] == "C" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="C" >Convênio
			<input <?php echo $_REQUEST['tipoExecucao'] == "T" ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value="T" >Termo
			<input <?php echo empty($_REQUEST['tipoExecucao']) ? "checked='checked'" : "" ?> type="radio" name="tipoExecucao" value=""   />Todos
		</td> 
	</tr>

	<tr>
		<td class="SubTituloDireita">Termo:</td>
		<td>
			<fieldset style="width: 380px"><legend>Filtros Termo:</legend>
			<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_1" value="1" >Gerado
			<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" id="termogerado_2" value="2" >Não Gerado
			<input <?php echo empty($_REQUEST['termogerado']) ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="" >Todos
		<br>	
			<table border="0"  align="left" bgcolor="#f5f5f5" cellpadding="0" cellspacing="0" width="380px">
					<tr>
						<td class="SubTituloDireita" style="width: 60px;text-align:center;" >Validação:</td>
						<td>
				<table border="0" style="width: 100%; bgcolor='#f5f5f5'">
					<tr>
						<td class="SubTituloDireita" style="width: 40%;" >Gestor:</td>
						<td>
							<input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "t" ? "checked='checked'" : "" ?>  type="radio" value="t" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Sim 
							<input <?php echo $_REQUEST['dopusucpfvalidacaogestor'] == "f" ? "checked='checked'" : "" ?>  type="radio" value="f" id="dopusucpfvalidacaogestor" name="dopusucpfvalidacaogestor" />Não
							<input <?php echo empty($_REQUEST['dopusucpfvalidacaogestor']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaogestor" value="" >Todos
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 40%;" >De outras Entidades:</td>
						<td>
							<input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "t" ? "checked='checked'" : "" ?> type="radio" value="t" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde" />Sim
							<input <?php echo $_REQUEST['dopusucpfvalidacaofnde'] == "f" ? "checked='checked'" : "" ?> type="radio" value="f" id="dopusucpfvalidacaofnde" name="dopusucpfvalidacaofnde"  />Não
							<input <?php echo empty($_REQUEST['dopusucpfvalidacaofnde']) ? "checked='checked'" : "" ?> type="radio" name="dopusucpfvalidacaofnde" value="" >Todos
						</td>
					</tr>
				</table>
			</table>
			</fieldset>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Tipo de Documento:</td>
		<td colspan="3">
                    <?php
                        $obTipoDocumento = new TipoDocumentoControle();
                        $sqlTipoDocumento = $obTipoDocumento->carregarTipoDocumento(TRUE);
                        $db->monta_combo("tpdcod", $sqlTipoDocumento, 'S', 'Selecione', 'verificaTipoDocumento', '', '', 400, 'N', 'tpdcod', '', $_REQUEST['tpdcod']);
                    ?>
		</td>
	</tr>
	<tr id="tr_modelodoc" style="display: none">
		<td class="SubTituloDireita" >Modelo de Documento:</td>
		<td colspan="3">
		<div id="div_modelodoc">
                    <?php
                    $db->monta_combo("mdoid", array(), 'S', 'Selecione', '', '', '', 400, 'N', 'mdoid');
                    ?>
                </div>
		</td>
	</tr>
	<tr id="tr_perc_obra" style="display: none">
		<td class="SubTituloDireita" >Execução de Obras Acima de:</td>
		<td colspan="3">
			<table border="0"  align="left" cellpadding="0" cellspacing="0" width="380px">
				<tr>
					<td width="75px"><span id="slider_target" class="form_widget_amount_slider" style="width: 40px; position: relative; *left: -41px; top: 5px; height: 3px; display:inline;"></span></td>
					<td><input type="text" name="valor" id="valorSlider" value="<?=($_REQUEST['valor'] ? $_REQUEST['valor'] : 100) ?>" style="font-size: 10px; width: 30px"  />%</td>
				</tr>
			</table>
		<script>
			form_widget_amount_slider('slider_target',document.getElementById( "valorSlider" ),70,0,100,false);
		</script>
		</td>
	</tr>
	<!--  <tr>
		<td class="SubTituloDireita">Pagamento:</td>
		<td>
			<input <?php echo $_REQUEST['pagamento'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="1" />Pagamento Solicitado
			<input <?php echo $_REQUEST['pagamento'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="2" />Pagamento Não Solicitado
			<input <?php echo $_REQUEST['pagamento'] == "3" ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="3" />Pagamento efetivado
			<input <?php echo empty($_REQUEST['pagamento']) ? "checked='checked'" : "" ?> type="radio" name="pagamento" value="" />Todos
		</td>
	</tr>-->
	<tr>
		<td class="SubTituloDireita">Reformulação:</td>
		<td colspan="3">
		<?php
			$aryReformulacao = array(
				array('codigo' => 't', 'descricao' => 'Termos em Reformulação'),
				array('codigo' => 'f', 'descricao' => 'Termos Reformulados')
			);
			$reformulacao = $_POST['reformulacao'];
			$db->monta_combo("reformulacao",$aryReformulacao, 'S', 'Todos', '', '', '',400,'N','reformulacao');	?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" >Termos com contrato vinculado:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['contratoVinculado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="contratoVinculado" value="1" >Sim
			<input <?php echo $_REQUEST['contratoVinculado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="contratoVinculado" value="2" >Não
			<input <?php echo empty($_REQUEST['contratoVinculado']) ? "checked='checked'" : "" ?> type="radio" name="contratoVinculado" value=""   />Todos
			<div id="div_termo_contrato" style="display: none">
				<table border="0" bgcolor='#f5f5f5'">
					<tr>
						<td class="SubTituloDireita" style="width: 18%;" >Data Inicio:</td>
						<td>
							<?
							$datainicio = $_REQUEST['datainicio'];
							$datafim = $_REQUEST['datafim'];
							?>
							<?=campo_data2('datainicio', 'S','S','Data Inicio','','','') ?>
						</td>
						<td class="SubTituloDireita" style="width: 18%;">Data Final:</td>
						<td>
							<?=campo_data2('datafim', 'S','S','Data Final','','','') ?>
						</td>
					</tr>
				</table>
			</div>
		</td> 
	</tr>
	<tr>
		<td class="SubTituloDireita" >Termos com Notas Fiscais:</td>
		<td colspan="3">
			<input <?php echo $_REQUEST['contratoNota'] == "1" ? "checked='checked'" : "" ?> type="radio" name="contratoNota" value="1" >Sim
			<input <?php echo $_REQUEST['contratoNota'] == "2" ? "checked='checked'" : "" ?> type="radio" name="contratoNota" value="2" >Não
			<input <?php echo empty($_REQUEST['contratoNota']) ? "checked='checked'" : "" ?> type="radio" name="contratoNota" value=""   />Todos
			<div id="div_termo_nota" style="display: none">
				<table border="0" bgcolor='#f5f5f5'">
					<tr>
						<td class="SubTituloDireita" style="width: 18%;" >Data Inicio:</td>
						<td>
							<?
							$datainicionota = $_REQUEST['datainicionota'];
							$datafimnota = $_REQUEST['datafimnota'];
							?>
							<?=campo_data2('datainicionota', 'S','S','Data Inicio','','','') ?>
						</td>
						<td class="SubTituloDireita" style="width: 18%;">Data Final:</td>
						<td>
							<?=campo_data2('datafimnota', 'S','S','Data Final','','','') ?>
						</td>
					</tr>
				</table>
			</div>
		</td> 
	</tr>
    <tr>
        <td class="SubTituloDireita">Obras MUNICIPAIS com pendências:</td>
        <td>
            <input <?php echo $_REQUEST['pendenciaObras'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_1" value="1" >Sim
            <input <?php echo $_REQUEST['pendenciaObras'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" id="pendenciaObras_2" value="2" >Não
            <input <?php echo empty($_REQUEST['pendenciaObras']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObras" value=""   />Todos
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Obras ESTADUAIS com pendências:</td>
        <td>
            <input <?php echo $_REQUEST['pendenciaObrasUF'] == "1" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_1" value="1" >Sim
            <input <?php echo $_REQUEST['pendenciaObrasUF'] == "2" ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" id="pendenciaObras_uf_2" value="2" >Não
            <input <?php echo empty($_REQUEST['pendenciaObrasUF']) ? "checked='checked'" : "" ?> type="radio" name="pendenciaObrasUF" value=""   />Todos
        </td>
    </tr>
    <?php if( $db->testa_superuser() ){?>
    <tr>
        <td class="SubTituloDireita" style="color:red">Possui erro na distribuição do pagamento:</td>
        <td>
            <input <?php echo $_REQUEST['pagamentoMaiorQueEmpenho'] == "S" ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" id="pagamentoMaiorQueEmpenho" value="S" >Sim
            <input <?php echo empty($_REQUEST['pagamentoMaiorQueEmpenho']) ? "checked='checked'" : "" ?> type="radio" name="pagamentoMaiorQueEmpenho" value=""   />Todos
        </td>
    </tr>
    <?php }?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td colspan="3">
			<input type="hidden" name="pesquisar" id="pesquisar" value="1">
			<input class="botao" type="button" name="btnPesquisar" value="Pesquisar" onclick="pesquisaPagamento();" />
			<input class="botao" type="button" name="todos" value="Ver todos" onclick="window.location='par.php?modulo=principal/pagamentoPar&acao=A';" />
			<input class="botao" type="submit" name="exportarexcel" value="Exportar Excel" />
			<input class="botao" type="submit" name="geraExcelEmpenho" value="Exportar Excel - Dados Empenho" />
		</td>
	</tr>
	</tbody>
</table>
</form>
<div id="debug"></div>
<?php

if($_REQUEST['pesquisar'] || $_REQUEST['exportarexcel'] ) {
    $where = array('(SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0) > 0');
	
	if($_REQUEST['grupoMunicipio'][0]){
			$where[] = "tm.tpmid in (". implode(",",$_REQUEST['grupoMunicipio']). ")";
			$joinGrupoMun = "LEFT JOIN territorios.muntipomunicipio mt ON mt.muncod = m.muncod
						 LEFT JOIN territorios.tipomunicipio tm ON tm.tpmid = mt.tpmid";
	}
	
	/*if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				$where[] = "(p.muncod is not null and p.muncod <> '')";
				break;
			case 'Estadual':
				$where[] = "(p.muncod is null or p.muncod = '')";
				break;
		}
	}*/
	
	if($_REQUEST['esfera']) {
		switch($_REQUEST['esfera']) {
			case 'Municipal':
				$where[] = "iu.itrid = 2";
				break;
			case 'Estadual':
				$where[] = "CASE WHEN iu.estuf = 'DF' THEN iu.itrid = 2 ELSE iu.itrid = 1 END AND iu.muncod IS NULL";
				break;
		}
	}
	
	if($_REQUEST['termogerado'] == "1") {
		if($_REQUEST['tpdcod']) {
			$where[] = "dp.dopid in (select dopid from par.documentopar dp1 
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
		}
			
		if($_REQUEST['mdoid']) {
			$where[] = "dp.mdoid = {$_REQUEST['mdoid']}";
		}
	} else if($_REQUEST['termogerado'] == "2") {
		if($_REQUEST['tpdcod']) {
			$where[] = "dp.dopid not in (select dopid from par.documentopar dp1 
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
		}
			
		if($_REQUEST['mdoid']) {
			$where[] = "dp.mdoid not in ({$_REQUEST['mdoid']})";
		}
	} else {
		if($_REQUEST['tpdcod']) {
			$where[] = "dp.dopid in (select dopid from par.documentopar dp1 
	            			inner join par.modelosdocumentos md on md.mdoid = dp1.mdoid and md.mdostatus = 'A'
	            			where md.tpdcod = {$_REQUEST['tpdcod']} ) ";
		}
		
		if($_REQUEST['mdoid']) {
			$where[] = "dp.mdoid = {$_REQUEST['mdoid']}";
			
		}
	}
			
	if( $_REQUEST['mdoid'] == 29 ){
		$where[] = "oi.obrpercentultvistoria > {$_REQUEST['valor']}";
							        
		$joinObrasVinculada = " INNER JOIN par.termocomposicao 			tec ON tec.dopid = dp.dopid
						        INNER JOIN par.subacaodetalhe 			sde ON sde.sbdid = tec.sbdid
						        INNER JOIN par.subacaoobravinculacao 	sov ON sov.sbaid = sde.sbaid AND sde.sbdano = sov.sovano
						        INNER JOIN obras2.obras 				oi  ON oi.obrid = sov.obrid  AND oi.obrstatus = 'A'  ";
	}
		
	if($_REQUEST['empenhosolicitado'] == "1") {
		$where[] = " p.prpnumeroprocesso IN(SELECT ep2.empnumeroprocesso FROM par.empenho ep2 WHERE empsituacao <> 'CANCELADO' and empstatus = 'A')";
	}
	
	if($_REQUEST['empenhosolicitado'] == "2") {
		$where[] = " p.prpnumeroprocesso NOT IN (SELECT ep2.empnumeroprocesso FROM par.empenho ep2 WHERE empsituacao <> 'CANCELADO' and empstatus = 'A')";
	}
	
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	
	if($_REQUEST['estuf']) {
		if($_REQUEST['esfera'] == 'Estadual'){
			$where[] = "iu.estuf='".$_REQUEST['estuf']."'";
		}else if ($_REQUEST['esfera'] == 'Municipal'){
			$where[] = "iu.mun_estuf='".$_REQUEST['estuf']."'";
		}else{
			$where[] = "(iu.estuf='".$_REQUEST['estuf']."' OR iu.mun_estuf='".$_REQUEST['estuf']."')";
		}
	}
	
	if($_REQUEST['pago'] == "1") {
		$where[] = " pagvalorparcela = (SELECT sum(vve.saldo) from par.empenho emp
										INNER JOIN par.v_dadosempenhosubacao vve ON vve.empid = emp.empid
										WHERE emp.empnumeroprocesso = p.prpnumeroprocesso) ";
	}if($_REQUEST['pago'] == "2") {
		$where[] = "pagvalorparcela IS NULL AND pge.pagvalorparcela_efetivada IS NULL";
	}if($_REQUEST['pago'] == "3") {
		$where[] =  "pagvalorparcela < (SELECT sum(vve.saldo) from par.empenho emp
										INNER JOIN par.v_dadosempenhosubacao vve ON vve.empid = emp.empid
										WHERE emp.empnumeroprocesso = p.prpnumeroprocesso) 
						AND pagvalorparcela > 0 ";
	}
	if($_REQUEST['pagamento'] == "1") {
		$where[] = "p.prpnumeroprocesso IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' AND pg.pagstatus = 'A')";
	}
	if($_REQUEST['pagamento'] == "2") {
		$where[] = "p.prpnumeroprocesso NOT IN ( SELECT em.empnumeroprocesso FROM par.empenho em INNER JOIN par.pagamento pg ON em.empid=pg.empid and empcodigoespecie not in ('03', '13', '02', '04') and empstatus = 'A' AND pg.pagstatus = 'A')";
	}
	if($_REQUEST['pagamento'] == "3") {
		$where[] = "pagvalorparcela > 0 ";
	}
	
	if($_REQUEST['tipoExecucao'] == "C") {
		$where[] = "p.prptipoexecucao = 'C'";
	}
	
	if($_REQUEST['tipoExecucao'] == "T") {
		$where[] = "p.prptipoexecucao = 'T'";
	}
	
	if($_REQUEST['tipoobra'] == "P") {
		$where[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$where[] = "protipo <> 'P'";
	}
	if($_REQUEST['resid']) {
		$where[] = "r.resid = '".$_REQUEST['resid']."'";
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 't') {
		$where[] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
								    inner join par.vm_documentopar_ativos dp on dp.mdoid = md.mdoid
								where
								    md.mdoqtdvalidacao > 1
								    and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
								    and md.mdostatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaofnde'] == 'f') {
		$where[] = "dp.dopid in (select distinct dp.dopid from par.modelosdocumentos md
										inner join par.vm_documentopar_ativos dp on dp.mdoid = md.mdoid
										inner join par.documentoparvalidacao dv on dv.dopid = dp.dopid and dv.dpvstatus = 'A'
									where
										md.mdoqtdvalidacao = 1
										and md.mdoqtdvalidacao = (select count(dv.dpvcpf) from par.documentoparvalidacao dv where dv.dopid = dp.dopid and dv.dpvstatus = 'A')
									    and md.mdostatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 't') {
		$where[] = "dp.dopid IN (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
	}
	if($_REQUEST['dopusucpfvalidacaogestor'] == 'f') {
		$where[] = "dp.dopid not in (select dopid from par.documentoparvalidacao where dpvdatavalidacao is not null and dpvstatus = 'A')";
	}
	
	if($_REQUEST['anoprocesso']){
		$where[]= "substring(p.prpnumeroprocesso,12,4) = '".$_REQUEST['anoprocesso']."'";
	}
	
	if( $_REQUEST['numeroprocesso'] ){
		$where[] = "p.prpnumeroprocesso = '".str_replace(Array('.','/','-'),'',$_REQUEST['numeroprocesso'])."'";
	}
	
	if($_REQUEST['frmid']) {
		$where[] = "p.prpid in (select ppc.prpid from par.processoparcomposicao ppc
		                            inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
		                            inner join par.subacao sub on sub.sbaid = sd.sbaid
		                        where ppc.ppcstatus = 'A' and sub.frmid = {$_REQUEST['frmid']})";
	}
	
	if($_REQUEST['ptsid']) {
		$where[] = "p.prpid in (select ppc.prpid from par.processoparcomposicao ppc
		                            inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
		                            inner join par.subacao sub on sub.sbaid = sd.sbaid
		                        where ppc.ppcstatus = 'A' and sub.ptsid = {$_REQUEST['ptsid']})";
	}
	if($_REQUEST['contratoVinculado'] == '1') {
		$where[] = "  p.prpnumeroprocesso in (
						 SELECT 
							DISTINCT
							p.prpnumeroprocesso
				
							 FROM par.instrumentounidade iu
							INNER JOIN par.processopar p on p.inuid = iu.inuid and p.prpstatus = 'A'
							INNER JOIN par.vm_documentopar_ativos dp on dp.prpid = p.prpid
							INNER JOIN par.modelosdocumentos em  ON dp.mdoid = em.mdoid
							INNER JOIN public.tipodocumento tp ON tp.tpdcod = em.tpdcod
							LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
				
							INNER JOIN par.termocomposicao tc ON tc.dopid = dp.dopid
							inner join par.subacaodetalhe sd on tc.sbdid = sd.sbdid
							inner join par.subacao 		s on sd.sbaid = s.sbaid 
				
							LEFT JOIN territorios.municipio  m on iu.muncod = m.muncod AND mun_estuf is not null
							LEFT JOIN territorios.estado e on e.estuf = iu.estuf AND mun_estuf is null
							where s.sbaid  in 
							(
							SELECT                                                                  
								distinct
								si.sbaid
									
							FROM
								par.contratos con
								INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
								INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid
							)
				
						 )";
	}
	if($_REQUEST['contratoVinculado'] == '2' ) {
		$where[] = "  p.prpnumeroprocesso in (
						 SELECT 
							DISTINCT
							p.prpnumeroprocesso
				
							 FROM par.instrumentounidade iu
							INNER JOIN par.processopar p on p.inuid = iu.inuid and p.prpstatus = 'A'
							INNER JOIN par.vm_documentopar_ativos dp on dp.prpid = p.prpid
							INNER JOIN par.modelosdocumentos em  ON dp.mdoid = em.mdoid
							INNER JOIN public.tipodocumento tp ON tp.tpdcod = em.tpdcod
							LEFT JOIN par.documentopar dp2 ON dp2.dopid = dp.dopnumerodocumento
				
							INNER JOIN par.termocomposicao tc ON tc.dopid = dp.dopid
							inner join par.subacaodetalhe sd on tc.sbdid = sd.sbdid
							inner join par.subacao 		s on sd.sbaid = s.sbaid 
				
							LEFT JOIN territorios.municipio  m on iu.muncod = m.muncod AND mun_estuf is not null
							LEFT JOIN territorios.estado e on e.estuf = iu.estuf AND mun_estuf is null
							where s.sbaid not in 
							(
							SELECT                                                                  
								distinct
								si.sbaid
									
							FROM
								par.contratos con
								INNER JOIN par.subacaoitenscomposicaoContratos sicon ON sicon.conid = con.conid
								INNER JOIN par.subacaoitenscomposicao si ON si.icoid = sicon.icoid
							)
				
						 )";
	}
	
	if($_REQUEST['contratoNota'] == '1'){
		$where[] = " p.prpnumeroprocesso in ( 
						SELECT 
							DISTINCT
							p.prpnumeroprocesso
						FROM 
							par.processopar p
						INNER JOIN par.vm_documentopar_ativos    dp on dp.prpid = p.prpid
						INNER JOIN par.termocomposicao tc on tc.dopid = dp.dopid
						INNER JOIN par.subacaodetalhe sd  on tc.sbdid = sd.sbdid
						INNER JOIN par.subacaoitenscomposicao sic on sd.sbaid = sic.sbaid and sd.sbdano = sic.icoano
						INNER JOIN par.subacaoitenscomposicaonotasfiscais snf ON sic.icoid = snf.icoid AND sic.icostatus = 'A' 
						where p.prpstatus = 'A') ";
	}
	
	if($_REQUEST['contratoNota'] == '2'){
		$where[] = " p.prpnumeroprocesso not in ( 
						SELECT 
							DISTINCT
							p.prpnumeroprocesso
						FROM 
							par.processopar p
						INNER JOIN par.vm_documentopar_ativos dp on dp.prpid = p.prpid
						INNER JOIN par.termocomposicao tc on tc.dopid = dp.dopid
						INNER JOIN par.subacaodetalhe sd  on tc.sbdid = sd.sbdid
						INNER JOIN par.subacaoitenscomposicao sic on sd.sbaid = sic.sbaid and sd.sbdano = sic.icoano
						INNER JOIN par.subacaoitenscomposicaonotasfiscais snf ON sic.icoid = snf.icoid AND sic.icostatus = 'A' 
						where p.prpstatus = 'A' ) ";
	}

	if($_REQUEST['reformulacao']){
		if($_REQUEST['reformulacao'] == 't'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid and p.prpstatus = 'A' WHERE dopreformulacao = true)";
		} elseif($_REQUEST['reformulacao'] == 'f'){
			$where[]= "p.prpid IN (SELECT DISTINCT p.prpid FROM par.documentopar d INNER JOIN par.processopar p ON p.prpid = d.prpid WHERE dopreformulacao = false and p.prpstatus = 'A')";
		}
	}

    $pendenciaObras = $_REQUEST['pendenciaObras'];
    if($pendenciaObras == '1'){ // com pendencias
        $where[] = " iu.itrid = '2' ";
        $where[] = " iu.muncod in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    if($pendenciaObras == '2'){ // sem pendencias
        $where[] = " iu.itrid = '2' ";
        $where[] = " iu.muncod not in (select coalesce(muncod, '') from obras2.vm_pendencia_obras where empesfera = 'M')  ";
    }

    $pendenciaObrasUF = $_REQUEST['pendenciaObrasUF'];
    if($pendenciaObrasUF == '1'){ // com pendencias
        $where[] = " iu.itrid = '1' ";
        $where[] = " iu.estuf in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }

    if($pendenciaObrasUF == '2'){ // sem pendencias
        $where[] = " iu.itrid = '1' ";
        $where[] = " iu.estuf not in (select coalesce(estuf, '') from obras2.vm_pendencia_obras where empesfera = 'E')  ";
    }
}

if($_REQUEST['exportarexcel']){
	$cabecalho = array("UF", "Município", "Nº Processo","Valor das Subações(R$)", "Valor do Termo(R$)", "Valor empenhado(R$)", "Valor pagamento(R$)","Valor pagamento efetivado(R$)","Valor a Pagar(R$)","% Pagamento", "Tipo", 'Entidade Habilitada', 'Situação', 'Número do Documento');
	$colunaHabilita = 'iue.iuesituacaohabilita';
} else {
	$cabecalho = array("&nbsp;", "&nbsp;", "UF", "Município", "Nº Processo", "Valor das Subações(R$)", "Valor do Termo(R$)", "Valor empenhado(R$)", "Valor pagamento(R$)","Valor pagamento efetivado(R$)","Valor a Pagar(R$)","% Pagamento", "Tipo", 'Entidade Habilitada', 'Situação', 'Número do Documento');
	
	$colunaHabilita = "case when iue.iuesituacaohabilita = 'Habilitado' then
		              		'<center><img src=../imagens/workflow/1.png title=\"'||iue.iuesituacaohabilita||'\" style=cursor:pointer;></center>' 
		              	else 
		              		'<center><img src=../imagens/workflow/2.png title=\"'||iue.iuesituacaohabilita||'\" onclick=\"verificaHabilita(\''||p.prpcnpj||'\');\" style=cursor:pointer;></center>' end as habilitado";
}

$consultaDocumenta  = "'<a href=\"#\" onclick=\"tramitar(\''||p.prpdocumenta||'\');\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\"></a>'";
$insereDocumenta  = "'<img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;\" onclick=\"cadastraDocumenta(\''||p.prpnumeroprocesso||'\', \'pagamentoPar\');\" title=\"Consultar histórico do documento no sistema Documenta do FNDE\" border=\"0\">'";

if($_REQUEST['empenhosolicitado']){
	//$emenhoSolicitado = "WHERE --dp.dopusucpfvalidacaogestor IS NOT NULL AND p.prpstatus = 'A'";
}else{
	//$emenhoSolicitado = "WHERE dp.dopusucpfvalidacaogestor IS NOT NULL AND p.prpstatus = 'A'";
}
if($_REQUEST['empenhosolicitado']){
	//$emenhoSolicitado =(($where)?" AND ".implode(" AND ", $where):"");
} else{
	//$emenhoSolicitado = (($where)?" AND ".implode(" AND ", $where):"");
}

if($_REQUEST['exportarexcel']){
	$sqlColunaNumeroProcesso = "to_char(p.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')";
} else {
	$sqlColunaNumeroProcesso = "
	    (
	        CASE WHEN (select count(*) as qtd from painel.dadosfinanceirosconvenios dfi where dfi.dfiprocesso = p.prpnumeroprocesso) > 0 THEN
	            '<span class=\"processoDetalhes processo_detalhe\" >'
	        ELSE
	            '<span class=\"processo_detalhe\" >'
	        END
	    ) ||
	    to_char(p.prpnumeroprocesso::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00') || '</span>'
	";
}

$joinsContrato = ''; 
$joinsCont = ''; 
$joins = '';
$joinsContrato = '';
$joinsNotas = '';
if( $_REQUEST['contratoVinculado'] == '1' ){
	if( $_REQUEST['datainicio'] && $_REQUEST['datafim'] ){
		$where[] = "scc.sccdata between '".formata_data_sql($_REQUEST['datainicio'])."' and '".formata_data_sql($_REQUEST['datafim'])."'";
	}elseif( $_REQUEST['datainicio'] ){
		$where[] = "scc.sccdata >= '".formata_data_sql($_REQUEST['datainicio'])."' ";
	}elseif( $_REQUEST['datafim'] ){
		$where[] = "scc.sccdata <= '".formata_data_sql($_REQUEST['datafim'])."' ";
	}
	$joins = "inner join par.processoparcomposicao ppc on ppc.prpid = p.prpid and  ppc.ppcstatus = 'A'
	        inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
	        inner join par.subacaoitenscomposicao sic on sic.sbaid = sd.sbaid and sic.icoano = sd.sbdano ";
	$joinsCont = "inner join par.subacaoitenscomposicaocontratos  scc on scc.icoid = sic.icoid and scc.sccstatus = 'A'";
}

if( $_REQUEST['contratoNota'] == '1' ){
	if( $_REQUEST['datainicionota'] && $_REQUEST['datafimnota'] ){
		$where[] = "scc.sccdata between '".formata_data_sql($_REQUEST['datainicionota'])."' and '".formata_data_sql($_REQUEST['datafimnota'])."'";
	}elseif( $_REQUEST['datainicionota'] ){
		$where[] = "scc.sccdata >= '".formata_data_sql($_REQUEST['datainicionota'])."' ";
	}elseif( $_REQUEST['datafimnota'] ){
		$where[] = "scc.sccdata <= '".formata_data_sql($_REQUEST['datafimnota'])."' ";
	}
	$joins = "inner join par.processoparcomposicao ppc on ppc.prpid = p.prpid and ppc.ppcstatus = 'A'
	        inner join par.subacaodetalhe sd on sd.sbdid = ppc.sbdid
	        inner join par.subacaoitenscomposicao sic on sic.sbaid = sd.sbaid and sic.icoano = sd.sbdano ";
	$joinsNotas = "inner join par.subacaoitenscomposicaonotasfiscais  sccn on sccn.icoid = sic.icoid and sccn.scnstatus = 'A'";
}

$joinsContrato = $joins.$joinsCont.$joinsNotas;

$colunaExcelEmpenho  = "";
$groupbyExcelEmpenho = "";
$orderbyExcelEmpenho = "";

if( $_REQUEST['geraExcelEmpenho'] ){
	//$colunaExcelEmpenho  = ', e.empnumero, e.empvalorempenho as "Valor Empenho", e.empsituacao';
	$cabecalho = array_merge($cabecalho, array('Nº do Empenho', 'Valor do Empenho', 'Situação do Empenho'));
	$colunaExcelEmpenho  = ', e.empnumero, Coalesce((select sum(vve.vrlempenhocancelado) from par.empenho e2
									inner join par.v_vrlempenhocancelado vve on vve.empid = e2.empid 
								where e2.empid = e.empid), 0.00)  as valorempenho, e.empsituacao';
	$groupbyExcelEmpenho = ', e.empnumero, e.empid, e.empsituacao';
	$orderbyExcelEmpenho = ', mundescricao, empnumero, valorempenho, empsituacao';
}

if( $db->testa_superuser() && $_POST['pagamentoMaiorQueEmpenho'] == 'S' ){
	$query = sql_verificaPagamentoMaiorQueEmpenhoSubacao();
	$where[] = "(SELECT
					COUNT(empid)
				FROM
					(
					".str_replace(Array('%processo%', '%filtro%'), Array('p.prpnumeroprocesso',''), $query)."					
					) as foo ) > 0";
}

//ver($_REQUEST,d);
$sql = "
	Select * From (	
		Select 	distinct ";

		if(!$_REQUEST['exportarexcel']){
			$sql .=	"
				'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarListaEmpenhoPagamentoPar(\''||p.prpnumeroprocesso||'\', this);\"></center>' as mais,
				'<center>'||
				case when p.prpdocumenta is not null then $consultaDocumenta else $insereDocumenta
				end || '&nbsp;<img src=../imagens/money.gif style=cursor:pointer; onclick=\"verificaDocumentos( \'' || p.prpnumeroprocesso || '\', ' || p.prpid || ', \'Empenho\' , '|| iu.inuid ||');\"></center>' as acoes,
			";
			$groupColuna = "acoes,";
		}

		$sql .="
				Case When iu.itrid = 1 Then es.estuf Else m.estuf End as uf,
				m.mundescricao, 
                $sqlColunaNumeroProcesso, 
                (SELECT 
					SUM( par.recuperavalorvalidadossubacaoporano( sbd.sbaid, sbd.sbdano ) ) as vlr
				FROM 
					par.processoparcomposicao prc 
				INNER JOIN par.subacaodetalhe sbd ON sbd.sbdid = prc.sbdid
				WHERE
                                        prc.prpid = p.prpid AND prc.ppcstatus = 'A') as vlrsubacao,
                CASE WHEN p.prptipoexecucao = 'C' 
                THEN 
                	COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00)) 
                ELSE 
                	CASE WHEN dp.dopid IS NOT NULL THEN
                		COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00))
                	ELSE
                		COALESCE(dp.dopvalortermo, 0.00)
                	END
                END as valor_termo,
				Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00)  as empvalorempenho,
				Coalesce(pg.pagvalorparcela, 0.00) as pagvalorparcela,
				Coalesce(pge.pagvalorparcela_efetivada, 0.00) as pagvalorparcela_efetivada,
				
				(SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0)-Coalesce(pg.pagvalorparcela, 0.00) as a_pagar,
				CASE WHEN (CASE WHEN p.prptipoexecucao = 'C' 
			                THEN 
			                	COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00)) 
			                ELSE 
			                	CASE WHEN dp.dopid IS NOT NULL THEN
			                		COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00))
			                	ELSE
			                		COALESCE(dp.dopvalortermo, 0.00)
			                	END
			                END) > 0
			    	THEN		
						Coalesce(( ((Coalesce(pg.pagvalorparcela, 0.00))/
									(CASE WHEN p.prptipoexecucao = 'C' 
					                THEN 
					                	COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00)) 
					                ELSE 
					                	CASE WHEN dp.dopid IS NOT NULL THEN
					                		COALESCE(dp.dopvalortermo, Coalesce((SELECT saldo FROM par.vm_saldo_empenho_do_processo vsep where vsep.processo = p.prpnumeroprocesso and vsep.saldo > 0), 0.00))
					                	ELSE
					                		COALESCE(dp.dopvalortermo, 0.00)
					                	END
					                END))*100 ),0.00)::numeric(7,2) 
					ELSE 0.00
				END AS porcentagemporsubacao,
				Case When p.prptipoexecucao = 'C' Then 'Convênio' Else 'Termo' End as tipo,
				$colunaHabilita,
				CASE WHEN ed.esddsc IS NULL THEN 'Diagnóstico' ELSE ed.esddsc END as situacao,
				
				array_to_string(array( SELECT dp.dopnumerodocumento || '/' || date_part('year',dopdatainclusao) as numerotermo FROM par.vm_documentopar_ativos dp WHERE dp.prpid = p.prpid AND dp.mdoid IN ( 18, 19, 21, 28, 34, 32, 20, 23, 38, 22, 30, 29, 31, 24, 27, 25, 26, 3, 2 ) ),',<br>') as numerotermo
				
				-- dp.dopnumerodocumento || '/' || date_part('year',dopdatainclusao) as numerotermo
				$colunaExcelEmpenho
		From par.processopar p
		LEFT join(
		   SELECT em.empnumeroprocesso as empnumeroprocesso, 
			  pagstatus,
			  COALESCE(SUM(pg.pagvalorparcela),0.00) as pagvalorparcela
		   FROM par.pagamento pg
		   JOIN par.empenho em ON em.empid = pg.empid AND empstatus = 'A' AND pg.pagstatus = 'A' AND trim(pg.pagsituacaopagamento) IN ('EFETIVADO', '2 - EFETIVADO', 'ENVIADO AO SIAFI', '0 - AUTORIZADO', 'AUTORIZADO', '8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA')
		   GROUP BY empnumeroprocesso, pagstatus
		) as pg on pg.empnumeroprocesso = p.prpnumeroprocesso and pg.pagstatus='A'
		LEFT JOIN(
		   Select em.empnumeroprocesso as empnumeroprocesso, 
			  pagstatus,
			  coalesce(sum(pg.pagvalorparcela),0.00) as pagvalorparcela_efetivada
		   From par.pagamento pg
		   join par.empenho em on em.empid = pg.empid and empstatus = 'A' AND pg.pagstatus = 'A' 
		   where trim(pg.pagsituacaopagamento) ilike '%EFETIVADO%'
		   group by empnumeroprocesso, pagstatus
		) as pge on pge.empnumeroprocesso = p.prpnumeroprocesso and pge.pagstatus='A'
		INNER Join par.instrumentounidade iu on iu.inuid = p.inuid {$itrid}
		INNER JOIN par.empenho e ON e.empnumeroprocesso = p.prpnumeroprocesso and empstatus = 'A'
		LEFT  JOIN territorios.municipio m on m.muncod = iu.muncod
		LEFT  JOIN territorios.estado es on es.estuf = iu.estuf	
		left  JOIN par.vm_documentopar_ativos dp ON dp.prpid = p.prpid --AND dp.mdoid IN ( 18, 19, 21, 28, 34, 32, 20, 23, 38, 22, 30, 29, 31, 24, 27, 25, 26, 3, 2 )
		$joinGrupoMun
		left join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid and iue.iuestatus = 'A' and iue.iuedefault = true
		LEFT JOIN workflow.documento d on d.docid = iu.docid AND d.tpdid 	= 44 
		LEFT JOIN workflow.estadodocumento ed on ed.esdid = d.esdid
		$joinObrasVinculada  
		$joinsContrato
		WHERE p.prpstatus = 'A'  
		".(($where)?" AND ".implode(" AND ", $where):"")." 
		".
		$emenhoSolicitado.
		"Group by
                $groupColuna
                $sqlColunaNumeroProcesso,
                p.prpnumeroprocesso,
                ed.esddsc,
                p.prpdocumenta,
                m.mundescricao,
                dp.dopvalortermo,
                p.prptipoexecucao,
                uf,
                dp.dopid,
                pagvalorparcela,
				pagvalorparcela_efetivada,
                tipo,
                iue.iuesituacaohabilita,
                p.prpcnpj,
                p.prpid --, dp.dopnumerodocumento, dopdatainclusao
                $groupbyExcelEmpenho
	) foo
	 order by tipo, uf $orderbyExcelEmpenho";
// 	ver($sql,d);
	if($_REQUEST['exportarexcel']) {
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
		exit;
	} else {
        // Só monta lista quando clicar em pesquisar
        if(isset($_REQUEST['estuf'])){
            $db->monta_lista($sql,$cabecalho,100,5,'N','center','S','formprocesso');
        }
    }
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","22","","","","","","","id='ws_usuario_consulta'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="23" id="ws_senha_consulta" name="ws_senha_consulta">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="hidden" name="ws_empid" id="ws_empid" value="" />
				<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
