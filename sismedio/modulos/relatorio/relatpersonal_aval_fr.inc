<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<p align="center" style="font-size:16px;"><b>Avaliação Final da Formação - OE</b></p>
<?php

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */

include_once '_funcoes.php';

$modoRelatorio = true;

$es = estruturaAvaliacaoFR(array());

$registros = $db->carregar("SELECT 
							  aoeinstalacoes1,
							  aoecomunicacao1,
							  aoecomunicacao2,
							  aoecomunicacao3,
							  aoecomunicacao4,
							  aoecomunicacao5,
							  aoecomunicacao6,
							  aoecomunicacao7,
							  aoecomunicacao8,
							  aoecomunicacao9,
							  aoecomunicacao10,
							  aoecomunicacao11,
							  aoeorganizacao1,
							  aoeorganizacao2,
							  aoeorganizacao3,
							  aoeorganizacao4,
							  aoeorganizacao5,
							  aoeorganizacao6,
							  aoeorganizacao7,
							  aoeorganizacao8,
							  aoeorganizacao9,
							  aoedocente1,
							  aoedocente2,
							  aoeinstalacao1,
							  aoeinstalacao2,
							  aoesuporte1,
							  aoesuporte2,
							  aoesuporte3,
							  aoesuporte4,
							  aoearticulacao1
							FROM sismedio.avaliacaofinalfr");

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

include APPRAIZ_SISMEDIO."montarQuestionario.inc";

?>