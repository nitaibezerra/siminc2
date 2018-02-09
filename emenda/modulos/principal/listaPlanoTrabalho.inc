<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
//include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if($_REQUEST['carregaEmendaDetalhePTA']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->carregaEmendaDetalhePTA_Ajax($_POST);
	exit;
}

if($_REQUEST['requisicao'] == 'exportar'){
	$arSql =  $obPTA->listaPlanoTrabalho();
	$obPTA->exportarExcelPTA( $arSql );
}

$usucpf = $_SESSION['usucpf'];	

// Destrói o id do plano de trabalho na sessão
unset($_SESSION["emenda"]["ptrid"]);
unset($_SESSION["emenda"]["ptridpai"]);
unset($_SESSION["emenda"]["ptrpermitealt"]);
unset($_SESSION['emenda']['tpopup']);
unset($_SESSION['emenda']['federal']);
unset($_SESSION['emenda']['ptiid']);
unset($_SESSION['emenda']['iniid']);

$perfil = $obPTA->pegaPerfil();

if( in_array( INSTITUICAO_BENEFICIADA, $perfil) ) {
	if(!$_SESSION["emenda"]["enbid"]) {
		echo "<script>
				alert('Você deve selecionar uma Instituição.');
				location.href='emenda.php?modulo=principal/listaInstituicoes&acao=A&retorno=listaPlanoTrabalho';
			  </script>";
	}
} else {
	unset($_SESSION["emenda"]["enbid"]);
}

if($_REQUEST['enbid']){
	$enbid = $_REQUEST['enbid']; 
	$_SESSION["emenda"]["enbid"] = $enbid;
}else{
	$enbid = $_SESSION["emenda"]["enbid"];
}

if( $_SESSION['emenda']['tpopup'] != 'popup' ){
	include  APPRAIZ."includes/cabecalho.inc";
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
		  <script type="text/javascript" src="/includes/prototype.js"></script>
		  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}
echo $obPTA->exibeInstituicaoBenefiada( $enbid );

