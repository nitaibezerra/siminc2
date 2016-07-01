<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

if ($_REQUEST['titleFor']) {
	/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
	 * Analista: Daniel Areas Programador Eduardo Duniceu
	* */
	$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
		FROM par.empenhoobra emo 
		INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
		INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
		INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
		WHERE emo.preid = '".$_REQUEST['titleFor']."' AND pag.pagstatus='A'";
	
	$pagamentoobra = $db->pegaLinha($sql);
	
	if($pagamentoobra) {
		echo "Pago<br>
			  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
			  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br> 
			  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
			  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
			  ";	
	} else {
		echo "Não pago";	
	}
	
	die();
}

$validacao = new Validacao();
$sql = $validacao->listaRelatorioValidacao($_REQUEST);

$cabecalho = array( "Id Obra", "ID Pré-Obra", "Unidade Implantadora", "Nome da Obra", "Nº Termo/Convênio", "Processo", "Situação","Município", "UF", "Valor Previsto", "(%) Executado", "Pagamento", "Total Pago", "Banco", "Agência", "Conta", "Total da conta", "Data de referencia do SALDO \"MES/ANO\"", "Homologado", "Observações", "Execução 25%", "Observações", "Execução 50%", "Observações","CA","CV","CT","CO","Fonte", "Programa", "Tipologia", "Técnico da Homologação", "Data da Homologação", "Técnico da Execução 25%", "Data da Execução 25%", "Técnico da Execução 50%", "Data da Execução 50%", "Data do último pagamento", "Situação do último pagamento", "% Pago", "Fim Vigência da Obra", "Fim Vigência Termo", "início Vigência Termo");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'relRelatorioValidacoes', $cabecalho);
}

if(isset($_REQUEST['totaliza'])){
	
	$sql = "SELECT 
				coalesce(sum(validadohomolog),0) as validadohomolog, 
				coalesce(sum(naovalidadohomolog),0) as naovalidadohomolog,
				coalesce(sum(naoanalisadohomolog),0) as naoanalisadohomolog,
				coalesce(sum(validado25),0) as validado25,
				coalesce(sum(naovalidado25),0) as naovalidado25,
				coalesce(sum(naoanalisado25),0) as naoanalisado25,
				coalesce(sum(validado50),0) as validado50,
				coalesce(sum(naovalidado50),0) as naovalidado50,
				coalesce(sum(naoanalisado50),0) as naoanalisado50
			FROM (
				SELECT DISTINCT oi.obrid,
					CASE WHEN v.vldstatushomologacao = 'S' THEN 1 ELSE 0 END AS validadohomolog,
					CASE WHEN v.vldstatushomologacao = 'N' THEN 1 ELSE 0 END AS naovalidadohomolog,
					CASE WHEN v.vldstatushomologacao IS NULL OR v.vldstatushomologacao not in ('S','N') THEN 1 ELSE 0 END AS naoanalisadohomolog,	
					CASE WHEN v.vldstatus25exec = 'S' THEN 1 ELSE 0 END AS validado25,
					CASE WHEN v.vldstatus25exec = 'N' THEN 1 ELSE 0 END AS naovalidado25,
					CASE WHEN v.vldstatus25exec IS NULL OR v.vldstatus25exec not in ('S','N') THEN 1 ELSE 0 END AS naoanalisado25,
					CASE WHEN v.vldstatus50exec = 'S' THEN 1 ELSE 0 END AS validado50,
					CASE WHEN v.vldstatus50exec = 'N' THEN 1 ELSE 0 END AS naovalidado50,
					CASE WHEN v.vldstatus50exec IS NULL OR v.vldstatus50exec not in ('S','N') THEN 1 ELSE 0 END AS naoanalisado50
				FROM obras2.empreendimento ep 
				INNER JOIN obras2.obras oi ON oi.empid = ep.empid
				LEFT JOIN workflow.documento doc ON doc.docid = oi.docid
				LEFT JOIN entidade.entidade ent on ent.entid = ep.entidunidade
		    	LEFT JOIN entidade.endereco ed on ed.entid = ent.entid
		    	left join entidade.endereco edo on edo.endid = oi.endid
				LEFT JOIN obras2.validacao v on v.obrid = oi.obrid
				LEFT JOIN obras2.tipologiaobra  tpo ON tpo.tpoid  = oi.tpoid
				WHERE oi.obrstatus = 'A' 
				and ep.orgid = 3 ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
			) AS foo";
	
	$rs = $db->pegaLinha($sql);

	echo simec_json_encode($rs);
	die;
}

