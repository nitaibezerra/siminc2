<?php
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = 'simec_espelho_producao';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';

include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'includes/workflow.php';

include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "includes/funcoesspo.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . "www/demandasfies/_constantes.php";
require_once APPRAIZ . "www/demandasfies/_funcoes.php";

$db = new cls_banco();
?>
<style>
	.tabela-listagem td, th{
		border: 1px solid #000000;
	}
</style>

<?

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '72324414104';
$_SESSION['usucpf'] = '72324414104';
$_SESSION['sisid'] = 194;

$demanda = new Demanda();

$table = $demanda->getDemandaVencida();

enviarEmail_($table);

function enviarEmail_($table){
	$paransEmail['remetente'] = array("nome" => SIGLA_SISTEMA, "email" => "noreply@mec.gov.br");
	$paransEmail['assunto'] = "[Demandas FIES] Listas de Demandas Vencidas ";
	$paransEmail['mensagem'] =
"
<style>
table tr td {
	  text-align: center;
}
</style>
<pre>Prezados,
Abaixo a relação das demandas que estão vencidas de acordo com a data do Núcleo Jurídico.

".$table."

Para maiores detalhes, favor entrar no SIMEC, módulo Demandas FIES em http://simec.mec.gov.br.

Atenciosamente,
Equipe ". SIGLA_SISTEMA. ".
</pre>
";
	$perfis = array();
	$perfis = array(PFL_PROCURADOR_FEDERAL, PFL_NUCLEO_JURIDICO, PFL_DTI_MEC, PFL_4_NIVEL, PFL_GESTOR_FIES, PFL_GERENCIA_DIGEF, PFL_ADVOGADO, PFL_DTI_MEC);
	$destinatarios = array();
	foreach( $perfis as $idPerfil){
		$emails = getEmailsPorIdPerfil($idPerfil);
		if ($emails) {
			$destinatarios = array_merge($destinatarios, $emails);
		}
	}
	$destinatarios[] = $_SESSION['email_sistema'];
	$destinatarios[] = $_SESSION['email_sistema'];

	$destinatarios = array_unique($destinatarios);
	enviar_email($paransEmail['remetente'], $destinatarios, $paransEmail['assunto'], $paransEmail['mensagem']);
	return true;
}