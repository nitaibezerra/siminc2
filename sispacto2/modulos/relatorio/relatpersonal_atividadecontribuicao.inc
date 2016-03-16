<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Questões contribuição do PACTO</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

include_once '_funcoes_professoralfabetizador.php';

$es = estruturaContribuicaoPacto(array());

$modoRelatorio = true;

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto2']['universidade']['uncid']) {
	$inner_join = "INNER JOIN sispacto2.identificacaousuario i ON i.iusd = a.iusd AND i.uncid='".$_SESSION['sispacto2']['universidade']['uncid']."'";
}


$registros = $db->carregar("SELECT
							  cpacontribuicao1,
							  cpacontribuicao2,
							  cpacontribuicao3,
							  cpacontribuicao4,
							  cpacontribuicao5,
							  cpacontribuicao6,
							  cpacontribuicao7,
							  cpacontribuicao8,
							  cpacontribuicao9,
							  cpacontribuicao10,
							  cpadificuldade1,
							  cpadificuldade2
		
							FROM sispacto2.contribuicaopacto a 

							{$inner_join}");

if($registros[0]) {
	foreach($registros as $imp) {
		$c = array_keys($imp);
		foreach($c as $indice) {
			if(strpos(trim($imp[$indice]),";")) {
				
				$indicep = explode(";",trim($imp[$indice]));
				
				if($indicep) {
					foreach($indicep as $indp) {
						
						if(strpos(trim($indp),"||")) {
							$indpp = explode("||",trim($indp));
							$arrFinal[$indice][((trim($indpp[0]))?trim($indpp[0]):'vazio')]++;
							
						} else {
							
							if(is_numeric($indp)) {
								$arrFinal[$indice][((trim($indp))?trim($indp):'vazio')]++;
							}
							
						}
						
					}
				}
				
			} else {

				if(strpos(trim($imp[$indice]),"||")) {
					$indpp = explode("||",trim($imp[$indice]));
					$imp[$indice]  = $indpp[0];
				}

				$arrFinal[$indice][((trim($imp[$indice]))?trim($imp[$indice]):'vazio')]++;
				
			}
			
		}
	}
}

include APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";

?>