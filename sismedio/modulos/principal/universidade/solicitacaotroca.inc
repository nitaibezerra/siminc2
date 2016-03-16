<?

$titulo         = "Efetivar Solicitações de substituição (Coordenador Geral)";
$pflabrev       = "coordenadories";
$wh             = " AND s.iusdsupervisories IS NOT NULL AND s.iusdcoordenadories IS NULL AND s.uncid='".$_SESSION['sismedio']['universidade']['uncid']."'";
$iusd_homologar = $_SESSION['sismedio']['universidade']['iusd'];

include_once APPRAIZ_SISMEDIO."solicitacaotroca.inc";

?>