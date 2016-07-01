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

if ($_SESSION['prgid']) $_REQUEST['prgid'] = $_SESSION['prgid'];
if ($_REQUEST['prgid'])
  {
     // achar os atributo de programa que interessam
     $sql= "select prgcod || ' - ' || prgdsc from programa where prgstatus='A' and prgano = '".$_SESSION['exercicio']."' and prgid=".$_REQUEST['prgid'];
     $programa = $db->pegaUm($sql);
}
?>
<title>Indicadores de Programas do Simec</title>
<br>

<?
$db->cria_aba($abacod_tela,$url,'');
monta_titulo("Indicadores de Programa", $programa);

$sql = "select idpdsc, p.perdsc, u.umidsc from monitora.indicadorprograma i 
		inner join monitora.unidademedidaindicador u on u.umiid = i.umiid 
		inner join painel.periodicidade p on p.perid = i.perid 
		where idpstatus='A' and prgid='".$_SESSION['prgid']."' and prgano='".$_SESSION['exercicio']."'";

$cabecalho = array("Indicador","Periodicidade","Unidade Medida");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);
?>