<?php
ini_set("memory_limit", "2048M");
set_time_limit(30000);  
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo_modulo = "Relatório - Obras Aprovadas";
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
		document.location.href = 'proinfantil.php?modulo=relatorio/relatorioObrasAprovadas&acao=A';
	});;
});
</script>
<?

if($_POST['estuf']){
	$arWhere[] = "tm.estuf = '{$_POST['estuf']}'";
}

if($_POST['mundescricao']){
	if( is_numeric( $_POST['mundescricao'] ) ){
		$arWhere[] = "tm.muncod = '".trim($_POST['mundescricao'])."'";
	} else {
		$arWhere[] = "UPPER( removeacento(tm.mundescricao) ) ilike '%".removeAcentos( str_to_upper( trim($_POST['mundescricao']) ) )."%'";
	}
}

$sql = "SELECT 
		    upper(ee.entnome) as nome_da_instituicao,
		    oi.obrid as id_da_bra,
		    upper(oi.obrnome) as nome_da_obra,
		    ed2.endcep as cep, 
		    ed2.endlog || ' - ' || ed2.endcom as endereco, 
		    ed2.endbai as bairro, 
			ed2.medlatitude as latitude, 
			ed2.medlongitude as longitude,
		    to_char(oi.obrdtinicio,'dd-mm-yyyy') as data_inicio_da_obra,
		    to_char(oi.obrdtfim,'dd-mm-yyyy')as data_termino_da_obra,
		    ed.estuf as uf,
		    tm.mundescricao as municipio,
			oo.orgdesc as tipo_de_ensino,
		    esd1.esddsc as situacao_da_obra,
		    oi.obrpercentultvistoria as percentual_executado,
		    pf.prfdesc as programa_fonte,
		    tp.tobdesc as tipo_de_obra,
		    coalesce(tpl.tpodsc,'n/a') as tipologia_da_obra
		FROM
			obras2.obras AS oi
		    INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
		    INNER JOIN entidade.endereco ed2 on ed2.endid = oi.endid
		    INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
		    INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
		    INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
		    INNER JOIN obras.preobra  pre on pre.obrid = oi.obrid and  pre.tooid = 1 and prestatus = 'A'
		    INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND oo.orgstatus = 'A'                                                 
		    INNER JOIN workflow.documento d1 ON d1.docid = oi.docid
		    INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
		    INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
		    INNER JOIN obras2.tipoobra AS tp ON oi.tobid = tp.tobid
		    LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
		    LEFT JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid
		    LEFT JOIN (SELECT                          q.pinid, r.resdsc
		                       FROM                               proinfantil.questionario q
		                       INNER JOIN     questionario.resposta r on r.qrpid = q.qrpid
		                       WHERE                             r.perid = 1587 ) as dt ON dt.pinid = pi.pinid                                                        
		    LEFT JOIN workflow.documento d ON d.docid = pi.docid
		    LEFT JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
		WHERE
		    oi.obrstatus = 'A'
		    and ee.entstatus = 'A'
		    and pf.prfid = 41
		    and oi.obrpercentultvistoria >= 90
		  	and esd1.esdid IN (690, 693)
		    and oi.obridpai IS NULL
		   	and tpl.tpoid in (16,9,10) 
		    and pi.pinid in (
		               select pinid from questionario.questionarioresposta qr 
		               inner join questionario.resposta r on r.qrpid = qr.qrpid and r.perid = 1587
		               inner join proinfantil.questionario q on q.qrpid = qr.qrpid
		               where qr.queid = 62
		               )
		    and edoc.esdid in ( 372, 373, 557, 407 ) -- obras aprovadas
			".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')." 
		ORDER BY
		  	ee.entnome";

$cabecalho = array('Nome da Instituição', 'ID da Obra', 'Nome da Obra', 'CEP', 'Endereço', 'Bairro', 'Latitude', 'Longitude', 'Data Inicio da Obra', 'Data Termino da Obra', 'UF', 'Município', 'Tipo de Ensino', 'Situação da Obra', 'Percentual Executado (Vistoria)', 'Programa Fonte', 'Tipo de Obra', 'Tipologia da Obra');
if( $_POST['requisicao'] == 'excel' ){
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Relatorio_Turmas_Aprovadas_".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Relatorio_Turmas_Aprovadas_".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($sql,$cabecalho,1000000,5,'N','100%','');
	exit;
} else {
	$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '95%', 'S', '', '', '', true);
}
?>