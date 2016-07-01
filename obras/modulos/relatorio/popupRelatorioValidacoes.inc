<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

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


//ver($_REQUEST);
/* if( $orgid ){
	array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
} */
// programa
if( $prfid[0] && $prfid_campo_flag ){
	if ( !$prfid_campo_excludente ){
		array_push($where, " oi.prfid  IN (" . implode( ',', $prfid ) . ") ");	
	}else{
		array_push($where, " ( oi.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR oi.prfid is null ) ");
	}
	
}
// Fonte
if( $tooid[0] && $tooid_campo_flag ){
	if ( !$tooid_campo_excludente ){
		array_push($where, " oi.tooid  IN (" . implode( ',', $tooid ) . ") ");	
	}else{
		array_push($where, " ( oi.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR oi.tooid IS NULL ) ");
	}
	
}

if( !empty( $entidunidade ) ) array_push($where, "oi.entidunidade = {$entidunidade} ");
if( !empty( $stoid ) ) array_push($where, "oi.stoid = {$stoid}");
if( !empty( $estuf ) ) array_push($where, "edo.estuf = '{$estuf}'");
if( !empty( $obrid ) ) array_push($where, "oi.obrid = $obrid");
if( !empty( $preid ) ) array_push($where, "oi.preid = $preid");
//if( !empty( $obrtextobusca ) ) array_push($where, "(oi.preid = ".(int)$obrtextobusca." or oi.obrid =".(int)$obrtextobusca.")");

if( $homologacao == 'S' ) array_push( $where, " v.vldstatushomologacao = 'S' " );
elseif( $homologacao == 'N' ) array_push( $where, " (v.vldstatushomologacao = 'N') " );
elseif( $homologacao == 'P' ) array_push( $where, " (v.vldstatushomologacao is null) " );

if( $execucao25 == 'S' ) array_push( $where, " v.vldstatus25exec = 'S' " );
elseif( $execucao25 == 'N' ) array_push( $where, " v.vldstatus25exec = 'N' " );
elseif( $execucao25 == 'P' ) array_push( $where, " v.vldstatus25exec IS NULL " );

if( $execucao50 == 'S' ) array_push( $where, " v.vldstatus50exec = 'S' " );
elseif( $execucao50 == 'N' ) array_push( $where, " v.vldstatus50exec = 'N' " );
elseif( $execucao50 == 'P' ) array_push( $where, " v.vldstatus50exec IS NULL " );


if( $arqanexo == 'S' ){
	$innerArquivo = "inner join obras.faselicitacao fl on fl.obrid = oi.obrid 
            	and fl.flcstatus = 'A' and fl.tflid = 9
            inner join obras.arquivosobra ao on ao.obrid = oi.obrid and ao.tpaid = '24' and ao.aqostatus = 'A'
            inner join public.arquivo arq on arq.arqid = ao.arqid 
            	and (arqtipo <> 'image/jpeg' and arqtipo <> 'image/png' and arqtipo <> 'image/gif')";
}
elseif( $arqanexo == 'N' ){
	array_push( $where, " oi.obrid not in (select distinct arq.obrid from obras.arquivosobra arq where arq.tpaid = 24) " );
}
elseif( $arqanexo == '' || !$arqanexo ){
	$innerArquivo = "";
}

