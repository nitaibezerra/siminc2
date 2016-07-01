<?
class AssInt{

	protected $montVal;
	protected $db;

	function __construct(){
		include(APPRAIZ. 'includes/classes/DBMontagemValidacao.inc');
		include(APPRAIZ. 'includes/classes/DBComando.inc');
		$this->db = new DBComando();
	}

	/*
	 * Função  manterBeneficiario
	 * Método usado para manter (insert/update) os dados da tabela (assint.beneficiario)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    26-11-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterBeneficiario ($dados, $where = null){
		$return   = true;
		$tabela   = "assint.beneficiario";

                #DEVIDA A ALTERAÇÃO DA VERSÃO DO SERVIDOR É NECESSÁRIO O stdClass() "CLASSE VAZIA"
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"benid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"paiid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"bennome" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => false,
						),
					"bentipo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"benpassaporte" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "20",
							"mascara" => null,
							"nulo"    => true,
						),
					"benuniversidade" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "100",
							"mascara" => null,
							"nulo"    => true,
						),
					"bencidade" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "100",
							"mascara" => null,
							"nulo"    => true,
						),
					"bentitulacao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						),
					"benstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"garid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"bencurso" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => true,
						),
					"benbolsasindiv" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => true,
						),
					"nivid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"benindbenef" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						),
					"benjustnivel" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => true,
						),
					"benvinculo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => true,
						),
					"entid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"bendtfinal" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"bendtinicial" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => true,
						),
					"entidorigem" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						)
				);



		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					if ($atributo->{$k}['chave'] == 'PK') {
						$benid = $val;
					}

					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			#SETA OS VALORES VINDOS NOS PARAMETROS, NOS RESPECTIVOS ATRIBUTOS DA TABELA                        
			foreach ($dados as $k => $val){
                            if (isset($atributo->{$k})){
                                $atributoUpdate->{$k}           = $atributo->{$k};
                                $atributo->{$k}['value']        = $val;
                                $atributoUpdate->{$k}['value']  = $val;
                            }
			}

			if ($atributo->nivid['value'] == 99999){
				// Seta o valor para INSERT
				$atributo->nivid['value'] 		= '';
				// Seta o valor para UPDATE
				$atributoUpdate->nivid['value'] = '';
			}else{
				// Seta o valor para INSERT
				$atributo->benjustnivel['value'] = '';
				// Seta o valor para UPDATE
				$atributoUpdate->benjustnivel['value'] = '';
			}

			if ($atributo->benvinculo['value'] == BEN_VINC_PER){
				// Seta o valor para INSERT
				$atributo->bendtinicial['value'] = '';
				$atributo->bendtfinal['value'] 	 = '';
				// Seta o valor para UPDATE
				$atributoUpdate->bendtinicial['value'] = '';
				$atributoUpdate->bendtfinal['value']   = '';
			}

			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}


		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		$benid = $benid ? $benid : $return;


		// Deleta Fonte Financiamento
		$this->db->delete("assint.beneficiariofontefin", array("benid" => $benid));

		// Insere Novamente a fonte de financiamento
		if (is_array($dados['fofid']) && !empty($dados['fofid'][0])){
			foreach ($dados['fofid'] as $val){
				$dFof = array(
								"fofid" => $val,
								"benid" => $benid,
							   );
				$return = $this->manterBeneficiarioFonteFinciamento($dFof);
			}
		}

		if ($return){
			$this->db->commit();
			$return = $benid;
		}else{
			$this->db->rollback();
		}
		return $return;
	}

/*
 * Função  manterBeneficiarioFonteFinciamento
 * Método usado para manter (insert/update) os dados da tabela (assint.beneficiariofontefin)
 *
 * @access   public
 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
 * @since    26-11-2009
 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
 * @tutorial Array(
			[evento] => manter
			[benid] => 26
			[prgid] => 40
			[bendtinicial] => 14/10/2009
			[bendtfinal] => 15/10/2009
			[btalterar] => Salvar
		)
 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
 * @tutorial Array(
			[evento] => manter
			[benid] => 26
			[prgid] => 40
			[bendtinicial] => 14/10/2009
			[bendtfinal] => 15/10/2009
			[btalterar] => Salvar
		)
 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
 */
	function manterBeneficiarioFonteFinciamento ($dados, $where = null){
		$return   = true;
		$tabela   = "assint.beneficiariofontefin";
                
                #DEVIDA A ALTERAÇÃO DA VERSÃO DO SERVIDOR É NECESSÁRIO O stdClass() "CLASSE VAZIA"
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"bffid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"benid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"fofid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"bffstatus" => array(
							"chave"   => null,
							"value"   => 'A',
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		return $return;
	}

	function carregaBeneficiario($benid, $param=null){
		$arrDados = array();
		$where    = array();

		if ($param['where']){
			foreach ($param['where'] as $k => $val){
				$where[] = "$k IN ($val)";
			}
		}

		if (is_numeric($benid)){
			$sql = "SELECT
						b.*,
						e.entnumcpfcnpj AS cpf
					FROM
						assint.beneficiario b
					LEFT JOIN
						entidade.entidade e ON e.entid = b.entid
					WHERE
						benid = {$benid}
					" . ( sizeof($where) > 0 ? ' AND ' . implode(' AND ', $where) : '');

			$arrDados = (array) $this->db->pegaLinha($sql);
		}

		if (!empty($arrDados['cpf'])){
			$arrDados['cpf'] = formatar_cpf($arrDados['cpf']);
		}

		return $arrDados;
	}

