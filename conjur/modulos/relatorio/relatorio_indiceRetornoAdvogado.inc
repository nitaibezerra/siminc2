<?php

include APPRAIZ . 'includes/workflow.php';

function listarHistorico( $request ){
	
	global $db;
					
	$docid = (integer) $request['docid'];
	$documento = wf_pegarDocumento( $docid );
	$atual = wf_pegarEstadoAtual( $docid );
	$historico = wf_pegarHistorico( $docid );
	
	?>
	<br>
		<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: 98%;">
		<thead>
			<tr>
				<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
					<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
					<div><?php echo $documento['docdsc']; ?></div>
				</td>
			</tr>
			<?php if ( count( $historico ) ) : ?>
				<tr>
					<td style="width: 20px;"><b>Seq.</b></td>
					<td style="width: 200px;"><b>Onde Estava</b></td>
					<td style="width: 200px;"><b>O que aconteceu</b></td>
					<td style="width: 90px;"><b>Quem fez</b></td>
					<td style="width: 120px;"><b>Quando fez</b></td>
					<td style="width: 17px;">&nbsp;</td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php $i = 1; ?>
		<?php foreach ( $historico as $item ) : ?>
			<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
			<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td align="right"><?=$i?>.</td>
				<td style="color:#008000;">
					<?php echo $item['esddsc']; ?>
				</td>
				<td valign="middle" style="color:#133368">
					<?php echo $item['aeddscrealizada']; ?>
				</td>
				<td style="font-size: 6pt;">
					<?php echo $item['usunome']; ?>
				</td>
				<td style="color:#133368">
					<?php echo $item['htddata']; ?>
				</td>
				<td style="color:#133368; text-align: center;">
					<?php if( $item['cmddsc'] ) : ?>
						<img
							align="middle"
							style="cursor: pointer;"
							src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
							onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
						/>
					<?php endif; ?>
				</td>
			</tr>
			<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td colspan="6">
					<div >
						<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
					</div>
				</td>
			</tr>
			<?php $i++; ?>
		<?php endforeach; ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="6">
				Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
			</td>
		</tr>
	</table>
<?php 
}


function listaProcessos( $request ){
	
	global $db;
	
	$where = Array();
	//Primeira Ação
	if( $request['aedid1'] ){
		$joinAcao = "INNER JOIN ( SELECT
									h1.hstid,
									min(h2.hstid) as hstid2,
									h1.docid
							    FROM
									workflow.historicodocumento h1
							    LEFT JOIN workflow.historicodocumento h2 ON h2.hstid > h1.hstid AND h1.docid = h2.docid
							    WHERE
									h1.aedid = ".$request['aedid1']."
							    GROUP BY
									h1.hstid, h1.docid
				   				) hst_acao1 ON hst_acao1.docid = doc.docid";
	}
	if( $request['aedid2'][0] && $request['aedid2_campo_flag'] ){
		
		$joinAcao .= " INNER JOIN workflow.historicodocumento hst_acao2 ON hst_acao2.docid = doc.docid 
		  																  AND hst_acao2.hstid = hst_acao1.hstid2 
		  																  AND hst_acao2.aedid " . (!$request['aedid2_campo_excludente'] ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $request['aedid2'] ) . "')";
	}
	// período
	if( $request['htddata_inicio'] && $request['htddata_fim'] ){
		$htddata_inicio = formata_data_sql($request['htddata_inicio']);
		$htddata_fim = formata_data_sql($request['htddata_fim']);
		array_push($where, " his.htddata >= TIMESTAMP '{$htddata_inicio}' AND his.htddata <= TIMESTAMP '{$htddata_fim} 23:59:59' ");
		array_push($where, " had.dtatribuicao >= TIMESTAMP '{$htddata_inicio}' AND his.htddata <= TIMESTAMP '{$htddata_fim} 23:59:59' ");
	}
	
	$sql = "SELECT DISTINCT
				prc.prcnumsidoc as processo,
				prc.prcid as processo_id,
				doc.docid
			FROM
			       workflow.historicodocumento his
			LEFT  JOIN workflow.comentariodocumento com ON com.hstid = his.hstid
			INNER JOIN workflow.documento 		doc ON doc.docid = his.docid
			INNER JOIN conjur.estruturaprocesso 	est ON est.docid = his.docid
			INNER JOIN conjur.processoconjur 	prc ON prc.prcid = est.prcid
			INNER JOIN (SELECT max(hstid) as hstid, docid 
					FROM workflow.historicodocumento 
					WHERE aedid = 1072
					GROUP BY docid) 	htsadv ON htsadv.docid = doc.docid 
			INNER JOIN conjur.historicoadvogados 	had ON had.hstid = htsadv.hstid
			INNER JOIN conjur.advogados 		adv ON adv.advid = had.advid
			$joinAcao
			WHERE
				doc.tpdid = 49 
				AND adv.advid = ".$request['advogado_id']."
				".($where[0] ? ' AND' . implode(' AND ', $where) : '' )."
			ORDER BY
				1";
