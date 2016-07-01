<?php
ini_set( "memory_limit", "1024M" );
global $db;
include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
monta_titulo( $titulo_modulo, '' ); ?>

<script type="text/javascript">
function submitFormulario(tipo){
	var formulario = document.formulario;
	selectAllOptions( formulario.grupo );
	if( tipo == 1 ){ //relatório
		formulario.exibirxls.value = 'exibe';
	} else {
		formulario.exibirxls.value = '';
	}
	formulario.submit();
}
</script>

<form method="post" name="formulario" id="formulario">
<input type="hidden" id="exibirxls" name="exibirxls" value="">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita">Grupos de Municípios:</td>
		<td valign="bottom">
			<?php
			// Filtros do relatório
			$stSql = "SELECT  	tpmid AS codigo, tpmdsc AS descricao
					  FROM	  	territorios.tipomunicipio mtq
					  WHERE	  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)";
							
			if( $_REQUEST['grupo'][0] != '' ){
				$grupos = implode( $_REQUEST['grupo'], "," );
				$sql = "SELECT	tpmid AS codigo, tpmdsc AS descricao
					    FROM  	territorios.tipomunicipio mtq
					    WHERE	tpmid IN ({$grupos})";
				$grupo = $db->carregar( $sql );
			}

			combo_popup("grupo", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400); ?>
		</td>
	</tr> 
	<tr>
		<td class="SubTituloDireita">Situação do Transporte:</td>
		<td>
			<?php
			$esdid = $_POST["esdid"];
			$sql = "SELECT 		esdid AS codigo, esddsc AS descricao 
					FROM		workflow.estadodocumento 
					WHERE		tpdid = 59 AND esdstatus = 'A'
					ORDER BY 	esddsc";
			
			$res = $db->carregar( $sql );
			$db->monta_combo( "esdid_trans", $res , 'S', 'Todas as Situações', '', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Situação do PAR:</td>
		<td>
			<?php
			$esdid = $_POST["esdid"];
			$sql = "SELECT 		esdid AS codigo, esddsc AS descricao 
					FROM		workflow.estadodocumento 
					WHERE		tpdid = 44 AND esdstatus = 'A'
					ORDER BY 	esddsc";
			
			$res = $db->carregar( $sql );
			$db->monta_combo( "esdid_par", $res , 'S', 'Todas as Situações', '', ''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Aderiu ao programa:</td>
		<td>
			<?php
			$arrayPrograma = array(
				array('codigo' => 'T', 'descricao' => 'Todos'),
				array('codigo' => 'S', 'descricao' => 'Sim'),
				array('codigo' => 'N', 'descricao' => 'Não')
			);
			$adpresposta = $_POST['adpresposta'];
			$db->monta_combo( "adpresposta", $arrayPrograma, 'S', '', '', '', 'Selecione o item caso queira filtrar por Recebeu Recurso!', '244');		
			?>		
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita">Recebeu Recurso:</td>
		<td>
			<?php
			$arrayRecurso = array(
				array('codigo' => 'T', 'descricao' => 'Todos'),
				array('codigo' => 'S', 'descricao' => 'Sim'),
				array('codigo' => 'N', 'descricao' => 'Não')
			);
			$recurso = $_POST['recurso'];
			$db->monta_combo( "recurso", $arrayRecurso, 'S', '', '', '', 'Selecione o item caso queira filtrar por Recebeu Recurso!', '244');		
			?>		
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ano:</td>
		<td>
			<?php 
			$arrayAno = array(
				array( 'codigo' => 'T', 'descricao' => 'Todos' ),
				array( 'codigo' => '2012', 'descricao' => '2012' ),
				array( 'codigo' => '2013', 'descricao' => '2013' )
			);
			
			$adpano = $_POST['adpano'];
			$db->monta_combo( "adpano", $arrayAno, 'S', '', '', '', 'Selecione o item caso queira filtrar por Ano', '244');		
			?>
		</td>
	</tr>		
	<tr bgcolor="#D0D0D0">
		<td align="center" colspan="2">
			<input type="button" name="buscar" value="Buscar" onclick="submitFormulario(2);"/>
			<input type="button" name="buscar" value="XLS" onclick="submitFormulario(1);"/>
		</td>
	</tr>
</table>
</form>
<br>

<?php
extract($_POST);

if($grupo[0]){
	$grupos = implode($grupo, "," );
	$join[0] .= "INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = m.muncod AND mtm.tpmid IN ({$grupos})";
}

if($esdid_trans != ''){
	$where[0] = " AND doc.esdid = {$esdid_trans}";
} else {
	$where[0] = "";
}

if($esdid_par != ''){
	$where[6] = " AND d.esdid = {$esdid_par}";
} else {
	$where[6] = "";
}


if($adpresposta == 'S'){
	$where[1] = "AND adpresposta = 'S'"; 
} elseif($adpresposta == 'N') {
	$where[1] = "AND adpresposta = 'N'"; 
} else {
	$where[1] = ""; 
}

if($recurso == 'S'){
	$where[2] = "AND quantidade_empenhada > 0"; 
} elseif($recurso == 'N') {
	$where[2] = "AND quantidade_empenhada = 0"; 
} else {	
	$where[2] = ""; 
} 

switch(trim($adpano)){
	case '2012':
			$where[3] = "AND map.pfaid = 5";
			$where[4] = "AND sic.icoano = '2012'";
			$where[5] = "AND eobano = '2012'";
			$cota = "cota::integer,";
			break;
	case '2013':
			$where[3] = "AND map.pfaid = 11";
			$where[4] = "AND sic.icoano = '2013'";
			$where[5] = "AND eobano = '2013'";
			$cota = "(cota::integer - (SELECT 	(COALESCE(SUM(icoquantidadetecnico),0)) 
									   FROM 	par.subacaoitenscomposicao sic3
									   WHERE 	sic3.sbaid IN ( SELECT s1.sbaid FROM par.subacao s1 
								    							INNER JOIN par.acao a1 on s1.aciid = a1.aciid AND a1.acistatus  = 'A'
															    INNER JOIN par.pontuacao p1 ON p1.ptoid = a1.ptoid AND p1.ptostatus = 'A'
															    INNER JOIN par.empenhosubacao es ON es.sbaid = s1.sbaid AND eobstatus = 'A' AND eobano = '2012'
															    INNER JOIN par.empenho e ON e.empid = es.empid and e.empsituacao = '2 - EFETIVADO' and empstatus = 'A' 
															    WHERE p1.inuid = foo.inuid AND s1.sbastatus = 'A' and s1.ppsid = 1074 ) 
																AND sic3.icovalidatecnico = 'S')::integer )  as cota2013,";
			break;			
	case 'T':
			$where[3] = "AND map.pfaid IN (11)";
			$where[4] = "AND sic.icoano IN ('2012','2013')";
			$where[5] = "AND eobano IN ('2012','2013')";
			$cota = "cota::integer,";
			break;
	default:
			$where[3] = "AND map.pfaid IN (11)";
			$where[4] = "AND sic.icoano IN ('2012','2013')";
			$where[5] = "AND eobano IN ('2012','2013')";
			$cota = "cota::integer,";
			break;
}

