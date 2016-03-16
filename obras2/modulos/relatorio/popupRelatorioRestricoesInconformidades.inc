<?php
ob_clean();
set_time_limit(30000);
ini_set("memory_limit", "2048M");
$where = array();
extract($_REQUEST);

/*
if( !empty( $obrid ) ) {
    $obridTmp = removeAcentos($obrid);
    $where[] = " ( UPPER(public.removeacento(oi.obrnome)) ILIKE ('%" . $obridTmp . "%') OR
    oi.obrid::CHARACTER VARYING ILIKE UPPER('%" . $obrid . "%') ) ";
}

if( !empty( $estuf ) ){ 
    array_push($where, "edo.estuf = '{$estuf}'");
}
    

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
*/

if (!empty($obrid)) {
    $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
    $where[] = " ( ( UPPER(public.removeacento(oi.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                oi.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
}

if (!empty($tpoid)) {
    $where[] = " oi.tpoid = $tpoid ";
}

if (!empty($item_restrict)) {
    if ($item_restrict != 'T') {
        $where[] = " r.rstitem = '$item_restrict' ";
    } else {
        $where[] = " r.rstitem IN ('R','I')  ";
    }
}

if (!empty($estuf)) {
    if (is_array($estuf)) {
        foreach ($estuf as $k => $v) {
            $estuf[$k] = "'{$v}'";
        }
    } else {
        $estuf = array("'{$estuf}'");
    }
    foreach ($estuf as $k => $v) {
        if (trim($v) == '' || $v == "''" || empty($v)) {
            unset($estuf[$k]);
        }
    }
    if (!empty($estuf)) {
        $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
    }
}

if (!empty($muncod)) {
    if (is_array($muncod)) {
        foreach ($muncod as $k => $v) {
            $muncod[$k] = "'{$v}'";
        }
    } else {
        $muncod = array("'{$muncod}'");
    }
    foreach ($muncod as $k => $v) {
        if (trim($v) == '' || $v == "''" || empty($v)) {
            unset($muncod[$k]);
        }
    }
    if (!empty($muncod)) {
        $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
    }
}

if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
    $where[] = " ed.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
}

if (!empty($esdid_ri) && is_array($esdid_ri) && $esdid_ri[0] != '') {
    $where[] = " esd_ri.esdid IN ( " . implode(",", $esdid_ri) . " ) ";
}

if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
    $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
}

if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
    $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
}

if(!empty($rstdtinclusao_de) && !empty($rstdtinclusao_ate)){
    $de  = explode('/', $rstdtinclusao_de);
    $ate = explode('/', $rstdtinclusao_ate);
    $rstdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
    $rstdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
    $where[] = " r.rstdtinclusao BETWEEN '{$rstdtinclusao_de}' AND '{$rstdtinclusao_ate}' ";
}elseif(!empty($rstdtinclusao_de) && empty($rstdtinclusao_ate)){
    $de  = explode('/', $rstdtinclusao_de);
    $rstdtinclusao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
    $where[] = " r.rstdtinclusao = '{$rstdtinclusao_de}' ";
}elseif(empty($rstdtinclusao_de) && !empty($rstdtinclusao_ate)){
    $ate = explode('/', $rstdtinclusao_ate);
    $rstdtinclusao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
    $where[] = " r.rstdtinclusao = '{$rstdtinclusao_ate}' ";
}

if(!empty($rstdtsuperacao_de) && !empty($rstdtsuperacao_ate)){
    $de  = explode('/', $rstdtsuperacao_de);
    $ate = explode('/', $rstdtsuperacao_ate);
    $rstdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
    $rstdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
    $where[] = " r.rstdtsuperacao BETWEEN '{$rstdtsuperacao_de}' AND '{$rstdtsuperacao_ate}' ";
}elseif(!empty($rstdtsuperacao_de) && empty($rstdtsuperacao_ate)){
    $de  = explode('/', $rstdtsuperacao_de);
    $rstdtsuperacao_de  = $de[2]. '-'.$de[1]. '-'.$de[0];
    $where[] = " r.rstdtsuperacao = '{$rstdtsuperacao_de}' ";
}elseif(empty($rstdtsuperacao_de) && !empty($rstdtsuperacao_ate)){
    $ate = explode('/', $rstdtsuperacao_ate);
    $rstdtsuperacao_ate = $ate[2].'-'.$ate[1].'-'.$ate[0];
    $where[] = " r.rstdtsuperacao = '{$rstdtsuperacao_ate}' ";
}