//			ver($sql,d);
	$processos = $db->carregar($sql);
	?>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" whidth="98%">
	<?php 
	if( is_array($processos) ){
	?>
		<tr>
			<td bgcolor="#CCCCCC"><b>Total de Processos: <?=count($processos) ?></b></td>
		</tr>
	<?php 
		foreach( $processos as $processo){ ?>
		<tr>
			<td>
				<b>Processo:</b> <?=$processo['processo'] ?> - <?=$processo['coordenacao_processo'] ?>
				<a class="exibirH" id="<?=$request['advogado_id'] ?>_<?=$processo['processo_id']?>" style="cursor:pointer">Exibir histórico</a><br>
				<input type="hidden" id="docid_<?=$request['advogado_id'] ?>_<?=$processo['processo_id'] ?>" value="<?=$processo['docid'] ?>" />
				<div id="div_<?=$request['advogado_id'] ?>_<?=$processo['processo_id'] ?>" style="display:none;"></div>
			</td>
		</tr>
	<?php 
		}
	}else{	
	?>
		<tr>
			<td bgcolor="#CCCCCC" style="color:red"><b>Não possui processos nesse periodo.</b></td>
		</tr>
	<?php 
	}
	?>
	</table>
	<?php 
}

if( $_REQUEST['reqAjax'] ){
	$_REQUEST['reqAjax']($_REQUEST);
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Incidência de Tramitação.";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">


function exibeRelatorioProcesso(tipoXLS){
	document.getElementById('req').value = tipoXLS;
	var colunas  = document.getElementById( 'colunas' );

	if ( !colunas.options.length ){
		alert( 'Favor selecionar ao menos uma coluna!' );
		return false;
	}
	
	selectAllOptions( colunas );	
	
	//valida data
	if(formulario.htddata_inicio.value != '' && formulario.htddata_fim.value != ''){
		if(!validaData(formulario.htddata_inicio)){
			alert("Data Início Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		
		if(!validaData(formulario.htddata_fim)){
			alert("Data Fim Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		
	}
	
	if(document.getElementById('req').value != 'geraxls'){
		formulario.target = 'historicoProcessos';
		var janela = window.open( '?modulo=relatorio/relatorio_historicoProcessos_resultado&acao=A', 'historicoProcessos', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();
	}else{
		formulario.target = '';
		formulario.submit();
	}
}
	
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
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
//JS workflow histórico
IE = !!document.all;

function exebirOcultarComentario( docid )
{
	id = 'comentario' + docid;
	div = document.getElementById( id );
	if ( !div )
	{
		return;
	}
	var display = div.style.display != 'none' ? 'none' : 'table-row';
	if ( display == 'table-row' && IE == true )
	{
		display = 'block';
	}
	div.style.display = display;
}

</script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript">

$(document).ready(function()
{
	$('#loader-container').slideUp();
	$('.exibirP').live('click',function(){
		$('#loader-container').show();
		var advogado_id = $(this).attr('id');
		selectAllOptions( document.getElementById('advid') );
		selectAllOptions( document.getElementById('coonid') );
		selectAllOptions( document.getElementById('aedid2') );
		if( $('#div_'+advogado_id).html() == '' ){
			$.ajax({
		   		type: "POST",
		   		url: window.location,
		   		data: "reqAjax=listaProcessos&advogado_id="+advogado_id+"&"+$('#filtro').serialize(),
		   		async: false,
		   		success: function(msg){
					$('#div_'+advogado_id).html(msg);
		   		}
		 	});
		}
		if( $('#div_'+advogado_id).css('display') == 'none' ){
			$('#div_'+advogado_id).slideDown();
			$('#loader-container').slideUp();
		}else{
			$('#div_'+advogado_id).slideUp();
			$('#loader-container').slideUp();
		}
	});
	$('.exibirH').live('click',function(){
		$('#loader-container').show();
		var ids = $(this).attr('id');
		var docid = $('#docid_'+ids).val();
		if( $('#div_'+ids).html() == '' ){
			$.ajax({
		   		type: "POST",
		   		url: window.location,
		   		data: "reqAjax=listarHistorico&docid="+docid,
		   		async: false,
		   		success: function(msg){
					$('#div_'+ids).html(msg);
		   		}
		 	});
		}
		if( $('#div_'+ids).css('display') == 'none' ){
			$('#div_'+ids).slideDown();
			$('#loader-container').slideUp();
		}else{
			$('#div_'+ids).slideUp();
			$('#loader-container').slideUp();
		}
	});
	$('.filtrar').click(function(){
		$('#loader-container').show();
		selectAllOptions( document.getElementById('advid') );
		selectAllOptions( document.getElementById('coonid') );
		selectAllOptions( document.getElementById('aedid2') );
		$('#filtro').submit();
	});
});

</script>
<div id="loader-container" style="position: absolute; background-color: white; opacity: .6; width:100%; height:2000%" >
	<center>
		<div id="loader" style="margin-top:100px;">
			<img src="../imagens/wait.gif" border="0" align="middle">
			<span>Aguarde! Carregando Dados...</span>
		</div>
	</center>
</div>
<form action="" method="post" name="filtro" id="filtro">
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<?php
			$arrVisivel = array("descricao");
			$arrOrdem = array("descricao");
			
			$stSqlCarregados = '';
			if( $_REQUEST['advid'][0] != '' ){
				$stSqlCarregados = "SELECT
										a.advid AS codigo,
										e.entnome AS descricao
									FROM 
										conjur.advogados a
									INNER JOIN entidade.entidade e ON e.entid=a.entid
									WHERE
										entstatus='A' AND
										a.advid IN (".implode(',', $_REQUEST['advid']).")
									ORDER BY  
										2";
			}
			// Advogado
			$stSql = " SELECT
							a.advid AS codigo,
							e.entnome AS descricao
						FROM 
							conjur.advogados a
						INNER JOIN entidade.entidade e ON e.entid=a.entid
						WHERE
							entstatus='A'
						ORDER BY
							2";
			mostrarComboPopup( 'Advogado:', 'advid',  $stSql, $stSqlCarregados, 'Selecione o(s) Advogado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
			
			// Coordenação
			$stSql = " SELECT
							coonid AS codigo,
							coodsc AS descricao
						FROM 
							conjur.coordenacao
						ORDER BY
							2 ";
			$stSqlCarregados = '';
			if( $_REQUEST['coonid'][0] != '' ){
				$stSqlCarregados = " SELECT
										coonid AS codigo,
										coodsc AS descricao
									FROM 
										conjur.coordenacao
									WHERE
										coonid IN (".implode(',', $_REQUEST['coonid']).")
									ORDER BY
										2 ";
			}
			mostrarComboPopup( 'Coordenação (do Advogado):', 'coonid',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		?>
		<tr>
			<td class="SubTituloDireita">Primeira Ação:</td>
			<?
			$aedid1 = $_REQUEST['aedid1'];
			$sql = "SELECT
						aedid AS codigo,
						esddsc||' - '||aeddscrealizar AS descricao 
					FROM 
						workflow.estadodocumento doc
					INNER JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = doc.esdid
					WHERE 
						tpdid = " . NOVO_TIPODOC . " AND
						esdstatus = 'A' AND
						aeddscrealizar != ''
					ORDER BY
						2";
			?>
			<td><?=$db->monta_combo("aedid1", $sql, 'S','Selecione...','', '', '',244,'N','aedid1', '', '', 'Expandir'); ?></td>
		</tr>
		<?php 
			//Ações Seguintes
			$stSql = "SELECT
								aedid AS codigo,
								esddsc||' - '||aeddscrealizar AS descricao 
							FROM 
								workflow.estadodocumento doc
							INNER JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = doc.esdid
							WHERE 
								tpdid = " . NOVO_TIPODOC . " AND
								esdstatus = 'A' AND
								aeddscrealizar != ''
							ORDER BY
								2";
			
			$stSqlCarregados = '';
			if( $_REQUEST['aedid2'][0] != '' ){
				$stSqlCarregados = "SELECT
										aedid AS codigo,
										esddsc||' - '||aeddscrealizar AS descricao 
									FROM 
										workflow.estadodocumento doc
									INNER JOIN workflow.acaoestadodoc aed ON aed.esdidorigem = doc.esdid
									WHERE 
										tpdid = " . NOVO_TIPODOC . " AND
										esdstatus = 'A' AND
										aeddscrealizar != '' AND
										aedid IN (".implode(',', $_REQUEST['aedid2']).")
									ORDER BY
										2";
			}
			mostrarComboPopup( 'Ação(ôes) Seguinte(s):', 'aedid2',  $stSql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ,null,null,null,null,$arrVisivel,$arrOrdem);
			
		?>
		<tr>
			<td class="SubTituloDireita">Período de Tramitação:</td>
			<td>
				<?php 
				$_REQUEST['htddata_inicio'] = $_REQUEST['htddata_inicio'] ? $_REQUEST['htddata_inicio'] : '01/'.date('m').'/'.date('Y');
				$_REQUEST['htddata_fim']    = $_REQUEST['htddata_fim'] ? $_REQUEST['htddata_fim'] : '01/'.(date('m')+1).'/'.date('Y');
				$htddata_inicio = explode('/', $_REQUEST['htddata_inicio']);
				$htddata_inicio = $htddata_inicio[1].'/'.$htddata_inicio[0].'/'.$htddata_inicio[2];
				$htddata_fim = explode('/', $_REQUEST['htddata_fim']);
				$htddata_fim = $htddata_fim[1].'/'.$htddata_fim[0].'/'.$htddata_fim[2];
				?>
				<?= campo_data2( 'htddata_inicio', 'N', 'S', '', 'S' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data2( 'htddata_fim', 'N', 'S', '', 'S' ); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Filtrar" style="cursor: pointer;" class="filtrar"/>
			</td>
		</tr>
	</table>
</form>
<form action="" method="post" name="formulario">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
				<b style="font-size: 10pt;"><br/></b>
			</td>
		</tr>
		<?php 
		
		$where = Array();
		
		// advogado
		if( $_REQUEST['advid'][0] && $_REQUEST['advid_campo_flag'] ){
			array_push($where, " adv.advid " . (!$_REQUEST['advid_campo_excludente'] ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_REQUEST['advid'] ) . "') ");
		}
		// unidade
		if( $_REQUEST['coonid'][0] && $_REQUEST['coonid_campo_flag'] ){
			array_push($where, " adv.coonid " . (!$_REQUEST['coonid_campo_excludente'] ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_REQUEST['coonid'] ) . "') ");
		}
		
		$sql = "SELECT DISTINCT
					ent.entnome as advogado,
					adv.advid as advogado_id,
					co2.coodsc as coordenacao_advogado
				FROM
				       workflow.historicodocumento his
				LEFT  JOIN workflow.comentariodocumento com ON com.hstid = his.hstid
				INNER JOIN workflow.documento 		doc ON doc.docid = his.docid
				INNER JOIN (SELECT max(hstid) as hstid, docid 
						FROM workflow.historicodocumento 
						WHERE aedid = ".WF_AEDID_ENCAMINHAR_ADVIGADO."
						GROUP BY docid) 	htsadv ON htsadv.docid = doc.docid 
				INNER JOIN conjur.historicoadvogados 	had ON had.hstid = htsadv.hstid
				INNER JOIN conjur.advogados 		adv ON adv.advid = had.advid
				INNER JOIN conjur.coordenacao 		co2 ON co2.coonid = adv.coonid
				INNER JOIN entidade.entidade 		ent ON ent.entid = adv.entid
				WHERE
					doc.tpdid = ".TPD_CONJUR_NOVO."
					".($where[0] ? ' AND' . implode(' AND ', $where) : '' )."
				ORDER BY
					1";
		$advogados = $db->carregar($sql);
		
		foreach( $advogados as $advogado){ 
		?>
		<tr bgcolor="white">
			<td>
				<?=$advogado['advogado'] ?> / <?=$advogado['coordenacao_advogado'] ?> - 
				<a class="exibirP" id="<?=$advogado['advogado_id']?>" style="cursor:pointer">Exibir processos</a>
			</td>
		</tr>
		<tr>
			<td>
				<div id="div_<?=$advogado['advogado_id'] ?>" style="display:none"></div>
			</td>
		</tr>
		<?php 
		}
		?>
	</table>
</form>