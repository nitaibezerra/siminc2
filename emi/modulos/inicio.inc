<?
 /*
   Sistema Simec
   Setor responsvel: MEC
   Desenvolvedor: Equipe Consultores Simec
   Analista: Cristiano Cabral (cristiano.cabral@gmail.com)
   Programador: Cristiano Cabral (cristiano.cabral@gmail.com)
   Mdulo:inicio.inc
   Finalidade: permitir abrir a página inicial
    */

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$tipos = array( EMI_PERFIL_CADASTRADOR, EMI_PERFIL_ANALISTACOEM, EMI_PERFIL_APROVADOR, EMI_PERFIL_ANALISTASEDUC );
$erro = 0;

foreach( $tipos as $perfil ){
	if( in_array( $perfil, emiRecuperaPerfil() ) ){
		$erro = 1;
	}
}

if( $erro == 0 ){

	if ( $db->testa_superuser() || emiPossuiPerfil( array( EMI_PERFIL_ADMINISTRADOR, EMI_PERFIL_ANALISTACOEM ) ) ){
		
		$emiAbas = array( 0 => array( "descricao" => "Lista de Estados", "link" => "/emi/emi.php?modulo=inicio&acao=C" ),
						  1 => array( "descricao" => "Status das Secretarias", "link" => "/emi/emi.php?modulo=principal/statusSecretaria&acao=A" ) );  
		
		echo montarAbasArray($emiAbas, "/emi/emi.php?modulo=inicio&acao=C" );
		
	}
	monta_titulo("Lista de Estados", "");
	
	$emi->listaEstados();
} else {
	echo '<center><h1>Sistema em manutenção</h1></center>';
}
?>