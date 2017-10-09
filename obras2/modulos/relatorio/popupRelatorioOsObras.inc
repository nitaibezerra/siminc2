<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
$where = array();
extract($_REQUEST);

$param = $_POST;
$where = array();
$join = array();

$param['orgid'] = (isset($param['orgid']) ? $param['orgid'] : $_SESSION['obras2']['orgid']);

if ($param['entid']) {
    $where[] = "ent3.entid = {$param['entid']} ";
}

if ($param['orgid']) {
    $param['orgid'] = (array) $param['orgid'];
    $where[] = "e.orgid IN(" . implode(", ", $param['orgid']) . ")";
}

if ($param['empbuscatexto']) {
    $empbuscaTextoTmp = removeAcentos($param['empbuscatexto']);
    $where[] = " ( UPPER(public.removeacento(e.empdsc)) ILIKE UPPER('%" . $empbuscaTextoTmp . "%') OR
						   e.empid::CHARACTER VARYING ILIKE UPPER('%" . $param['empbuscatexto'] . "%') ) OR
                        ( UPPER(public.removeacento(o.obrnome)) ILIKE UPPER('%" . $empbuscaTextoTmp . "%') OR
						   o.obrid::CHARACTER VARYING ILIKE UPPER('%" . $param['empbuscatexto'] . "%') )";
}

if ($param['sosnum']) {
    $where[] = " os.sosnum::text like('%" . $param['sosnum'] . "%')";
}
if ($param['tobid']) {
    $param['tobid'] = (array) $param['tobid'];
    $where[] = "e.tobid IN(" . implode(", ", $param['tobid']) . ")";
}

if ($param['cloid']) {
    $param['cloid'] = (array) $param['cloid'];
    $where[] = "e.cloid IN(" . implode(", ", $param['cloid']) . ")";
}

if ($param['tpoid']) {
    $param['tpoid'] = (array) $param['tpoid'];
    $where[] = "e.tpoid IN(" . implode(", ", $param['tpoid']) . ")";
}

if ($param['prfid']) {
    $param['prfid'] = (array) $param['prfid'];
    $where[] = "e.prfid IN(" . implode(", ", $param['prfid']) . ")";
}

if ($param['moeid']) {
    $param['moeid'] = (array) $param['moeid'];
    $where[] = "e.moeid IN(" . implode(", ", $param['moeid']) . ")";
}

if ($param['obrestuf']) {
    $param['obrestuf'] = (array) $param['obrestuf'];
    $where[] = "edr.estuf IN('" . implode("', '", $param['obrestuf']) . "')";
//    $join['endereco'] = "JOIN entidade.endereco ede ON ede.endid = e.endid AND ede.endstatus = 'A'";
}

if ($param['obrmuncod']) {
    $param['obrmuncod'] = (array) $param['obrmuncod'];
    $where[] = "mun.muncod IN('" . implode("', '", $param['obrmuncod']) . "')";
}

if ($param['obrmescod']) {
    $param['obrmescod'] = (array) $param['obrmescod'];
    $where[] = "mes.mescod IN('" . implode("', '", $param['obrmescod']) . "')";
}

if ($param['obrmiccod']) {
    $param['obrmiccod'] = (array) $param['obrmiccod'];
    $where[] = "mic.miccod IN('" . implode("', '", $param['obrmiccod']) . "')";
}

if ($param['empesfera']) {
    $param['empesfera'] = (array) $param['empesfera'];
    $where[] = "e.empesfera IN('" . implode("', '", $param['empesfera']) . "')";
}

if ($param['empvalorprevisto_menor']) {
    $where[] = "e.empvalorprevisto < " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_menor']);
}

if ($param['empvalorprevisto_maior']) {
    $where[] = "e.empvalorprevisto > " . str_replace(array(".", ","), array("", "."), $param['empvalorprevisto_maior']);
}

if ($param['esdidsituaco']) {
    $where[] = "esd.esdid = " . $param['esdidsituaco'];
}

if ($param['esdidsupervisao']) {
    $w = '';
    if(count($param['esdidsupervisao']) > 0){
        foreach($param['esdidsupervisao'] as $k => $v){
            if ($v == "nao_iniciado") {
                $w = "ed1.esdid is null";
                unset($param['esdidsupervisao'][$k]);
            }
        }
        if(count($param['esdidsupervisao']) > 0){
            if($w)
                $w .= " OR ed1.esdid IN ( '" . implode('\',\'',$param['esdidsupervisao']) . "' ) ";
            else
                $w = "ed1.esdid IN ( '" . implode('\',\'',$param['esdidsupervisao']) . "' ) ";
        }
        $where[] = $w;
    }
}

if ($param['percentualinicial'] > 0) {
    $where[] = "obrpercentultvistoria >= " . $param['percentualinicial'];
}

if ($param['percentualfinal'] != '') {
    if ($param['percentualfinal'] < 100) {
        $where[] = "obrpercentultvistoria <= " . $param['percentualfinal'];
    }
}

//if ($param['obraestuf']) {
//    $param['obraestuf'] = (array) $param['obraestuf'];
//    $where[] = "tes.estuf IN('" . implode("', '", $param['obraestuf']) . "')";
//    $join['endereco'] = "JOIN territorios.estado tes ON tes.estuf = edr.estuf ";
//}
//
//if ($param['obramescod']) {
//    $where[] = " mes.mescod = '" . $param['obramescod'] . "'";
//}
//
//if ($param['obramiccod']) {
//    $where[] = " mic.miccod = '" . $param['obramiccod'] . "'";
//}

if ($param['doco']) {
    $where[] = " doco.esdid = '" . $param['doco'] . "'";
}

