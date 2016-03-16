<?

$titulo         = "Homologar Solicitações de substituição (Supervisor)";
$pflabrev       = "supervisories";
$wh             = " AND s.iusdsupervisories IS NULL AND s.uncid='".$_SESSION['sismedio']['supervisories']['uncid']."'";
$iusd_homologar = $_SESSION['sismedio']['supervisories']['iusd'];

include_once APPRAIZ_SISMEDIO."solicitacaotroca.inc";

?>