	function listaBeneficiario( Array $filtro = null, $param=null, $entid=null){

		$where = array();
		$inner = array();

		$filtroDefault = "b.prgid IS NULL";

		if($filtro['listaEstudanteDocente']) unset($filtroDefault);

		foreach($filtro as $k => $val){
			if (empty($val)){continue;}
			switch ($k){
				case 'bennome':
					array_push($where, "b.bennome ilike '%{$val}%'");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'bentipo':
					if( $val != 'T' ) array_push($where, "b.bentipo = '{$val}'");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'paiid':
					array_push($where, "b.paiid = {$val}");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'benuniversidade':
					array_push($where, "b.benuniversidade ilike '%{$val}%'");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'entidorigem':
					array_push($where, "(b.entidorigem = {$val} or pr.entid = {$val})");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'benindbeneficiario':
					if( $val != 'T' )
					{
						if($val == 'E') $val = "'E'";
						if($val == 'D') $val = "'D','P','C'";
						array_push($where, "b.benindbenef IN ({$val})");
					}
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'prgid':
					array_push($where, "b.prgid = {$val}");
					unset($filtroDefault);
					continue;
				break;
				case 'benindbenef':
					array_push($where, "benindbenef IN ({$val})");
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'ligados_programa':
					if( $val != 'T' )
					{
						if($val == 'S') $val = "is not null";
						if($val == 'N') $val = "is null";
						array_push($where, "b.prgid {$val}");
					}
					//array_push($where, "benindbenef IN ('D', 'P')");
					continue;
				break;
				case 'prgid_prog_projeto':
					array_push($where, "b.prgid = {$val}");
					continue;
				break;
				case 'enttipo':
					if( $val != 'T' )
					{
						array_push($inner, " INNER JOIN assint.entidadeassessoriainternacional eas ON eas.enttipo = '{$val}' AND (eas.entid = b.entid OR eas.entid = b.entidorigem) ");
					}
					continue;
				break;
			}
		}

		if ($filtroDefault){
			array_push($where, $filtroDefault);
		}

		$modulo = $param['modulo'] ? $param['modulo'] : '';
		$acao   = $param['acao'] ? $param['modulo'] : $_REQUEST['acao'];

		$op = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Beneficiário" onclick="redireciona(\'?modulo=$modulo&acao=$acao&benid=' || b.benid || '\');">&nbsp;
				 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Beneficiário" onclick="confirmExcluir(\'Deseja Excluir o Beneficiário ' || bennome || '?\', \'?modulo=$modulo&acao=$acao&evento=excluir&benid=' || b.benid || '\');">'
ASDF;

		$op2 = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Beneficiário" onclick="redireciona(\'?modulo=$modulo&acao=$acao&benid=' || b.benid || '\');">&nbsp;
				 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Excluir Beneficiário" onclick="">'
ASDF;

		/*** Recupera o array com os perfis do usuário ***/
		$perfis = recuperaPerfil();
		/**
		 * Verifica se o usuário possui perfil de Universidade
		 * e quais estão associadas a seu perfil
         */
		if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
			$case = $op;
		}else{
			 if(in_array(PERFIL_UNIVERSIDADES, $perfis)) {
				$entids = recuperaUniversidades();

				if($entids) {
					$case = "CASE WHEN (e.entid in (".implode(",", $entids).") OR e2.entid in (".implode(",", $entids).") )
							 THEN $op
							 ELSE $op2 END AS opcoes";
				}else{
					$case = $op2;
				}
			} else {
				$case = $op2;
			}
		}

		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
			$case = $case.",";
		else
			$case = "";

		$sql = "SELECT
					".$case."
					bennome,
					CASE bentipo
						WHEN '" . BEN_TIPO_BRA_EXT . "' THEN 'Brasileiro no Exterior'
						WHEN '" . BEN_TIPO_EST_INS . "' THEN 'Estrangeiro na Instituição'
						WHEN '" . BEN_TIPO_COO_TEC . "' THEN 'Acordo de Cooperação Técnica'
					END AS tipo,
					paidescricao,
					benuniversidade,
					case when b.prgid is null then e.entnome else e2.entnome end as origem
				FROM
					assint.beneficiario b
				INNER JOIN
					territorios.pais p ON p.paiid = b.paiid
				LEFT JOIN
					entidade.entidade e ON e.entid = b.entidorigem
				LEFT JOIN
					assint.programa pr ON pr.prgid = b.prgid
				LEFT JOIN
					entidade.entidade e2 ON e2.entid = pr.entid
				".implode(' ', $inner)."
				WHERE
					benstatus = 'A'
					" . ( sizeof($where) > 0 ? ' AND ' . implode(' AND ', $where) : '');
		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
		{
			$cabecalho = array("Opções", "Nome", "Tipo", "País de Origem/Destino", "Instituição", "Instituição de Origem");
			$this->db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
		}
		else
		{
			return $sql;
		}
	}

	//traz instituicoes do acordo
	function listaAcordoInstituicao(Array $filtro){

		$where = array();

		$op = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Acordo" onclick="redireciona(\'?modulo=principal/cadAcordo&acao=E&acoid=' || a.acoid || '\');">&nbsp;
				 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Acordo" onclick="confirmExcluir(\'Deseja Excluir o Acordo (' || acoconvenio || ')?\', \'?modulo=principal/listAcordo&acao=A&evento=excluir&acoid=' || a.acoid || '\');">',
ASDF;
		$sql = "SELECT
					$op
					acoconvenio,
					assint.acordopais(a.acoid) AS pais,
					assint.acordoorgint(a.acoid) AS orgint,
					assint.acordofontefin(a.acoid) AS fontefin
				FROM
					assint.acordoinstituicao a

				WHERE
					acostatus = 'A'
				   " . ( sizeof($where) > 0 ? ' AND ' . implode(' AND ', $where) : '');

		//dbg($sql, 1);
		$cabecalho = array("Opções", "Nome", "País", "Organismo(s) Internacional(is)", "Fonte(s) de Financiamento(s)");
		$this->db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');

	}

	function listaAcordo(Array $filtro){

		$where = array();
		foreach($filtro as $k => $val){
			if (empty($val)){continue;}
			switch ($k){
				case 'acoconvenio':
					array_push($where, "acoconvenio ILIKE ('" . $val . "%')");
					continue;
				break;
				case 'paiid':
					array_push($where, "assint.acordopais(a.acoid) LIKE ('%" . $val . "%')");
					continue;
				break;
				case 'oriid':
					array_push($where, "assint.acordoorgint(a.acoid) LIKE ('%" . $val . "%')");
					continue;
				break;
				case 'fofid':
					array_push($where, "assint.acordofontefin(a.acoid) LIKE ('%" . $val . "%')");
					continue;
				break;
				case 'entid':
					array_push($where, "ent.entid = '" . $val . "'");
					continue;
				break;

			}
		}

		if( empty($filtro['entid']) && !empty($filtro['enttipo']) )
		{
			if( $filtro['enttipo'] == 'T' ):
				$tipo = "'I','U'";
			else:
				$tipo = "'".$filtro['enttipo']."'";
			endif;

			array_push($where, "ea.enttipo in (".$tipo.")");
		}

		$op = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Acordo" onclick="redireciona(\'?modulo=principal/cadAcordo&acao=E&acoid=' || a.acoid || '\');">&nbsp;
				 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Acordo" onclick="confirmExcluir(\'Deseja Excluir o Acordo (' || acoconvenio || ')?\', \'?modulo=principal/listAcordo&acao=A&evento=excluir&acoid=' || a.acoid || '\');">'
ASDF;

		$op2 = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Acordo" onclick="redireciona(\'?modulo=principal/cadAcordo&acao=E&acoid=' || a.acoid || '\');">&nbsp;
				 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Excluir Acordo" onclick="">'
ASDF;

		/*** Recupera o array com os perfis do usuário ***/
		$perfis = recuperaPerfil();
		/**
		 * Verifica se o usuário possui perfil de Universidade
		 * e quais estão associadas a seu perfil
         */
		if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
			$case = $op;
		}else{
			 if(in_array(PERFIL_UNIVERSIDADES, $perfis)) {
				$entids = recuperaUniversidades();

				if($entids) {
					$case = "CASE WHEN a.entid in (".implode(",", $entids).")
						 THEN $op
						 ELSE $op2 END AS opcoes";
				}else{
					$case = $op2;
				}
			} else {
				$case = $op2;
			}
		}

		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
			$case = $case . ',';
		else
			$case = '';

		$sql = "SELECT
					$case
					acoconvenio,
					entnome,
					assint.acordopais(a.acoid) AS pais,
					assint.acordoorgint(a.acoid) AS orgint,
					assint.acordofontefin(a.acoid) AS fontefin
				FROM
					assint.acordo a
				INNER JOIN
					entidade.entidade ent ON ent.entid = a.entid
				INNER JOIN
				  	assint.entidadeassessoriainternacional ea ON ea.entid = ent.entid AND
				  												 ea.entstatus = 'A'
				WHERE
					acostatus = 'A'
				   " . ( sizeof($where) > 0 ? ' AND ' . implode(' AND ', $where) : '');

		//dbg($sql, 1);
		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
		{
			$cabecalho = array("Opções", "Título do Convênio", "Instituição", "País(es)", "Organismo(s) Internacional(is)", "Fonte(s) de Financiamento(s)");
			$this->db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
		}
		else
		{
			return $sql;
		}
	}

	function carregaAcordo($acoid){

		if (is_numeric($acoid)){
			$sql = "SELECT
						*
					FROM
						assint.acordo
					WHERE
						acoid = {$acoid}";

			$arrDados = (array) $this->db->pegaLinha($sql);

			if ($arrDados['acotipo'] == PROG_TIPO_BI){
				$sql = "SELECT
							paiid
						FROM
							assint.acordopais
						WHERE
							acoid = {$acoid}";
				$arrDados['paiid'] = $this->db->pegaUm($sql);

			}
		}
		return (array) $arrDados;
	}

	/*
	 * Função  manterAcordo
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordo)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    01-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordo($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordo";
                
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"acoid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"carid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoconvenio" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"acorepresentante" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"acodatainicial" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"acodatafinal" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"acotipo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"acostatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"acosituacao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"acocontato" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "500",
							"mascara" => null,
							"nulo"    => true,
						),
						"entid" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){ 
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					if ( $atributo->{$k}['chave'] == 'PK' ){
						$acoid = $val;
					}
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}
                
		if (is_array($dados)  && !empty($dados)){
                    //Seta os valores vindos nos parametros, nos respectivos atributos da tabela
                    foreach ($dados as $k => $val){
                        if( isset( $atributo->{$k} ) ){
                        
                            $atributoUpdate->{$k}           = $atributo->{$k};
                            $atributo->{$k}['value'] 	    = $val;
                            $atributoUpdate->{$k}['value']  = $val;
                        }
                    }
                  
                    // Caso seja update, desconsidera os valores padrões
                    if (!is_null($atributoWhere)){
                            $atributo = $atributoUpdate;
                    }
                // Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		$acoid = $acoid ? $acoid : $return;

		// Deleta Acordo Pais
		$this->db->delete("assint.acordopais", array("acoid" => $acoid));

		// Deleta Acordo Organismo Internacional
		$this->db->delete("assint.acordoorginternacional", array("acoid" => $acoid));

		// Deleta Acordo Atividades
		$this->db->delete("assint.acordoatividade", array("acoid" => $acoid));

		// Deleta Acordo Fontes de Financiamentos
		$this->db->delete("assint.acordofontefin", array("acoid" => $acoid));

		// Deleta Acordo Instituições Parceiras
		$this->db->delete("assint.acordoinstituicao", array("acoid" => $acoid));

		/*
		// Atualiza status Pais
		$atributoPais = (Object) array("acpstatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
		$this->db->insert("assint.acordopais", $atributoPais, $atributoWhere);

		// Atualiza status Organismos Internacionais
		$atributoOrgInter = (Object) array("aoistatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
		$this->db->insert("assint.acordoorginternacional", $atributoOrgInter, $atributoWhere);

		// Atualiza status Atividades
		$atributoAtividades = (Object) array("aatstatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
		$this->db->insert("assint.acordoatividade", $atributoAtividades, $atributoWhere);

		// Atualiza status Fontes de Financiamento
		$atributoFonteFin = (Object) array("affstatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
		$this->db->insert("assint.acordofontefin", $atributoFonteFin, $atributoWhere);

		// Atualiza status Instituições Parceiras
		$atributoInstit = (Object) array("acistatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
		$this->db->insert("assint.acordoinstituicao", $atributoInstit, $atributoWhere);
		*/

		if ( $dados['acotipo'] == PROG_TIPO_BI && $return ){

			if($dados['paiid'] != null){
				$dPais = array (
								"acoid" => $acoid,
								"paiid" => $dados['paiid']
								);

				$return = $this->manterAcordoPais($dPais);
			}

		}elseif ($return){

			//INSERE NA TABELA ACORDO.PAIS
			if ( is_array($dados['arrPaiid']) && !empty($dados['arrPaiid']) ){
				if($dados['arrPaiid'][0] != ''){ //VERIFICA SE O ARRAY VEIO VAZIO
					foreach ($dados['arrPaiid'] as $paiid){
						$dPais = array(
										"acoid" => $acoid,
										"paiid" => $paiid
										);
						$return = $this->manterAcordoPais($dPais);
						if ( !$return ) break;
					}
				}
			}

			//INSERE NA TABELA ACORDO.ORGINTERNACIONAL
			if ( is_array($dados['oriid']) && !empty($dados['oriid']) && $return ){
				if($dados['oriid'][0] != ''){ //VERIFICA SE O ARRAY VEIO VAZIO
					foreach ($dados['oriid'] as $oriid){
						$dOrg = array(
										"acoid" => $acoid,
										"oriid" => $oriid
										);
						$return = $this->manterAcordoOrganismo($dOrg);
						if ( !$return ) break;
					}
				}
			}
		}

		if ($return) {

			//INSERE NA TABELA ACORDO.ATIVIDADE
			if ( is_array($dados['atiid']) && !empty($dados['atiid']) && $return ){
				if($dados['atiid'][0] != ''){ //VERIFICA SE O ARRAY VEIO VAZIO
					foreach ($dados['atiid'] as $atiid){
						$dAti = array(
										"acoid" => $acoid,
										"atiid" => $atiid
										);
						$return = $this->manterAcordoAtividade($dAti);
						if ( !$return ) break;
					}
				}
			}

			//INSERE NA TABELA ACORDO.FONTEFIN
			if ( is_array($dados['fofid']) && !empty($dados['fofid']) && $return ){
				if($dados['fofid'][0] != ''){ //VERIFICA SE O ARRAY VEIO VAZIO
					foreach ($dados['fofid'] as $fofid){
						$dFof = array(
										"acoid" => $acoid,
										"fofid" => $fofid
										);
						$return = $this->manterAcordoFonteFin($dFof);
						if ( !$return ) break;
					}
				}
			}

			//INSERE NA TABELA ACORDO.INSTITUICAO
			if ( is_array($dados['insid']) && !empty($dados['insid']) && $return ){
				foreach ($dados['insid'] as $ind => $insid){
					$dIns = array(
									"acoid" => $acoid,
									"insid" => $insid,
									"acidescricao" => $dados['acidescricao'][$ind]
									);
					$return = $this->manterAcordoInstituicao($dIns);
					if ( !$return ) break;
				}
			}
		}

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			//dbg($dados, 1);
			$return = $acoid;
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}

	/*
	 * Função  manterAcordoatividade
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordoatividade)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    01-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordoAtividade($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordoatividade";
                
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"aatid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"atiid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"aatstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}

	/*
	 * Função  manterAcordofontefin
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordofontefin)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    01-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordoFonteFin($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordofontefin";
                
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"affid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"fofid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"affstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}



	/*
	 * Função  manterAcordoorginternacional
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordoorginternacional)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    01-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordoOrganismo($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordoorginternacional";

                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"aoiid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"oriid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"aoistatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}

	/*
	 * Função  manterAcordopais
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordopais)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    01-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordoPais($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordopais";

                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                
		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"acpid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"paiid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acpstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		return $return;
	}

	/*
	 * Função  manterAcordoinstituicao
	 * Método usado para manter (insert/update) os dados da tabela (assint.acordoinstituicao)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    02-12-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterAcordoInstituicao($dados, $where = null){
		$return   = true;
		$tabela   = "assint.acordoinstituicao";
                
                $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"aciid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"insid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acoid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"acidescricao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "200",
							"mascara" => null,
							"nulo"    => true,
						),
					"acistatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
                        $atributoUpdate = is_object($atributoUpdate) ? $atributoUpdate : new stdClass();
                    
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}
		//dbg($atributo, 1);
		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);

		// Verificação do retorno
		// Este IF só deve ser usado no código, quando for a última operação de banco
		if ($return){
			$this->db->commit();
		}else{
			$this->db->rollback();
		}

		return $return;
	}


	function listaPrograma(Array $filtro){
		$where = array();
		foreach($filtro as $k => $val){
			if (empty($val)){continue;}
			switch ($k){
				case 'prgnome':
					array_push($where, "prgnome ILIKE ('" . $val . "%')");
					continue;
				break;
				case 'entid':
					array_push($where, "e.entid=" . $val);
					continue;
				break;
				case 'paiid':
					array_push($where, "assint.agrupapais(p.prgid) LIKE ('%" . $val . "%')");
					continue;
				break;
				case 'oriid':
					array_push($where, "assint.agrupaorgint(p.prgid) LIKE ('%" . $val . "%')");
					continue;
				break;
				case 'fofid':
					array_push($where, "assint.agrupafontefin(p.prgid) LIKE ('%" . $val . "%')");
					continue;
				break;
			}
		}

		if( empty($filtro['entid']) && !empty($filtro['enttipo']) )
		{
			if( $filtro['enttipo'] == 'T' ):
				$tipo = "'I','U'";
			else:
				$tipo = "'".$filtro['enttipo']."'";
			endif;

			array_push($where, "ea.enttipo in (".$tipo.")");
		}

		$op = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Programa/Projeto" onclick="redireciona(\'?modulo=principal/cadPrograma&acao=E&prgid=' || p.prgid || '\');">&nbsp;
				 <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir Programa/Projeto" onclick="confirmExcluir(\'Deseja Excluir o Programa/Projeto (' || prgnome || ')?\', \'?modulo=principal/listPrograma&acao=A&evento=excluir&prgid=' || p.prgid || '\');">'
ASDF;

		$op2 = <<<ASDF
				'<img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Programa/Projeto" onclick="redireciona(\'?modulo=principal/cadPrograma&acao=E&prgid=' || p.prgid || '\');">&nbsp;
				 <img src="/imagens/excluir_01.gif" style="cursor:pointer;" border=0 title="Excluir Programa/Projeto" onclick="">'
ASDF;

		/*** Recupera o array com os perfis do usuário ***/
		$perfis = recuperaPerfil();
		/**
		 * Verifica se o usuário possui perfil de Universidade
		 * e quais estão associadas a seu perfil
         */
		if (in_array(PERFIL_SUPER_USUARIO, $perfis)) {
			$case = $op;
		}else{
			 if(in_array(PERFIL_UNIVERSIDADES, $perfis)) {
				$entids = recuperaUniversidades();

				if($entids) {
					$case = "CASE WHEN ea.entid in (".implode(",", $entids).")
							 THEN $op
							 ELSE $op2 END AS opcoes";
				}else{
					$case = $op2;
				}
			} else {
				$case = $op2;
			}
		}

		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
			$case = $case . ',';
		else
			$case = '';

		$sql = "SELECT
					$case
					prgnome,
					entnome,
					assint.agrupapais(p.prgid) AS pais,
					assint.agrupaorgint(p.prgid) AS orgint,
					assint.agrupafontefin(p.prgid) AS fontefin
				FROM
					assint.programa p
				INNER JOIN
					assint.entidadeassessoriainternacional ea ON ea.entid = p.entid
																 AND ea.entstatus = 'A'
				INNER JOIN
					entidade.entidade e ON e.entid = ea.entid
										   --AND e.entstatus = 'A'
				WHERE
					prgstatus = 'A'
				   " . ( sizeof($where) > 0 ? ' AND ' . implode(' AND ', $where) : '');

		//dbg($sql, 1);
		if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' )
		{
			$cabecalho = array("Opções", "Nome", "Instituição", "País(es)", "Organismo(s) Internacional(is)", "Fonte(s) de Financiamento(s)");
			$this->db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '');
		}
		else
		{
			return $sql;
		}
	}

	function carregaPrograma($prgid){
		if (is_numeric($prgid)){
			$sql = "SELECT
						*
					FROM
						assint.programa
					WHERE
						prgid = {$prgid}";

			$arrDados = (array) $this->db->pegaLinha($sql);

			if ($arrDados['prgtipo'] == PROG_TIPO_BI){
				$sql = "SELECT
							paiid
						FROM
							assint.programapais
						WHERE
							prgid = {$prgid}";
				$arrDados['paiid'] = $this->db->pegaUm($sql);

			}

		}

		return (array) $arrDados;
	}

	/*
	 * Função  manterPrograma
	 * Método usado para manter (insert/update) os dados da tabela (assint.programa)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    26-11-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterPrograma ($dados, $where = null){
		$return   = true;
		$tabela   = "assint.programa";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"prgid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"entid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgdescricao" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prginterface" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"prgdatainicial" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"prgdatafinal" => array(
							"chave"   => null,
							"value"   => date('d-m-Y'),
							"type"    => "data",
							"tamanho" => null,
							"mascara" => "data",
							"nulo"    => false,
						),
					"prgmeta" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => true,
						),
					"prgnome" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgtipo" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"prgstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
					"prgcontato" => array(
							"chave"   => null,
							"value"   => null,
							"type"    => "string",
							"tamanho" => "300",
							"mascara" => null,
							"nulo"    => true,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$$k = ($atributo->{$k}['chave'] == 'PK') ? $val : $$k;
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		$prgid = $prgid ? $prgid : $return;

		if(!empty($where)){

			// Deleta Organismos Internacionais
			$this->db->delete("assint.programaorginternacional", array("prgid" => $prgid));
			// Deleta pais
			$this->db->delete("assint.programapais", array("prgid" => $prgid));
			// Deleta Fonte financeira
			$this->db->delete("assint.programafontefin", array("prgid" => $prgid));

			// Atualiza status Organismos Internacionais
			$atributoOrgInter = (Object) array("poistatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
			$this->db->insert("assint.programaorginternacional", $atributoOrgInter, $atributoWhere);
			// Atualiza status Organismos Internacionais
			$atributoPais = (Object) array("popstatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
			$this->db->insert("assint.programapais", $atributoPais, $atributoWhere);
			// Atualiza status Fonte financeira
			$atributoFonteFin = (Object) array("pffstatus" => array("chave" => null, "value" => "I", "type" => "string", "tamanho" => "1", "mascara" => null, "nulo" => false,));
			$this->db->insert("assint.programafontefin", $atributoFonteFin, $atributoWhere);
		}

		// Verifica tipo do programa
		if ($dados['prgtipo'] == PROG_TIPO_MU){

			// Insere Novamente Organismos Internacionais
			if (is_array($dados['oriid']) && !empty($dados['oriid'][0])){
				foreach ($dados['oriid'] as $val){
					$dOrg = array(
									"oriid" => $val,
									"prgid" => $prgid,
								   );
					$return = $this->manterProgramaOrganismo ($dOrg);
				}
			}
			// Insere Novamente Pais
			if (is_array($dados['arrPaiid']) && !empty($dados['arrPaiid'][0])){
				foreach ($dados['arrPaiid'] as $val){
					$dPai = array(
									"paiid" => $val,
									"prgid" => $prgid,
								   );
					$return = $this->manterProgramaPais ($dPai);
				}
			}

		}elseif ($dados['prgtipo'] == PROG_TIPO_BI  && !empty($dados['paiid'])){
			// Insere Novamente Pais
			$dPai = array(
							"paiid" => $dados['paiid'],
							"prgid" => $prgid,
						   );
			$return = $this->manterProgramaPais ($dPai);
		}

		// Insere Fonte financeira
		if( is_array($dados['fofid']) && $dados['fofid'][0] && $prgid && $return ){
			foreach ($dados['fofid'] as $val){
				$dFin =  array (
								"fofid" => $val,
								"prgid" => $prgid
							   );
				$return = $this->manterProgramafontefin($dFin);
			}
		}

		if ($return){
			$this->db->commit();
			$return = $prgid;
		}else{
			$this->db->rollback();
		}
		return $return;
	}

	/*
	 * Função  manterProgramafontefin
	 * Método usado para manter (insert/update) os dados da tabela (assint.programafontefin)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    27-11-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterProgramafontefin($dados, $where = null){
		$return   = true;
		$tabela   = "assint.programafontefin";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"pffid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"fofid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"pffstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}

		// Se houver alguma incompatibilidade nos DADOS passados no método "insert"
		// retornará FALSE
		// senão o ID do insert
		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		return $return;
	}


	/*
	 * Função  manterProgramaOrganismo
	 * Método usado para manter (insert/update) os dados da tabela (assint.programaorginternacional)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    26-11-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterProgramaOrganismo ($dados, $where = null){
		$return   = true;
		$tabela   = "assint.programaorginternacional";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"poiid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"oriid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"poistatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}


		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		return $return;
	}

	/*
	 * Função  manterProgramaPais
	 * Método usado para manter (insert/update) os dados da tabela (assint.programapais)
	 *
	 * @access   public
	 * @author   FELIPE TARCHIANI CERÁVOLO CHIAVICATTI
	 * @since    26-11-2009
	 * @param    array $dados - Deve conter os valores que seram setados nos campos (INSERT/UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @param    array $where - Deve conter os valores que seram setados nas CLAUSULAS dos campos (UPDATE).
	 * @tutorial Array(
				[evento] => manter
				[benid] => 26
				[prgid] => 40
				[bendtinicial] => 14/10/2009
				[bendtfinal] => 15/10/2009
				[btalterar] => Salvar
			)
	 * @return   ID || boolean (id do insert realizado, no update retorna TRUE e se houver falha retorna FALSE)
	 */
	function manterProgramaPais ($dados, $where = null){
		$return   = true;
		$tabela   = "assint.programapais";

		// Mapeamento dos campos da tabela
		$atributo = (Object) array(
					"popid" => array(
							"chave"   => "PK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"paiid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"prgid" => array(
							"chave"   => "FK",
							"value"   => null,
							"type"    => "integer",
							"tamanho" => null,
							"mascara" => null,
							"nulo"    => false,
						),
					"popstatus" => array(
							"chave"   => null,
							"value"   => "A",
							"type"    => "string",
							"tamanho" => "1",
							"mascara" => null,
							"nulo"    => false,
						),
				);

		if (is_array($where) && !empty($where)){
			// Clona o OBJ $atributo, para usá-lo nas clausulas WHERE
			//$atributoWhere = clone $atributo;

			// Seta os valores vindos no parametro $where no $atributoWhere, desde que existam em $atributo
			foreach ($where as $k => $val){
				if (isset($atributo->{$k})){
					$atributoWhere->{$k}['value'] = $val;
				}
			}
		}else{
			$atributoWhere = null;
		}

		if (is_array($dados)  && !empty($dados)){
			// Seta os valores vindos nos parametros, nos respectivos atributos da tabela
			foreach ($dados as $k => $val){
				if (isset($atributo->{$k})){
					$atributoUpdate->{$k} 		     = $atributo->{$k};
					$atributo->{$k}['value'] 	     = $val;
					$atributoUpdate->{$k}['value'] = $val;
				}
			}
			// Caso seja update, desconsidera os valores padrões
			if (!is_null($atributoWhere)){
				$atributo = $atributoUpdate;
			}
		// Caso os $dados estejam vazios, não haverá ATUALIZAÇÃO nem INSERÇÃO
		}else{
			return false;
		}


		$return = $this->db->insert($tabela, $atributo, $atributoWhere);
		return $return;
	}

	function carregaSessionPrgid($prgid){
		if (empty($prgid)) return false;

		$sql = "SELECT
					COUNT(*) AS q
				FROM
					assint.programa
				WHERE
					prgid = " . $prgid;
		if ($this->db->pegaUm($sql) < 1){
			redir('?modulo=inicio&acao=C', 'Programa/Projeto inexistente!');
		}

		$_SESSION['assint']['prgid'] = $prgid;
		return $_SESSION['assint']['prgid'];
	}

	function carregaSessionAcoid($acoid){
		if (empty($acoid)) return false;

		$sql = "SELECT
					COUNT(*) AS q
				FROM
					assint.acordo
				WHERE
					acoid = " . $acoid;
		if ($this->db->pegaUm($sql) < 1){
			redir('?modulo=inicio&acao=C', 'Acordo inexistente!');
		}

		$_SESSION['assint']['acoid'] = $acoid;
		return $_SESSION['assint']['acoid'];
	}

	function cabecalhoPrograma($prgid=null, $txt=null){
		$prgid = $prgid ? $prgid : $_SESSION['assint']['prgid'];

		if (empty($prgid)){
			redir('?modulo=inicio&acao=C', 'O Programa/Projeto não está carregado na sessão!');
		}

		$sql = "SELECT
					p.prgnome,
					e.entnome
				FROM
					assint.programa p
				INNER JOIN
					entidade.entidade e ON e.entid = p.entid
				WHERE
					prgid = $prgid";

		$cab = $this->db->pegaLinha($sql);

		$html .= "<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>";
		if (!empty($txt)){
			$html .= "	<tr>";
			$html .= "		<td class='SubTituloCentro' colspan='2'>{$txt}</td>";
			$html .= "	</tr>";
		}
		$html .= "	<tr>";
		$html .= "		<td class='SubTituloDireita'>Programa/Projeto:</td><td>" . $cab['prgnome'] . "</td>";
		$html .= "	</tr>";
		$html .= "	<tr>";
		$html .= "		<td class='SubTituloDireita'>Instituição:</td><td>" . $cab['entnome'] . "</td>";
		$html .= "	</tr>";
		$html .= "</table>";

		return $html;
	}

        function recuperaArquivosPEC_G($request) {

        global $db;

        if ($request['pecid']) {

            if ($request['dis']) {
                $excluir = true;
            } else {
                $excluir = false;
            }

            $sql = "SELECT
                                            anxid,
                                            a.arqid,
                                            pecid,
                                            to_char(anxdtinclusao,'DD/MM/YYYY') as anxdtinclusao,
                                            anxdesc
                                    FROM
                                            assint.anexos anx
                                    INNER JOIN public.arquivo a ON a.arqid = anx.arqid
                                    WHERE
                                            anxstatus = 'A' AND
                                            pecid = " . $request['pecid'];
            $arquivos = $db->carregar($sql);
            if (is_array($arquivos)) {
                foreach ($arquivos as $k => $arquivo) {
                    echo '<tr class="linha" id="arq' . $arquivo['anxid'] . '" name="' . $arquivo['anxid'] . '">' .
                    '<td style="border-bottom: 1px solid #cccccc;">' .
                    '<input type="text" class=" normal" title="" onblur="MouseBlur(this);"
                                                            onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();"
                                                            onmouseover="MouseOver(this);"  disabled
                                                            value="' . $arquivo['anxdesc'] . '" maxlength="50" size="51" name="arqdsc[' . $arquivo['anxid'] . ']" id="' . $arquivo['anxid'] . '" style="text-align:left;">' .
                    '<input type="hidden" name="arqdsc_old[' . $arquivo['anxid'] . ']"/>' .
                    '</td>' .
                    '<td style="border-bottom: 1px solid #cccccc;">' .
                    '<a onclick="abreArquivo(\'' . $arquivo['arqid'] . '\')">' . $arquivo['anxdesc'] . '</a>' .
                    '</td>' .
                    '<td style="border-bottom: 1px solid #cccccc;">' . $arquivo['anxdtinclusao'] .
                    '</td>' .
                    '<td style="border-bottom: 1px solid #cccccc;">' .
                    '<center>' .
                    ($excluir ? '<img src="../imagens/excluir.gif" title="Excluir" class="excluirarq" name="arq' . $arquivo['anxid'] . '" id="' . $arquivo['anxid'] . '" />' : '') .
                    '</center>' .
                    '</td>' .
                    '</tr>';
                }
            }
        }
    }

    function excluirPEC_G($pecid){
        global $db;

        if( $pecid ){

            $this->backUpAtividadesPEC_G( $pecid, 'D' );

            $sql = "UPDATE assint.pecg SET pecstatus = 'I' WHERE pecid = {$pecid} RETURNING pecid;";
            $pecid = $db->pegaUm($sql);

            if( $pecid > 0){
                $db->commit();
                $db->sucesso( 'principal/PEC_G', '&aba=listaPEC_G', "A operação foi realizada com sucesso!");
            }else{
                $db->rollback();
                $db->sucesso( 'principal/PEC_G', '&aba=listaPEC_G', "Não foi possível realizar a operação, tente novamente mais tarde!");
            }
        }else{
            $db->sucesso( 'principal/PEC_G', '&aba=listaPEC_G', "Não foi possível realizar a operação, tente novamente mais tarde!");
        }
    }

    function manterPEC_G($dados, $paramUpdate){
        global $db;
//ver($dados, $paramUpdate, d);
        $colunas = Array();
        $valores = Array();

        $data_ingresso = formata_data_sql( $dados['form']['pecdataingresso'] );
        $dados['form']['pecdataingresso'] = $data_ingresso;

        $data_conclusao = formata_data_sql( $dados['form']['pecdataprevconclusao'] );
        $dados['form']['pecdataprevconclusao'] = $data_conclusao;

        $dados['form']['peccpf'] = str_pad(str_replace(Array('.','-'),'',$dados['form']['peccpf']), 11, "0", STR_PAD_LEFT)." ";
        $dados['form']['co_ies'] = $dados['form']['co_ies'];

        foreach($dados['form'] as $k => $dado){
            if( $dado == '' ){
                $dados['form'][$k] = 'null';
            }elseif( !is_numeric($dado) ){
                    $dados['form'][$k] = "'".trim($dado)."'";
            }

            if( $dados['pecid'] != '' ){
                $colunas[] = $k." = ".trim($dados['form'][$k]);
            }else{
                $colunas[] = $k;
                $valores[] = trim($dados['form'][$k]);
            }
        }

        if( $dados['pecid'] != '' ){
            $colunas[] = "pecdataatualizacao = now()";
            $sql = "UPDATE assint.pecg SET ".implode(',',$colunas)." WHERE pecid = ".$dados['pecid']." RETURNING pecid";

            #PREPARAÇÃO DAS DATAS PARA O FORMATO PADRÃO BR.
            $dados['form']['pecdataingresso'] = formata_data( str_replace("'", '', $dados['form']['pecdataingresso'] ) );
            $dados['form']['pecdataprevconclusao'] = formata_data( str_replace("'", '', $dados['form']['pecdataprevconclusao'] ) );

            $backup = $this->backUpAtividadesPEC_G( $dados, 'A' );

        }else{
            $colunas[] = "pecdataatualizacao";
            $valores[] = "now()";
            $sql = "INSERT INTO assint.pecg(".implode(',',$colunas).") VALUES (".implode(',',$valores).") RETURNING pecid";
            $backup = true;
        }

        $erro = false;
        $pecid = $db->pegaUm($sql);

        if($pecid && $backup == true ){
            include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

            if(is_array($_FILES)){
                foreach($_FILES as $k => $file){
                    $campos = array("pecid" => $pecid, "anxdesc" => "'".$dados['arqdsc'][$k+1]."'");

                    $file = new FilesSimec("anexos", $campos,"assint");

                    if(is_file($_FILES[$k]["tmp_name"]) && !$erro){
                        $arquivoSalvo = $file->setUpload("arquivo assint PEC-G",$k);
                        $erro = false;
                    }else{
                        $erro = true;
                    }
                }
            }

            if($erro){
                $db->rollback();
                echo "<script> alert('Os Dados não foram salvos, erro ao gravar. Tente novamente mais tarde!'); </script>";
            }else{
                $db->commit();
                echo "<script>  alert('Dados salvos com sucesso!'); window.location = 'assint.php?modulo=principal/cadPEC_G&acao=A&pecid=$pecid'; </script>";
            }
        }else{
            $db->rollback();
            echo "<script> alert('Os Dados não foram salvos, erro ao gravar. Tente novamente mais tarde!'); </script>";
        }
    }

    function backUpAtividadesPEC_G( $dados = NULL, $tipo ){
        global $db;

        #OPERAÇÃO DE DELEÇÃO
        if( $tipo == 'D' ){
            $pecid              = $dados;
            $usucpf             = $_SESSION['usucpf'];
            $atvdsc_dados_alt   = 'Operação de deleção, não dados!';

            $sql = "
                SELECT  peccpf AS cpf,
                        pecrne AS rne,
                        pecnome AS nome,
                        CASE WHEN pecsexo = 'M'
                            THEN 'Maculino'
                            ELSE 'Feminino'
                        END AS sexo,
                        pecmatricula AS matrícula,
                        paidescricao AS pais,
                        boldesc AS bolsa,
                        no_curso AS curso,
                        no_ies AS ies,
                        habdesc as habilitação,
                        to_char(pecdataingresso, 'DD/MM/YYYY') AS ingresso,
                        to_char(pecdataprevconclusao,  'DD/MM/YYYY') AS conclusao,
                        pecrendimento AS rendimento,
                        siadesc AS situação
                FROM assint.pecg AS pec

                JOIN assint.bolsa bol ON bol.bolid = pec.bolid
                JOIN assint.habilitacao hab ON hab.habid = pec.habid
                JOIN territorios.pais pai ON pai.paiid = pec.paiid
                JOIN assint.situacaoaluno sia ON sia.siaid = pec.siaid
                JOIN emec.cursos cur ON cur.co_curso = pec.co_curso AND cur.co_ies = pec.co_ies
                JOIN emec.ies ies ON ies.co_ies = pec.co_ies

                JOIN gestaodocumentos.instituicaoensino AS est ON est.iesid = ies.co_ies
                JOIN gestaodocumentos.categoriaadm AS ctg ON ctg.caiid = est.caiid

                WHERE pecid= {$dados['pecid']};
            ";
            $data = $db->pegaLinha($sql);

            #BUSCA NO BANCO OS DADOS COM OS SEUS RESPECTIVOS VALORES ANTES DA ALTERAÇÃO.
            foreach( $data as $key => $campos ){
                if( !is_numeric( $campos ) || $key == 'peccpf' ){
                    $campos = trim($campos);
                }
                $regist_ant[] = $key.':'.$campos;
            }
            $atvdsc_dados_ant = addslashes( implode(';', $regist_ant) );

            $sql = "
                INSERT INTO assint.atividadehistorico(
                        usucpf, pecid, atvdsc_dados_ant, atvdsc_dados_alt, atvtipo, atvaba, atvdtinclusao
                    )VALUES(
                        '{$usucpf}', {$pecid}, '{$atvdsc_dados_ant}', '{$atvdsc_dados_alt}', 'D', 'P', 'NOW()'
                ) RETURNING atvid;
            ";
            $atvid = $db->pegaUm($sql);
        }

        #OPERAÇÃO DE ALTERAÇÃO - UPDATE
        if( $tipo == 'A' ){
            $usucpf = $_SESSION['usucpf'];
            $pecid  = $dados['pecid'];

            $sql = "
                SELECT  peccpf AS cpf,
                        pecrne AS rne,
                        pecnome AS nome,
                        CASE WHEN pecsexo = 'M'
                            THEN 'Maculino'
                            ELSE 'Feminino'
                        END AS sexo,
                        pecmatricula AS matrícula,
                        paidescricao AS pais,
                        boldesc AS bolsa,
                        no_curso AS curso,
                        no_ies AS ies,
                        habdesc as habilitação,
                        to_char(pecdataingresso, 'DD/MM/YYYY') AS ingresso,
                        to_char(pecdataprevconclusao,  'DD/MM/YYYY') AS conclusao,
                        pecrendimento AS rendimento,
                        siadesc AS situação
                FROM assint.pecg AS pec

                JOIN assint.bolsa bol ON bol.bolid = pec.bolid
                JOIN assint.habilitacao hab ON hab.habid = pec.habid
                JOIN territorios.pais pai ON pai.paiid = pec.paiid
                JOIN assint.situacaoaluno sia ON sia.siaid = pec.siaid
                JOIN emec.cursos cur ON cur.co_curso = pec.co_curso AND cur.co_ies = pec.co_ies
                JOIN emec.ies ies ON ies.co_ies = pec.co_ies

                JOIN gestaodocumentos.instituicaoensino AS est ON est.iesid = ies.co_ies
                JOIN gestaodocumentos.categoriaadm AS ctg ON ctg.caiid = est.caiid

                WHERE pecid= {$dados['pecid']};
            ";
            $data = $db->pegaLinha($sql);

            #BUSCA NO BANCO OS DADOS COM OS SEUS RESPECTIVOS VALORES ANTES DA ALTERAÇÃO.
            foreach( $data as $key => $campos ){
                if( !is_numeric( $campos ) || $key == 'peccpf' ){
                    $campos = trim($campos);
                }
                $regist_ant[] = $key.':'.$campos;
            }
            $atvdsc_dados_ant = addslashes( implode(';', $regist_ant) );

            #MONTA OS DADOS QUE VEM DO FORMULÁRIO.
            foreach( $dados['form'] as $key => $campos ){
                switch($key){
                    case 'peccpf':
                        $key = 'cpf';
                        break;
                    case 'pecrne':
                        $key = 'rne';
                        break;
                    case 'pecnome':
                        $key = 'nome';
                        break;
                    case 'pecsexo':
                        $key = 'nome';
                        if($campos == 'M'){
                           $campos = 'Masculino';
                        }else{
                            $campos = 'Feminino';
                        }
                        $key = 'sexo';
                        break;
                    case 'pecmatricula':
                        $key = 'Matrícula';
                        break;
                    case 'paiid':
                        $key = 'País';
                        $campos = $this->buscarDadosPec('P', $campos);
                        break;
                    case 'bolid':
                        $key = 'Bolsa';
                        $campos = $this->buscarDadosPec('B', $campos);
                        break;
                    case 'co_curso':
                        $key = 'Curso';
                        $campos = $this->buscarDadosPec('C', $campos);
                        break;
                    case 'co_ies':
                        $key = 'IES';
                        $campos = $this->buscarDadosPec('I', $campos);
                        break;
                    case 'habid':
                        $key = 'Habilitação';
                        $campos = $this->buscarDadosPec('H', $campos);
                        break;
                    case 'pecdataingresso':
                        $key = 'Ingresso';
                        break;
                    case 'pecdataprevconclusao':
                        $key = 'Conclusão';
                        break;
                    case 'pecrendimento':
                        $key = 'Rendimento';
                        break;
                    case 'siaid':
                        $key = 'Situação';
                        $campos = $this->buscarDadosPec('S', $campos);
                        break;
                }
                $regist_alt[] = $key.':'.str_replace("'", "", $campos);
            }
            $atvdsc_dados_alt = addslashes( implode(';', $regist_alt) );

            $sql = "
                INSERT INTO assint.atividadehistorico(
                        usucpf, pecid, atvdsc_dados_ant, atvdsc_dados_alt, atvtipo, atvaba, atvdtinclusao
                    )VALUES(
                        '{$usucpf}', {$pecid}, '{$atvdsc_dados_ant}', '{$atvdsc_dados_alt}', 'A', 'P', 'NOW()'
                ) RETURNING atvid;
            ";
            $atvid = $db->pegaUm($sql);
        }

        if( $atvid > 0 ){
            $db->commit();
            return true;
        }else{
            $db->rollback();
            return false;
        }
    }

    function buscarDadosPec( $tipo, $value ){
        global $db;

        if( $tipo == 'P' ){
            $sql = "
                SELECT paidescricao FROM territorios.pais WHERE paiid = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'B' ){
            $sql = "
                SELECT boldesc FROM assint.bolsa WHERE bolid = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'C' ){
            $sql = "
                SELECT no_curso FROM emec.cursos WHERE co_curso = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'I' ){
            $sql = "
                SELECT no_ies FROM emec.ies WHERE co_ies = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'H' ){
            $sql = "
                SELECT habdesc FROM assint.habilitacao WHERE habid = {$value};
            ";
            return $db->pegaUm($sql);
        }

        if( $tipo == 'S' ){
            $sql = "
                SELECT siadesc FROM assint.situacaoaluno WHERE siaid = {$value};
            ";
            return $db->pegaUm($sql);
        }
        die();
    }



    function carregaPEC_G( $pecid ){

		$pecid = (int)$pecid;
		global $db;
		if( !empty($pecid) ){
			$sql = "SELECT
						pecid,
						co_ies,
						co_curso,
						trim(replace(to_char(peccpf::numeric,'000,000,000-00'),',','.')) as peccpf,
						pecnome,
						pecsexo,
						pecrne,
						pecmatricula,
						paiid,
						bolid,
						habid,
						to_char(pecdataingresso,'DD/MM/YYYY') as pecdataingresso,
						to_char(pecdataprevconclusao,'DD/MM/YYYY') as pecdataprevconclusao,
						pecrendimento,
						siaid
					FROM
						assint.pecg
					WHERE
						pecstatus = 'A'
						AND pecid = $pecid";
			return $db->pegaLinha($sql);
		}
		return false;
	}

    function listaPEC_G( Array $filtro = null, $param=null, $entid=null){
        $perfis = recuperaPerfil();

        $where = array();
        $inner = array();

        $filtroDefault = "1=1";

        if($filtro['listaPEC_G']) unset($filtroDefault);

        foreach($filtro as $k => $val){
            if(empty($val)){
                continue;
            }

            switch ($k){
                case 'co_ies':
                    if(is_array($val) && !empty($val)){
                            array_push($where, "pec.co_ies IN (".implode(',',$val).")");
                    }
                    continue;
                break;
                case 'peccpf':
                    array_push($where, "trim(pec.peccpf) = '".str_replace(Array('.','-'),'',$val)."'");
                    continue;
                break;
                case 'siaid':
                    array_push($where, "pec.siaid = {$val}");
                    continue;
                break;
                case 'pecnome':
                    array_push($where, "pec.pecnome ilike '%{$val}%'");
                    continue;
                break;
                case 'habid':
                    array_push($where, "pec.habid = {$val}");
                    continue;
                break;
                case 'paiid':
                    array_push($where, "pec.paiid = {$val}");
                    continue;
                break;
                case 'bolid':
                    array_push($where, "pec.bolid = {$val}");
                    continue;
                break;
            }
        }
        $ies = $this->pegaIES();

        if(in_array(PERFIL_PEC_G, $perfis) && count($ies) < 1){
            echo '<table class="tabela text-center" cellspacing="1" cellpadding="3" align="center"><tr><td><b>Você não possui entidade vinculada!</b></td></tr></table>';
            return false;
        } else if(in_array(PERFIL_PEC_G, $perfis) && $ies){
            array_push($where, "pec.co_ies IN (".implode(',',$ies).")");
        }

            array_push($where, "pec.pecstatus = 'A'");

            if ($filtroDefault){
                array_push($where, $filtroDefault);
            }

            $co_ies_filtro = $this->pegaIES();
            $co_ies_filtro = $co_ies_filtro ? implode(',',$co_ies_filtro) : 'null';

            $op = <<<ASDF
                '<center>
                    <img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar Beneficiário" onclick="redireciona(\'?modulo=principal/cadPEC_G&acao=A&pecid=' || pec.pecid || '\');">&nbsp;
                    <img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir PEC-G" onclick="confirmExcluir(\'Deseja Excluir o PEC-G de '||pec.pecnome||'?\', \'?modulo=principal/listaPEC_G&acao=A&evento=excluir&pecid='||pec.pecid||'\');">
                </center>'
ASDF;

            $op2 = <<<ASDF
                '<center>
                    <img src="/imagens/alterar.gif" style="cursor:pointer;" border=0 title="Alterar PEC-G" onclick="redireciona(\'?modulo=principal/cadPEC_G&acao=A&pecid=' || pec.pecid || '\');">&nbsp;'||

                CASE WHEN pec.co_ies in ({$co_ies_filtro})
                    THEN '<img src="/imagens/excluir.gif" style="cursor:pointer;" border=0 title="Excluir PEC-G" onclick="confirmExcluir(\'Deseja Excluir o PEC-G de '||pec.pecnome|| '?\', \'?modulo=principal/listaPEC_G&acao=A&evento=excluir&pecid=' || pec.pecid || '\');">'
                    ELSE '<img src="/imagens/excluir_01.gif" style="cursor:pointer;">'
                END
ASDF;

            #RECUPERA O ARRAY COM OS PERFIS DO USUÁRIO
            $perfis = recuperaPerfil();

            #VERIFICA SE O USUÁRIO POSSUI PERFIL DE UNIVERSIDADE E QUAIS ESTÃO ASSOCIADAS A SEU PERFIL
            if( in_array(PERFIL_SUPER_USUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis) ) {
                $case = $op;
            }else{
                $case = $op2;
            }

            if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' ){
                $case = $case.",";
            }else{
                $case = "";
            }

            $sql = "
                SELECT  ".$case."
                        to_char(pecdataatualizacao,'DD/MM/YYYY') AS ultimaAlteracao,
                        no_ies,
                        caidsc,
                        no_curso,
                        peccpf,
                        pecnome,
                        pecrne,
                        pecmatricula,
                        paidescricao,
                        boldesc,
                        habdesc,
                        to_char(pecdataingresso, 'DD/MM/YYYY') AS dataIngresso,
                        to_char(pecdataprevconclusao, 'DD/MM/YYYY') AS dataConclusao,
                        pecrendimento,
                        siadesc
                FROM assint.pecg pec

                JOIN assint.bolsa bol ON bol.bolid = pec.bolid
                JOIN assint.habilitacao hab ON hab.habid = pec.habid
                JOIN territorios.pais pai ON pai.paiid = pec.paiid
                JOIN assint.situacaoaluno sia ON sia.siaid = pec.siaid
                JOIN emec.cursos cur ON cur.co_curso = pec.co_curso AND cur.co_ies = pec.co_ies
                JOIN emec.ies ies ON ies.co_ies = pec.co_ies

                JOIN gestaodocumentos.instituicaoensino AS est ON est.iesid = ies.co_ies
                JOIN gestaodocumentos.categoriaadm AS ctg ON ctg.caiid = est.caiid

                WHERE ".( sizeof($where) > 0 ? ' ' . implode(' AND ', $where) : '')."
                ORDER BY 1
            ";

        if( !$filtro['tipoPesquisa'] || $filtro['tipoPesquisa'] == 'lista' ){
            $cabecalho = array("Opção", "Última Alteração", "IES", "Nat. IES", "Curso", "CPF", "Nome", "RNE", "Matrícula", "País", "Bolsa", "Habilitação", "Data Ingresso", "Data Conclusão", "Rendimento Academico", "Situação");

            $param['ordena'] = true;
            $param['totalLinhas'] = true;
            $param['managerOrder'] = array(
                2  => array('campo' => "pecdataatualizacao", 'alias' => "ultimaAlteracao"),
                12 => array('campo' => "pecdataingresso", 'alias' => "dataIngresso"),
                13 => array('campo' => "pecdataprevconclusao", 'alias' => "dataConclusao")
            );

            $this->db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '', '', '', '', '', $param);
        }else{
            return $sql;
        }
    }

	function pegaIES(){

		global $db;
		if( !$db->testa_superuser() ){
			$sql = "SELECT
						co_ies
					FROM
						assint.usuarioresponsabilidade
					WHERE
						co_ies IS NOT NULL
                    AND
                        rpustatus = 'A'
                    AND
						usucpf = '".$_SESSION['usucpf']."'
						";
			return $db->carregarColuna($sql);
		}
	}

	function validaAlunosIES( $request ){

		if( !$this->db->testa_superuser() ){
			$sql = "UPDATE assint.pecg SET
						pecvalidado = true
					WHERE
						co_ies in (".implode(',',$request['ies']).")";
			$this->db->executar($sql);
			$this->db->commit();
		}
		echo "<script>window.location = 'assint.php?modulo=principal/listaPEC_G&acao=A';</script>";
	}

}
?>