if ( $_REQUEST['pesquisa'] == 'xls' ){
	$campos = "upper(oi.obrdesc) as obrdesc,
			    mun.mundescricao as municipio,
			    edo.estuf as uf,
			    COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
			   	COALESCE(oi.obrpercexec, 0.00) as obrpercexec,
			   	CASE WHEN oi.preid IS NOT NULL THEN
					'Sim'
				else
					'Não'
				END as pagamento,
				pag.pagvalorparcela,
				pag.probanco || ' ' AS probanco,
				pag.proagencia || ' ' AS proagencia,
				pag.nu_conta_corrente || ' ' AS nu_conta_corrente,
				v.vldstatushomologacao,
			    CASE WHEN (v.vldobshomologacao IS NULL or v.vldobshomologacao = '') THEN 'Sem observações' ELSE v.vldobshomologacao END as observacao,
			    v.vldstatus25exec,
			    v.vldstatus50exec,";
}else{
	$campos = "'<a onclick=\"obrIrParaCaminhoPopUp(\'' || oi.obrid || '\',\'cadastro\');\">' || upper(oi.obrdesc) || '</a>' as obrdesc,
			    mun.mundescricao as municipio,
			    edo.estuf as uf,
			    COALESCE(oi.obrvalorprevisto, 0.00) as obrvalorprevisto,
			   	COALESCE(oi.obrpercexec, 0.00) as obrpercexec,
			   	CASE WHEN oi.preid IS NOT NULL THEN
					'<a onmouseover=\"SuperTitleAjax(u+\'' || oi.preid || '\',this)\" onmouseout=\"SuperTitleOff(this);\"><center><img src=\"/imagens/money_g.gif\" style=\"cursor:pointer; width:15px;\"></center></a>'
				END as pagamento,
				pag.pagvalorparcela,
				pag.probanco || ' ' AS probanco,
				pag.proagencia || ' ' AS proagencia,
				pag.nu_conta_corrente || ' ' AS nu_conta_corrente,
			    '<center>'||v.vldstatushomologacao||'</center>' as vldstatushomologacao,
			    '<center>' || CASE WHEN (v.vldobshomologacao IS NULL or v.vldobshomologacao = '') THEN 'Sem observações' ELSE v.vldobshomologacao END || '</center>' as observacao,
			    CASE WHEN v.vldstatus25exec = 'N' THEN '<center>'||v.vldstatus25exec||'&nbsp;<img id=\"25_' || vldid || '\" title=\"' || CASE WHEN v.vldobs25exec IS NULL THEN 'Sem observações' ELSE v.vldobs25exec END || '\" height=\"15\" src=\"../imagens/atencao.png\" /></center>' ELSE '<center>'||v.vldstatus25exec||'</center>' END as vldstatus25exec,
			    CASE WHEN v.vldstatus50exec = 'N' THEN '<center>'||v.vldstatus50exec||'&nbsp;<img title=\"' || CASE WHEN v.vldobs50exec IS NULL THEN 'Sem observações' ELSE v.vldobs50exec END || '\" height=\"15\" src=\"../imagens/atencao.png\" /></center>' ELSE '<center>'||v.vldstatus50exec||'</center>' END as vldstatus50exec,";
}

$sql = "SELECT DISTINCT
			oi.obrid,
		    oi.preid,
		    ent.entnome,
		    {$campos}
		    too.toodescricao,
		    pf.prfdesc,
		    u.usunome AS usunome_hom,
		    TO_CHAR(v.vlddtinclusaosthomo, 'DD-MM-YYYY') AS data_hom,
		    u1.usunome AS usunome_25,
		    TO_CHAR(v.vlddtinclusaost25exec, 'DD-MM-YYYY') AS data_25,
		    u2.usunome AS usunome_50,
		    TO_CHAR(v.vlddtinclusaost50exec, 'DD-MM-YYYY') AS data_50
		FROM
			obras.obrainfraestrutura oi
			
			LEFT JOIN (SELECT
						SUM(p.pagvalorparcela) AS pagvalorparcela,
						probanco,
						proagencia,
						nu_conta_corrente,
						emo.preid
					   FROM
						par.empenhoobra emo
						INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
						INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso  and pro.prostatus = 'A'						
						INNER JOIN par.pagamento p ON p.empid = emo.empid AND 
													 p.pagstatus='A'
					   GROUP BY
					   	emo.preid, pro.probanco, pro.proagencia, pro.nu_conta_corrente) pag ON pag.preid = oi.preid
			
		    left join entidade.entidade ent on ent.entid = oi.entidunidade
		    left join entidade.endereco ed on ed.entid = ent.entid
		    left join entidade.endereco edo on edo.endid = oi.endid
		    left join territorios.municipio mun on mun.muncod = ed.muncod
		    left join obras.validacao v on v.obrid = oi.obrid
		    left join seguranca.usuario u ON u.usucpf = v.usucpf_homo
		    left join seguranca.usuario u1 ON u1.usucpf = v.usucpf_25
		    left join seguranca.usuario u2 ON u2.usucpf = v.usucpf_50
		    left join obras.tipoorigemobra too on too.tooid = oi.tooid
		    left join obras.programafonte pf on pf.prfid = oi.prfid
		    $innerArquivo
		WHERE
			oi.obsstatus = 'A' and oi.orgid = 3 ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )." order by oi.obrid";
