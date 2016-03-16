<?php

$obProcesso = new Processos();

if($_POST['requisicao'] == 'salvar'){
	$obProcesso->cadastraProcessos( $_POST );
} 

if( $_REQUEST['prcid'] || $_SESSION['seed']['prcid'] ){
	$prcid = $_REQUEST['prcid'] ? $_REQUEST['prcid'] : $_SESSION['seed']['prcid'];
	$_SESSION['seed']['prcid'] = $prcid;
	$arProcesso = $obProcesso->carregaProcesso( $prcid );
	$arProcesso['prcdatainiciovigencia'] = formata_data( $arProcesso['prcdatainiciovigencia'] );
	$arProcesso['prcdatafimvigencia'] = formata_data( $arProcesso['prcdatafimvigencia'] );
	if( !empty( $arProcesso['prcvalor'] ) )
		$arProcesso['prcvalor'] = number_format( $arProcesso['prcvalor'], 2, ',', '.' );
	if( !empty( $arProcesso['prcdataentradaccon'] ) )
		$arProcesso['prcdataentradaccon'] = formata_data( $arProcesso['prcdataentradaccon'] );
	extract( $arProcesso );
	
	$prcnumprocesso = substr($prcnumprocesso,0,5) . "." .
					  substr($prcnumprocesso,5,6) . "/" .
					  substr($prcnumprocesso,11,4) . "-" .
					  substr($prcnumprocesso,15,2);
}

if( $_POST['tpcid'] ){
	$tpcid = $_POST['tpcid'];
}

#acesso aos campos pelo tipo de processo
$arNumProcesso = array(1, 2, 3, 4, 5, 6, 7, 8, 9);
$arDataEntrada = array(2, 3, 4, 5, 6, 7, 8, 9);
$arDeptoResponsavel = array(1, 2, 3, 4, 5, 6, 7, 8, 9);
$arObjeto = array(1, 2, 3, 4, 5, 6, 7, 8, 9);
$arNumPortaria = array( 3 );
$arRetificacao = array( 3 );
$arValor = array(1, 2, 3, 4);
$arModalidade = array( 2);
$arNumDispensa = array( 4 );
$arPortaria = array( 3 );
$arIniVigencia = array(1, 2, 3, 5, 6, 7, 8);
$arFimVigencia = array(1, 2, 3, 4, 5, 6, 7, 8, 9);
$arFimVigencia = array(1, 2, 3, 4, 5, 6, 7, 8);
$arInteressado = array(1, 2, 3, 4, 5, 6, 9);
$arNumConvenio = array( 1 );
$arNumConvenioSiaf = array( 1 );
$arNumContrato = array( 2, 4 );
$arInexigibilidade = array( 2 );
$arCedente = array( 8 );
$arCessionario = array( 8 );
$arSecretaria = array( 7 );
$arNumAcordo = array( 7 );
$arPublicacao = array( 7 );
$arNumDoacao = array( 6 );
$arGerente = array( 1, 2, 3 );

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