if ( !empty($rstflressalva)){
    if($rstflressalva == 'T'){
        $where[] = " (r.rstflressalva IN('S','N') OR r.rstflressalva IS NULL) ";
    }else{
        $where[] = " r.rstflressalva = '".$rstflressalva."' ";
    }
}

$sql = "SELECT
            oi.obrid as obrid,
            oi.preid as preid,
            oi.obrnome as obrnome,
            mun.mundescricao as mundescricao,
            edo.estuf as estuf,
            pf.prfdesc as prfdesc,
            too.toodescricao as toodescricao,
            ed.esddsc as situacao,
            oi.obrpercentultvistoria || '%' as obrpercentultvistoria,
            TO_CHAR(oi.obrdtultvistoria, 'DD/MM/YYYY') as obrdtultvistoria,
            ep.emppercentultvistoriaempresa || '%' as emppercentultvistoriaempresa,

            (
                SELECT to_char(se.suedtatualizacao, 'DD/MM/YYYY') from obras2.supervisao_os_obra 		oo
                LEFT JOIN obras2.supervisao_os 			os ON os.sosid = oo.sosid AND os.sosstatus = 'A'
                LEFT JOIN obras2.supervisaoempresa se ON se.sosid = os.sosid AND se.suestatus = 'A'
                WHERE oo.soostatus = 'A' and oo.empid = ep.empid order by oo.sooid desc limit 1
            ) as suedtatualizacao,
            CASE WHEN r.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
            CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
            tr.tprdsc as tprdsc,
            TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
            r.rstdsc as rstdsc,
            r.rstdscprovidencia as rstdscprovidencia,
            TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
            usu.usunome AS criadopor,
            CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
            sup.usunome AS ususuperacao
            

		FROM
            obras2.restricao r

        INNER JOIN obras2.obras          oi ON oi.obrid = r.obrid
        INNER JOIN obras2.empreendimento ep ON ep.empid = oi.empid

        -- Dados da restrição
        INNER JOIN obras2.tiporestricao     tr ON tr.tprid   = r.tprid AND tr.tprstatus = 'A'
        LEFT JOIN obras2.faserestricao      fr ON fr.fsrid   = r.fsrid AND fr.fsrstatus = 'A'
        LEFT JOIN seguranca.usuario        usu ON usu.usucpf = r.usucpf
        LEFT JOIN seguranca.usuario        sup ON sup.usucpf = r.usucpfsuperacao
        
	LEFT JOIN workflow.documento       doc ON doc.docid  = oi.docid 
        LEFT JOIN workflow.estadodocumento  ed ON ed.esdid   = doc.esdid
        
        LEFT JOIN workflow.documento        doc_ri ON doc_ri.docid  = r.docid
        LEFT JOIN workflow.estadodocumento  esd_ri ON esd_ri.esdid  = doc_ri.esdid

        LEFT JOIN entidade.entidade        ent ON ent.entid = ep.entidunidade
        LEFT JOIN obras2.tipoorigemobra    too ON too.tooid = oi.tooid
        LEFT JOIN obras2.programafonte      pf ON pf.prfid = ep.prfid
        LEFT JOIN obras.preobra            pre ON pre.preid = oi.preid

        -- Endereço
        LEFT JOIN entidade.endereco        edo ON edo.endid = oi.endid
        LEFT JOIN territorios.municipio    mun ON mun.muncod = edo.muncod

		    $inner
		WHERE
         --oi.preid IS NOT NULL and
	 r.rststatus = 'A' 
         AND oi.obrstatus = 'A' 
         AND oi.obridpai IS NULL 
         
         AND ep.empstatus = 'A' ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )."
        
        order by r.rsqid, oi.obrid
        ";

$cabecalho = array("ID da obra", "ID pré-obra", "Nome da obra", "Município", "UF", "Programa", "Fonte", "Situação da obra", 
                   "Supervisão Unidade", "Última supervisão unidade", "Supervisão EMPRESA", "Última supervisão empresa", "Item", 
                   "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", 
                   "Criado Por", "Superação", "Superado Por");

// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
   
        $dados = $db->carregar($sql, null, 3600);
        $dados = (empty($dados) ? array() : $dados);
        
        foreach ($dados as $k => $dd) {
            foreach ($dd as $y => $d) {
                if(is_string($d) && strlen($d) >= 2000){
                    //Limitado o número de caracteres, pois estoura o limite do Excel.
                    $dados[$k][$y] = substr(strip_tags($d),0,2000)."...";
                }else{
                    $dados[$k][$y] = strip_tags($d);
                    
                }
            }
        }
        
    ini_set("memory_limit", "7000M");
	$db->sql_to_excel($dados, 'relatorio_restricao_Inconformidade', $cabecalho);
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