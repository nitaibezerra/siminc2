<?php


if( $_REQUEST['requisicao'] == 'carregaDadosItens' ){
	carregaDadoItens();
	exit();
}

if( $_REQUEST['requisicao'] == 'carregaDadosSubacao' ){
	carregaDadoSubacao();
	exit();
}

function carregaDadoSubacao(){
	global $db;
	
	$sql = "SELECT DISTINCT
				'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaItens(this.id,\'' || sd.sbdid || '\');\" border=\"0\" />' as acao,
				s.sbadsc,
				sd.sbdano,
				s.sbadatareformulacao || '</td></tr>
								            	<tr style=\"display:none\" id=\"listaDocumentos2_' || s.sbaid || '\" >
								            		<td id=\"trI_' || s.sbaid || '\" colspan=8 ></td>
								            </tr>' as data
			FROM 
				par.subacao s 
			INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
			WHERE
				s.sbaidpai = ".$_POST['sbaid'];
	
	$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Data da Reformulação" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
	
}

function carregaDadoItens(){
	global $db;
	
	$cronograma = $db->pegaUm( "SELECT sbacronograma FROM par.subacao WHERE sbaid = ".$_POST['sbaid'] );
	
	if( $cronograma == 1 ){ // GLOBAL
	
	$sql = "SELECT 
				sic.icodescricao,
				sic.icoquantidadetecnico as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR( ( sic.icoquantidadetecnico * sic.icovalor), '999G999G999D99') as valortotal
			FROM
				par.subacaoitenscomposicao sic
			WHERE
				sic.icovalidatecnico = 'S' AND
				sic.sbaid = ".$_POST['sbaid'];

	} else { // ESCOLA
		
	$sql = "SELECT 
				sic.icodescricao,
				SUM( sse.seiqtdtecnico ) as quantidade,
				TO_CHAR( sic.icovalor, '999G999G999D99'),
				TO_CHAR(SUM( sse.seiqtdtecnico * sic.icovalor ), '999G999G999D99') as valortotal
			FROM
				par.subacaoitenscomposicao sic
			INNER JOIN par.subescolas_subitenscomposicao  sse ON sse.icoid = sic.icoid
			WHERE
				sic.icovalidatecnico = 'S' AND
				sic.sbaid = ".$_POST['sbaid']."
			GROUP BY
				sic.icodescricao, sic.icovalor, sic.icoquantidadetecnico";

	}
	
	$cabecalho = array("Descrição do Item", "Quantidade", "Valor", "Valor Total" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');

}

if( !$_REQUEST['prpid'] ){
	echo '<script>alert("Escolha um Processo para reformular!");
			history.back(-1);
	</script>';
	die();
}

include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );
//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
//$parametros = array( 1 => '&prpid='.$_REQUEST['prpid'] );

/*if( $_SESSION['baselogin'] == 'simec_desenvolvimento' ){
	$mnuid = array();
} else {
	$mnuid = array(  1 => 11863 ); // Esconde a Aba "Montar Projeto"
}*/
echo montarAbasArray( $obProjeto->criaAbaPar(3), "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacao" );
//$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Histórico de Reformulações' );

$sql = "select 
			substring(p.prpnumeroprocesso, 12, 4) as ano, 
		    p.prpnumeroprocesso as processo, 
		    case when p.sisid = 57 then 'Subações de Emendas no PAR'
		                        	else 'Subações Genérico do PAR' end as tipo
		from par.processopar p where prpid = {$_REQUEST['prpid']}";
$arProcesso = $db->pegaLinha($sql);
?>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <?=cabecalhoProcesso($arProcesso); ?>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr id="tr_div">
			<td> 
			<?
			
			$sql = "SELECT DISTINCT
							'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || sd.sbaid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarListaSubacao(this.id,\'' || sd.sbaid || '\');\" border=\"0\" />' as acao,
							s.sbadsc,
							sd.sbdano,
							TO_CHAR( ems.eobvalorempenho, '999G999G999D99') || '</td></tr>
											            	<tr style=\"display:none\" id=\"listaDocumentos_' || sd.sbaid || '\" >
											            		<td id=\"tr_' || sd.sbaid || '\" colspan=8 ></td>
											            </tr>' as valor
						FROM 
							par.processoparcomposicao ppc
						INNER JOIN par.subacaodetalhe sd ON sd.sbdid = ppc.sbdid
						INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
						LEFT JOIN par.documentopar dop ON dop.prpid = ppc.prpid and dop.dopreformulacao = true
						INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano and eobstatus = 'A'
						WHERE
							ppc.ppcstatus = 'A' and
							ppc.prpid = ".$_REQUEST['prpid']."
						ORDER BY
							s.sbadsc";

				$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S','formulariomontalista');
			?></td>
		</tr>
	</table>
<script type="text/javascript">

function carregarListaSubacao(idImg, sbaid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos_'+ sbaid;
	var td_nome  = 'tr_'+ sbaid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacao(sbaid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaSubacao(sbaid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/historicoReformulacao&acao=A",
	   		data: "requisicao=carregaDadosSubacao&sbaid="+sbaid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregarListaItens(idImg, sbdid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos2_'+ sbdid;
	var td_nome  = 'trI_'+ sbdid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaItens(sbdid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/administrarProjeto&acao=A",
	   		data: "requisicao=carregaDadosItens&sbdid="+sbdid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

</script>
