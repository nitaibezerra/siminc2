<?php

function montar_barra_execucao( $atividade, $cor = true ){
	if ( !$cor ) {
		$atividade['s'] = STATUS_NAO_INICIADO;
	}
	switch ( $atividade['staid'] ) {
		case STATUS_CANCELADO:
			$cor_texto = '#aa2020';
			$cor_barra = '#cc3333';
			$cor_sombra = '#ffe7e7';
			break;
		case STATUS_CONCLUIDO:
			$cor_texto = '#2020aa';
			$cor_barra = '#3333cc';
			$cor_sombra = '#d4e7ff';
			break;
		case STATUS_EM_ANDAMENTO:
			$cor_texto = '#209020';
			$cor_barra = '#339933';
			$cor_sombra = '#dcffdc';
			break;
		case STATUS_SUSPENSO:
			$cor_texto = '#aa9020';
			$cor_barra = '#bba131';
			$cor_sombra = '#feffbf';
			break;
		case NAO_SE_APLICA:
			$cor_texto = '#aa9020';
			$cor_barra = '#bba131';
			$cor_sombra = '#feffbf';
		break;	
		default:
		case STATUS_NAO_INICIADO:
			$cor_texto = '#909090';
			$cor_barra = '#bbbbbb';
			$cor_sombra = '#efefef';
			break;
	}
	if($atividade['staid'] == NAO_SE_APLICA){
		return '<span style="color: #000000;font-size: 10px;">Não se Aplica</span>';
	}else{
		return sprintf(
		'<span style="color: %s;font-size: 10px;">%s</span>' .
		'<div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color: %s;" title="%d%%">' .
		'<div style="font-size:4px;width: %d%%; height: 6px; max-height: 6px; background-color: %s;">' .
		'</div>'.
		'</div>',
		$cor_texto,
		$atividade['stadsc'],
		$cor_sombra,
		$atividade['andamento'],
		$atividade['andamento'],
		$cor_barra
		);
	}
}

?>