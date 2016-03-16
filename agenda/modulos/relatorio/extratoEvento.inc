<?php

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');
		
		$estuf = $_POST['estuf'];
		$sql = "SELECT 
					muncod AS codigo, 
					mundescricao AS descricao 
				FROM
					territorios.municipio
				WHERE
					estuf = '{$estuf}'
				ORDER BY
					mundescricao";
		echo $db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', 240, 'N', 'muncod');
		exit;		
		break;
	case 'camposImpressao':
		
		$campo = $_POST['campo'];
		
		$parametroagenda = new ParametroRelatorio();
		
		$parametroagenda->controleCamposRelatorio( $_SESSION['agenda']['agdid'], $campo );
		
		if( in_array( $campo, $_SESSION['arrCamposImpressao'] ) ){
			
			$newArr = array();
			
			foreach( $_SESSION['arrCamposImpressao'] as $arr ){
				
				if( $arr != $campo )
					$newArr[] = $arr;
			}
			$_SESSION['arrCamposImpressao'] = $newArr;
		
		}else{
			
			array_push( $_SESSION['arrCamposImpressao'], $campo ); 
		}
		exit;
		break;
	case 'controleParecerRelatorio':
		
		$id = $_POST['id'];
		
		if( in_array( $id, $_SESSION['arrParecerInfoExibido'] ) ){
			
			$newArr = array();
			
			foreach( $_SESSION['arrParecerInfoExibido'] as $arr ){
				
				if( $arr != $id )
					$newArr[] = $arr;
			}
			$_SESSION['arrParecerInfoExibido'] = $newArr;
		
		}else{
			
			array_push( $_SESSION['arrParecerInfoExibido'], $id ); 
		}
		exit;
		break;
	case 'controleMostraPareceres':
		
		$_SESSION['arrParecerInfoExibido'] = array();
		exit;
		break;		
}

function verificaOcultacaoParecer( $id ){
	
	if( !$_SESSION['arrParecerInfoExibido'] ){
		
		$_SESSION['arrParecerInfoExibido'] = array();
	}
	if( in_array( $id, $_SESSION['arrParecerInfoExibido'] ) ){
		
		return false;
	}
	return true;
}

$agdid = $_SESSION['agenda']['agdid'];



