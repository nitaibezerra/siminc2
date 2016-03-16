<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();

extract($_REQUEST);

if( !empty( $estuf ) ) array_push($where, "edr.estuf = '{$estuf}'");
if ( $muncod ){
    $muncod  = (array) $muncod;
    $where[] = "m.muncod IN('" . implode("', '", $muncod) . "')";
}

if(!empty($dthomol_de) ){
    $dthomol_de = ajusta_data($dthomol_de);
    $where[] = "
      (SELECT
        MAX(hd.htddata) AS htddata
      FROM
          workflow.historicodocumento hd
      WHERE
        hd.docid = se.docid AND
        hd.aedid = 1726 GROUP BY hd.docid) >= '{$dthomol_de}' ";
}

if(!empty($dthomol_ate)){
    $dthomol_ate = ajusta_data($dthomol_ate);
    $where[] = "
      (SELECT
        MAX(hd.htddata) AS htddata
      FROM
          workflow.historicodocumento hd
      WHERE
        hd.docid = se.docid AND
        hd.aedid = 1726 GROUP BY hd.docid) <= '{$dthomol_ate}' ";
}


if ($esdid) {
    $w = '';

    if(count($esdid) > 0){
        foreach($esdid as $k => $v){
            if ($v == "nao_iniciado") {
                $w = "ed1.esdid is null";
                unset($esdid[$k]);
            }
        }
        if(count($esdid) > 0){
            if($w)
                $w .= " OR ed1.esdid IN ( '" . implode('\',\'',$esdid) . "' ) ";
            else
                $w = "ed1.esdid IN ( '" . implode('\',\'',$esdid) . "' ) ";
        }
        $where[] = $w;
    }
}

if ( $sosnum ){
    $where[] = "os.sosnum = '$sosnum'";
}

if ( $entid ){
    $where[] = "ent3.entid = '$entid'";
}

if ($empbuscatexto) {
    $where[] = " ( UPPER(o.obrnome) ILIKE UPPER('%" . $empbuscatexto . "%') OR
						   o.obrid::CHARACTER VARYING ILIKE UPPER('%" . $empbuscatexto . "%') ) ";
}

if ($empid) {
    $where[] = " ( e.empid::CHARACTER VARYING ILIKE UPPER('%" . $empid . "%') ) ";
}

if($h_atrasada == "sim"){
    $where[] = " true = ((ed1.esdid IN (734, 756, 757) AND (os.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid)::date)) AND os.sosdttermino < NOW())";
}
if($h_atrasada == "nao"){
    $where[] = " true != ((ed1.esdid IN (734, 756, 757) AND (os.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid)::date)) AND os.sosdttermino < NOW())";
}

