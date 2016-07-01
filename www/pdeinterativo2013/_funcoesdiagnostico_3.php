<?

function diagnostico_3_3_sintesedimensao3($dados,$salvaAba = true) {
	global $db;
	
	if($dados['critico']) {
		foreach($dados['critico'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostapergunta SET critico=".$valor." WHERE repid='".$indice."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($dados['respostatempoaprendizagem']) {
		foreach($dados['respostatempoaprendizagem'] as $indice => $valor) {
			$sql = "UPDATE pdeinterativo2013.respostatempoaprendizagem SET ".$indice."=".$valor." WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
			$db->executar($sql);
			$db->commit();
		}
	}
	
	if($salvaAba){
		salvarAbaResposta("diagnostico_3_3_sintesedimensao3");	
		echo "<script>
				alert('Dados gravados com sucesso');
				window.location='".$dados['togo']."';
			  </script>";
	}
	
}

function diagnostico_3_2_tempodeaprendizagem($dados) {
	global $db;
	
	if(!$dados['rtacaso']) {
		
		die("<script>
			 alert('Marque se a escola desenvolve ações de Educação Integral');
			window.location='pdeinterativo2013.php?modulo=principal/diagnostico&acao=A&aba=diagnostico_3_ensinoeaprendizagem&aba1=diagnostico_3_2_tempodeaprendizagem';
			</script>");
		
	}
	
	salvarRespostasPorEscola();
	
	if($dados['atv']) {
		foreach($dados['atv'] as $atv) {
			$rtaatividade[] = $atv.",".$dados['qtd'][$atv];
		}
	}
	
	if(!$dados['rtaestudantepart'][0] && !$dados['rtaestudantepart'][1]) {
		$dados['rtaestudantepart'] = array();
	}
	
	$sql = "SELECT rtaid FROM pdeinterativo2013.respostatempoaprendizagem WHERE pdeid='".$_SESSION['pdeinterativo2013_vars']['pdeid']."'";
	$rtaid = $db->pegaUm($sql);
	
	if($rtaid) {
		
		$sql = "UPDATE pdeinterativo2013.respostatempoaprendizagem
   				SET rtacaso='".$dados['rtacaso']."', 
   					rtaestrategia=".(($dados['rtaestrategia'])?"'".implode(";",$dados['rtaestrategia'])."'":"NULL").", 
   					rtaestudantepart=".(($dados['rtaestudantepart'])?"'".implode(";",$dados['rtaestudantepart'])."'":"NULL").", 
   					rtacargahoraria=".(($dados['rtacargahoraria'])?"'".$dados['rtacargahoraria']."'":"NULL").", 
       				rtaatividade=".(($rtaatividade)?"'".implode(";",$rtaatividade)."'":"NULL").", 
       				rtaporque=".(($dados['porque'])?"'".implode(";",$dados['porque'])."'":"NULL").",
       				rtamacrocampo=".(($dados['rtamacrocampo'])?"'".implode(";",$dados['rtamacrocampo'])."'":"NULL").",
       				rtaporqueoutro=".(($dados['rtaporqueoutro'])?"'".$dados['rtaporqueoutro']."'":"NULL")."
 				WHERE rtaid='".$rtaid."';";
		
		$db->executar($sql);
		$db->commit();
		
	} else {
	
	
		$sql = "INSERT INTO pdeinterativo2013.respostatempoaprendizagem(
	            rtacaso, rtaestrategia, rtaestudantepart, rtacargahoraria, 
	            rtaatividade, rtaporque, rtastatus, pdeid, rtamacrocampo, rtaporqueoutro)
			    VALUES ('".$dados['rtacaso']."', 
			    		 ".(($dados['rtaestrategia'])?"'".implode(";",$dados['rtaestrategia'])."'":"NULL").", 
			    		 ".(($dados['rtaestudantepart'])?"'".implode(";",$dados['rtaestudantepart'])."'":"NULL").", 
			    		 ".(($dados['rtacargahoraria'])?"'".$dados['rtacargahoraria']."'":"NULL").", 
			             ".(($rtaatividade)?"'".implode(";",$rtaatividade)."'":"NULL").", 
			             ".(($dados['porque'])?"'".implode(";",$dados['porque'])."'":"NULL").", 'A',
			             '".$_SESSION['pdeinterativo2013_vars']['pdeid']."',
			             ".(($dados['rtamacrocampo'])?"'".implode(";",$dados['rtamacrocampo'])."'":"NULL").",
			             ".(($dados['rtaporqueoutro'])?"'".$dados['rtaporqueoutro']."'":"NULL").");";
		
		$db->executar($sql);
		$db->commit();
	
	}
	
	salvarAbaResposta("diagnostico_3_2_tempodeaprendizagem");
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
}

function diagnostico_3_1_planejamentopedagogico($dados) {
	global $db;
	
	$dados['abacod']="diagnostico_3_1_planejamentopedagogico";
	
	salvarJustificativaEvidencias($dados);
	salvarAbaResposta($dados['abacod']);
	
	salvarRespostasPorEscola();
	
	echo "<script>
			alert('Dados gravados com sucesso');
			window.location='".$dados['togo']."';
		  </script>";
	
	
}


?>