<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";

/* TRATAMENTO DE SEGURANÇA */
if(validaVariaveisSistema()) {
	echo "<script>
			alert('Problemas nas variáveis do sistema.');
			window.location='?modulo=inicio&acao=A';
		  </script>";
	exit;
}
$permissoes = verificaPerfilRehuf();
validaAcessoHospital($permissoes['verhospitais'], $_SESSION['rehuf_var']['entid']);
// Validando os acessos as funções autorizadas
switch($_REQUEST['acao']) {
	case 'A';
		$_FUNCOESAUTORIZADAS = array('salvarRegistroEntidade'=>true);
	break;
}
if($_REQUEST['requisicao']) {
	if($_FUNCOESAUTORIZADAS[$_REQUEST['requisicao']]) {
		$_REQUEST['requisicao']($_REQUEST);
	}
}
/* FIM - TRATAMENTO DE SEGURANÇA */

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';

/* montando o menu */
echo montarAbasArray(carregardadosmenurehuf(), "/rehuf/rehuf.php?modulo=principal/dadoshospital&acao=A");

/* montando cabeçalho */
monta_titulo( "REHUF", "Programa Nacional de Re-estruturação dos Hospitais Universitários Federais");
monta_cabecalho_rehuf($_SESSION['rehuf_var']['entid']);

$entidade = new Entidades();
/*
 * TODOS OS HOSPITAIS DEVEM ESTA ASSOCIADOS A UMA FUNDAÇÃO UNIVERSITARIA
 * (com exceção do HCPA que é associado e ele mesmo)
 */
$entidassociado = $db->pegaUm("SELECT fea.entid FROM entidade.entidade ent 
							   LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
							   LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
							   WHERE fen.funid='".HOSPITALUNIV."' AND fen.entid='".$_SESSION['rehuf_var']['entid']."'");
if(!$entidassociado) {
	echo "<script>
			alert('Hospital sem universidade associada. Entre em contato com os administradores.');
			window.location='?modulo=inicio&acao=C';
		  </script>";
	exit;
}
/*
 * FIM
 * TODOS OS HOSPITAIS DEVEM ESTA ASSOCIADOS A UMA FUNDAÇÃO UNIVERSITARIA
 * (com exceção do HCPA que é associado e ele mesmo)
 */

$entidade->carregarPorEntid($_SESSION['rehuf_var']['entid']);
echo $entidade->formEntidade("rehuf.php?modulo=principal/dadoshospital&acao=A&requisicao=salvarRegistroEntidade",
							 array("funid" => HOSPITALUNIV, "entidassociado" => $entidassociado),
							 array("enderecos"=>array(1),"fotos"=>true)
							 );

?>
<script type="text/javascript">
document.getElementById('tr_entcodent').style.display = 'none';
document.getElementById('tr_entnuninsest').style.display = 'none';
document.getElementById('tr_entungcod').style.display = 'none';
document.getElementById('tr_tpctgid').style.display = 'none';

/*
 * DESABILITANDO O CAMPO DE CNPJ
 */
document.getElementById('entnumcpfcnpj').readOnly = true;
document.getElementById('entnumcpfcnpj').className = 'disabled';
document.getElementById('entnumcpfcnpj').onfocus = "";
document.getElementById('entnumcpfcnpj').onmouseout = "";
document.getElementById('entnumcpfcnpj').onblur = "";
document.getElementById('entnumcpfcnpj').onkeyup = "";

/*
 * DESABILITANDO O CÓDIGO DA UNIDADE
 */
document.getElementById('entunicod').readOnly = true;
document.getElementById('entunicod').className = 'disabled';
document.getElementById('entunicod').onfocus = "";
document.getElementById('entunicod').onmouseout = "";
document.getElementById('entunicod').onblur = "";
document.getElementById('entunicod').onkeyup = "";

/*
 * DESABILITANDO O NOME DA ENTIDADE
 */
document.getElementById('entnome').readOnly = true;
document.getElementById('entnome').className = 'disabled';
document.getElementById('entnome').onfocus = "";
document.getElementById('entnome').onmouseout = "";
document.getElementById('entnome').onblur = "";
document.getElementById('entnome').onkeyup = "";

/*
 * DESABILITANDO A SIGLA DA ENTIDADE
 */
document.getElementById('entsig').readOnly = true;
document.getElementById('entsig').className = 'disabled';
document.getElementById('entsig').onfocus = "";
document.getElementById('entsig').onmouseout = "";
document.getElementById('entsig').onblur = "";
document.getElementById('entsig').onkeyup = "";

</script>