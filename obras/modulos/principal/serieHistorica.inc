<?php
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo_modulo = "Série Histórica";
monta_titulo( $titulo_modulo, '' );

if($_POST['pesquisarSerieHistorica']){
	extract($_POST);
}

?>
<form name="formulario" id="form_seh" method="post" action="">
	<input type="hidden" name="requisicao" value="pesquisarSerieHistorica"/>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding=3 align="center">
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
			<td>
				<?php 
					$tobaid = $_POST['tobaid'];
					$sql = "SELECT 
								tobaid as codigo, 
								tobadesc as descricao 
							FROM 
								obras.tipoobra 
							ORDER BY 
								tobadesc";
				
					$db->monta_combo( "tobaid", $sql, "S", "Todos", "", "", "", "", "N", "tobaid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
			<td>
				<?php 
					$stoid = $_POST['stoid'];
					$sql = "SELECT 
								stoid as codigo, 
								stodesc as descricao 
							FROM 
								obras.situacaoobra 
							WHERE
								stostatus = 'A'
							ORDER BY 
								stodesc";
				
					$db->monta_combo( "stoid", $sql, "S", "Todas", "", "", "", "", "N", "stoid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
			<td>
				<?php 
					$cloid = $_POST['cloid'];
					$sql = "SELECT 
								cloid as codigo,
								clodsc as descricao
							FROM 
							  	obras.classificacaoobra
							ORDER BY
								clodsc";
				
					$db->monta_combo( "cloid", $sql, "S", "Todas", "", "", "", "", "N", "cloid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
			<td>
				<?php 
						$tpoid = $_POST['tpoid'];
						$sql = "SELECT 
									tpoid AS codigo, 
									tpodsc AS descricao
  								FROM 
  									obras.tipologiaobra
								WHERE
  									tpostatus = 'A'
								ORDER BY
									tpodsc";
					
						$db->monta_combo( "tpoid", $sql, "S", "Todas", "", "", "", "", "N", "tpoid" );
						
					?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Programa:</td>
			<td>
				<?php 
					$prfid = $_POST['prfid'];
					$sql = "SELECT 
								prfid as codigo,
								prfdesc as descricao
						  	FROM 
						  		obras.programafonte
						  	WHERE
						  		orgid = {$_SESSION["obras"]["orgid"]}
						  	ORDER BY
						  		prfdesc";
				
					$db->monta_combo( "prfid", $sql, "S", "Todos", "", "", "", "", "N", "prfid" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Fonte:</td>
			<td>
				<?php 
						$tooid = $_POST['tooid'];
						$sql = "SELECT 
									tooid AS codigo, 
									toodescricao AS descricao
  								FROM 
  									obras.tipoorigemobra
  								WHERE
  									toostatus = 'A'	
								ORDER BY
  									toodescricao";
					
						$db->monta_combo( "tooid", $sql, "S", "Todos", "", "", "", "", "N", "tooid" );
						
					?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
			<td>
				<?php 
						$moeid = $_POST['moeid'];
						$sql = "SELECT 
									moeid AS codigo, 
									moedsc AS descricao
  								FROM 
  									obras.modalidadeensino
								WHERE
									moestatus = 'A'
								ORDER BY
									moedsc";
					
						$db->monta_combo( "moeid", $sql, "S", "Todos", "", "", "", "", "N", "moeid" );
						
					?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
			<td>
				<?php 
					$entidunidade= $_POST['entidunidade'];
					$sql = "SELECT 
								ee.entid as codigo, 
								upper(ee.entnome) as descricao 
							FROM
								entidade.entidade ee
							INNER JOIN 
								obras.obrainfraestrutura oi ON oi.entidunidade = ee.entid 
							WHERE
								orgid = {$_SESSION["obras"]["orgid"]} AND
								obsstatus = 'A'
							GROUP BY 
								ee.entnome, 
								ee.entid 
							ORDER BY 
								ee.entnome";
				
					$db->monta_combo( "entidunidade", $sql, "S", "Todos", "", "", "", "", "N", "entidunidade" );
					
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Nome da Obra / ID:</td>
			<td>
				<?php $obrtextobusca = $_POST['obrtextobusca']; print campo_texto( 'obrtextobusca', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">ID do Grupo:</td>
			<td>
				<?php $gpdid = $_POST['gpdid']; print campo_texto( 'gpdid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">UF:</td>
			<td>
				<?php 
					$estuf = $_POST['estuf'];
					$sql = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado
							ORDER BY
								estdescricao";
				
					$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );
					
				?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td style="width: 190px;"></td>
			<td>
				<input type="button" name="obrBtPesquisaInicio" value="Pesquisar" onclick="listaSerieHistoria();" style="cursor: pointer;"/>
				<input type="button" name="obrBtVerTudoInicio" value="Ver Todas" onclick="verTodas();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<?php
if($_POST['tobaid']){
	$arrWhere[] = "tob.tobaid = {$_POST['tobaid']}";
}
if($_POST['stoid']){
	$arrWhere[] = "oi.stoid = {$_POST['stoid']}";
}
if($_POST['tpoid']){
	$arrWhere[] = "oi.tpoid = {$_POST['tpoid']}";
}
if($_POST['prfid']){
	$arrWhere[] = "oi.prfid = {$_POST['prfid']}";
}
if($_POST['tooid']){
	$arrWhere[] = "oi.tooid = {$_POST['tooid']}";
}
if($_POST['moeid']){
	$arrWhere[] = "oi.moeid = {$_POST['moeid']}";
}
if($_POST['entid']){
	$arrWhere[] = "oi.entid = {$_POST['entidunidade']}";
}
if($_POST['obrtextobusca']){
	$arrWhere[] = "oi.obrid::text like('%{$_POST['obrtextobusca']}%') or removeacento(oi.obrdesc) ilike removeacento(('%{$_POST['obrtextobusca']}%'))";
}
if($_POST['estuf']){
	$arrWhere[] = "mun.estuf = '{$_POST['estuf']}'";
}
if($_POST['gpdid']){
	$arrWhere[] = "ig.gpdid::text like('%{$_POST['gpdid']}%')";
}

$sql = "SELECT DISTINCT
				 '<img src=\"../imagens/fluxodoc.gif\" style=\"cursor:pointer\" onclick=\"wf_exibirHistorico(' || seh.docid || ')\" />' as acao,
				 ig.gpdid,
				 oi.obrid,
				 oi.obrdesc,
				 COALESCE(tobadesc,'N/A') as tobadesc,
				 COALESCE(stodesc,'N/A') as stodesc,
				 COALESCE(clodsc,'N/A') as clodsc,
				 COALESCE(tpodsc,'N/A') as tpodsc,
				 COALESCE(prfdesc,'N/A') as prfdesc,
				 COALESCE(toodescricao,'N/A') as toodescricao,
				 COALESCE(moedsc,'N/A') as moedsc,
				 COALESCE(upper(ee.entnome),'N/A') as entnome,
				 mun.mundescricao || '/' || mun.estuf as mun
			FROM
				obras.itemgrupo ig
			INNER JOIN
				obras.repositorio ore ON ore.repid = ig.repid
			INNER JOIN
				obras.obrainfraestrutura oi ON oi.obrid = ore.obrid
			INNER JOIN
		        entidade.endereco ed ON ed.endid = oi.endid
		    INNER JOIN
		        territorios.municipio mun ON mun.muncod = ed.muncod
			INNER JOIN
				obras.movsupervisao seh ON seh.obrid = oi.obrid
			LEFT JOIN
				obras.tipoobra tob ON tob.tobaid = oi.tobraid
			LEFT JOIN
				obras.situacaoobra sto ON sto.stoid = oi.stoid
			left join
				obras.tipologiaobra tpo ON tpo.tpoid = oi.tpoid
			left join
				obras.programafonte prf ON prf.prfid = oi.prfid
			left join
				obras.tipoorigemobra too ON too.tooid = oi.tooid
			left join
				obras.modalidadeensino moe ON moe.moeid = oi.moeid
			left join
				obras.classificacaoobra clo ON clo.cloid = oi.cloid
			left join
				entidade.entidade ee ON ee.entid = oi.entidunidade
			WHERE
				repstatus = 'A'
			and
				obsstatus = 'A'
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");


$cabecalho = array("Ação", "Grupo","ID", "Nome da Obra", "Tipo de Obra" ,"Situação da Obra","Classificação da Obra","Tipologia da Obra","Programa","Fonte","Modalidade de Ensino","Unidade","Município/UF" );
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', '');
?>
<script>
function wf_exibirHistorico( docid )
{
	var url = 'http://simec-local/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}

function listaSerieHistoria()
{
	document.getElementById('form_seh').submit();
}

function verTodas()
{
	window.location.href = 'obras.php?modulo=principal/serieHistorica&acao=A';
}
</script>