?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
	<head>
		<title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	</head>
	<body>
		<center>
			<!--  Cabeçalho Brasão -->
			<?php echo monta_cabecalho_relatorio( '95' ); ?>
		</center>
		
	</body>
	<script type="text/javascript">
		var u='/obras2/obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&titleFor=';
		function obrIrParaCaminhoPopUp( obrid, tipo, orgid, arqid ){
			switch( tipo ){			
				case "cadastro":
					<?$_SESSION['obras']['orgid'] = 3; ?>
//					window.opener.location.href = "obras2.php?modulo=principal/cadastro&acao=A&obrid=" + obrid+"&orgid=3";
					window.opener.location.href = "obras2.php?modulo=principal/cadObra&acao=A&obrid=" + obrid+"&orgid=3";
					window.close(); 
				break;			
			}
		
		}
			
	</script>
</html>
<?php
//if( $where ){

//ver($sql);
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
//}
?>
<center>
	<p><input type="button" id="btnGeraExcel" value="Gerar Arquivo Excel"></p>
</center>
<?php
echo '<form id="request_Form" method="post">';
if(is_array($_POST)){
	if(count($_POST)){		
		$naoProcessar = array('pesquisa');
		foreach($_POST as $k => $v){
			if(!in_array($k, $naoProcessar)){
				if(is_array($v)){
					foreach($v as $vv){
						echo '<input type="hidden" name="'.$k.'[]" value="'.$vv.'" />';	
					}			
				}else{
					echo '<input type="hidden" name="'.$k.'" value="'.$v.'" />';	
				}
			}
		}
	}	
}
echo "</form>";
?>
<div id="dv_sql"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$.ajax({
		url: 'obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&totaliza=true&'+$('#request_Form').serialize(),
		type: 'post',		
		dataType: 'json',
		success: function(data){
			data.validadohomolog = typeof data.validadohomolog == 'undefined' ? 0 : data.validadohomolog;
			data.naovalidadohomolog = typeof data.naovalidadohomolog == 'undefined' ? 0 : data.naovalidadohomolog;
			data.validado25 = typeof data.validado25 == 'undefined' ? 0 : data.validado25;
			data.naovalidado25 = typeof data.naovalidado25 == 'undefined' ? 0 : data.naovalidado25;
			data.naovalidado50 = typeof data.naovalidado50 == 'undefined' ? 0 : data.naovalidado50;
			data.naovalidado50 = typeof data.naovalidado50 == 'undefined' ? 0 : data.naovalidado50;		
			data.naoanalisadohomolog = typeof data.naoanalisadohomolog == 'undefined' ? 0 : data.naoanalisadohomolog;
			data.naoanalisado25 = typeof data.naoanalisado25 == 'undefined' ? 0 : data.naoanalisado25;
			data.naoanalisado50 = typeof data.naoanalisado50 == 'undefined' ? 0 : data.naoanalisado50; 
				
			texto = '<b>Homologação</b> - Validados: '+data.validadohomolog+'&nbsp;';
			texto += '&nbsp;Não Validados: '+data.naovalidadohomolog+'&nbsp;';
			texto += '&nbsp;Não Analisados: '+data.naoanalisadohomolog+'&nbsp;';		
			$('table').last().find('tbody').append('<tr><td colspan="2">'+texto+'</td></tr>')
			
			texto = '<b>Execução 25%</b> - Validados: '+data.validado25+'&nbsp;';		
			texto += '&nbsp;Não Validados: '+data.naovalidado25+'&nbsp;';
			texto += '&nbsp;Não Analisados: '+data.naoanalisado25+'&nbsp;';
			$('table').last().find('tbody').append('<tr><td colspan="2">'+texto+'</td></tr>')
			
			texto = '<b>Execução 50%</b> - Validados: '+data.validado50+'&nbsp;';
			texto += '&nbsp;Não Validados: '+data.naovalidado50+'';
			texto += '&nbsp;Não Analisados: '+data.naoanalisado50+'&nbsp;';
			$('table').last().find('tbody').append('<tr><td colspan="2">'+texto+'</td></tr>')		
		}
	});
	
	$('#btnGeraExcel').click(function(){	
		window.open ('obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"geraXls");		
	});
	
});
</script>
