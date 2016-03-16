<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);  

include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo_modulo = "Relatório - Novas Turmas Aprovadas 2012";
monta_titulo( $titulo_modulo, '' );
?>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" id="requisicao" name="requisicao" value=""/>
	
	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td class="subtituloDireita" width="25%">UF:</td>
			<td>
				<?php 
				$sql = "SELECT		estuf as codigo,
									estdescricao as descricao
						FROM		territorios.estado
						ORDER BY 	estuf";
				$estuf = $_POST['estuf'];
				$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', ''); 
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município:</td>
			<td>
				<?php
				$mundescricao = $_POST['mundescricao']; 
				echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', ''); ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Gerar Excel" id="btnGerarExcel" />
				<input type="button" value="Limpar" id="btnLimpar" />
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
$(function(){
	$('#btnPesquisar').click(function(){	
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == ''){
			alert('Preencha pelo menos um filtro!');
			return false;
		}
		$('[name="requisicao"]').val('pesquisar');
		$('#formulario').submit();
	});
	$('#btnGerarExcel').click(function(){
		$('[name="requisicao"]').val('excel');
		$('#formulario').submit();
	});
	$('#btnLimpar').click(function(){
		document.location.href = 'proinfantil.php?modulo=relatorio/relatorioNovasTurmas2012&acao=A';
	});;
});
</script>
<?

if($_POST['estuf']){
	$arWhere[] = "mun.estuf = '{$_POST['estuf']}'";
}

if($_POST['mundescricao']){
	if( is_numeric( $_POST['mundescricao'] ) ){
		$arWhere[] = "mun.muncod = '".trim($_POST['mundescricao'])."'";
	} else {
		$arWhere[] = "UPPER( removeacento(mun.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($_POST['mundescricao']) ) )."%'";
	}
}

$sql = "SELECT distinct
			public.formata_cpf_cnpj(iue.iuecnpj) as cnpj,
		    mun.estuf,
		    mun.muncod,
		    mun.mundescricao
		FROM 
        	territorios.municipio mun
			inner join proinfantil.novasturmasmunicipios ntm on ntm.muncod = mun.muncod
        	inner join proinfantil.turma tur on tur.muncod = mun.muncod
		    inner join workflow.documento doc on doc.docid = ntm.docid
			inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			left join par.instrumentounidade iu
				inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
			on iu.muncod = mun.muncod
		 WHERE  
		 	doc.esdid in (".WF_NOVASTURMAS_EM_APROVADA.", ".WF_NOVASTURMAS_AGUARDANDO_PAGAMENTO.", ".WF_NOVASTURMAS_PAGAMENTO_EFETUADO.")
		 	AND tur.turstatus = 'A'
		 	".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."
		 order by
		 	mun.estuf, mun.mundescricao";

$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
$arrRegistro = array();

