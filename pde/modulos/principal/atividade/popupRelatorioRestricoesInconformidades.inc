<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

if( !empty( $obrid ) ) {
			$where[] = " ( UPPER(oi.obrnome) ILIKE UPPER('%" . $obrid . "%') OR
			oi.obrid::CHARACTER VARYING ILIKE UPPER('%" . $obrid . "%') ) ";
}

if( !empty( $estuf ) ) array_push($where, "edo.estuf = '{$estuf}'");

if ( $muncod ){
    $muncod  = (array) $muncod;
    $where[] 		  = "mun.muncod IN('" . implode("', '", $muncod) . "')";
}

if ( $esdid ){
    $where[] = "doc.esdid = $esdid";
}

$sql = "SELECT
            oi.obrid,
            oi.preid,
            oi.obrnome,
            mun.mundsc,
            edo.estuf,
            pf.prfdesc,
            too.toodescricao,
            ed.esddsc as situacao,
            oi.obrpercentultvistoria || '%' obrpercentultvistoria,
            TO_CHAR(oi.obrdtultvistoria, 'DD/MM/YYYY') obrdtultvistoria,
            ep.emppercentultvistoriaempresa || '%' emppercentultvistoriaempresa,

            (
                SELECT to_char(se.suedtatualizacao, 'DD/MM/YYYY') from obras2.supervisao_os_obra 		oo
                LEFT JOIN obras2.supervisao_os 			os ON os.sosid = oo.sosid AND os.sosstatus = 'A'
                LEFT JOIN obras2.supervisaoempresa se ON se.sosid = os.sosid AND se.suestatus = 'A'
                WHERE oo.soostatus = 'A' and oo.empid = ep.empid order by oo.sooid desc limit 1
            ) as suedtatualizacao,

            CASE WHEN r.rstitem = 'R' THEN 'Restrição' ELSE 'Inconformidade' END AS item,
            CASE WHEN r.fsrid IS NOT NULL THEN fr.fsrdsc ELSE 'Não Informada' END AS fase,
            tr.tprdsc,
            TO_CHAR(r.rstdtinclusao, 'DD/MM/YYYY') AS rstdtinclusao,
            r.rstdsc,
            r.rstdscprovidencia,
            TO_CHAR(r.rstdtprevisaoregularizacao, 'DD/MM/YYYY') AS rstdtprevisaoregularizacao,
            usu.usunome AS criadopor,
            CASE WHEN r.rstsituacao = TRUE THEN TO_CHAR(r.rstdtsuperacao, 'DD/MM/YYYY') ELSE 'Não' END AS rstdtsuperacao,
            sup.usunome AS ususuperacao
            

		FROM
            obras2.restricao r

        INNER JOIN obras2.obras oi ON oi.obrid = r.obrid
        INNER JOIN obras2.empreendimento ep ON ep.empid = oi.empid

        -- Dados da restrição
        INNER JOIN obras2.tiporestricao tr ON tr.tprid = r.tprid AND tr.tprstatus = 'A'
        LEFT JOIN obras2.faserestricao fr ON fr.fsrid = r.fsrid AND fr.fsrstatus = 'A'
        LEFT JOIN seguranca.usuario usu ON usu.usucpf = r.usucpf
        LEFT JOIN seguranca.usuario sup ON sup.usucpf = r.usucpfsuperacao

		LEFT JOIN workflow.documento doc ON doc.docid = oi.docid 
        LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid

        LEFT JOIN entidade.entidade ent ON ent.entid = ep.entidunidade
        LEFT JOIN obras2.tipoorigemobra too ON too.tooid = oi.tooid
        LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
        LEFT JOIN obras.preobra pre ON pre.preid = oi.preid

        -- Endereço
        LEFT JOIN entidade.endereco edo on edo.endid = oi.endid
        LEFT JOIN public.municipio mun on mun.muncod = edo.muncod

		    $inner
		WHERE
         --oi.preid IS NOT NULL and
			r.rststatus = 'A' AND oi.obrstatus = 'A' AND oi.obridpai IS NULL AND ep.empstatus = 'A' ".( !empty($where) ? ' AND ' . implode(' AND ', $where) : '' )." order by r.rsqid, oi.obrid";

//ver($sql);

$cabecalho = array("ID da obra",
"ID pré-obra", "Nome da obra", "Município", "UF", "Programa", "Fonte", "Situação da obra", "Supervisão Unidade", "Última supervisão unidade", "Supervisão EMPRESA", "Última supervisão empresa", "Item", "Fase","Tipo", "Data da Inclusão", "Descrição", "Providência", "Previsão da Providência", "Criado Por", "Superação", "Superado Por");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	
	$db->sql_to_excel($sql, 'relRelatorioIndicadores', $cabecalho, '');
	exit;
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
<div id="dv_sql"></div>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$(function(){
	
	$('#btnGeraExcel').click(function(){	
		window.open ('obras2.php?modulo=relatorio/popupRelatorioValidacoes&acao=A&pesquisa=xls&'+$('#request_Form').serialize(),
					"geraXls");		
	});
	
});
</script>