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

if($_REQUEST['acao']=='A') {

	if ($_SESSION['prgid']) $_REQUEST['prgid'] = $_SESSION['prgid'];
	
	if ($_REQUEST['prgid']) {
	     // achar os atributo de programa que interessam
	     $sql= "select prgcod || ' - ' || prgdsc from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid'];
	     $subtitulo = $db->pegaUm($sql);
	     $titulo = "Objetivos de Programa";
	}
	
	$sql_lista = "select objcod,objdsc as descricao from monitora.objetivo o 
			where objstatus='A' and prgid='".$_SESSION['prgid']."' and prgano='".$_SESSION['exercicio']."'";
	

} elseif($_REQUEST['acao']=='O') {

	if ($_SESSION['acaid']) $_REQUEST['acaid'] = $_SESSION['acaid'];
	
	if ($_REQUEST['acaid']) {
    	$sql= "select acacod, acadsc from monitora.acao where acaid='".$_REQUEST['acaid']."'";
		$acao = $db->pegaLinha($sql);
		$titulo = "Objetivos da Aзгo";
		$subtitulo = $acao['acacod']." - ".$acao['acadsc'];
	}
	
	$sql_lista = "select objcod,objdsc as descricao from monitora.objetivo o 
				  inner join monitora.objetivoiniciativa oi on o.objid = oi.objid 
				  inner join monitora.iniciativaacao ia on ia.ictid = oi.ictid 
				  where o.objstatus='A' and ia.acacod='".$acao['acacod']."'";
	

}

$db->cria_aba($abacod_tela,$url,'');
monta_titulo($titulo, $subtitulo);

$cabecalho = array("Cуdigo","Descriзгo");
$db->monta_lista($sql_lista,$cabecalho,100,5,'N','center',$par2);
?>