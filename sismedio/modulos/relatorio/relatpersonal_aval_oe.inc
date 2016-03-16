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

$es = estruturaAvaliacaoOE(array());

$registros = $db->carregar("SELECT 
								  aoesuporte1,
								  aoeinstalacao1,
								  aoedocente1,
								  aoeorganizacao9,
								  aoeorganizacao8,
								  aoeorganizacao7,
								  aoeorganizacao5, 
								  aoeorganizacao6,
								  aoeorganizacao4,
								  aoeorganizacao3,
								  aoeorganizacao2,
								  aoeorganizacao1,
								  aoecomunicacao11,
								  aoecomunicacao10,
								  aoecomunicacao9,
								  aoecomunicacao8,
								  aoecomunicacao7,
								  aoecomunicacao6,
								  aoecomunicacao5,
								  aoecomunicacao4,
								  aoecomunicacao3,
								  aoecomunicacao2,
								  aoecomunicacao1,
								  aoeencontropresenciais4,
								  aoeencontropresenciais3,
								  aoeencontropresenciais2,
								  aoeencontropresenciais1,
								  aoeinstalacoes1
		
							FROM sismedio.avaliacaofinaloe");

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