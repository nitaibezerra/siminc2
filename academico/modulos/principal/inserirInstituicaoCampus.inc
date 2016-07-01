<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";
// Verificando a segurança
$permissoes = verificaPerfilAcademico( array() );
$entid 		= $_SESSION['academico']['entid'];

#verifica se a entidade e uma reitoria ou  um capus.
$tipoEntidade = $_REQUEST['tipoEntidade'];

if ( !$permissoes['insereEntidade'] || !$entid ){
	die("<script>
			alert('Seu perfil não lhe permite acessar esta tela!');
			history.go(-1);
		 </script>");
}

if($_REQUEST['opt']/* && $_REQUEST['entid']*/) {
    if($_REQUEST['opt'] == 'salvarRegistro') {
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->adicionarFuncoesEntidade( $_REQUEST['funcoes'] );
		$entidade->salvar();
		
    	if( $_REQUEST['tipoEntidade'] == 'reitoria' ){
			$modulo = 'alterarReitoria';
		} else {
			$modulo = 'alterarCampus';
		}
		
        echo "<script>
        		alert('Dados gravados com sucesso');
        		window.location='?modulo=principal/$modulo&acao=A&entid=" . $entidade->getEntid() . "';
        	  </script>";
        exit;
    }
}

if( $tipoEntidade == 'reitoria' ){
	$funid = ACA_ID_REITORIA;
	$titulo = 'Inserir Reitoria';
} else {
	$titulo = 'Inserir Campus';	
	switch ( $_SESSION['academico']['orgid'] ){
		case '1':
			$funid = ACA_ID_CAMPUS;
		break;
		case '2':
			$funid = ACA_ID_UNED;
		break;
	}
}

$entidade = new Entidades();

//validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
//// Fim segurança
//$bloqueado = $permissoes['gravar'] ? false :  true;

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo, "");

//$entidade->carregarPorEntid($entidcampus);
echo $entidade->formEntidade("academico.php?modulo=principal/inserirInstituicaoCampus&acao=C&opt=salvarRegistro&tipoEntidade=$tipoEntidade",
							 array("funid" => $funid, "entidassociado" => $entid),
							 array("enderecos"=>array(1),"fotos"=>false),
							 "F"
							 );
?>
<script type="text/javascript">
document.getElementById('tr_entcodent').style.display 			= 'none';
document.getElementById('tr_entnuninsest').style.display 		= 'none';
document.getElementById('tr_entungcod').style.display 			= 'none';
document.getElementById('tr_tpctgid').style.display 			= 'none';
document.getElementById('tr_entnumcpfcnpj').style.display 		= 'none';
document.getElementById('tr_njuid').style.display 				= 'none';
document.getElementById('tr_tpcid').style.display 				= 'none';
document.getElementById('tr_tplid').style.display 				= 'none';
document.getElementById('tr_tpsid').style.display 				= 'none';
document.getElementById('tr_funcoescadastradas').style.display  = 'none';

/*
 * Habilitando o campo NOME 
 */
var entnome = document.getElementById('entnome'); 
entnome.readOnly = false;
entnome.className = 'normal';
entnome.setAttribute('onfocus',    'MouseClick(this);this.select();');
entnome.setAttribute('onmouseout', 'MouseOut(this);');
entnome.setAttribute('onblur',	   'MouseBlur(this);');
//document.getElementById('entnumcpfcnpj').setAttribute('onkeyup', 	'');

/*
 * Habilitando o campo Razão Social 
 */
var entnome = document.getElementById('entrazaosocial'); 
entnome.readOnly = false;
entnome.className = 'normal';
entnome.setAttribute('onfocus',    'MouseClick(this);this.select();');
entnome.setAttribute('onmouseout', 'MouseOut(this);');
entnome.setAttribute('onblur',	   'MouseBlur(this);');
//document.getElementById('entnumcpfcnpj').setAttribute('onkeyup', 	'');


</script>