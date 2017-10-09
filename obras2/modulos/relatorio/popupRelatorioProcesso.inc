<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

if( !empty( $obrid ) ) {
    $obridTmp = removeAcentos($obrid);
    $where[] = " ( UPPER(public.removeacento(obr.obrnome)) ILIKE ('%" . $obridTmp . "%') OR
    obr.obrid::CHARACTER VARYING ILIKE UPPER('%" . $obrid . "%') ) ";
}

if( !empty( $estuf ) ) array_push($where, "edo.estuf = '{$estuf}'");

if ( $muncod ){
    $muncod  = (array) $muncod;
    $where[] 		  = "mun.muncod IN('" . implode("', '", $muncod) . "')";
}

if ( $esdid ){
    $where[] = "doc.esdid = $esdid";
}

if ( $tprid ){
    $where[] = "r.tprid = $tprid";
}

if ( $prfid && count($prfid) > 0 && $prfid[0] !== ''){
    $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
}
if ( $tooid && count($tooid) > 0 && $tooid[0] !== ''){
    $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
}

$sql = "
    SELECT

            obr.obrid,
            obr.preid,
            obr.obrnome,
            obr.n_termo,

            CASE WHEN obr.preid IS NULL THEN
                obr.obrnumprocessoconv || '/' || obr.obranoconvenio
            ELSE
                obr.pronumeroprocesso
            END as pronumeroprocesso,

            obr.prazo,
            obr.mundescricao,
            obr.estuf,
            obr.prfdesc,
            obr.toodescricao,
            obr.situacao

     FROM (
         SELECT DISTINCT
            obr.obrnumprocessoconv,
            obr.obranoconvenio,
            obr.numconvenio,
            obr.obrid,
            obr.preid,
            obr.obrnome,
            ''||lpad(ter.terid||'/'||to_char(terdatainclusao,'YYYY'),10,'0') as n_termo,
            to_char(pro.pronumeroprocesso::bigint, 'FM00000\\\".\\\"000000\\\"/\\\"0000\\\"-\\\"00') as pronumeroprocesso,
            CASE WHEN popdataprazoaprovado IS NOT NULL
            THEN to_char(pop.popdataprazoaprovado, 'DD/MM/YYYY')
            ELSE to_char(MIN(pag.pagdatapagamentosiafi) + 720,'DD/MM/YYYY')
            END as prazo,
            mun.mundescricao,
            edo.estuf,
            pf.prfdesc,
            too.toodescricao,
            ed.esddsc as situacao
            FROM
            obras2.obras obr
            INNER JOIN obras.preobra                         pre ON pre.preid = obr.preid
            INNER JOIN par.termoobraspaccomposicao      toc ON toc.preid = pre.preid
            INNER JOIN par.termocompromissopac              ter ON ter.terid = toc.terid AND ter.terstatus = 'A'
            INNER JOIN par.processoobra                  pro ON pro.proid = ter.proid and pro.prostatus = 'A'
            LEFT  JOIN par.pagamentoobra                                pgo ON pgo.preid = pre.preid
            LEFT  JOIN par.pagamento                         pag ON pag.pagid = pgo.pagid AND pag.pagstatus = 'A'
            LEFT  JOIN obras.preobraprorrogacao   pop ON pop.preid = pre.preid AND popstatus = 'A' AND popvalidacao = 't'
            LEFT JOIN obras2.empreendimento ep  ON ep.empid = obr.empid
            LEFT  JOIN obras2.tipoorigemobra too ON too.tooid = obr.tooid
            LEFT  JOIN obras2.programafonte  pf  ON pf.prfid = ep.prfid
            LEFT  JOIN entidade.endereco     edo ON edo.endid = obr.endid
            LEFT  JOIN territorios.municipio mun ON mun.muncod = edo.muncod
            LEFT JOIN workflow.documento     doc ON doc.docid = obr.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
            WHERE
              obr.obridpai IS NULL AND obr.obrstatus = 'A' AND too.tooid = 1 ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
            GROUP BY
            ter.terid,
            ter.terdatainclusao,
            pro.pronumeroprocesso,
            pop.popdataprazoaprovado,
            obr.obrid,
            obr.preid,
            obr.obrnome,
            mun.mundescricao,
            edo.estuf,
            pf.prfdesc,
            too.toodescricao,
            obr.obrnumprocessoconv,
            obr.obranoconvenio,
            obr.numconvenio,
            ed.esddsc

            UNION

            SELECT DISTINCT
            obr.obrnumprocessoconv,
            obr.obranoconvenio,
            obr.numconvenio,
            obr.obrid,
            obr.preid,
            obr.obrnome,
            ''||lpad(dop.dopid||'/'||to_char(dopdatainclusao,'YYYY'),10,'0') as n_termo,
            to_char(pro.pronumeroprocesso::bigint, 'FM00000\\\".\\\"000000\\\"/\\\"0000\\\"-\\\"00') as pronumeroprocesso,
            dop.dopdatafimvigencia,
            mun.mundescricao,
            edo.estuf,
            pf.prfdesc,
            too.toodescricao,
            ed.esddsc as situacao
            FROM
            obras2.obras obr
            LEFT JOIN obras.preobra                         pre ON pre.preid = obr.preid
            LEFT JOIN par.termocomposicao         toc ON toc.preid = pre.preid
            LEFT JOIN par.documentopar                               dop ON dop.dopid = toc.dopid AND dop.dopstatus = 'I' AND dop.dopstatus = 'A'
            LEFT JOIN par.processoobraspar         pro ON pro.proid = dop.proid and pro.prostatus = 'A'
            LEFT JOIN obras2.empreendimento ep  ON ep.empid = obr.empid
            LEFT  JOIN obras2.tipoorigemobra too ON too.tooid = obr.tooid
            LEFT  JOIN obras2.programafonte  pf  ON pf.prfid = ep.prfid
            LEFT  JOIN entidade.endereco     edo ON edo.endid = obr.endid
            LEFT  JOIN territorios.municipio mun ON mun.muncod = edo.muncod
            LEFT JOIN workflow.documento     doc ON doc.docid = obr.docid
            LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
            WHERE
            obr.obridpai IS NULL AND obr.obrstatus = 'A' AND too.tooid != 1

                 ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
         ) as obr

        "
        ;


$cabecalho = array("ID da obra",
"ID pré-obra", "Nome da obra", "Nº do Termo", "Nº do Processo", "Validade do Termo", "Município", "UF", "Programa", "Fonte", "Situação da obra");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'relatorio_processos', $cabecalho);
}


?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
	<head>
		<title> <?php echo NOME_SISTEMA; ?> </title>
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
					window.opener.location.href = "obras2.php?modulo=principal/cadastro&acao=A&obrid=" + obrid+"&orgid=3";
					window.close(); 
				break;			
			}
		
		}
			
	</script>
</html>
<?php
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
<div id="dv_sql"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$('#btnGeraExcel').click(function(){	
		window.open ('obras2.php?modulo=relatorio/popupRelatorioRestricoesInconformidades&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"RelatorioRestricoesInconformidades");
	});
	
});
</script>