if ( empty($agdid) ){
	die("<script>
			alert('Faltam parametros para acessar a tela!');
			window.close();
		 </script>");
}
$agenda = new Agenda( $agdid );
$dados  = $agenda->getDados();
extract( $dados );	

extract( $_POST );
$evedtinicio = ($evedtinicio ? formata_data_sql( $evedtinicio ) : $evedtinicio);
$evedtfinal  = ($evedtfinal ? formata_data_sql( $evedtfinal ) : $evedtfinal);

if(!$evedtinicio) {
	$_POST['evedtinicio'] = date("01/m/Y");
	$evedtinicio = formata_data_sql( date("01/m/Y") );
	$_POST['op'] = 'pesquisar';
	$_POST['agdid'] = $agdid;
}
if(!$evedtfinal) {
	$_POST['evedtfinal'] = date("t/m/Y", strtotime(date('Y/m/d')));
	$evedtfinal = formata_data_sql( date("t/m/Y", strtotime(date('Y/m/d'))) );
	$_POST['op'] = 'pesquisar';
	$_POST['agdid'] = $agdid;
}


if ( $_POST['op'] != 'imprimir' ):
	include APPRAIZ . 'includes/cabecalho.inc';
	print '<br/>';
	$arAba = abaEdicaoAgenda();
	echo montarAbasArray($arAba, "?modulo=relatorio/extratoEvento&acao=A");
		
	monta_titulo( 'Extrato de Eventos', '');

?>
<style>
@media print {.notprint { display: none }}
@media screen {.notscreen { display: none }}
</style>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
<!--

function mostraOpcoesImpressao(){

	var input = $('#config_ativo');
	var dv = $('#dvOpcoesImpressao');
	if( input ){
		if( input.val() == 'F' ){
		
			dv.css("display", "")
			input.val("T");
		}else{
		
			dv.css("display", "none")
			input.val("F");
		}
	}
}

function marcaCampoImpressao(obj){

	var url = location.href;
	$.ajax({
		  url  		 : url,
		  type 		 : 'post',
		  data 		 : {ajax  : 'camposImpressao', 
		  		  	    'campo' : obj.id},
		  dataType   : "html",
		  async		 : false,
		  beforeSend : function (){
		  	divCarregando();
		  },
		  error 	 : function (){
		  	divCarregado();
		  },
		  success	 : function ( data ){
		  	
		  	divCarregado();
		  	
			validarFormEvento('checkbox');
		  }
	});	
}

function ocultaInfoParecer(id){

	if( document.getElementById('img['+id+']').title == "Mostrar Informação" ){
	
		document.getElementById('info['+id+']').style.color = "#1C1C1C";
		document.getElementById('img['+id+']').src = "/imagens/hide.png";
		document.getElementById('img['+id+']').title = "Ocultar Informação";
		document.getElementById('info['+id+']').className = "";
	
	}else{
		document.getElementById('info['+id+']').style.color = "#C1CDCD";
		document.getElementById('img['+id+']').src = "/imagens/show.png";
		document.getElementById('img['+id+']').title = "Mostrar Informação";
		document.getElementById('info['+id+']').className = "notprint";
		
		
	}
	var url = location.href;
	$.ajax({
		  url  		 : url,
		  type 		 : 'post',
		  data 		 : {ajax  : 'controleParecerRelatorio', 
		  		  	    'id' : id},
		  dataType   : "html",
		  async		 : true,
		  beforeSend : function (){

		  },
		  error 	 : function (){
		  	divCarregado();
		  },
		  success	 : function ( data ){

		  }
	});	
}

function mostrarPareceres(){

	var url = location.href;
	$.ajax({
		  url  		 : url,
		  type 		 : 'post',
		  data 		 : {ajax  : 'controleMostraPareceres', 
		  		  	    'mostra' : 'true'},
		  dataType   : "html",
		  async		 : false,
		  beforeSend : function (){
		  	divCarregando();
		  },
		  error 	 : function (){
		  	divCarregado();
		  },
		  success	 : function ( data ){
		  	
		  	divCarregado();
		  	
			validarFormEvento('checkbox');
		  }
	});	
}

function validarFormEvento( type ){
	$('#op').val('pesquisar');
	$('#sltid option').attr('selected', true);
	$('#etaid option').attr('selected', true);
	$('#cevid option').attr('selected', true);
	if( type != 'checkbox' )
		$('[name=formEvento]').attr('target', '').attr('action', '').submit();
	else					  
		$('[name=formEvento]').attr('target', '').attr('action', '?modulo=relatorio/extratoEvento&acao=A&chk=true').submit();
}
function imprimirRel(){
	$('#op').val('imprimir');
	$('#sltid option').attr('selected', true);
	$('#etaid option').attr('selected', true);
	$('#cevid option').attr('selected', true);
	$('[name=formEvento]').attr('target', 'printExtratoEvento')
						  .attr('action', '?modulo=relatorio/extratoEvento&acao=A')
						  .submit();
}

function carregarMunicipio( estuf ){
					var td	= $('#td_municipio');
					if ( estuf != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'municipio', 
									  		  	    estuf : estuf},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.find('select option:first').attr('selected', true);
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
									  }
								});	
					}else{
						td.find('select option:first').attr('selected', true);
						td.find('select').attr('selected', true)
										 .attr('disabled', true);
					}			
}
//-->
</script>
<?php
//controlador dos campos que aparecerão no relatório (customização feita pelo usuário)


