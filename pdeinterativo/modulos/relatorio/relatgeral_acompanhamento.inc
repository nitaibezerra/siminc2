<?php

	if($_REQUEST['esdid'])
		$where[] = "esd.esdid = '".$_REQUEST['esdid']."'";
	
	if($_REQUEST['estuf'])
		$where[] = "pde.estuf = '".$_REQUEST['estuf']."'";
	
	if($_REQUEST['muncod'])
		$where[] = "pde.muncod = '".$_REQUEST['muncod']."'";
	
	if($_REQUEST['pflcod'])
		$where[] = "pfl.pflcod = '".$_REQUEST['pflcod']."'";
	
	if($_REQUEST['usucpf'])
		$where[] = "usu.usucpf = '".$_REQUEST['usucpf']."'";
	
	$sql = "select DISTINCT ";
	
	if($_REQUEST['visualizar']=='html'){
		$sql .= "
			(CASE WHEN esd.esdid = ".WF_ESD_VALIDADO_MEC."
				THEN '<span style=\"white-space:nowrap;\" ><img style=\"cursor:pointer;\" onclick=\"informarPagamento(\'' || pde.pdeid || '\')\" src=\"../imagens/money.gif\" /></span>'
				ELSE '&nbsp;'
			END) as acao,
		";
	}
	
	$sql .= "	
			mun.estuf,				
			mun.mundescricao,
			pdicodinep,
			pdenome,
			pdiesfera,
			CASE WHEN ten.id_localizacao=1 THEN 'Urbana' ELSE 'Rural' END localizacao,
			CASE WHEN aeddscrealizada IS NOT NULL THEN aeddscrealizada ELSE 'Em elaboração' END as realizado,
			((CASE WHEN 
				(select count(distinct abaresp.abaid) from pdeinterativo.abaresposta abaresp where abaresp.pdeid = pde.pdeid) > 0
					THEN round(((select count(distinct abaresp.abaid) from pdeinterativo.abaresposta abaresp where abaresp.pdeid = pde.pdeid)::numeric(10,2) /(select count(distinct abaid) from pdeinterativo.aba where (abatipo != 'O' or abatipo is null) and abaidpai is not null and abaid not in (2,3,4,5,6,7,8,54))::numeric(10,2))*100,0)
					ELSE 0
			END)) as percent,
			CASE WHEN hst.htddata IS NULL THEN 'Não tramitou' ELSE to_char(hst.htddata,'dd/mm/YYYY') END as htddata,
			esd.esddsc,
			usu.usunome
		from pdeinterativo.pdinterativo pde 
			left join pdeinterativo.cargacapitalcusteio ccc ON ccc.codinep = pde.pdicodinep::integer  
			left join territorios.municipio             mun ON pde.muncod  = mun.muncod 
			left join workflow.documento                doc ON doc.docid   = pde.docid
			left join workflow.historicodocumento       hst ON hst.hstid   = doc.hstid 
			left join workflow.acaoestadodoc            atd ON atd.aedid   = hst.aedid 
			left join workflow.estadodocumento          esd ON esd.esdid   = doc.esdid 
			left join educacenso_2010.tab_entidade      ten ON ten.pk_cod_entidade = pde.pdicodinep::bigint
			left join seguranca.usuario                 usu ON usu.usucpf  = hst.usucpf
		where pde.pditempdeescola = TRUE
		AND   pde.pdistatus       = 'A'
		AND   pde.pdenome         IS NOT NULL ".
		(($where) ? "AND ".implode(" AND ", $where) : "")."
		order by mun.estuf, mun.mundescricao";
	
	$cabecalho = array();
	if($_REQUEST['visualizar']=='html')
		array_push($cabecalho, "&nbsp;");
	array_push($cabecalho, "UF","Município","Código INEP","Nome da escola","Rede","Localização","Situação do Plano","Preenchimento do PDE","Data Tramitação","Situação","Responsável");

	if($_REQUEST['visualizar']=='html') {
?>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script>
			function filtro_relatorio_acompanhamento(vlr) {
				var filtro_municipio='';
				if(document.getElementById('muncod')) {
					filtro_municipio = '&muncod='+document.getElementById('muncod').value;
				}
				window.location='pdeinterativo.php?modulo=relatorio/relatoriosgerais&acao=A&visualizar=html&buscar=1&relatorio=relatgeral_acompanhamento.inc&esdid='+document.getElementById('esdid').value+'&estuf='+document.getElementById('estuf').value+filtro_municipio/*+'&pflcod='+document.getElementById('pflcod').value+'&usucpf='+document.getElementById('usucpf').value*/;
			}
			
			function informarPagamento(pdeid) {
				var url = 'pdeinterativo.php?modulo=principal/informarPagamento&acao=A&pdeid=' + pdeid;
				window.location.href = url;
			}
		</script> 
		<table class="tabela">
			<tr>
				<td>Situação:</td>
				<td>
					<?php
						$sql_combo = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='".TPD_WF_FLUXO."'";
						$db->monta_combo('esdid', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'esdid', '', $_REQUEST['esdid']);
					?>
				</td>
			</tr>
			<tr>
				<td>Estado:</td>
				<td>
					<?php
						$sql_combo = "SELECT estuf as codigo, estdescricao as descricao FROM territorios.estado";
						$db->monta_combo('estuf', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'estuf', '', $_REQUEST['estuf']);
					?>
				</td>
			</tr>
			<?php
				if($_REQUEST['estuf']){
			?>
					<tr>
						<td>Município:</td>
						<td>
							<?php
								$sql_combo = "SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio WHERE estuf='".$_REQUEST['estuf']."' ORDER BY mundescricao";
								$db->monta_combo('muncod', $sql_combo, 'S', 'Selecione', 'filtro_relatorio_acompanhamento', '', '', '200', 'S', 'muncod', '', $_REQUEST['muncod']);
							?>
						</td>
					</tr>
			<?php
				}
			?>
		</table>
<?php
		$_SESSION['pdeinterativo_vars']['btn_cancelar'] = 'pdeinterativo.php?modulo=relatorio/relatoriosgerais&acao=A&visualizar=html&buscar=1&relatorio=relatgeral_acompanhamento.inc&esdid='.$_REQUEST['esdid'].'&estuf='.$_REQUEST['estuf'].'&muncod='.$_REQUEST['muncod']/*.'&pflcod='.$_REQUEST['pflcod'].'&usucpf='.$_REQUEST['usucpf']*/;
		$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','100%','N',true);
		$db->monta_lista_simples("SELECT 'TOTAL', count(*) FROM (".$sql.") foo",array(),100000,5,'N','100%','N',true);
	}
	else{
		ob_clean();
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_Acomp_".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->sql_to_excel($sql, 'RelatorioAcompanhamento', $cabecalho);		
	}
?>