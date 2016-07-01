<?php

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

$codigo = $_REQUEST['muncod'];
//dbg($_REQUEST,1);
$db = new cls_banco();
$condicao = "";
if($_REQUEST['labinstalado'] != "" ){
	$condicao .= " bl.bdllab ='".$_REQUEST['labinstalado']."' and ";
}
if($_REQUEST['BLInstal'] != "" ){
	$condicao .= " bl.bdlbanda ='".$_REQUEST['BLInstal']."' and ";
}

$funid = $_REQUEST[BLpolo]== 'true' ? "en.funid in (3, 10)" : "en.funid = 3";

$sql = "select
    '<a href=\"javascript:incluirEscola( \'' ||  en.entid || '\' )\">' || en.entnome || '</a>' as entnome,
    bl.tpaid,
    bl.bdlalunos,
    bl.bdlprofessor,
    case when bl.bdllab = true then 'Sim' else 'Nao' end as bdllab,
    case when bl.bdllabmec = true then 'Sim' else 'Nao' end as bdllabmec,
    case when bl.bdlbanda = true then 'Sim' else 'Nao' end as bdlbanda,
    op.oprdsc,
	to_char( bl.bdlofertaini, 'DD/MM/YYYY' ) as bdlofertaini,
	to_char( bl.bdlofertafinal, 'DD/MM/YYYY' ) as bdlofertafinal,
    to_char( bl.bdldatainst, 'DD/MM/YYYY' ) as bdldatainst,
    to_char( bl.bdldataprevbandainst, 'DD/MM/YYYY' ) as bdldataprevbandainst,
    case when bl.bdlareaoperadora = true then 'Sim' else 'Nao' end as bdlareaoperadora
from cte.bandalarga bl
    inner join entidade.entidade en on en.entid = bl.entid
    left join cte.operadora op on op.oprid = bl.oprid
where
	".$condicao."
    ".$funid." and bl.muncod = '$codigo'
order by
    en.entnome
";
/*
$sql = "select
    '<a href=\"javascript:incluirEscola( \'' ||  en.entid || '\' )\">' || en.entnome || '</a>' as entnome,
    bl.tpaid,
    bl.bdlalunos,
    bl.bdlprofessor,
    case when bl.bdllab = true then 'Sim' else 'Nao' end as bdllab,
    case when bl.bdllabmec = true then 'Sim' else 'Nao' end as bdllabmec,
    case when bl.bdlbanda = true then 'Sim' else 'Nao' end as bdlbanda,
    op.oprdsc,
	to_char( bl.bdlofertaini, 'DD/MM/YYYY' ) as bdlofertaini,
	to_char( bl.bdlofertafinal, 'DD/MM/YYYY' ) as bdlofertafinal,
    to_char( bl.bdldatainst, 'DD/MM/YYYY' ) as bdldatainst,
    to_char( bl.bdldataprevbandainst, 'DD/MM/YYYY' ) as bdldataprevbandainst,
    case when bl.bdlareaoperadora = true then 'Sim' else 'Nao' end as bdlareaoperadora
from cte.bandalarga bl
    inner join entidade.entidade en on en.entid = bl.entid
    left join cte.operadora op on op.oprid = bl.oprid
where
	".$condicao."
    en.funid = 3 and bl.muncod = '$codigo'
order by
    en.entnome
";
*/
    //dbg($sql,1);
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<?php
    
$cabecalho = array( "Escolas", "Nível de prioridade", "Nº de alunos", "Nº de professores", "Possui Laboratório?", "Laboratório instalado pelo MEC", "Possui Banda Larga","Operadora","Data ofertada inicial", "Data de oferta final", "Data da instalação da banda larga","Data da previsão de instalação da banda larga até","Cobertura da operadora abrange a região" );
$db->monta_lista($sql, $cabecalho, 600, 10, 'N', '', '' );






