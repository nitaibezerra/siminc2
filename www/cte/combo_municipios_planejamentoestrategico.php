<?php
	// inicializa sistema
	require_once "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	include "_constantes.php";
	include "_funcoes.php";
	$db = new cls_banco();
	$boEstado = ($_REQUEST["item"] == "estcod") ? true : false;
	if(isset($_REQUEST['value'])){
		$sqlListaMunicipios = "
							SELECT
                   				m.muncod as codigo, m.mundescricao as nome, m.estuf as estados
               				FROM territorios.municipio m";
               				if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
                   			$sqlListaMunicipios .= 
                   				" INNER JOIN cte.usuarioresponsabilidade ur ON
                       			 	ur.muncod = m.muncod ";
                       		}
               				$sqlListaMunicipios .= " WHERE ";
							if(!cte_possuiperfil(array(CTE_PERFIL_SUPER_USUARIO, CTE_PERFIL_ADMINISTRADOR))){
								$sqlListaMunicipios .= " ur.usucpf = '" . $_SESSION['usucpf'] . "' AND rpustatus = 'A' AND ";
                   			}
                   			
                   			if($boEstado){
                   				$sqlListaMunicipios .= " m.estuf = '" . $_REQUEST['value'] . "'";
                   			}else{
                   				$sqlListaMunicipios .= " m.miccod =  " . $_REQUEST['value'] . " ";
                   			}
		$sqlListaMunicipios .= "GROUP BY codigo, nome, estados ORDER BY nome";
		$municipios = $db->carregar( $sqlListaMunicipios );
	}
	$municipios = (count($municipios) > 0) ? $municipios : array();
	echo "|--Todos--";
	foreach($municipios as $municipio){
		echo "#%",  $municipio['codigo'], "|", $municipio['nome'];
	}
?>