if( $tipo == 'cadastrar' ){
	monta_titulo('SEED', 'INCLUIR PROCESSO');
} else {
	monta_titulo('SEED', 'CADASTRO DE PROCESSO');
}
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="js/processo.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="prcid" id="prcid" value="<?=$prcid; ?>">
<input type="hidden" name="sitid" id="sitid" value="8">
<input type="hidden" name="tipo" id="tipo" value="<?=$tipo; ?>">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Tipo Processo:</td>
		<td><?
			$sql = "SELECT 
					  tpcid as codigo,
					  tpcdsc as descricao
					FROM 
					  seed.tipoprocesso
					WHERE
						tpcstatus = 'A'";
			$db->monta_combo("tpcid", $sql, 'S', 'Selecione...', 'verificaTipoProcesso', '', '', '265','S','tpcid', '', '', 'Tipo Processo');
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Número Processo:</td>
		<td><?=campo_texto( 'prcnumprocesso', 'S', 'S', 'Número Processo', 30, 100, '#####.######/####-##', '','','','','id="prcnumprocesso"'); ?></td>
	</tr>
	<?if( in_array( $tpcid, $arDataEntrada ) ){?>
	<tr id="dataentrada" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Data Entrada:</td>
		<td><?=campo_data2('prcdataentradaccon', 'S','S','Data Entrada','','',''); ?></td>
	</tr>
	<?} ?>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Objeto:</td>
		<td><?=campo_textarea('prcobjeto', 'S', 'S', 'Objeto', 80, 5, 4000, '', '', '', '', 'Objeto' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Departamento Responsável:</td>
		<td><?
			$sql = "SELECT 
					  	dreid as codigo,
					  	dredsc as descricao
					FROM 
					  	seed.departamentoresponsavel
					WHERE
						drestatus = 'A'";
			$db->monta_combo("dreid", $sql, 'S', 'Selecione...', '', '', '', '265','S','dreid', '', '', 'Departamento Responsável');?></td>
	</tr>
	<tr id="inivigencia" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Início Vigência:</td>
		<td><?=campo_data2('prcdatainiciovigencia', 'S','S','Início Vigência','','',''); ?></td>
	</tr>
	<tr id="fimvigencia" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Término Vigência:</td>
		<td><?=campo_data2('prcdatafimvigencia', 'S','S','Término Vigência','','',''); ?></td>
	</tr>
	<?if( in_array( $tpcid, $arInteressado ) ){?>
	<tr id="interessado" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Interessado:</td>
		<td><?
			$sql = "SELECT 
					  	intid as codigo,
					  	intdsc as descricao
					FROM 
					  seed.interessado
					WHERE
						intstatus = 'A'";
			$db->monta_combo("intid", $sql, 'S', 'Selecione...', '', '', '', '265','','intid', '', '', 'Interessado');
			?></td>
	</tr>
	<?} if( in_array( $tpcid, $arValor ) ){ ?>
	<tr id="valor" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Valor (R$):</td>
		<td><?=campo_texto('prcvalor', 'S', 'S', 'Valor (R$)', 20, 15, '[###.]###,##', '','','','','id="prcvalor"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumConvenio ) ){
			$prcnumconvenio = $arProcesso['prcnumdocumento']; 
	?>
	<tr id="numconvenio" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número Convênio:</td>
		<td><?=campo_texto('prcnumconvenio', 'S', 'S', 'Número Convênio', 10, 8, '', '','','','','id="prcnumconvenio"', 'formataNumDocumento(this);'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumConvenioSiaf ) ){ ?>
	<tr id="numconveniosiaf" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número Convênio SIAF:</td>
		<td><?=campo_texto('prcnumconveniosiafi', 'S', 'S', 'Número Convênio SIAF', 10, 6, '[#]', '','','','','id="prcnumconveniosiafi"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumContrato ) ){
			$prcnumcontrato = $arProcesso['prcnumdocumento']; 
	?>
	<tr id="numcontrato" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número Contrato:</td>
		<td><?=campo_texto('prcnumcontrato', 'S', 'S', 'Número Contrato', 10, 8, '', '','','','','id="prcnumcontrato"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arModalidade ) ){ ?>
	<tr id="modalidade" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Modalidade da Licitação:</td>
		<td><?
			$sql = "SELECT 
					  	tilid as codigo,
					  	tildsc as descricao
					FROM 
					  	seed.tipolicitacao
					WHERE
						tilstatus = 'A'";
			$db->monta_combo("tilid", $sql, 'S', 'Selecione...', '', '', '', '265','S','tilid', '', '', 'Modalidade da Licitação');
			?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumDispensa ) ){
			$prcnumdispensa = $arProcesso['prcnumdocumento']; 
	?>
	<tr id="numdispensa" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número da Dispensa:</td>
		<td><?=campo_texto('prcnumdispensa', 'S', 'S', 'Número da Dispensa', 10, 8, '', '','','','','id="prcnumdispensa"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arInexigibilidade ) ){ 
			$prcnuminegixibilidade = $arProcesso['prcnumdocumento'];
		?>
	<tr id="numinegixibilidade" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número da Inegixibilidade:</td>
		<td><?=campo_texto('prcnuminegixibilidade', 'S', 'S', 'Número da Inegixibilidade', 10, 8, '', '','','','','id="prcnuminegixibilidade"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arCedente ) ){ ?>
	<tr id="cedente" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Cedente:</td>
		<td><?=campo_texto('prccedente', 'S', 'S', 'Cedente', 70, 50, '', '','','','','id="prccedente"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arCessionario ) ){ ?>
	<tr id="cessionario" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Cessionário:</td>
		<td><?
			$sql = "SELECT 
					  	cesid as codigo,
					  	cesdsc as descricao
					FROM 
					  seed.cessionario
					WHERE
						cesstatus = 'A'";
			$db->monta_combo("cesid", $sql, 'S', 'Selecione...', '', '', '', '265','S','cesid', '', '', 'Cessionário');
			?></td>
	</tr>
	<?} if( in_array( $tpcid, $arSecretaria ) ){ ?>
	<tr id="secretaria" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Secretaria:</td>
		<td><?
			$sql = "SELECT 
					  	secid as codigo,
					  	secdsc as descricao
					FROM 
					  	seed.secretaria
					WHERE
						secstatus = 'A'";
			$db->monta_combo("secid", $sql, 'S', 'Selecione...', '', '', '', '265','S','secid', '', '', 'Secretaria');
			?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumAcordo ) ){
			$prcnumacordo = $arProcesso['prcnumdocumento']; 
	?>
	<tr id="numacordo" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número do Acordo:</td>
		<td><?=campo_texto('prcnumacordo', 'S', 'S', 'Número do Acordo', 10, 8, '', '','','','','id="prcnumacordo"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arPublicacao ) ){ ?>
	<tr id="publicacao" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Publicação no DOU:</td>
		<td><?=campo_texto('prcdou', 'S', 'S', 'Publicação no DOU', 30, 20, '', '','','','','id="prcdou"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arNumDoacao ) ){
			$prcnumdoacao = $arProcesso['prcnumdocumento']; 
	?>
	<tr id="numdoacao" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número da Doação:</td>
		<td><?=campo_texto('prcnumdoacao', 'S', 'S', 'Número da Doação', 10, 8, '', '','','','','id="prcnumdoacao"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arPortaria ) ){ ?>
	<tr id="portaria" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Número Portaria:</td>
		<td><?=campo_texto('prcnumportaria', 'S', 'S', 'Número Portaria', 10, 8, '', '','','','','id="prcnumportaria"'); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arRetificacao ) ){ ?>
	<tr id="retificacao" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Retificação:</td>
		<td><?=campo_textarea( 'prcretificacao', 'N', 'S', 'Retificação', 80, 5, 4000, '', '', '', '', 'Retificação' ); ?></td>
	</tr>
	<?} if( in_array( $tpcid, $arGerente ) ){ ?>
	<tr id="gerente" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Gerente:</td>
		<td><?
			$sql = "SELECT 
					  	gerid as codigo,
					  	gerdsc as descricao
					FROM 
					  	seed.gerente
					WHERE
						gerstatus = 'A'";
			$db->monta_combo("gerid", $sql, 'S', 'Selecione...', '', '', '', '265','S','gerid', '', '', 'Gerente');
			?></td>
	</tr>
	<?} ?>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<?if( $tpcid ){ ?>
				<input type="button" value="Salvar" onclick="salvarProcesso(<?=$tpcid; ?>);" style="cursor: pointer;"/>
			<?}else{ ?>
				<input type="button" value="Salvar" disabled="disabled" style="cursor: pointer;"/>
			<?} ?>
			<input type="button" value="Cancelar" onclick="voltarProcesso();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>