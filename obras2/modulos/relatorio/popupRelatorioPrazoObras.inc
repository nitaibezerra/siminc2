<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

//ver($_REQUEST);
/* if( $orgid ){
	array_push($where, " oi.orgid in (" . implode( ',', $orgid ) . ") ");
} */
// programa
if( $prfid[0] && $prfid_campo_flag ){
	if ( !$prfid_campo_excludente ){
		array_push($where, " ep.prfid  IN (" . implode( ',', $prfid ) . ") ");	
	}else{
		array_push($where, " ( ep.prfid  NOT IN (" . implode( ',', $prfid ) . ") OR ep.prfid is null ) ");
	}
	
}
// Fonte
if( $tooid[0] && $tooid_campo_flag ){
	if ( !$tooid_campo_excludente ){
		array_push($where, " o.tooid  IN (" . implode( ',', $tooid ) . ") ");	
	}else{
		array_push($where, " ( o.tooid  NOT IN (" . implode( ',', $tooid ) . ") OR o.tooid IS NULL ) ");
	}
	
}



if( !empty( $esdid ) ) {array_push($where, "doc.esdid = {$esdid}");}
if( !empty( $tpoid ) ) {array_push($where, "tpo.tpoid= {$tpoid}");}
if( !empty( $stoid ) ) {array_push($where, "o.stoid = {$stoid}");}
if( !empty( $estuf ) ) {array_push($where, "edo.estuf = '{$estuf}'");}
if( !empty( $obrid ) ) {array_push($where, "o.obrid = $obrid");}
if( !empty( $paralisacao ) ) {
    
    
    if( $paralisacao == "Sim" ) {
    
    array_push($where, "(SELECT COUNT(*)
                            FROM obras2.obras o1
                            JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                            WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid AND s.staid = 2 ) > 0");
    
    }else{
       array_push($where, "(SELECT COUNT(*)
                            FROM obras2.obras o1
                            JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                            WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid AND s.staid = 2 ) <= 0"); 
    }
}


 $sql = "SELECT DISTINCT
                    o.obrid,
                    o.obrnome,
                    pf.prfdesc as programa,
                    too.toodescricao as fonte,
                    ed.esddsc as situacao,
                    mun.estuf,
                    mun.mundescricao,
                    c.crtvalorexecucao as valorcontrato,
                    c.crtdtassinatura as datacontrato,
                    o.obrvalorprevisto as valorpactuado,
                    (SELECT TO_CHAR(fl.flchomlicdtprev, 'DD/MM/YYYY') FROM obras2.obralicitacao ol
                        JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND fl.flcstatus = 'A' AND fl.tflid = 9
                        WHERE ol.obrid = o.obrid AND ol.oblstatus = 'A' LIMIT 1) as dtlicitacao,
                    (SELECT TO_CHAR(s.supdata, 'DD/MM/YYY')
                        FROM obras2.obras o1
                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                        ORDER BY o1.obrid, s.supdata ASC
                        OFFSET 0
                        LIMIT 1) as dtprimeiravistoria,
                    (SELECT TO_CHAR(s.supdata, 'DD/MM/YYY')
                        FROM obras2.obras o1
                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                        ORDER BY o1.obrid, s.supdata DESC
                        OFFSET 0
                        LIMIT 1) as dtultimavistoria,
                   CASE WHEN (SELECT COUNT(*)
                            FROM obras2.obras o1
                            JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                            WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid AND s.staid = 2 ) > 0 THEN 'SIM' ELSE 'NÃO' END as paralisacao
                FROM obras2.obras o
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
                LEFT JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN workflow.documento doc ON doc.docid = o.docid
                LEFT JOIN workflow.estadodocumento                ed ON ed.esdid = doc.esdid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
                LEFT JOIN entidade.endereco edo on edo.endid = o.endid
                LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
                

                
                ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )." order by mun.estuf";



$cabecalho = array( "Id Obra", "Nome da Obra", "Programa", "Fonte", "Situação", "Estado", "Município","Valor do Contrato", "Data do contrato", "Valor pactuado", "Data da licitação", "Data da primeira vistoria", "Data da ultima vistoria", "Paralisação");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'relRelatorioValidacoes', $cabecalho);
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
		window.open ('obras2.php?modulo=relatorio/popupRelatorioPrazoObras&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"geraXls");		
	});
	
});
</script>