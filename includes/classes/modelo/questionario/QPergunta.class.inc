<?
	
class QPergunta extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "questionario.pergunta";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "perid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    
    protected $arAtributos     = array(
									  	'perid' => null, 
									  	'grpid' => null, 
									  	'queid' => null, 
									  	'itpid' => null, 
//									  	'perboleana' => null, 
//									  	'perflagitem' => null, 
//									  	'perflagmultiescolha' => null, 
									  	'pertitulo' => null, 
									  	'perordem' => null, 
									  	'perajuda' => null,
    									'perobrigatorio' => null, 
    									'permascara' => null, 
    									'pertipo' => null, 
    									'permultipla' => null, 
    									'perposacao' => null,
    									'pertamanho' => null,
    									'perfuncao' => null
									  );

	public function lista(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $item){
			if (is_object($item) && simec_get_class($item) == 'QGrupo'){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
		
		return $this->carregar($sql, simec_get_class($this));
	}									  
									  
	
	public function listaObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ( is_object($item) && simec_get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . " ORDER BY perordem";
		return $this->carregarObjeto($sql, simec_get_class($this));
	}							

//	$this->pegaUmObjeto()
	
	public function pegaUmObjeto(Array $where = null, $coluna = null){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;	
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;	
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;	
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;	
			}elseif ( simec_get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( simec_get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( simec_get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( simec_get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . 
				" " . ( $order ? " ORDER BY " . $order : "" ) . 
				" LIMIT 1";
///		dbg($sql);			
		return parent::pegaUmObjeto($sql, simec_get_class($this));
	}	
								  
	public function carregaUm(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;

		foreach ($where as $k => $item){
			if ( $k === 'itpid' ){
				$codicao[] = "itpid" . ( $item ? " = " . $item : ' IS NULL' );
				continue;	
			}elseif ($k === 'perid'){
				$codicao[] = "perid = {$item}";
				continue;	
			}elseif ($k === 'grpid'){
				$codicao[] = "grpid = {$item}";
				continue;	
			}elseif ($k === 'queid'){
				$codicao[] = "queid = {$item}";
				continue;	
			}elseif ($k === 'perordem'){
				$codicao[] = "perordem = {$item}";
				continue;	
			}elseif ( is_object($item) && simec_get_class($item) == 'QPergunta' ){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QQuestionario' ){
				$codicao[] = "queid = " . $item->queid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QItemPergunta' ){
				$codicao[] = "itpid = " . $item->itpid;
				continue;
			}elseif ( $k == 'ordem' ){
				$order = $item;
				continue;
			}	
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" ) . 
				" " . ( $order ? " ORDER BY " . $order : "" ) . 
				" LIMIT 1";
//		dbg($sql);			
		return parent::pegaUm($sql);
	}								  

	public function carregarColuna(Array $where = null, $coluna = 'perid'){
		$coluna = (array) $coluna;
		$coluna = implode(",", $coluna);
		$where  = (array) $where;
		
		foreach ($where as $k => $item){
			if (is_object($item) && simec_get_class($item) == 'QPergunta'){
				$codicao[] = "perid = " . $item->perid;
				continue;
			}elseif ( is_object($item) && simec_get_class($item) == 'QGrupo' ){
				$codicao[] = "grpid = " . $item->grpid;
				continue;
			}elseif ($k === 'itpid'){
				$codicao[] = "itpid IN(" . (is_array($item) ? implode(', ', $item) : $item) . ")";
				continue;
			}
		}
		
		$sql = "SELECT
					" . ($coluna ? $coluna : "*") . "
				FROM
					{$this->stNomeTabela}
				" . ( count($codicao) ? " WHERE " . implode(" AND ", $codicao) : "" );
					
		return parent::carregarColuna($sql, $coluna);
	}		
	
	
	public function montaPergunta(Array $arr, $obPai){
		
		if (simec_get_class($obPai) == 'QQuestionario'){
			$idPai = 'que_' . $obPai->queid;
		}elseif (simec_get_class($obPai) == 'QGrupo'){
			$idPai = 'gru_' . $obPai->grpid;
		}elseif (simec_get_class($obPai) == 'QItemPergunta'){
			$idPai = 'item_' . $obPai->itpid;
		}
		
		foreach($arr as $obPergunta){

				// IMG nível 4
				$imgAltera = addslashes('<img src=\'/imagens/alterar.gif\' title=\'Alterar Tipo Documento\' onclick="w = window.open(\'seguranca.php?modulo=principal/questionario/popup/cadastroPergunta&acao=A&perid=' . $obPergunta->perid . '\',\'Janela1\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=530,height=400\'); w.focus();">'); 
				$imgExclui = addslashes('<img src=\'/imagens/excluir.gif\' title=\'Excluir Tipo Documento\' onClick="apagar(\'' . $_SERVER['REQUEST_URI'] . '&instrucao=excluir&perid=' . $obPergunta->perid . '\', \'Deseja apagar a Pergunta ( ' . substr($obPergunta->pertitulo, 0, 80) . (strlen($obPergunta->pertitulo) > 80 ? '...' : '') . ' )?\') "/>');
				$arvore .= "arvore.add('per_" . $obPergunta->perid . "', '{$idPai}', '$imgAltera $imgExclui " . substr($obPergunta->pertitulo, 0, 100) . (strlen($obPergunta->pertitulo) > 100 ? '...' : '') . "', 'javascript:void(0)', '', '', '../includes/dtree/img/pixel_hidden.gif', '../includes/dtree/img/pixel_hidden.gif');\n";
				
				//$arrObItem = (array) $it->listaObjeto( array( $obPergunta) );
		}
		return $arvore;
	}
	
	public function perguntaRespondida( $perid = null, $qrpid = null ){
		
		$perid = $perid ? $perid : $this->perid;
		
		if($qrpid){
			$where = "AND r.qrpid = {$qrpid}";
		}
		
		$sql = "SELECT
						r.perid
				FROM
						questionario.resposta r
				WHERE
					r.perid = {$perid} {$where}";
		
		return parent::carregarColuna($sql);
		
	}
	
	public function perguntaRespondidaComItem( $perid, $qrpid ){
		
		$sql = "SELECT
						ip.itpid
				FROM
						questionario.itempergunta ip
				INNER JOIN questionario.resposta r using(itpid) 
				LEFT JOIN questionario.pergunta p using(itpid)
				LEFT JOIN questionario.grupopergunta g using(itpid)
				WHERE
					ip.perid = {$perid}
					AND r.qrpid = {$qrpid}
		--			AND	(p.itpid IS NOT NULL OR g.itpid IS NOT NULL)";

		return parent::carregarColuna($sql);
		
	}
	
	public function ordena(Array $where){
		if($where['grpid']){
			$where = "grpid = " . $where['grpid'];
		} elseif ($where['queid']){
			$where = "queid = " . $where['queid'];
		} else {
			$where = "itpid = " . $where['itpid'];
		}
		$sql = "SELECT perid FROM questionario.pergunta WHERE $where ORDER BY perordem";
		$arrPerid = (array) parent::carregarColuna($sql);
		if($arrPerid){
			$i = 0;
			foreach($arrPerid as $peridA){
				$i++;
				$ordemA = $i;
				$sql = "UPDATE questionario.pergunta SET perordem = $ordemA  WHERE perid = $peridA";
				$this->executar($sql);
			}
			$this->commit();
		}
	}
	
	public function antesExcluir(){
		$itp 		   = new QItemPergunta();
		$arrObItemPerg = (array) $itp->listaObjeto( array($this), "itpid" );
		// Faz iteração verificando se os "itens perguntas" podem ser deletados
		$retorno = true;	
		foreach($arrObItemPerg as $obItemPerg){
			if ( !$obItemPerg->antesExcluir() ){
				$retorno = false;
				break;
			}
		}
		
		// Se TODOS os "itens perguntas" podem ser deletados, ou seja,
		// não possuem sub-pergunta, faz a deleção dos mesmos.
		if ($retorno){
			foreach($arrObItemPerg as $obItemPerg){
				$obItemPerg->excluir();
				$this->commit();
			}
		}
		return $retorno;
	}	
	
	public function pegaQuestionario( $perid ){
		
		$sql = "SELECT queid FROM questionario.pergunta WHERE perid = ".$perid;
		$queid = parent::pegaUm( $sql );
		
		if( $queid ){
			return $queid;	
		} else {
			$sql = "SELECT grpid FROM questionario.pergunta WHERE perid = ".$perid;
			$grpid = parent::pegaUm( $sql );
			if( $grpid ){
				$grupo = new QGrupo();
				return $grupo->pegaQuestionario( $grpid );
			} else {
				$sql = "SELECT itpid FROM questionario.pergunta WHERE perid = ".$perid;
				$itpid = parent::pegaUm( $sql );
				if( $itpid ){
					$item = new QItemPergunta();
					return $item->pegaQuestionario( $itpid );
				} else {
					echo "<script>
							alert('Erro de operação.'); 
							window.close();
							window.opener.location.replace(window.opener.location);
						  </script>";
				}
			}
		}
	}
	
	public function verificaFuncao( $perid, $qrpid ){
		
		$sql = "SELECT perfuncao FROM questionario.pergunta WHERE perid = ".$perid;
		$funcaoP = parent::pegaUm($sql);
		
		if( $funcaoP ){
			
			// captura dados da chamada
			$chamada    = $this->quest_tratarChamada( $funcaoP, array('perid' => $perid, 'qrpid' => $qrpid) );
			$funcao     = $chamada['funcao'];
			$parametros = $chamada['parametros'];
		
		    $return     = true;
		    
			if ( $funcao )
				$return = call_user_func_array( $funcao, $parametros );
		
			return $return;
			
		} else {
			return true;
		}
		
	}
	
	function quest_tratarChamada( $chamada, array $dados )
	{
		
		// verifica se formato básico da condição
		$posAbre = strpos( $chamada, "(" );
		$posFecha = strrpos( $chamada, ")" );
		if ( $posAbre === false || $posFecha === false )
		{
			return array(
				"funcao" => "",
				"parametros" => array()
			);
		}
		
		// captura a funcao
		$funcao = trim( substr( $chamada, 0, $posAbre ) );
		
		// verifica se função é "chamável" 
		if ( !is_callable( $funcao ) )
		{
			return array(
				"funcao" => "",
				"parametros" => array()
			);
		}
		
		// captura parâmetros
		$parametrosCru = substr( $chamada, $posAbre + 1, $posFecha - $posAbre - 1 );
		$parametrosCru = explode( ",", trim( $parametrosCru ) );
		$parametrosCru = array_map( "trim", $parametrosCru );
		$parametros = array();
		foreach ( $parametrosCru as $item )
		{
			if ( array_key_exists( $item, $dados ) )
			{
				array_push( $parametros, $dados[$item] );
			}
		}
		
		return array(
			"funcao" => $funcao,
			"parametros" => $parametros
		);
	}
	
}