if($_REQUEST["excluir_plano_trabalho"] && $_REQUEST["excluir_plano_trabalho"] != "") {
	if( $db->testa_superuser() || in_array( ADMINISTRADOR_INST, $perfil) ){
		$sql = "UPDATE emenda.planotrabalho SET ptrstatus = 'I', usucpfalteracao = '".$_SESSION['usucpf']."', ptrdataalteracao = now() WHERE ptrid = " . $_REQUEST["excluir_plano_trabalho"];
		$db->executar($sql);
		
		$xml  = "";
		$xml .= "<ptrstatus>I</ptrstatus>";
		$xml .= "<usucpfalteracao>".$_SESSION['usucpf']."</usucpfalteracao>";
		$xml .= "<ptrdataalteracao>now()</ptrdataalteracao>";
	
		$sql = "INSERT INTO 
					emenda.historicogeral
					(
						tabid,
						histidtabela,
						histtxtalteracao,
						histdatainclusao,
						usucpfinclusao,
						ptrid
					)
				VALUES
					(
						'1',
						'{$_REQUEST[excluir_plano_trabalho]}',
						'{$xml}',
						NOW(),
						'{$_SESSION[usucpf]}',
						'{$_REQUEST[excluir_plano_trabalho]}'
					)";
	
		$db->executar($sql);	
		
		
		$db->commit();
		
		echo "<script type=\"text/javascript\">
				alert(\"Plano de Trabalho excluído com sucesso.\");
				window.location.href = window.location;
			  </script>";
		exit;
	} else {
		echo "<script type=\"text/javascript\">
				alert(\"Seu perfil não pode excluir este plano de trabalho!.\");
				window.location.href = window.location;
			  </script>";
		exit;
	}
}
if( !$db->testa_superuser() && empty($filtro) ) {
	$sql = "select count(a.arqid) as c
		 	from emenda.anexo de
			inner join emenda.planotrabalho pt on pt.ptrid = de.ptrid
			inner join emenda.entidadebeneficiada eb on eb.enbid = pt.enbid
			inner join public.arquivo a on a.arqid=de.arqid 
			-- inner join emenda.emendadetalheentidade ee on ee.enbid = eb.enbid
			inner join emenda.usuarioresponsabilidade ur on ur.enbid = eb.enbid
			inner join seguranca.usuario u ON u.usucpf = ur.usucpf 
			where a.arqid/1000 between 647 and 725 and 
			a.arqid not in(select arqid from public.arquivo_recuperado) and a.sisid=57 and a.usucpf='".$_SESSION['usucpf']."'";
	
	$carq = $db->pegaUm($sql);
}
if($carq > 0 ) {
	$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>Alguns arquivos anexados por você no SIMEC foram corrompidos,<br />
           	     para corrigir o problema você poderá enviá-los novamente.<br />
           	     Clique abaixo para ver a relação e proceder a correção.</p>
           	     <input type=\"button\" value=\"Ver a relação de arquivos\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"window.location='emenda.php?modulo=principal/emenda_arquivo&acao=A'\" />
          	  </center>";
	popupAlertaGeral($texto,"580px","260px");
}
$db->cria_aba( $abacod_tela, $url, '' );
$titulo = "Nesta seção você pode visualizar e editar todos os planos de trabalho cadastrados para sua(s) entidade(s). Para editar um plano de trabalho já cadastrado, clique em \"ações\", ao lado do número do PTA. Utilize os filtros de pesquisa para facilitar a busca";
monta_titulo( 'Listar Planos de Trabalho', $titulo);

?>
<html>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript">
form		=	$("formListaPlanoTrabalho");
btPesquisar	=	$("btPesquisar");
btLimpar	=	$("btLimpar");
btnExportar	=	$("btnExportar");

verificaResponsavel();

function verificaResponsavel(){
	campo_select = $('resid');
	if ( !campo_select ){
		return;
	}
	var j = campo_select.options.length;

	if(j == 1){
		for ( var i = 0; i < j; i++ ){
			campo_select.options[i].selected = true;
		}
		form.action = "";
		form.target = '';
		form.submit();
	}
}

function submetePesquisa() {
	btPesquisar.disabled 	= true;
	btLimpar.disabled 		= true;
	$('btnExportar').disabled 	= true;

	enbnome	=	document.getElementsByName("enbnome")[0];
	
	if(enbnome.value.length > 0 && enbnome.value.length < 3) {
		alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
		enbnome.focus();
		btPesquisar.disabled 	= false;
		btLimpar.disabled 		= false;
		$('btnExportar').disabled 		= false;
		return;
	}

	if($('espid_campo_flag').value == "1"){
		selectAllOptions( form.espid );
	}
	form.action = "";
	form.target = '';
	document.getElementById('requisicao').value = '';
	document.getElementById('exportar').value = 'false';
	form.submit();
}

function exportarPTAExcel(){
	document.getElementById('requisicao').value = 'exportar';
	document.getElementById('exportar').value = 'true';
	form.action = "";
	form.target = '';
	form.submit();
}

function alterarPlanoTrabalho(ptrid,emeid,solicitarReformulacao) {
	if (solicitarReformulacao == 1) 
		window.location = "emenda.php?modulo=principal/solicitarReformulacaoConfirmacao&acao=A&ptrid=" + ptrid + "&emeid=" + emeid + "";
	else
		window.location = "emenda.php?modulo=principal/alteraDefinirRecursoPTA&acao=A&ptrid=" + ptrid + "&emeid=" + emeid + "";
}

function excluirPlanoTrabalho(ptrid) {
	if(confirm("Deseja realmente excluir este Plano de Trabalho ?")) {

		excluirPlanoTrabalho = $("excluir_plano_trabalho");
		
		excluirPlanoTrabalho.value = ptrid;
		form.action = "";
		form.target = '';
		form.submit();
	}
}

function limparPesquisa() {
	btPesquisar.disabled 	= true;
	btLimpar.disabled 		= true;
	$('btnExportar').disabled = true;
	
	var resid			=	document.getElementsByName("resid")[0];
	var ptrid			=	document.getElementsByName("ptrid")[0];
	var enbcnpj			=	document.getElementsByName("enbcnpj")[0];
	var enbnome			=	document.getElementsByName("enbnome")[0];
	var estuf			=	document.getElementsByName("estuf")[0];
	var mundescricao	=	document.getElementsByName("mundescricao")[0];
	var emecod			=	document.getElementsByName("emecod")[0];
	var autid			=	document.getElementsByName("autid")[0];
	var esdid			=	document.getElementsByName("esdid")[0];
	
	resid.value			=	"";
	ptrid.value			=	"";
	emecod.value		=	"";
	autid.value			=	"";
	enbcnpj.value	=	"";
	enbnome.value		=	"";
	estuf.value			=	"";
	mundescricao.value	=	"";
	esdid.value			=	"";
	
	btPesquisar.disabled 	= false;
	btLimpar.disabled 		= false;
	$('btnExportar').disabled = false;
	
	document.getElementById('requisicao').value = 'limpar';
	document.getElementById('exportar').value = 'false';
	form.action = "";
	form.target = '';
	form.submit();
}

function visualizarRelatorio(ptrid){
	window.open('emenda.php?modulo=principal/relatorio_planoTrabalho&acao=A&ptrid='+ptrid,'page','toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no,fullscreen=yes');
}

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function carregaEmendaDetalhePTA(idImg, ptrid, emecod, autid){
	var img 	 = $( idImg );
	var tr_nome = 'listaEmendaDetalhePTA_'+ ptrid;
	var td_nome  = 'trV_'+ ptrid;
	
	if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == ""){
		$(td_nome).innerHTML = 'Carregando...';
		img.src = '../imagens/menos.gif';
		carregaEmendaDetalhe(ptrid, td_nome, emecod, autid);
	}if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
		$(tr_nome).style.display = '';
		img.src = '../imagens/menos.gif';
	} else {
		$(tr_nome).style.display = 'none';
		img.src = '/imagens/mais.gif';
	}
}
function carregaEmendaDetalhe(ptrid, td_nome, emecod, autid){
	var myajax = new Ajax.Request('emenda.php?modulo=principal/listaPlanoTrabalho&acao=A', {
		        method:     'post',
		        parameters: '&carregaEmendaDetalhePTA=true&ptrid='+ptrid+'&emecod='+emecod+'&autid='+autid,
		        asynchronous: false,
		        onComplete: function (res){
					$(td_nome).innerHTML = res.responseText;
		        }
		  });
}