//dbg($sql, d);

$cabecalho = array( "Id Obra", "ID Pré-Obra", "Unidade Implantadora", "Nome da Obra", "Município", "UF", "Valor Previsto", "(%) Executado", "Pagamento", "Total Pago", "Banco", "Agência", "Conta", "Homologado", "Observações", "Execução 25%", "Execução 50%", "Fonte", "Programa", "Técnico da Homologação", "Data da Homologação", "Técnico da Execução 25%", "Data da Execução 25%", "Técnico da Execução 50%", "Data da Execução 50%" );
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	
	$db->sql_to_excel($sql, 'relRelatorioValidacoes', $cabecalho, '');
	exit;
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
				SELECT 
					CASE WHEN v.vldstatushomologacao = 'S' THEN COUNT(v.vldstatushomologacao) END AS validadohomolog,
					CASE WHEN v.vldstatushomologacao = 'N' THEN COUNT(v.vldstatushomologacao) END AS naovalidadohomolog,
					CASE WHEN v.vldstatushomologacao IS NULL OR v.vldstatushomologacao not in ('S','N') THEN COUNT(*) END AS naoanalisadohomolog,	
					CASE WHEN v.vldstatus25exec = 'S' THEN COUNT(v.vldstatus25exec) END AS validado25,
					CASE WHEN v.vldstatus25exec = 'N' THEN COUNT(v.vldstatus25exec) END AS naovalidado25,
					CASE WHEN v.vldstatus25exec IS NULL OR v.vldstatus25exec not in ('S','N') THEN COUNT(*) END AS naoanalisado25,
					CASE WHEN v.vldstatus50exec = 'S' THEN COUNT(v.vldstatus50exec) END AS validado50,
					CASE WHEN v.vldstatus50exec = 'N' THEN COUNT(v.vldstatus50exec) END AS naovalidado50,
					CASE WHEN v.vldstatus50exec IS NULL OR v.vldstatus50exec not in ('S','N') THEN COUNT(*) END AS naoanalisado50
				FROM obras.obrainfraestrutura oi        
				LEFT JOIN entidade.entidade ent on ent.entid = oi.entidunidade
		    	LEFT JOIN entidade.endereco ed on ed.entid = ent.entid
		    	left join entidade.endereco edo on edo.endid = oi.endid
				LEFT JOIN obras.validacao v on v.obrid = oi.obrid    
				WHERE oi.obsstatus = 'A' 
				and oi.orgid = 3 ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
				group by v.vldstatus25exec, v.vldstatus50exec, v.vldstatushomologacao
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
		var u='/obras/obras.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&titleFor=';
		function obrIrParaCaminhoPopUp( obrid, tipo, orgid, arqid ){
			switch( tipo ){			
				case "cadastro":
					<?$_SESSION['obras']['orgid'] = 3; ?>
					window.opener.location.href = "obras.php?modulo=principal/cadastro&acao=A&obrid=" + obrid+"&orgid=3";
					window.close(); 
				break;			
			}
		
		}
			
	</script>
</html>
<?
//if( $where ){
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
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$.ajax({
		url: 'obras.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&totaliza=true&'+$('#request_Form').serialize(),
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
		window.open ('obras.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"geraXls");		
	});
	
});
</script>