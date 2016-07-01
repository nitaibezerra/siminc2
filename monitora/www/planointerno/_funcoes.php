<?

/*
 * Tratando uma regra específica solicitada por Wesley Lira e Henrique Xavier
 * REGRA : Para os perfis 112;"Gestor da Unidade de Planejamento", 113;"Equipe de Apoio ao Gestor da Unidade de Planejamento"
 * Os usuários somente poderão filtrar as Unidades na qual estão ligados(perfil). Estes perfis não poderão "Enviar para revisão"
 * CÓDIGO DE IDENTIFICAÇÃO DA REGRA : 0001
 * Executado por Alexandre Dourado : 03/04/09
 */

function aplicarregra_0001() {
	global $db;
	/*
	 * REGRA 0001
	 */
	// carregando os perfis
	$sql = "SELECT prf.pflcod FROM seguranca.perfil prf 
	 		LEFT JOIN seguranca.perfilusuario pru ON pru.pflcod=prf.pflcod 
	 		WHERE sisid='".$_SESSION['sisid']."' AND usucpf='".$_SESSION['usucpf']."'";
	$perfis = $db->carregar($sql);
	if($perfis[0]) {
		$retorno['dadosentidades'] = array();
		foreach($perfis as $perfil) {
			switch($perfil['pflcod']) {
				case EQUIPEAPPLANEJAMENTO:
				case GESTORPLANEJAMENTO:
					$sql = "SELECT DISTINCT ent.entid as codigo, ent.entunicod||' - '||ent.entnome as descricao FROM monitora.usuarioresponsabilidade usr LEFT JOIN entidade.entidade ent ON ent.entunicod=usr.unicod WHERE pflcod='".$perfil['pflcod']."' AND usucpf='".$_SESSION['usucpf']."' AND usr.unicod IS NOT NULL";
					$uni = $db->carregar($sql);
					if($uni[0]) {
						$retorno['dadosentidades'] = array_merge($retorno['dadosentidades'], $uni);
					}
					$sql = "SELECT DISTINCT ent.entid as codigo, ent.entungcod||' - '||ent.entnome as descricao FROM monitora.usuarioresponsabilidade usr LEFT JOIN entidade.entidade ent ON ent.entungcod=usr.ungcod WHERE pflcod='".$perfil['pflcod']."' AND usucpf='".$_SESSION['usucpf']."' AND usr.ungcod IS NOT NULL";
					$ung = $db->carregar($sql);
					if($ung[0]) {
						$retorno['dadosentidades'] = array_merge($retorno['dadosentidades'], $ung);
					}
					$retorno['btn_enviadorevisao_disabled']=true;
					$retorno['btn_alterahomologado_disabled']=true;
					break;
			}
		}
	}

	return $retorno;
	/*
	 * FIM REGRA 0001
 	 */
}


?>