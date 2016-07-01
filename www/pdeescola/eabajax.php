<?php

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "www/pdeescola/_constantes.php";
include_once APPRAIZ . "www/pdeescola/_funcoes.php";

$db = new cls_banco();

$retorno = "";

if( isset($_GET['tipo']) ) {
	
	switch($_GET['tipo']) {
		
		case 'redirecionaeab':
			$entid = $_GET['entid'];
						
			if($entid) {
				$_SESSION["entid"] = $entid;
				
				// para controlar o erro com acesso de alguns usuários
				$_SESSION["exercicio"] = ($_SESSION["exercicio"]) ? $_SESSION["exercicio"] : date('Y');
				
				// Quando for perfil de cadastrador, verifica em quais anos de exercício que a entidade existe.
				if( in_array( PDEESC_PERFIL_CAD_ESCOLA_ABERTA, arrayPerfil() ) ) {
					$sql = "SELECT
								eabanoreferencia
							FROM
								pdeescola.eabescolaaberta
							WHERE
								entid = ".$entid." AND 
								eabstatus = 'A'";
					$anoReferencia = $db->carregar($sql);
					
					if(count($anoReferencia) == 0) {
						die("erro");
					} elseif(count($anoReferencia) == 1) {
						$_SESSION["exercicio"] = $anoReferencia[0]["eabanoreferencia"];
					} else {
						die("eablista_ano_exercicio");
					}
				}
				
				$sql = "SELECT
							eabid
						FROM
							pdeescola.eabescolaaberta
						WHERE
							eabanoreferencia = " . $_SESSION["exercicio"] . " AND
							entid = ".$entid." AND 
							eabstatus = 'A'";
				$eabid = $db->pegaUm($sql);
				
				if($eabid) {
					$_SESSION["eabid"] = $eabid;
				} else {
					unset($_SESSION["eabid"]);
				}
			}
			else {
				unset($_SESSION["entid"]);
				unset($_SESSION["eabid"]);
				
				echo "erro";
			}
			
			break;
	}
}

?>