function enviarEmailAnaliseEntidade(){
	enbnome	=	document.getElementsByName("enbnome")[0];
	
	if(enbnome.value.length > 0 && enbnome.value.length < 3) {
		alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
		enbnome.focus();
		btnPesquisar.disabled 	= false;
		return;
	}

	
	if(document.getElementById('espid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById('espid') );
	}
	var largura = 673;
	var altura = 500;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
	
	document.getElementById('requisicao').value = 'filtraemail';
	
	form.action = "emenda.php?modulo=principal/enviarEmailEmenda&acao=A";
	form.target = 'page';
	var janela = window.open('emenda.php?modulo=principal/enviarEmailEmenda&acao=A&tipoEmail=analisePTA','page','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
	janela.focus();
	form.submit();
}

</script>
<body>
<form action="" method="POST" id="formListaPlanoTrabalho" name="formListaPlanoTrabalho">
<?

if( $perfil != INSTITUICAO_BENEFICIADA) {
?>

	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tbody>
		<tr>
			<td width="15%" class="subtitulodireita"><b>Emenda N°:</b></td>
			<td colspan="3" width="35%">
				<?
					$emecod = $_REQUEST['emecod'];
					echo campo_texto('emecod', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '','', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Número do PTA:</b></td>
			<td colspan="3">
				<?
					$ptrid = $_REQUEST['ptrid']; 
					echo campo_texto( 'ptrid', 'N', 'S', '', 27, 27, '[#]', '', '', '', 0, '', '', '', "this.value=mascaraglobal('[#]',this.value);" );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Autor:</b></td>
			<td colspan="3">
				<?
					$autid = $_REQUEST['autid'];
					
					$sql ="SELECT 
							  	autid as codigo,
							  	autnome as descricao
							FROM 
							  	emenda.autor
							WHERE 
							  	autstatus = 'A'
							ORDER BY
								autnome";
					
					$db->monta_combo( "autid", $sql, 'S', 'Selecione...', '', '', '', '265','','autid', '', '', '', 'class="sel_chosen"');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>Responsável:</b></td>
			<td colspan="3">
				<?
					$resid = $_REQUEST['resid'];
					echo comboResponsavel($_SESSION['usucpf'], $resid, 'verificaResponsavel(this)');
				?>
			</td>
		</tr>
		<tr>
			<td width="190px" class="subtitulodireita">Situação:</td>
			<td>
				<?php
					$esdid = $_REQUEST['esdid'];
					
					$sql = "(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 8 
							      AND esdstatus = 'A'
							      AND esdid in (52,53,54,55,56,57,58,60,67,68,69,119,70,120,167,196,197,199)
							  ORDER BY 
							      esdordem) 
							UNION ALL
							  SELECT  0 as codigo, 
							      '------------ Fluxo de Emendas Impositivo ------------' as descricao
							UNION ALL
							(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 163 
							      AND esdstatus = 'A'
							  ORDER BY 
							      esdordem)       
							UNION ALL
							  SELECT  0 as codigo, 
							      '------------ SITUAÇÕES DA REFORMULAÇÃO ------------' as descricao
							UNION ALL
							(SELECT 
							      esdid as codigo, 
							      esddsc as descricao
							  FROM 
							      workflow.estadodocumento
							  WHERE 
							      tpdid = 8 
							      AND esdstatus = 'A'
							      AND esdid NOT in (52,53,54,55,56,57,58,60,67,68,69,119,70,120,167,196,197,199)
							  ORDER BY 
							      esdordem)";
					$db->monta_combo("esdid", $sql, 'S', 'Selecione...', "", '', '', '300','','esdid' );
					
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita"><b>Iniciativa:</b></td>
			<td colspan="3">
				<?php 
					$iniid = $_REQUEST['iniid'];
					
					$sql = "SELECT DISTINCT
							    ini.iniid as codigo,
							    ini.ininome as descricao
							FROM 
							    emenda.ptiniciativa pti
							    inner join emenda.iniciativa ini
							        on ini.iniid = pti.iniid
							        --and ini.inistatus = 'A'
							    inner join emenda.planotrabalho ptr
							    	on ptr.ptrid = pti.ptrid
							    	and ptr.ptrstatus = 'A'
							    	 and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
							ORDER BY
							    descricao";
					
					$arIniciativa = $db->carregar( $sql );
					$arIniciativa = ($arIniciativa ? $arIniciativa : array() );
					$arDados = array();
					foreach ($arIniciativa as $chave => $v) {
						$sql = "SELECT								
									res.resdsc
								FROM 
								  emenda.responsavel res
								  inner join emenda.iniciativaresponsavel inr
								  	on res.resid = inr.resid
								    and inr.irestatus = 'A'
								WHERE
									inr.iniid = ".$v['codigo']."
								ORDER BY
									res.resdsc";
						
						$resdsc = $db->carregarColuna( $sql );
						
						array_push($arDados, array("codigo" => $v['codigo'],
												 "descricao" => $v['codigo'] .' - '. $v['descricao']."(".implode( ', ', $resdsc ).")"
											));
					}
					
					$db->monta_combo( "iniid", $arDados, 'S', 'Selecione...', '', '', '', '450', '', '', '', '', 'Lista de iniciativa(s) vinculada(s) ao plano de trabalho' );
					//$arIniciativa = $db->carregar($sql);
				?>
			</td>
		<tr>
		<tr>
			<td class="subtitulodireita"><b>CNPJ:</b></td>
			<td colspan="3">
				<?
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>
			<td class="subtitulodireita" width="15%"><b>Nome do Órgão ou Entidade:</b></td>
			<td width="35%">
				<?
					$enbnome = $_REQUEST['enbnome']; 
					echo campo_texto( 'enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita"><b>UF:</b></td>
			<td colspan="3">
				<?
					$estuf = $_REQUEST['estuf'];
					$sql = "SELECT
								estuf AS codigo,
								estdescricao AS descricao
							FROM
								territorios.estado
							order by descricao";
					$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', '', '', '', '150' );
				?>
			</td>
			<td class="subtitulodireita"><b>Município:</b></td>
			<td>
				<?
					$mundescricao = $_REQUEST['mundescricao']; 
					echo campo_texto( 'mundescricao', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		</tr>
		<?php 
				$sql = "SELECT DISTINCT
						    e.espid as codigo,
						    e.espid|| ' - ' ||e.espnome as descricao
						FROM
						    emenda.especificacao e
						    inner join emenda.iniciativaespecificacao ie on ie.espid = e.espid
						    inner join emenda.ptiniciativaespecificacao ptie on ptie.iceid = ie.iceid
						    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid
						    inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid
						    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
						WHERE 
						    --e.espstatus = 'A' and
						    ptie.ptestatus <> 'P'
						    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
						ORDER BY
							e.espid";

				if ( !empty($_REQUEST['espid'][0]) && is_array($_REQUEST['espid']) ){
					$sqlCarregados = "SELECT DISTINCT
										    e.espid as codigo,
										    e.espid|| ' - ' ||e.espnome as descricao
										FROM
										    emenda.especificacao e
										    inner join emenda.iniciativaespecificacao ie on ie.espid = e.espid
										    inner join emenda.ptiniciativaespecificacao ptie on ptie.iceid = ie.iceid
										    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid
										    inner join emenda.planotrabalho ptr on ptr.ptrid = pti.ptrid
										    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
										WHERE 
										    --e.espstatus = 'A' and
										    ptie.ptestatus <> 'P'
										    and e.espid IN (" . ( implode("," , $_REQUEST['espid']) ) . ")
										    and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
										ORDER BY
											e.espid";
					$espid = $db->carregar( $sqlCarregados );
				}
				mostrarComboPopup('<b>Especificações</b>', 'espid',  $sql, $stSqlCarregados, 'Selecione a(s) Especificações(s)' );
		?>
		<tr bgcolor="#D0D0D0">
			<td></td>
			<td colspan="5">					
				<input type="button" id="btPesquisar" value="Pesquisar" onclick="submetePesquisa();" />
				<input type="button" id="btLimpar" value="Limpar Campos" onclick="limparPesquisa();" />
				<input type="button" value="Exportar para Excel" id="btnExportar" onclick="exportarPTAExcel();">
				<input type="button" value="Enviar E-mail" name="btnEnviarEmail" id="btnEnviarEmail" onclick="enviarEmailAnaliseEntidade();" style="cursor: pointer;">
			</td>
		</tr>
	</tbody>
</table>
<? }

if($_POST['check_expandir']){
	$checked = "checked=\"checked\"";
} else {
	$checked = "";
}

?>
<input type="hidden" id="excluir_plano_trabalho" name="excluir_plano_trabalho" value="" />
<input type="hidden" id="expandir" name="expandir" value="" />
<input type="hidden" id="exportar" name="exportar" value="" />
<input type="hidden" name="requisicao" id="requisicao" value="<?=$exporta; ?>">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td style="width: 100px;"><label for="check_expandir"><input type="checkbox" <?=$checked; echo ($arDados ? "" : "disabled='disabled'" ); ?> name="check_expandir" id="check_expandir" onclick="expandirArvore();">Expandir Todos</label></td>
		<td align="center"><b>Lista de Planos de Trabalho Cadastrados</b></td>
	</tr>
</table>
</form>
<div id="lista"></div>
<?

echo $obPTA->listaPlanoTrabalho();
/*$cabecalho = array("&nbsp;Ações&nbsp;&nbsp;&nbsp;&nbsp;", "Número do PTA", "CNPJ", "Órgão ou Entidade", "UF", "Município", "Nível de Ensino", "Valor Total", "Situação", "Imprimir PTA");
if ($arDados) $db->monta_lista_array($arDados, $cabecalho, 5000, 20, '', 'center', '');*/

?>
</body>
<script type="text/javascript">
form		=	$("formListaPlanoTrabalho");
btPesquisar	=	$("btPesquisar");
btLimpar	=	$("btLimpar");

function expandirArvore(){
	$('expandir').value = "S";
	form.action = "";
	form.target = '';
	form.submit();
}

/* Função para substituir todos */
function replaceAll5(str, de, para){
    var pos5 = str.indexOf(de);
    while (pos5 > -1){
		str = str.replace(de, para);
		pos5 = str.indexOf(de);
	}
    return (str);
}
function replaceAll7(str, de, para){
    var pos7 = str.indexOf(de);
    while (pos7 > -1){
		str = str.replace(de, para);
		pos7 = str.indexOf(de);
	}
    return (str);
}
/* Função para adicionar linha nas tabelas */

	
	jq('.sel_chosen').chosen({allow_single_deselect:true});
</script>
</html>