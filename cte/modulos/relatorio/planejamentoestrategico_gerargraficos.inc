<?php
	var_dump($_POST);
	
	# Setando os valores dos parametros
	$_REQUEST ['estcod'] 		? $uf 			= (integer) $_REQUEST ['estcod'] 		: $uf 			= null;
	$_REQUEST ['microregioes'] 	? $microRegiao 	= (integer) $_REQUEST ['microregioes'] 	: $microRegiao 	= null;
	$_REQUEST ['municipios'] 	? $municipio 	= (integer)	$_REQUEST ['municipios']	: $municipio	= null;
	$_REQUEST ['fodid']			? $disciplina 	= (integer) $_REQUEST ['fodid']			: $disciplina 	= null;
	$_REQUEST ['grafico']		? $tipoGrafico 	= (string)	$_REQUEST ['grafico']		: $tipoGrafico 	= null;
	
	switch( $tipoGrafico ) {
		case '1':
			# Demanda de capacitação
			
			break;
		case '2':
			# Oferta de capacitação
			
			break;
		case '3':
			# Necessidade X Oferta
		
			break;
		case '4':
			# Necessidade X Oferta (Professores em Exercício)
		
			break;
		case '5':
			# Necessidade X Oferta (Vagas Abertas)
		
			break;
		case '6':
			# Necessidade X Oferta (Professores em exercicio + vagas abertas)
		
			break;
		case '7':
			# Expansão IFET
			
			break;
		case '8':
			# Expansão UAB
		
			break;
		case '9':
			# Reuni
		
			break;
		case '10':
			# Demanda com evasão
		
			break;
		case '11':
			# Oferta Existente X Demanda
			break;
		case '12':
			# Oferta Existente X Demanda (professores em exercicio)
			
			break;
		case '13':
			# Oferta Existente X Demanda (vagas abertas)
		
			break;
		case '14':
			# Oferta Existente X Demanda (professores em exercicio + vagas abertas)
		
			break;
		case '15':
			# Demanda - Oferta Planejamento
		
			break;
		case '16':
			# Demanda - Oferta Planejamento (professores em exercicio)
			
			break;
		case '17':
			# Demanda - Oferta Planejamento (vagas abertas)
		
			break;
		default:
			# Demanda - Oferta Planejamento (professores em exercicio + vagas abertas)
		
			break;
	}
	
	

?>