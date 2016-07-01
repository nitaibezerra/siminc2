<?
 /*
   Sistema Simec
   Setor responsvel: SPO-MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Gilberto Arruda Cerqueira Xavier, Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br), Cristiano Cabral (cristiano.cabral@gmail.com), Fabrício Mendonça (fabriciomendonca@gmail.com)
   Mdulo:iniciopje.inc
   Finalidade: permitir abrir a pÃ¡gina inicial so simec
    */
//recupera todas as variaveis que veio pelo post ou get
foreach($_REQUEST as $k=>$v) ${$k}=$v;

//Chamada de programa
// testa para verificar se é um Alta-Gestão e se tem visão de projetos cadastrados.
$sql = "select pe.pjeid from monitora.projetoespecial pe inner join monitora.usuarioresponsabilidade ur on ur.pjeid=pe.pjeid and ur.pflcod=58 and ur.usucpf='".$_SESSION['usucpf']."'";

$rsvg = @$db->carregar( $sql );
if (  $rsvg && count($rsvg) > 0 )
{ 
	include  APPRAIZ."includes/cabecalho.inc"; 	

	foreach ( $rsvg as $linhavg)
	{
       foreach($linhavg as $k=>$v) ${$k}=$v;
       include  APPRAIZ."monitora/modulos/principal/projespec/visaogerencial.inc";
	}
} 
else {	
	header("Location:monitora.php?modulo=principal/projespec/admprojespec&acao=2");
	exit();
}
?>

