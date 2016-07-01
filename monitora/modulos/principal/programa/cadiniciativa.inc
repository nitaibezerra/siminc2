<?
 /*
   Sistema Simec
   Setor responsável: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Módulo:cadindicador.inc
   Finalidade: permitir a listagem de indicadores de um programa
   */

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";

if ($_SESSION['acaid']) $_REQUEST['acaid'] = $_SESSION['acaid'];
if ($_REQUEST['acaid']) {
    $sql= "select acacod, acadsc from monitora.acao where acaid='".$_REQUEST['acaid']."'";
	$acao = $db->pegaLinha($sql);
}
?>
<title>Indicadores de Programas do Simec</title>
<br>

<?
$db->cria_aba($abacod_tela,$url,'');
monta_titulo("Iniciativas da Programa", $acao['acacod']." - ".$acao['acadsc']);

$sql = "select ictcod, ictdsc from monitora.iniciativa i 
		inner join monitora.iniciativaacao ia on ia.ictid = i.ictid
		where ia.icastatus='A' and ia.acacod='".$acao['acacod']."' and ictstatus='A'";

$cabecalho = array("Código","Descrição");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);

?>