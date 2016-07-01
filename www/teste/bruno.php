<?
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
restore_error_handler();
restore_exception_handler();
error_reporting( E_ALL );
print 'enviou ---->'.date('d/m/Y h:i:s');
flush();
				ob_flush();
				flush();
				ob_flush();
				flush();
				ob_flush();
require_once APPRAIZ . "includes/Snoopy.class.php";
$conexao = new Snoopy;
//$urlReferencia = "http://www.fnde.gov.br/pls/simad/internet_fnde.liberacoes_result_pc?p_ano=%s&p_uf=%s&p_municipio=%s&p_tp_entidade=&p_cgc=%s";
//$url = sprintf($urlReferencia, $ano, $uf, $municipio, $cnpj);
$url = "http://".$_REQUEST['url'];		
//$conexao->submit($url, $postdata);

		$conexao->fetch($url);
		$resultado = $conexao->results;
		print $resultado;
flush();
				ob_flush();
				flush();
				ob_flush();
				flush();
				ob_flush();
print 'acabou!!--->'.date('d/m/Y h:i:s');
die();
	?>