<?
 /*
   Sistema Simec
   Setor responsбvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
   Mуdulo:cadindicador.inc
   Finalidade: permitir a listagem de indicadores de um programa
   */

$modulo=$_REQUEST['modulo'] ;//
include APPRAIZ."includes/cabecalho.inc";

echo "<br>";

if ($_SESSION['acaid']) $_REQUEST['acaid'] = $_SESSION['acaid'];
	
if ($_REQUEST['acaid']) {
   	$sql= "select acacod, acadsc from monitora.acao where acaid='".$_REQUEST['acaid']."'";
	$acao = $db->pegaLinha($sql);
	$titulo = "Metas da Aзгo";
	$subtitulo = $acao['acacod']." - ".$acao['acadsc'];
}
	
$sql_lista = "select m.metdsc,p.perdsc from monitora.meta m 
			  inner join painel.periodicidade p on p.perid = m.metid 
			  inner join monitora.objetivometa om on om.metid = m.metid  
			  inner join monitora.objetivoiniciativa oi on om.objid = oi.objid 
			  inner join monitora.iniciativaacao ia on ia.ictid = oi.ictid 
			  where m.metstatus='A' and ia.acacod='".$acao['acacod']."'";

$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo, $subtitulo);

$cabecalho = array("Meta","Periodicidade");
$db->monta_lista($sql_lista,$cabecalho,100,5,'N','center',$par2);
?>