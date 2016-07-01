<?php 
include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function fazerDownload(){
	
	$file = new FilesSimec(null,null,'obras');
	$file->getDownloadArquivo($_REQUEST['arqid']);
	echo "<script>window.close();</script>";
}

function montaFiltrosRelatorio(){
	
	global $db;
	
	$where = Array();
	
	$where[] = "pp.prpstatus = 'A'
                AND empstatus = 'A'
                AND e.empsituacao <> 'CANCELADO'
                AND (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar]))";
	if( $_POST['data_inicio'] ) 			$where[] = "c.condata::date >= '".formata_data_sql($_POST['data_inicio'])."'::date";
	if( $_POST['data_fim'] ) 				$where[] = "c.condata::date <= '".formata_data_sql($_POST['data_fim'])."'::date";
	if( $_POST['possui_pagamento'] == 'S' ) $where[] = "e.empid IN ( SELECT DISTINCT empid FROM par.pagamento )";
	if( $_POST['possui_pagamento'] == 'N' ) $where[] = "e.empid NOT IN ( SELECT DISTINCT empid FROM par.pagamento )";
	
	return $where;
}

function montaSQLRelatorio(){
	
	global $db;
	
	$where = montaFiltrosRelatorio();
	
	
	if( $_POST['esfera'] == 'E' ){
		$where[] = "inu.itrid = 1";
	}else{
		$where[] = "inu.itrid = 2";
	}
		
	$sql = "SELECT 
				'<img border=0 arqid='||arqid||' width=20px src=../imagens/searchbar_download_sel.bmp class=download style=cursor:pointer />' AS acao,
				coalesce(mun.estuf, inu.estuf) AS uf, 
                coalesce(mun.muncod, '-') AS ibge, 
                coalesce(mun.mundescricao, '-') AS municipio, 
                pp.prpnumeroprocesso AS processo, 
                removeacento(trim(sbadsc)) AS subacao, 
                e.empnumero AS notaempenho, 
                e.empvalorempenho AS valorne,
                es.eobvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric) AS valorempenhoparasubacao,
                'SIM' AS temcontrato, 
                to_char( c.condata, 'DD-MM-YYYY') AS datadocontrato
			FROM par.processopar pp
			INNER JOIN par.processoparcomposicao 			ppc ON  pp.prpid = ppc.prpid
			INNER JOIN par.subacaodetalhe 					sd ON sd.sbdid = ppc.sbdid
			INNER JOIN par.subacao 							s ON s.sbaid = sd.sbaid
			INNER JOIN par.subacaoitenscomposicao 			si ON si.sbaid = s.sbaid AND si.icoano = sd.sbdano AND icostatus = 'A'  
			INNER JOIN par.subacaoitenscomposicaoContratos 	ic ON si.icoid = ic.icoid
			INNER JOIN par.contratos 						c ON c.conid = ic.conid AND constatus = 'A'
			INNER JOIN par.empenho 							e ON e.empnumeroprocesso = pp.prpnumeroprocesso
			INNER JOIN par.empenhosubacao 					es ON es.empid = e.empid AND es.sbaid = s.sbaid AND es.eobano = sd.sbdano AND es.eobstatus = 'A'
			LEFT  JOIN 
                ( 
					SELECT e.empnumeroprocesso, e.empidpai, sum(es.eobvalorempenho) AS vrlcancelado
					FROM par.empenho e
					INNER JOIN par.empenhosubacao es ON es.empid = e.empid AND es.eobstatus = 'A'
					WHERE 
						(e.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) 
						AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
					GROUP BY e.empnumeroprocesso, e.empidpai
                ) ep ON ep.empidpai = e.empid
			LEFT JOIN 
                ( 
					SELECT e.empnumeroprocesso, e.empidpai, sum(es.eobvalorempenho) AS vlrreforco
					FROM par.empenho e
					JOIN par.empenhosubacao es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
					WHERE 
						e.empcodigoespecie = '02'::bpchar AND e.empstatus = 'A'::bpchar 
						AND e.empsituacao <> 'CANCELADO'::bpchar
					GROUP BY e.empnumeroprocesso, e.empidpai
                ) er ON er.empidpai = e.empid
			INNER JOIN par.instrumentounidade	inu ON inu.inuid  = pp.inuid
			LEFT  JOIN territorios.municipio 	mun ON mun.muncod = inu.muncod
			LEFT  JOIN territorios.estado 		est ON est.estuf  = inu.estuf
			WHERE                 
				".implode(' AND ', $where)."
			ORDER BY uf,municipio, processo, subacao";
	
	return $sql;
}

