<?php
	
class PreItensComposicao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras.preitenscomposicao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "itcid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'itcid' => null, 
									  	'itcdescricao' => null, 
									  	'itcordem' => null,
    									'itcstatus' => null,
    									'itcdtinclusao' => null,
    									'ptoid' => null,
    									'itcvalorunitario' => null,
    									'umdid' => null,
    									'itcidpai' => null,
    									'itctipofundacao' => null,
    									'itcquantidade' => null,
    									'itccodigoitem' => null,
									  );
									  
	public function recuperarItensComposicaoPlanilha($ptoid, $icoid, $tipoFundacao)
	{
				
		$sql = "SELECT 
					itc.itcid,
					LOWER(itc.itcdescricao) as itcdescricao,
					itc.itcordem,
					itc.itcstatus,
					itc.itcdtinclusao,
					itc.ptoid,
					itc.itcidpai as idpai,
					itc.itcvalorunitario,
					ppo.ppovalorunitario,
					umd.umdeesc,
					ppo.ppoid,
					pre.preid,
					itc.itctipofundacao,
					itc.itccodigoitem,
					itc.itcidpai,
					itc.itccodigoitemcodigo						
				FROM obras.preobra pre
				INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
				LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
				WHERE pre.ptoid = {$ptoid}
				AND pre.preidsistema = '{$icoid}'
				AND pre.presistema = '{$_SESSION['sisid']}'
				AND itcstatus = 'A'
				".($tipoFundacao != null ? "AND itctipofundacao = '".$tipoFundacao."'" : '')."
				ORDER BY itccodigoitemcodigo
				";
		
//		ver($sql, d);
		
		return $this->carregar($sql);
	}
	
	public function recuperarItensComposicaoPlanilhaProInfancia($ptoid, $preid, $tipoFundacao)
	{
				
		$sql = "SELECT 
					itc.itcid,
					LOWER(itc.itcdescricao) as itcdescricao,
					itc.itcordem,
					itc.itcstatus,
					itc.itcdtinclusao,
					itc.ptoid,
					itc.itcidpai as idpai,
					itc.itcvalorunitario,
					ppo.ppovalorunitario,
					umd.umdeesc,
					ppo.ppoid,
					pre.preid,
					itc.itctipofundacao,
					itc.itccodigoitem,
					itc.itcidpai,
					itc.itccodigoitemcodigo						
				FROM obras.preobra pre
				INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
				LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
				WHERE pre.ptoid = {$ptoid}
				AND pre.preid = '{$preid}'
				AND itcstatus = 'A'
				".($tipoFundacao != null ? "AND itctipofundacao = '".$tipoFundacao."'" : '')."
				
				
				and itccodigoitemcodigo <> ''
				
				
				ORDER BY itccodigoitemcodigo
				limit 3
				";
		
		//ver($sql, d);
		
		return $this->carregar($sql);
	}
	
	public function recuperarValorTotalItensComposicaoPlanilha($ptoid, $icoid)
	{
				
		$sql = "SELECT					
    				sum((ppo.ppovalorunitario*itc.itcvalorunitario::numeric)) as valor
				FROM obras.preobra pre
				INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
				INNER JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND ppo.preid = pre.preid
				WHERE PRE.ptoid = {$ptoid}
				AND pre.preidsistema = '{$icoid}'
				AND pre.presistema = '{$_SESSION['sisid']}'
				AND itcstatus = 'A'";
		
		return $this->pegaUm($sql);
	}
	
	public function recuperarItensComposicaoCronograma($ptoid, $icoid, $boProinfancia = false, $tipofundacao = null)
	{
		
		if(!$ptoid || $ptoid == "" || is_null($ptoid) || empty($ptoid)){
			return false;
		}
		
		if(!$boProinfancia){
			$stWhere = "AND pre.preidsistema = '{$icoid}'
				AND pre.presistema = '{$_SESSION['sisid']}'";
		}else{
			$stWhere = "AND length(itc.itccodigoitemcodigo) = 3";
		}
		
		$tipoB = Array(2,7);
		if( in_array($ptoid,$tipoB) ){
			$tipofundacao = ($tipofundacao && $tipofundacao != ' ') ? ' = \''.$tipofundacao.'\'' : 'is null';
			$stWhere .= " AND itctipofundacao {$tipofundacao} ";
		}
		
		$sql = "SELECT 
					itc.itcid, 
				  	LOWER(itc.itcdescricao) AS itcdescricao, 
				  	--itc.itcordem,
				  	itc.itccodigoitem as itcordem,
    				itc.itcstatus,
    				itc.itcdtinclusao,
    				itc.ptoid,
    				itc.itcvalorunitario,
    				itc.itccodigoitem,
    				umd.umdeesc,
    				pre.preid
				FROM obras.preobra pre
				INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
				LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND pre.preid = ppo.preid
				WHERE pre.ptoid = {$ptoid}	
				AND pre.preid = '{$_SESSION['par']['preid']}'			
				{$stWhere}
				AND itcstatus = 'A'
				ORDER BY itc.itccodigoitemcodigo";
//		ver($sql);
		return $this->carregar($sql);
	}
	
	public function recuperarValorTotalItensComposicaoCronograma($ptoid, $icoid, $boProinfancia = false, $tipofundacao = null )
	{
		
		if(!$boProinfancia){
			$stWhere = "AND pre.preidsistema = '{$icoid}'
				AND pre.presistema = '{$_SESSION['sisid']}'";
		}else{
			$stWhere = "AND length(itc.itccodigoitemcodigo) = 3";
		}
		
		if($tipofundacao){
			$stWhere .= " AND pre.pretipofundacao = '$tipofundacao' ";
		}
				
		$sql = "SELECT					
    				sum((ppo.ppovalorunitario*itc.itcvalorunitario::numeric)) as valor
				FROM obras.preobra pre
				INNER JOIN obras.preitenscomposicao itc ON itc.ptoid = pre.ptoid
				LEFT JOIN obras.unidademedida umd ON umd.umdid = itc.umdid
				LEFT JOIN obras.preplanilhaorcamentaria ppo ON ppo.itcid = itc.itcid AND pre.preid = ppo.preid
				WHERE PRE.ptoid = {$ptoid}
				{$stWhere}
				AND itcstatus = 'A'";
		
//		ver($sql, d);
		
		return $this->pegaUm($sql);
	}
	
	public function verificaFilho($idpai)
	{
		$sql = "SELECT itcid FROM obras.preitenscomposicao WHERE itcidpai = '{$idpai}'";
		return $this->pegaUm($sql);
	}
	
	public function identacaoRecursiva($arDados, $idpai = null, $arConteudo = array())
	{
		
		if($idpai == null){
			foreach($arDados as $x => $dados){
				if($dados['itcidpai'] == null){
					
					if($boIdentifica){
						$identifica = "<div style=\"float:right\"><b>id</b>(".$dados['itcid'].") <b>idpai</b>(".$dados['itcidpai'].")</div>";
					}
					
					$idpai = $dados['itcid'];
					
					array_push($arConteudo, array('itcid' => $dados['itcid'],
												  'itcdescricao' => $dados['itccodigoitem']." - <span title=\"{$dados['itcdescricao']}\">".delimitador(ucfirst($dados['itcdescricao']), 60)."</span>".$identifica,
												  'itcvalorunitario' => $dados['itcvalorunitario'],
												  'umdeesc' => ucfirst($dados['umdeesc']),
												  'ppoid' => $dados['ppoid'],
												  'preid' => $dados['preid'],
												  'ppovalorunitario' => $dados['ppovalorunitario'],
												  'campo' => (!$this->verificaFilho($idpai)) ? campo_texto('ppovalorunitario[]', 'S', 'S', '', 10, 20, '', '', '', '', '', '', '', $item['ppovalorunitario']) : ""));
					
					unset($arDados[$x]);
					unset($_SESSION['idpai']);					
					return self::identacaoRecursiva($arDados, $idpai, &$arConteudo);
				}
			}
		}
		
		if($idpai != null){			
			foreach($arDados as $x => $dados){
				
				if($dados['itcidpai'] == $idpai){
					
					if($boIdentifica){
						$identifica = "<div style=\"float:right\"><b>id</b>(".$dados['itcid'].") <b>idpai</b>(".$dados['itcidpai'].")</div>";
					}
					
					if($i == 0 && !$_SESSION['idpai']){
						$_SESSION['idpai'] = $idpai; 
					}
					$tab .= "&nbsp;&nbsp;&nbsp;";
					$img = '<img src="../imagens/seta_filho.gif" />';

					$idfilho = $dados['itcid'];
					
					array_push($arConteudo, array('itcid' => $dados['itcid'],
												  'itcdescricao' => $tab.$img.$dados['itccodigoitem']." - <span title=\"{$dados['itcdescricao']}\">".delimitador(ucfirst($dados['itcdescricao']), 60)."</span>".$identifica,
												  'itcvalorunitario' => $dados['itcvalorunitario'],
												  'umdeesc' => ucfirst($dados['umdeesc']),
												  'ppoid' => $dados['ppoid'],
												  'preid' => $dados['preid'],
												  'ppovalorunitario' => $dados['ppovalorunitario'],
												  'campo' => (!$this->verificaFilho($dados['itcid'])) ? campo_texto('ppovalorunitario[]', 'S', 'S', '', 10, 20, '', '', '', '', '', '', '', $item['ppovalorunitario']) : ""));
					
					
					unset($arDados[$x]);
					$i++;
					return self::identacaoRecursiva($arDados, $idfilho, &$arConteudo, $tab, $i);
				}
			}
			
			if($_SESSION['idpai']){
				foreach($arDados as $x => $dados){
					if($dados['itcidpai'] == $_SESSION['idpai']){
						
						if($boIdentifica){
							$identifica = "<div style=\"float:right\"><b>id</b>(".$dados['itcid'].") <b>idpai</b>(".$dados['itcidpai'].")</div>";
						}
						
						unset($tab);						
						$tab = "&nbsp;&nbsp;&nbsp;";
						$img = '<img src="../imagens/seta_filho.gif" />';	
						
						array_push($arConteudo, array('itcid' => $dados['itcid'],
												  'itcdescricao' => $tab.$img.$dados['itccodigoitem']." - <span title=\"{$dados['itcdescricao']}\">".delimitador(ucfirst($dados['itcdescricao']), 60)."</span>".$identifica,
												  'itcvalorunitario' => $dados['itcvalorunitario'],
												  'umdeesc' => ucfirst($dados['umdeesc']),
												  'ppoid' => $dados['ppoid'],
												  'preid' => $dados['preid'],
												  'ppovalorunitario' => $dados['ppovalorunitario'],
												  'campo' => (!$this->verificaFilho($dados['itcid'])) ? campo_texto('ppovalorunitario[]', 'S', 'S', '', 10, 20, '', '', '', '', '', '', '', $item['ppovalorunitario']) : ""));
						
						$idfilho = $_SESSION['idpai'];
						unset($arDados[$x]);
						return self::identacaoRecursiva($arDados, $idfilho, &$arConteudo);
					}
				}
			}
			return self::identacaoRecursiva($arDados, null, &$arConteudo);						
		}		
	}
}									  