foreach ($arrDados as $key => $v) {
	$sql = "SELECT ntmid, muncod, ntmstatus, ntmano, ntmsemestre, ntmqtdmatriculacrecheparcialpublica, ntmqtdmatriculacrecheintegralpublica, ntmqtdmatriculapreescolaparcialpublica,
			  	ntmqtdmatriculapreescolaintegralpublica, ntmqtdmatriculacrecheparcialconveniada, ntmqtdmatriculacrecheintegralconveniada, ntmqtdmatriculapreescolaparcialconveniada,
			  	ntmqtdmatriculapreescolaintegralconveniada, ntmqtdturmacrechepublica, ntmqtdturmapreescolapublica, ntmqtdturmaunificadapublica, ntmqtdturmacrecheconveniada,
			  	ntmqtdturmapreescolaconveniada, ntmqtdturmaunificadaconveniada
			FROM 
			  	proinfantil.novasturmasdadosmunicipios
			WHERE
				muncod = '{$v['muncod']}'
				and ntmstatus = 'A'";			
	$arrMatricula = $db->pegaLinha($sql);
	$arrMatricula = $arrMatricula ? $arrMatricula : array();
	
	$sql = "SELECT distinct
			    tur.turdsc,
			    (CASE WHEN mds.timid = 1 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.titid = 5 THEN 'Integral' ELSE 'Parcial' END) AS nome,
			    tr.tirdescricao,
			    ala.total_alunos, 
			    vr.vrtvalor, 
			    tur.periodo_repasse||'/12',
			    (ala.total_alunos*tur.periodo_repasse*vr.vrtvalor)/12 AS valor_total
			FROM proinfantil.mdsalunoatendidopbf mds
			INNER JOIN (SELECT alaid, sum(alaquantidade) AS total_alunos FROM proinfantil.mdsalunoatendidopbf GROUP BY alaid) as ala on ala.alaid = mds.alaid
			INNER JOIN (SELECT turid, muncod, turstatus, ttuid, turdtinicio, tirid, turdsc, (select proinfantil.calculorepasse2012(turdtinicio)) AS periodo_repasse                            
			            FROM proinfantil.turma) AS tur ON tur.turid = mds.turid
			INNER JOIN proinfantil.tiporede tr ON tr.tirid = tur.tirid                                                
			INNER JOIN (     SELECT vrtvalor, ttuid, tirid, tatid  
			                 FROM proinfantil.valorreferencianovasturmas where vrtano = '{$_SESSION['exercicio']}' and vrtstatus = 'A' ) AS vr on vr.ttuid = tur.ttuid and vr.tirid = tur.tirid and ( case when (mds.titid = 5) then  vr.tatid = 1 else vr.tatid = 2 end ) 
								INNER JOIN proinfantil.analisenovasturmasaprovacao ana ON ana.turid = tur.turid
							WHERE muncod = '{$v['muncod']}'
							                AND ana.anatipo = 'A'
							                AND tur.turstatus = 'A'
			ORDER BY tur.turdsc";
	
	$aryRepasse = $db->carregar($sql);
	$aryRepasse = $aryRepasse ? $aryRepasse : array();
	
	$valorTotal = 0;
	foreach ($aryRepasse as $repasse) {
		$valorTotal += $repasse['valor_total'];
	}
	
	$arrRegistro[$key]= array(
							'cnpj' 						=> $v['cnpj'],
							'estuf' 						=> $v['estuf'],
							'municipio' 					=> $v['mundescricao'],
							'ibge' 							=> $v['muncod'],
							'crechepublicaparcial'			=> (int)$arrMatricula['ntmqtdmatriculacrecheparcialpublica'] + (int)$arrMatricula['ntmqtdmatriculacrecheintegralpublica'],
							//'crechepublicaintegral' 		=> (int)$arrMatricula['ntmqtdmatriculacrecheintegralpublica'],
							'crecheconveniadaparcial' 		=> (int)$arrMatricula['ntmqtdmatriculacrecheparcialconveniada'] + (int)$arrMatricula['ntmqtdmatriculacrecheintegralconveniada'],
							//'crecheconveniadaintegral' 		=> (int)$arrMatricula['ntmqtdmatriculacrecheintegralconveniada'],
			
							'preescolapublicaparcial'		=> (int)$arrMatricula['ntmqtdmatriculapreescolaparcialpublica'] + (int)$arrMatricula['ntmqtdmatriculapreescolaintegralpublica'],
							//'preescolapublicaintegral' 		=> (int)$arrMatricula['ntmqtdmatriculapreescolaintegralpublica'],
							'preescolaconveniadaparcial'	=> (int)$arrMatricula['ntmqtdmatriculapreescolaparcialconveniada'] + (int)$arrMatricula['ntmqtdmatriculapreescolaintegralconveniada'],
							//'preescolaconveniadaintegral' 	=> (int)$arrMatricula['ntmqtdmatriculapreescolaintegralconveniada'],
							'valorRepasse' 					=> number_format((float)$valorTotal, 2, ',', '.')
						);
}
				
$cabecalho = array('CNPJ', 'UF', 'Municípo', 'Código IBGE', 'Creche Púb/Conv Parcial', 'Creche Púb/Conv Integral', 'Pré-Escola Púb/Conv Parcial', 'Pré-Escola Púb/Conv Integral', 'Valor do Repasse');
if( $_POST['requisicao'] == 'excel' ){
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatNovasTurmas2012".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatNovasTurmas2012".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arrRegistro,$cabecalho,1000000,5,'N','100%','');
	exit;
} else {
	$db->monta_lista_simples($arrRegistro, $cabecalho, 6000, 1, '', '95%', 'S', '', '', '', true);
}
?>