if( $_REQUEST['op'] != 'pesquisar'){
	
	$arrAllCampos = array('numero', 'sltdsc','etadsc', 'evedtinicio', 'eveassunto', 'local', 'tpedsc','tppdsc', 'area' , 'info', 'situacao');
	
	$parametroagenda = new ParametroRelatorio();
	
	$dadosconsulta = $parametroagenda->carregaDados($agdid);
	
	$dadosparametroagenda = array();
	
	foreach( $dadosconsulta as $arr ){
	
		array_push($dadosparametroagenda, $arr['pardsccampo'] );
	}		

	$_SESSION['arrCamposImpressao'] = array_diff($arrAllCampos, $dadosparametroagenda);
	
	if( $_REQUEST['chk'] != 'true')
		$_SESSION['arrParecerInfoExibido'] = array();
}
?>
<form name="formEvento" method="post" action="">
<input name="op" id="op" value="" type="hidden">
<input name="agdid" id="agdid" value="<?php echo $agdid; ?>" type="hidden">
<table class="tabela" align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Dono da Agenda</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome:</td>
		<td>
		<b><?php echo $agddsc; ?></b>
		</td>
	</tr>
	<tr>
		<td class="SubTituloCentro" colspan="2">Filtro</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Assunto:</td>
		<td>
		<?=campo_texto("eveassunto","N","S","Assunto",50,100,"","")?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipo:</td>
		<td>
		<?php
		$evento = new TipoEvento();
		$dados = $evento->listaCombo();
		echo $db->monta_combo("tpeid", $dados, 'S','Selecione...','', '', '',240,'N','tpeid');
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Prioridade:</td>
		<td>
		<?php
		$prioridade = new TipoPrioridade();
		$dados = $prioridade->listaCombo();
		echo $db->monta_combo("tppid", $dados, 'S','Selecione...','', '', '',240,'N','tppid');
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">UF:</td>
		<td>
		<?
		$estuf = $_POST['estuf'];
		$sql = "SELECT
					estuf AS codigo,
					estuf || ' - ' || estdescricao AS descricao
				FROM
					territorios.estado
				order by 2";
		?>
		<?=$db->monta_combo("estuf", $sql, "S",'Selecione...','carregarMunicipio', '', '',240,'N','estuf'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Município:</td>
		<td id="td_municipio">
		<?php 
		if ($estuf){
			$sql = "SELECT 
						muncod AS codigo, 
						mundescricao AS descricao 
					FROM
						territorios.municipio
					WHERE
						estuf = '{$estuf}'
					ORDER BY
						mundescricao";
			$habMun = 'S';
		}else{
			$sql = array();
			$habMun = 'N';
		}		
		$muncod = $_POST['muncod'];
		$habMun = ($disable == 'N' ? $disable : $habMun);
		echo $db->monta_combo("muncod", $sql, $habMun,'Selecione...','', '', '',240,'N','muncod'); 
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Solicitante:</td>
		<td>
		<?php
		$solicitante = new Solicitante();
		$sql 		 = $solicitante->listaComboPopupSQL();
		$sltid 		 = $solicitante->carregaComboPopupBySltid( (array) $sltid );
		
		$filtro = array(
						array("codigo" => "sltdsc",
							  "descricao" => "<b>Solicitante:</b>",
							  "string" => "1")
						);
		combo_popup('sltid', $sql, 'Solicitante(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Cargo:</td>
		<td>
		<?=campo_texto("sltcargo","N","S","Assunto",50,100,"","")?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Entidade/Pessoa:</td>
		<td id="td_entidade">
		<?php
		$entidade = new EntidadeAgenda();
		$sql 	  = $entidade->listaComboPopupSQL();
		$etaid 	  = $entidade->carregaComboPopupByEtaid( (array) $etaid );
		
		$filtro = array(
						array("codigo" 		=> "etadsc",
							  "descricao" 	=> "<b>Entidade/Pessoa:</b>",
							  "string" 		=> "1")
						);
		combo_popup('etaid', $sql, 'Entidade(s)/Pessoa(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Contato:</td>
		<td id="td_entidade">
		<?php
		$contato = new ContatosEvento();
		$sql 		 = $contato->listaComboPopupSQL();
		$cevid		 = $contato->carregaComboPopupByCevid( (array) $cevid );
		
		$filtro = array(
						array("codigo" => "cevcargo || cevexpressao || cevdsc",
							  "descricao" => "<b>Contato:</b>",
							  "string" => "1")
						);
						
		combo_popup('cevid', $sql, 'Contato(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
		?>
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Data:</td>
		<td>
		<?=campo_data2( 'evedtinicio', 'N', 'S', '', 'S' ); ?>
		&nbsp;
		Até
		&nbsp;
		<?=campo_data2( 'evedtfinal', 'N', 'S', '', 'S' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Hora:</td>
		<td>
		<?=campo_texto("evehsinicio","N","S","Hora Início",4,5,"##:##","","right")?>
		&nbsp;
		Até
		&nbsp;
		<?=campo_texto("evehsfinal","N","S","Hora Fim",4,5,"##:##","","right")?>
		</td>
	</tr>	
	<tr bgcolor="#CCCCCC">
	   <td>&nbsp;</td>
	   <td>
	   	<input type="button" name="btpesq" value="Pesquisar" onclick="validarFormEvento('form')" class="botao">
	   </td>
	</tr> 
</table>
</form>
<?php 
else:
?> 
<html>
	<head>
		<title>Extrato de Eventos</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<style>
			@page {size:landscape}
		</style>
		<script>
		window.print();
		</script>tpedsc
		
	</head>	
<body marginheight="0" marginwidth="0">
<?php
endif;

if ( $_POST['op'] == 'pesquisar' || $_POST['op'] == 'imprimir' ):
	$evento = new Evento();
	$dados  = $evento->extratoByAgenda( $_POST );
	
//	for ( $i = 0; $i < 40; $i++ ){
//		$dados[] = current( $dados );		
//	}
?>
<table class="tb_rel_extrato" align="center" style="border-style:solid;" width="95%" border="<?php echo ($_POST['op'] == 'imprimir' ? 1 : 0)  ?>" bgcolor="#FFF">
	<thead>
		<tr>
			<td colspan="10" style="border: 0px; border-bottom: 1px double #000000;">
				<?php 
				if ($_POST['op'] != 'imprimir'):
				?>
				<center>
				<!-- javascript:imprimirRel(); -->
					<a style="text-decoration: underline; cursor: pointer;" onclick="mostraOpcoesImpressao();">
						<img border="0" style="" src="/imagens/print_config.png">
						[Configurar Impressão]
					</a>
					<br />
					<input type="hidden" name="config_ativo" id="config_ativo" <?php if( $_REQUEST['chk'] == "true" ) { ?> value="T"  <?php } else if( $_REQUEST['chk'] != "true" ) { ?> value="F"  <?php } ?>/>
					<div id="dvOpcoesImpressao" <?php if( $_REQUEST['chk'] != "true" ) { ?> style="display: none;"<?php } ?> >
						<br />
						<table class="tabela" align="center" >
							<tr>
								<td class="SubTituloCentro" colspan="2">Campos do Relatório</td>
							</tr>
							<tr>
								<td class="SubTituloDireita" width="30%" style="font-style: italic; font-size: 10pt;">Selecione os campos que aparecerão no relatório:</td>
								<td>								
									<br />
									<input type="checkbox" <?php if( in_array( 'numero', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="numero" id="numero" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Nº SAG
									<br />
									<input type="checkbox" <?php if( in_array( 'sltdsc', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="sltdsc" id="sltdsc" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Solicitante
									<br />
									<input type="checkbox"   <?php if( in_array( 'etadsc', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="etadsc" id="etadsc" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Entidade
									<br />
									<input type="checkbox"  <?php if( in_array( 'evedtinicio', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="evedtinicio" id="evedtinicio" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Data/Hora
									<br />
									<input type="checkbox" <?php if( in_array( 'eveassunto', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="eveassunto" id="eveassunto" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Assunto
									<br />
									<input type="checkbox" <?php if( in_array( 'local', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?>  name="local" id="local" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Local
									<br />
									<input type="checkbox" <?php if( in_array( 'tpedsc', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?>  name="tpedsc" id="tpedsc" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Tipo
									<br>
									<input type="checkbox" <?php if( in_array( 'tppdsc', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?>  name="tppdsc" id="tppdsc" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Prioridade
									<br />
									<input type="checkbox"  <?php if( in_array( 'area', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?>  name="area" id="area" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Encaminhado para Área
									<br />									
									<input type="checkbox" checked=checked disabled=disabled name="info" id="info" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Informação
									<br />
									<input type="checkbox" <?php if( in_array( 'situacao', $_SESSION['arrCamposImpressao'] ) ){ ?> checked=checked <?php } ?> name="situacao" id="situacao" onclick="marcaCampoImpressao(this);" value="T"> &nbsp;
									Situação Encaminhamento
									<br />
								</td>
							</tr>
						</table>
						<br />
						<center>
						<a href="javascript:imprimirRel();">
							<img border="0" style="" src="/imagens/print.png">
							[Imprimir]
						</a>
				</center>
					</div>
				</center>
				<?php 
				endif;
				?>
				<div style="float: right; margin-right: 2px; font-weight: bold;">
				<br>
				<?php 
				echo "Data:&nbsp;" . date('d/m/Y') . "<br>";
				echo "Hora:&nbsp;" . date('H:i') . "<br>";
//				echo "Pág.:&nbsp;&nbsp;1<br>";
				?>
				</div>
				<div style="float:left; height: 50px; margin-right: 3px; width: 30px;">
				MEC
				</div>
				<div style="font-weight: bold;">
				Ministério da Educação
				</div>
				<div style="font-weight: bold;">
				Chefia de Gabinete do Ministro
				</div>
				<div style="font-weight: bold;">
				Solicitação de Agenda(SA) e Controle de Documentos(CD)
				</div>
			</td>
		</tr>
		<tr>
		
			<?php if( in_array( 'numero', $_SESSION['arrCamposImpressao'] ) ){ ?>
			<th width="30">
				Nº SAG
			</th>
			<?php } ?>
			<?php if( in_array( 'sltdsc', $_SESSION['arrCamposImpressao'] ) ) { ?>
			<th width="200">
				Solicitante
			</th>
			<?php } ?>
			<?php if( in_array('etadsc', $_SESSION['arrCamposImpressao'] ) ){ ?>
			<th width="200">
				Entidade
			</th>
			<?php } ?>
			<?php if( in_array( 'evedtinicio', $_SESSION['arrCamposImpressao'] ) ){ ?>
			<th width="30">
				Data/Hora
			</th>
			<?php } ?>
			<?php if( in_array( 'eveassunto', $_SESSION['arrCamposImpressao'] )){ ?>
			<th width="250">
				Assunto
			</th>
			<?php } ?>
			<?php if( in_array( 'local', $_SESSION['arrCamposImpressao'] )) { ?>
			<th width="80">
				Local
			</th>
			<?php } ?>
			<?php if( in_array( 'tpedsc', $_SESSION['arrCamposImpressao'] )){ ?>
			<th width="30">
				Tipo
			</th>
			<?php } ?>
			<?php if( in_array( 'tppdsc', $_SESSION['arrCamposImpressao'] )){ ?>
			<th width="30">
				Prioridade
			</th>
			<?php } ?>
			<?php if( in_array( 'area', $_SESSION['arrCamposImpressao'] )){ ?>
			<th width="80">
				Encaminhado para Área
			</th>
			<?php } ?>
			
			<th width="100" align="left">
				<?php if(  $_POST['op'] != 'imprimir' ) { ?>
					<img title="Mostrar Todos" id="imgMostraTodos" border="0" src="/imagens/show.png" style="cursor: pointer; text-align: left; " onclick="mostrarPareceres();"/>
				<?php } ?>
				Informação
			</th>
			<?php if( in_array( 'situacao', $_SESSION['arrCamposImpressao'] )){ ?>
			<th width="100">
				Situação Encaminhamento
			</th>
			<?php } ?>
		</tr>
	</thead>
	<?php 
	if($dados){
		$cont = 1;
		foreach ( $dados as $dados ):
			$cor = ($cont%2 ? '#FFFFFF' : '#E5EAED');
			$cont++;
			$area = new EventoArea();
			$dadoArea = $area->listaDscAreaSituacaoByEveid( $dados['eveid'] );
			
			$dscArea 	 = "";
			$dscSituacao = ""; 
			foreach ($dadoArea as $dadoArea){
				$dscArea 	 .= "<div style='border: 0px dashed #CCCCCC; height: 30px;'>" . strtoupper( $dadoArea['aevdsc'] ) . "</div>";
				$dscSituacao .= "<div style='border: 0px dashed #CCCCCC; height: 30px;'>" . strtoupper( $dadoArea['esddsc'] ) . "</div>"; 
			}
			if ( empty($dscArea) ){
				$dscArea 	 = '-';
				$dscSituacao = 'PARA DESPACHO';
			}
			
	//		dbg($dscArea);
	//		dbg($dscSituacao);
	//		dbg($dadoArea, d);
			
		?>
		<tr bgcolor="<?php echo $cor ?>">
			<?php if( in_array( 'numero', $_SESSION['arrCamposImpressao'] ) ){ ?>
				<td>
					<?php echo $dados['numero'] ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'sltdsc', $_SESSION['arrCamposImpressao'] ) ) { ?>
				<td>
					<?php echo $dados['sltdsc'] ?>
				</td>
			<?php } ?>
			<?php if( in_array('etadsc', $_SESSION['arrCamposImpressao'] ) ){ ?>
				<td>
					<?php echo $dados['etadsc'] ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'evedtinicio', $_SESSION['arrCamposImpressao'] ) ){ ?>
				<td>
					<?php echo $dados['evedtinicio'] ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'eveassunto', $_SESSION['arrCamposImpressao'] )){ ?>
				<td>
					<?php echo $dados['eveassunto'] ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'local', $_SESSION['arrCamposImpressao'] )) { ?>
				<td>
					<?php echo $dados['mundescricao'] . ' - ' . $dados['estuf']  ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'tpedsc', $_SESSION['arrCamposImpressao'] )){ ?>
				<td>
					<?php echo $dados['tpedsc'] ?>
				</td>
			<?php } ?>			
			<?php if( in_array( 'tppdsc', $_SESSION['arrCamposImpressao'] )){ ?>
				<td>
					<?php echo $dados['tppdsc'] ?>
				</td>
			<?php } ?>
			<?php if( in_array( 'area', $_SESSION['arrCamposImpressao'] )){ ?>
				<td>
					<?php echo $dscArea; ?>
				</td>
			<?php } ?>
			<td>
				<?php 
					$sql = "SELECT
							he.hseid,
							TO_CHAR(he.hsedtinclusaoparecer, 'DD/MM/YYYY HH24:MI') AS data,
							he.hseparecer,
							u.usunome AS usuarioparecer
						FROM
							agenda.eventoarea ea
						JOIN agenda.histencaminhamento he ON he.evaid = ea.evaid
						JOIN workflow.estadodocumento ed ON ed.esdid = he.esdid				
						JOIN seguranca.usuario u ON u.usucpf = he.usucpfparecer		
						WHERE
							he.hsestatus = 'A'
						AND
							ea.eveid = ".$dados['eveid']."
						ORDER BY
							he.hsedtinclusaoparecer DESC"; 
					//$cabecalho = array("Data","Parecer","Responsável");
					//$db->monta_lista_simples($sql,$cabecalho,100,1,'N');
					$parecer = $db->carregar($sql);
					if($parecer){
						foreach ($parecer as $v){
							
							if(  $_POST['op'] == 'imprimir' ) {
								if( verificaOcultacaoParecer($v['hseid'])  ){
									echo "<div id=\"info[".$v['hseid']."]\">";							
										echo "<b>Data:</b> ".$v['data'];
										echo "<br><b>Informações:</b> ".$v['hseparecer'];
										echo "<br><b>Responsável:</b> ".$v['usuarioparecer'];
										echo "<br><b>Informações Complementares:</b> ".$v['hseinfrespagenda'];
										echo "<br>";
										echo "</div>";
									echo "<br>";
								}
							}else{
								if( verificaOcultacaoParecer($v['hseid'])  ){
									
									echo "<div id=\"info[".$v['hseid']."]\">";							
										echo '<img title="Ocultar Informação" id="img['.$v['hseid'].']" border="0" src="/imagens/hide.png" style="cursor: pointer;" onclick="ocultaInfoParecer(\''.$v['hseid'].'\')"/>';
										echo "&nbsp;  <b>Data:</b> ".$v['data'];
										echo "<br><b>Informações:</b> ".$v['hseparecer'];
										echo "<br><b>Responsável:</b> ".$v['usuarioparecer'];
										echo "<br><b>Informações Complementares:</b> ".$v['hseinfrespagenda'];
										echo "<br>";
										echo "</div>";
									echo "<br>";
								}else{
									
									echo "<div id=\"info[".$v['hseid']."]\" style=\" color: #C1CDCD !important;\">";							
										echo '<img title="Mostrar Informação" id="img['.$v['hseid'].']" border="0" src="/imagens/show.png" style="cursor: pointer; " onclick="ocultaInfoParecer(\''.$v['hseid'].'\')"/>';
										echo "&nbsp;  <b>Data:</b> ".$v['data'];
										echo "<br><b>Informações:</b> ".$v['hseparecer'];
										echo "<br><b>Responsável:</b> ".$v['usuarioparecer'];
										echo "<br><b>Informações Complementares:</b> ".$v['hseinfrespagenda'];
										echo "<br>";
										echo "</div>";
									echo "<br>";
								}
							}
						}
					}
				?>
				&nbsp;
			</td>
			<?php if( in_array( 'situacao', $_SESSION['arrCamposImpressao'] )){ ?>
				<td>
					<?php echo $dscSituacao; ?>
				</td>
			<?php } ?>
		</tr>
		<?php 
		endforeach;
	}
	else{
		echo "<tr><td colspan=10 align=center><font color=red>Não existem registros.</font></td></tr>";
	}
	?>
</table>
<?php 
endif;
?>