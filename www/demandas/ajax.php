<?php 
// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include("_constantes.php");

$db = new cls_banco();

function fechaDb()
{
    global $db;
    $db->close();
}

register_shutdown_function('fechaDb');

if(isset($_GET['tipo'])) {
	switch($_GET['tipo']) {
		case 'carregar_demandas':
			
			$sql="SELECT dmdid FROM demandas.demanda WHERE usucpfexecutor = '".$_GET['usucpfexecutor']."'";
			
			$demandas = $db->carregar($sql);
			//$retorno = $_GET['usucpfexecutor'];
			$retorno = "";
			
			if(($demandas != "") || ($demandas != NULL)) {
				foreach ($demandas as $demanda) {					
					$sql = "SELECT 
                    	d.dmdid,
                    	d.dmdtitulo as assunto,
                    	loc.lcadescricao||' '||a.anddescricao||' '||d.dmdsalaatendimento  as local,	
                    	u.usunome as executor,
                    	d.dmddatainiprevatendimento as dtinic,
                    	d.dmddatafimprevatendimento as dtfim 
                    FROM 
                    demandas.demanda d									  
                    INNER JOIN
                    seguranca.usuario u ON u.usucpf = d.usucpfexecutor    
                    INNER JOIN 
							demandas.localandaratendimento laa ON laa.laaid = d.laaid
					INNER JOIN 
						demandas.andaratendimento a ON a.andid  = laa.andid
					INNER JOIN 
						demandas.localatendimento loc ON loc.lcaid  = laa.lcaid                                                     
                    WHERE 
                    d.dmdid = ".$demanda['dmdid']."
                   "; 					               
                    $dados = $db->pegaLinha( $sql ); 
					$retorno .= '@'.$_GET['usucpfexecutor'].'||'.
									$dados["dmdid"].'||'.
									$dados["executor"].'||'.
									trim($dados["assunto"]).'||'.
									trim($dados["local"]).'||'.									
									$dados["dtinic"].'||'.
									$dados["dtfim"];
				}				
			}
			echo $retorno;
			break;
	}
} 
?>