$sql = "SELECT *
		FROM 
			(	SELECT
						estuf,
						muncod,
						mundescricao,
						$cota
						SUM(COALESCE(quantidade_solicitada,0)) as quantidade_solicitada,
						SUM(COALESCE(quantidade_aprovada,0)) as quantidade_aprovada,
						(SELECT (COALESCE(SUM(icoquantidadetecnico),0)) FROM par.subacaoitenscomposicao sic3
						WHERE sic3.sbaid = foo.sbaid AND sic3.icovalidatecnico = 'S'  AND sic3.sbaid IN (SELECT DISTINCT sbaid FROM par.empenhosubacao es
						INNER JOIN par.empenho e ON e.empid = es.empid and empstatus = 'A'
						WHERE eobstatus = 'A' and e.empsituacao = '2 - EFETIVADO' $where[5]) ) as quantidade_empenhada,
						situacao_par,
						situacao_transporte
				FROM (
						SELECT  
										DISTINCT m.muncod, 
										m.mundescricao, 
										map.estuf,
										mprqtd  as cota,
										SUM(COALESCE(sic.icoquantidade,0)) as quantidade_solicitada,
										CASE WHEN sic.icovalidatecnico = 'S' THEN SUM(COALESCE(sic.icoquantidadetecnico,0)) END as quantidade_aprovada,
										s.sbaid,
										iu.inuid,
										e.esddsc as situacao_par,
										e2.esddsc as situacao_transporte
						FROM 			par.municipioadesaoprograma map
						INNER JOIN 		territorios.municipio m ON map.muncod = m.muncod
						INNER JOIN 		par.instrumentounidade iu ON iu.muncod = m.muncod
						INNER JOIN 		workflow.documento d	  ON iu.docid = d.docid
						INNER JOIN		workflow.estadodocumento e ON d.esdid = e.esdid						
						$join[0]	
						LEFT JOIN 		par.pfadesaoprograma pap ON pap.inuid = iu.inuid AND pap.pfaid = map.pfaid 
						LEFT JOIN 		par.pontuacao p 
								INNER JOIN 		par.acao a ON p.ptoid = a.ptoid AND a.acistatus = 'A'
								INNER JOIN 		par.subacao s ON s.aciid = a.aciid AND s.ppsid = 1074 AND s.sbastatus = 'A'
								INNER JOIN 		par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid AND sic.icostatus = 'A' $where[4]
									ON p.inuid = iu.inuid AND p.ptostatus = 'A'
						LEFT JOIN 		workflow.documento doc 
											INNER JOIN	workflow.estadodocumento e2 ON doc.esdid = e2.esdid
												ON doc.docid = pap.docid
						WHERE 			1 = 1 $where[0] $where[1] $where[3] $where[6]
						GROUP BY 		m.muncod, m.mundescricao , map.estuf, mprqtd, sic.icovalidatecnico, s.sbaid,  iu.inuid, e.esddsc, e2.esddsc
						ORDER BY 		map.estuf, m.mundescricao
			) as foo
		GROUP BY
					muncod,
					mundescricao,
					estuf,
					cota, 
					sbaid, 
					inuid,
					situacao_par,
					situacao_transporte					
		ORDER BY
					estuf, mundescricao ) as foo2
		WHERE
				1 = 1
				$where[2]";

$dadosLista = $db->carregar($sql);	
$cabecalho = array("Estado","IBGE","Município","Cota","Quantidade Solicitada","Quantidade Aprovada","Quantidade Contemplada","Situação PAR","Situação Transporte");
$db->monta_lista_array($dadosLista, $cabecalho,50,10,'N','center');	

if($_POST['exibirxls'] == 'exibe') {
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatTransp".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatTransp".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,100000,5,'N','100%',$par2);
	exit;
}
?>