$sql = "
    SELECT
      DISTINCT
      o.obrid,
      e.empid,
      os.sosnum,
      ent3.entnome,
      m.estuf,
      mes.mesdsc,
      mic.micdsc,
      m.mundescricao,
      TO_CHAR(se.suedtsupervisao, 'dd/mm/yyyy'),
      to_char(se.suedtatualizacao, 'DD/MM/YYYY') || ' ( ' || DATE_PART('days', NOW() - se.suedtatualizacao) || ' dia(s) )' as suedtatualizacao,
      to_char(os.sosdttermino, 'DD/MM/YYYY') AS data_termino,
      COALESCE(ed1.esddsc, 'Não Iniciada')   AS situacao_supervisao,
      --ed.esddsc                              AS situacao_os,
      (SELECT
        TO_CHAR(MAX(hd.htddata), 'DD/MM/YYYY') AS htddata
      FROM
          workflow.historicodocumento hd
      WHERE
        hd.docid = se.docid AND
        hd.aedid = 1726
      GROUP BY
        hd.docid) AS data_homologacao,

      CASE WHEN ((ed1.esdid IN (734, 756, 757) AND (os.sosdttermino::date < (SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid)::date)) AND os.sosdttermino < NOW()) THEN
           DATE_PART('day',(SELECT MAX(htddata) FROM workflow.historicodocumento WHERE aedid = 1726 AND docid = d1.docid GROUP BY docid) - os.sosdttermino)
      END as diashomolatraso,

      pg.pagnrnotafiscal,
      pesd.esddsc,
      (SELECT SUM(sg.sgevalorunitario)  FROM obras2.pagamento p
        JOIN obras2.pagamento_supervisao_empresa ps ON ps.pagid = p.pagid AND ps.psestatus = 'A'
        JOIN obras2.supervisao_grupo_empresa sg ON sg.sgeid = p.sgeid
      WHERE p.pagid = pg.pagid) as valor

    FROM
        obras2.empreendimento e

        JOIN obras2.supervisao_os_obra oo
          ON oo.empid = e.empid AND oo.soostatus = 'A'
        JOIN obras2.supervisao_os os
          ON os.sosid = oo.sosid AND os.sosstatus = 'A'
        JOIN workflow.documento d
          ON d.docid = os.docid
        JOIN workflow.estadodocumento ed
          ON ed.esdid = d.esdid
        LEFT JOIN obras2.tipologiaobra tpo
          ON tpo.tpoid = e.tpoid
        LEFT JOIN entidade.endereco edr
          ON edr.endid = e.endid
        LEFT JOIN territorios.municipio m
          ON m.muncod = edr.muncod
        LEFT JOIN territorios.mesoregiao mes
          ON mes.mescod = m.mescod
        LEFT JOIN territorios.microregiao mic
          ON mic.miccod = m.miccod
        LEFT JOIN obras2.supervisao_grupo_empresa sge1
          ON sge1.sgeid = os.sgeid
        LEFT JOIN entidade.entidade ent3
          ON ent3.entid = sge1.entid
        LEFT JOIN obras2.supervisaoempresa se
          ON se.sosid = os.sosid AND se.suestatus = 'A' AND se.empid = e.empid
        LEFT JOIN workflow.documento d1
          ON d1.docid = se.docid
        LEFT JOIN workflow.estadodocumento ed1
          ON ed1.esdid = d1.esdid
        LEFT JOIN obras2.supervisao su
          ON su.sueid = se.sueid AND su.supstatus = 'A'
        LEFT JOIN obras2.obras o
          ON o.empid = e.empid AND o.obrstatus = 'A' AND obridpai IS NULL
        LEFT JOIN obras2.pagamento_supervisao_empresa pe
          ON pe.sueid = se.sueid AND pe.psestatus = 'A'
        LEFT JOIN obras2.pagamento pg
          ON pg.pagid = pe.pagid
        LEFT JOIN workflow.documento dpg
          ON dpg.docid = pg.docid
        LEFT JOIN workflow.estadodocumento pesd
          ON pesd.esdid = dpg.esdid

    WHERE
      e.empstatus = 'A'
      ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
      AND e.orgid IN (3)
      ORDER BY 10, 8 DESC

 ";

$cabecalho = array("ID Obra", "ID Emp.", "Nº OS",
"Empresa", "UF", "Mesorregião", "Microregião", "Município", "Data da Supervisão", "Data da Última Atualização da Supervisão da Empresa", "Data Fim da Execução", "Situação da Supervisão", "Data Homologação do Laudo", "Qtd. dias Atraso Homologação", "Nº da Nota Fiscal", "Situação do Pagamento", "Valor da Nota");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'relRelatorioSupervisaoEmpresa', $cabecalho);
}


?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
	<script type="text/javascript" src="/includes/remedial.js"></script>
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
		var u='/obras2/obras2.php?modulo=relatorio/popupSupervisaoEmpresa&acao=A&titleFor=';
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
		window.open ('obras2.php?modulo=relatorio/popupRelatorioSupervisaoEmpresa&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"RelatorioSupervisaoEmpresa");
	});
	
});
</script>