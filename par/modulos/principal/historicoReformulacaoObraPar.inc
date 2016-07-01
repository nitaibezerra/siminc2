<?php

//ver($_REQUEST,d);
if( $_REQUEST['requisicao'] == 'carregaDadosObraReformulada' ){
	carregaDadosObraReformulada();
	exit();
}

function carregaDadosObraReformulada(){
	global $db;
	
	$sql = "SELECT DISTINCT
				pre.predescricao,
				pre.preano,
				to_char(pre.predatareformulacao, 'DD/MM/YYYY HH24:MI:SS') as data
			FROM 
				obras.preobra pre 
			WHERE
				pre.preidpai = ".$_POST['preid'];
	
	$cabecalho = array("Descrição da Obra", "Ano da Obra", "Data da Reformulação" );
	$db->monta_lista($sql,$cabecalho,50000,5,'N','90%','N');
	
}

if( !$_REQUEST['proid'] ){
	echo '<script>alert("Escolha um Processo para reformular!");
			history.back(-1);
	</script>';
	die();
}

include_once APPRAIZ.'par/classes/Projeto.class.inc';
$obProjeto = new Projeto( $_REQUEST );
//include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

echo montarAbasArray( $obProjeto->criaAbaPar(5), "par.php?modulo=principal/projetos&acao=A&abas=historicoReformulacaoObraPar" );
//$db->cria_aba( $abacod_tela, $url, $parametros, $mnuid );
$entidadePar = ($_SESSION['par']['itrid'] == 1) ? $_SESSION['par']['estuf'] : $_SESSION['par']['muncod'];
$nmEntidadePar = EntidadeParControle::recuperaDescricaoEntidadePar($entidadePar,$_SESSION['par']['itrid']);
monta_titulo( $nmEntidadePar, 'Histórico de Reformulações' );

?>
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
		<tr id="tr_div">
			<td> 
			<?
			
			$sql = "SELECT DISTINCT 
							'<center><img style=\"cursor:pointer\" id=\"img_subacao_' || poc.preid || '\" src=\"/imagens/mais.gif\" onclick=\"carregarObras(this.id,\'' || poc.preid || '\');\" border=\"0\" />' as acao,
							pre.predescricao,
							pre.preano,
							TO_CHAR( pre.prevalorobra, '999G999G999D99') || '</td></tr>
											            	<tr style=\"display:none\" id=\"listaDocumentos_' || poc.preid || '\" >
											            		<td id=\"tr_' || poc.preid || '\" colspan=8 ></td>
											            </tr>' as valor
						FROM 
							par.processoobrasparcomposicao poc
                            inner join obras.preobra pre on pre.preidpai = poc.preid
                            left join par.documentopar dop ON dop.proid = poc.proid and dop.dopreformulacao = true
                            --inner join par.empenhoobra emo ON emo.preid = pre.preid and eobstatus = 'A'
						WHERE
							poc.proid = ".$_REQUEST['proid']."
						ORDER BY
							pre.predescricao";
				//ver(simec_htmlentities($sql),d);
				$cabecalho = array("&nbsp;Ações&nbsp;", "Descrição da Subação", "Ano da Subação", "Valor da Subação" );
				$db->monta_lista($sql,$cabecalho,50000,5,'N','95%','S','formulariomontalista');
			?></td>
		</tr>
	</table>
<script type="text/javascript">

function carregarObras(idImg, preid){
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos_'+ preid;
	var td_nome  = 'tr_'+ preid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == ""){
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaObra(preid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else {
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaObra(preid, td_nome){
	divCarregando();
	$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/historicoReformulacaoObraPar&acao=A",
	   		data: "requisicao=carregaDadosObraReformulada&preid="+preid,
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

</script>