function listarRelaltorio(){
	
	global $db;
	
	$sql = montaSQLRelatorio();

	$param['nome'] 				= 'relContratosPagamento';
	$param['descricaoBusca']	= 'Rapida';
	$param['instrucaoBusca']	= '(Digite aqui a sua busca rápida)';
	$param['nomeXLS']			= "Relatório de Pagamento/Contratos do PAR $descricao-$dataAtual";
	$param['numeroColunasXLS']	= '1,2,3,4,5,6,7,8,9,10,11,12';
	$param['arrDados']			= $db->carregar($sql);
	$param['arrCabecalho']		= Array( 'Ação', 'UF', 'Código IBGE', 'Município', 'Processo', 'Subação', 'Nota de <br>Empenho', 'Valor da <br>NE', 
										 'Valor do <br>Empenho', 'Possui <br>Contrato', 'Data do <br>Contrato' );
	$param['arrSemSpan']		= Array( 'N' );
	$param['arrCampoNumerico']	= Array();
	
	$lista = new listaDT();
	echo $lista->lista($param);
}

if( $_REQUEST['req'] ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

include_once APPRAIZ . 'includes/cabecalho.inc';
echo'<br>';
monta_titulo( $titulo_modulo, $tabela_execucao );
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<!-- <link href="../par/css/jquery-ui-1.11.2.custom/jquery-ui.min.css" type="text/css" rel="stylesheet"></link> -->

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript" src="../par/js/jquery-1.11.1.min.js"></script>
<!-- <script type="text/javascript" src="../par/css/jquery-ui-1.11.2.custom/jquery-ui.min.js"></script> -->
<!-- <script type="text/javascript" src="../par/js/jquery.media.js"></script> -->
<!-- <script type="text/javascript" src="../par/js/jquery.metadata.js"></script> -->

<!-- <script type="text/javascript" src="../par/js/jquery-gdocsviewer-js.js"></script> -->
<script type="text/javascript">
jQuery.noConflict();
jQuery(document).ready(function(){

// 	jQuery('a.embed').gdocsViewer({width: 740, height: 742});
// 	jQuery('#embedURL').gdocsViewer();
	 
	jQuery('.pesquisar').click(function(){

		jQuery('#formulario').submit();
	});

	jQuery('.pdf').click(function(){
		alert('foi');
		jQuery( "#div_pdf" ).html( '<a class="media" link="'+jQuery(this).attr('pdf')+'" ></a>' );
		jQuery( '#div_pdf' ).show();
		jQuery('a.media').media({width:500, height:400});
		jQuery( "#div_pdf" ).dialog({
			resizable: true,
			width: 700,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				"Fechar": function() {
					jQuery( this ).dialog( "close" );
				},
			}
		});
	});

	jQuery(document).on('click','.download', function(){

		return window.open(window.location.href+'&req=fazerDownload&arqid='+jQuery(this).attr('arqid'), 
		    	   'Download', 
		    	   "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();	
	});	
});
</script>
<!--  <a href="par.php?modulo=relatorio/relatorioContratosPagamento&acao=A&req=montaCaminho&arqid=10027230" class="embed" style="text-align : center; display : block;">PDF test georgetown.edu</a>
<a href="http://simec-local/par/Doc_Eduardo_DUnice_Filho.pdf" class="embed" style="text-align : center; display : block;">PDF test georgetown.edu</a>-->
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="req" id="req" value="" />
	<input type="hidden" name="arqid" id="arqid" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">FILTROS</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<td>
				<input type="radio" name="esfera" value="M" <?=($_POST['esfera']!='E'?'checked="checked"':'') ?>/> Municipal 
				<input type="radio" name="esfera" value="E" <?=($_POST['esfera']=='E'?'checked="checked"':'') ?>/> Estadual
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Datas do contrato:</td>
			<td>
				<?$data_inicio 	= $_POST['data_inicio'];?>
				<?$data_fim 	= $_POST['data_fim'];?>
				De: <?=campo_data2('data_inicio', 'N', 'S', '', '' ); ?>
				Até: <?=campo_data2('data_fim', 'N', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Pagamento:</td>
			<td>
				<input type="radio" name="possui_pagamento" value="S" <?=($_POST['possui_pagamento']=='S'?'':'checked="checked"') ?>/> Possui
				<input type="radio" name="possui_pagamento" value="N" <?=($_POST['possui_pagamento']=='N'?'checked="checked"':'') ?>/> Não Possui
				<input type="radio" name="possui_pagamento" value="T" <?=(!in_array($_POST['possui_pagamento'],Array('S','N'))?'checked="checked"':'') ?>/> Todos
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<input type="button" class="pesquisar" name="pequisar" value="Gerar Relatório" />
			</td>
		</tr>
	</table>
</form>
<?php 
	if( $_POST['esfera'] ) listarRelaltorio();
?>
<div id="div_pdf"></div>