if ($param['obramuncod']) {
    $where[] = " m.muncod = '" . $param['obramuncod'] . "'";
}

//obras2.supervisaoempresa pegar o campo suedtsupervisao e ver se a data de hj é maior
if ($param['atrasado'] == "sim") {
    $where[] = "se.suedtsupervisao > now()";
}
if ($param['atrasado'] == "nao") {
    $where[] = "se.suedtsupervisao <= now()";
}

//obras2.supervisaoempresa pegar o campo sueendcorreto, para fazer a verificação de endereço correto da obra/supervisão
if ($param['endereco_obr'] == "sim") {
    $where[] = "se.sueendcorreto = 's'";
}
if ($param['endereco_obr'] == "nao") {
    $where[] = "se.sueendcorreto = 'n'";
}
if ($param['endereco_obr'] == "todos") {
    $where[] = "(se.sueendcorreto = 'n' OR se.sueendcorreto = 's' OR se.sueendcorreto is NULL)";
}
if ($param['pos_obr'] == "sim") {
    $where[] = "soo.soostatus = 'A' AND
                os.sosstatus = 'A'";
} else if($param['pos_obr'] == "nao"){
    $where[] = "(soo.sooid IS NULL or  e.empid IN (SELECT sue.empid FROM obras2.supervisaoempresa sue
                                            JOIN workflow.documento d ON d.docid = sue.docid
                                            WHERE d.esdid = 1188 AND sue.empid NOT IN (SELECT empid FROM obras2.supervisaoempresa s
                                                            JOIN workflow.documento d1 ON d1.docid = s.docid
                                                            WHERE d1.esdid != 1188)) )";
}

$sql = "SELECT
  o.obrid,
  o.obrnome,
  esdo.esddsc as esdo,
  os.sosnum,
  TO_CHAR(os.sosdtinicio, 'dd/mm/yyyy') AS sosdtinicio,
  TO_CHAR(os.sosdttermino, 'dd/mm/yyyy') AS sosdttermino,
  os.sosvalortotal,
  ent3.entnome,
  esd.esddsc as esddsc,
  (CASE
   WHEN os.sosemergencial=true THEN 'Sim'
   WHEN os.sosemergencial=false THEN 'Não'
   END) as sosemergencial,
   CASE WHEN soo.sooid IS NOT NULL THEN
    coalesce(ed1.esddsc,'Não Iniciada')
  ELSE
    ''
  END  as esddscsup,
  obrpercentultvistoria,
  edr.estuf,
  mes.mesdsc,
  mic.micdsc,
  mun.mundescricao,
  tpo.tpodsc,
  pf.prfdesc
  FROM
  obras2.obras o
    JOIN obras2.empreendimento e ON e.empid = o.empid
    LEFT JOIN obras2.supervisao_os_obra soo ON soo.empid = e.empid AND soo.soostatus = 'A'
    LEFT JOIN obras2.supervisao_os os ON os.sosid = soo.sosid AND os.sosstatus = 'A'
    LEFT JOIN ( SELECT * FROM obras2.prorrogacao_prazo_os prp WHERE prp.prposstatus = 'A' AND prp.prposid IN (SELECT MAX(prposid) as prposid FROM obras2.prorrogacao_prazo_os WHERE prposstatus = 'A' GROUP BY sosid)) as prp ON prp.sosid = os.sosid -- última prorrogacao

    LEFT JOIN workflow.documento doco ON doco.docid = o.docid
    LEFT JOIN workflow.estadodocumento esdo ON doco.esdid = esdo.esdid

    LEFT JOIN workflow.documento doc ON doc.docid = os.docid
    LEFT JOIN workflow.estadodocumento esd ON doc.esdid = esd.esdid

    LEFT JOIN obras2.tipologiaobra             tpo ON tpo.tpoid  = o.tpoid
    LEFT JOIN obras2.programafonte             pf ON pf.prfid    = e.prfid
    LEFT JOIN entidade.endereco                edr ON edr.endid  = e.endid

    LEFT JOIN territorios.municipio            mun ON mun.muncod   = edr.muncod
    LEFT JOIN territorios.mesoregiao           mes ON mes.mescod = mun.mescod
    LEFT JOIN territorios.microregiao          mic ON mic.miccod = mun.miccod

    LEFT JOIN obras2.supervisao_grupo_empresa sge1 ON sge1.sgeid = os.sgeid
    LEFT JOIN entidade.entidade               ent3 ON ent3.entid = sge1.entid
    LEFT JOIN obras2.supervisaoempresa          se ON se.sosid   = os.sosid AND se.suestatus = 'A' AND se.empid = e.empid
    LEFT JOIN workflow.documento                d1 ON d1.docid   = se.docid
    LEFT JOIN workflow.estadodocumento         ed1 ON ed1.esdid  = d1.esdid
    LEFT JOIN obras2.supervisao                 su ON su.sueid   = se.sueid AND su.supstatus = 'A' AND su.obrid = o.obrid

    " . (count($join) ? implode(" ", $join) : "") . "
  WHERE
    o.obrstatus = 'A' AND
    o.obridpai IS NULL
    " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
    ";
$cabecalho = array("ID","Obra","Situação da Obra","Nº OS","Data Inicio da Execução","Data Fim da Execução", "Valor total", "Empresa responsável", "Situação da OS", "Emergencial", "Situação da supervisão", "Percentual", "Estado", "Mesorregião", "Microrregião", "Município", "Tipologia", "Programas");
// Gera o XLS do relatório

if ( $_REQUEST['pesquisa'] == 'xls' ){
	$db->sql_to_xml_excel($sql, 'Relatorio_de_Supervisao_Obras', $cabecalho);
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
</html>
<?
//    $dados = $db->carregar($sql);
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '', '', '','');
?>
