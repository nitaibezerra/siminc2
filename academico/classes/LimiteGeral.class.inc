<?php

require_once APPRAIZ . "academico/classes/Lancamento.class.inc";

/**
 * Classe responsavel pela manipulacao dos dados da tabela de limite geral
 * @author Silas Matheus
 * @name LimiteGeral
 *
 */
class LimiteGeral extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.limitegeral";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("ltgid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ltgid' => null,
        'entid' => null,
        'nvcid' => null,
        'ltg2008' => null,
        'ltg2009' => null,
        'ltg2010' => null,
        'ltg2011' => null,
        'ltg2012' => null
    );

    /**
     * Atributos
     * @var object
     * @access protected
     */
    protected $oLancamento = null;

    /**
     * Verifica se existe informações de limites gerais para o nível
     * @author Silas Matheus
     * @access public
     * @name existeNivel
     * @param int $nvcid - Código do Nível
     * return int
     */
    public function existeNivel($nvcid){

        $sql = "SELECT
                    COUNT(*)
                FROM
                    $this->stNomeTabela
                WHERE
                    nvcid=$nvcid";

        return ($this->pegaUm($sql) > 0);
        
    }
    
    public function existeNivelIndividual( $nvcid, $entid){
    
    	$sql = "SELECT
		    		COUNT(*)
		    	FROM
		    		$this->stNomeTabela
		    	WHERE
		    		nvcid=$nvcid
    				AND entid=$entid";
		    
    	return ($this->pegaUm($sql) > 0);
    
    }
    
    public function existeNivelSemEntidade($nvcid){
    
    	$sql = "SELECT
    	COUNT(*)
    	FROM
    	$this->stNomeTabela
    	WHERE
    	entid is null
    	and nvcid=$nvcid";

    	return ($this->pegaUm($sql) > 0);
    
    }

    /**
     * Salva os dados no banco
     * @author Silas Matheus
     * @access public
     * @name salvar
     * @param array $arrDados - Array dos dados para salvar
     * @param int $entid - Código da Entidade
     * return int
     */
    public function salvar($arrDados, $entid){

        foreach($arrDados as $nvcid=>$ltg){

            $this->oLancamento = new Lancamento();
            $validacao = true;
            foreach($ltg as $key=>$value){
                
                if($this->oLancamento->pegarValorLancamentoTotal($key, $nvcid, $entid) > $value)
                    $validacao = false;

            }

            if($validacao){

                if( $this->existeNivelIndividual( $nvcid, $entid ) ){
                    
                    $ltgid = $this->editar($ltg, $nvcid, $entid);
                }else{

                	$ltgid = $this->gravar($ltg, $nvcid, $entid);                   

                }

            }

            if(!$ltgid)
                break;

        }


        return $ltgid;

    }

    /**
     * Grava os limites gerais no banco
     * @author Silas Matheus
     * @access private
     * @name gravar
     * @param array $arrDados - Array dos dados para salvar
     * @param int $nvcid - Código do Nível
     * @param int $entid - Código da Entidade
     * return int
     */
    public function gravar($arrDados, $nvcid, $entid){
    	
        $arrDados['2008'] = $arrDados['2008'] ? $arrDados['2008'] : 0;
        $arrDados['2009'] = $arrDados['2009'] ? $arrDados['2009'] : 0;
        $arrDados['2010'] = $arrDados['2010'] ? $arrDados['2010'] : 0;
        $arrDados['2011'] = $arrDados['2011'] ? $arrDados['2011'] : 0;
        $arrDados['2012'] = $arrDados['2012'] ? $arrDados['2012'] : 0;

        $sql = "INSERT INTO $this->stNomeTabela
                    (entid, nvcid, ltg2008, ltg2009, ltg2010, ltg2011, ltg2012)
                VALUES
                    ($entid, $nvcid, {$arrDados['2008']}, {$arrDados['2009']}, {$arrDados['2010']}, {$arrDados['2011']}, {$arrDados['2012']})
                RETURNING
                    ltgid";

        $ltgid = $this->pegaUm($sql);
        $this->commit();
        return $ltgid;

    }
    
    public function gravarLimiteCotas($arrDados, $nvcid){
    
    	$arrDados['2008'] = $arrDados['2008'] ? $arrDados['2008'] : 0;
    	$arrDados['2009'] = $arrDados['2009'] ? $arrDados['2009'] : 0;
    	$arrDados['2010'] = $arrDados['2010'] ? $arrDados['2010'] : 0;
    	$arrDados['2011'] = $arrDados['2011'] ? $arrDados['2011'] : 0;
    	$arrDados['2012'] = $arrDados['2012'] ? $arrDados['2012'] : 0;
    
    	$sql = "INSERT INTO $this->stNomeTabela
    	(nvcid, ltg2008, ltg2009, ltg2010, ltg2011, ltg2012)
    	VALUES
    	($nvcid, {$arrDados['2008']}, {$arrDados['2009']}, {$arrDados['2010']}, {$arrDados['2011']}, {$arrDados['2012']})
    	RETURNING
    	ltgid";
    
    	$ltgid = $this->pegaUm($sql);
    	$this->commit();
    	return $ltgid;
    
    }

    /**
     * Atualiza os limites gerais no banco
     * @author Silas Matheus
     * @access private
     * @name editar
     * @param array $arrDados - Array dos dados para salvar
     * @param int $nvcid - Código do Nível
     * @param int $entid - Código da Entidade
     * return int
     */
    private function editar($arrDados, $nvcid, $entid){

        extract($arrDados);
        $usucpf = $_SESSION['usucpf'];

        $arrDados['2008'] = $arrDados['2008'] ? $arrDados['2008'] : 0;
        $arrDados['2009'] = $arrDados['2009'] ? $arrDados['2009'] : 0;
        $arrDados['2010'] = $arrDados['2010'] ? $arrDados['2010'] : 0;
        $arrDados['2011'] = $arrDados['2011'] ? $arrDados['2011'] : 0;
        $arrDados['2012'] = $arrDados['2012'] ? $arrDados['2012'] : 0;

        $sql = "UPDATE
                    $this->stNomeTabela
                SET
                    nvcid=$nvcid,
                    ltg2008={$arrDados['2008']},
                    ltg2009={$arrDados['2009']},
                    ltg2010={$arrDados['2010']},
                    ltg2011={$arrDados['2011']},
                    ltg2012={$arrDados['2012']}
                WHERE
                    nvcid=$nvcid
                    AND entid=$entid
                RETURNING
                    ltgid";

        $ltgid = $this->pegaUm($sql);
        $this->commit();
        return $ltgid;

    }
    
    public function editarLimiteCotas($arrDados, $nvcid){
    
    	extract($arrDados);
    	$usucpf = $_SESSION['usucpf'];
    
    	$arrDados['2008'] = $arrDados['2008'] ? $arrDados['2008'] : 0;
    	$arrDados['2009'] = $arrDados['2009'] ? $arrDados['2009'] : 0;
    	$arrDados['2010'] = $arrDados['2010'] ? $arrDados['2010'] : 0;
    	$arrDados['2011'] = $arrDados['2011'] ? $arrDados['2011'] : 0;
    	$arrDados['2012'] = $arrDados['2012'] ? $arrDados['2012'] : 0;
    
    	$sql = "UPDATE
    	$this->stNomeTabela
    	SET
	    	nvcid=$nvcid,
	    	ltg2008={$arrDados['2008']},
	    	ltg2009={$arrDados['2009']},
	    	ltg2010={$arrDados['2010']},
	    	ltg2011={$arrDados['2011']},
	    	ltg2012={$arrDados['2012']}
    	WHERE
	    	nvcid=$nvcid
	    	AND entid is null
    	RETURNING
    	ltgid";

    	$ltgid = $this->pegaUm($sql);
    	$this->commit();
    	return $ltgid;
    
    }

    public function listaLimiteDeCotasGeral(){
    
    	$dados = array();
    
    	$sql = "SELECT * FROM " . $this->stNomeTabela . " WHERE entid is null ";
    
    	$limites = $this->carregar($sql);
    
    	if($limites){
    
	    	foreach($limites as $limite){
	    
		    	$dados[$limite['nvcid']] = array(
		    	'2008' => $limite['ltg2008'],
		    		'2009' => $limite['ltg2009'],
		    		'2010' => $limite['ltg2010'],
		    			'2011' => $limite['ltg2011'],
		    			'2012' => $limite['ltg2012'],
		    			);
	    
	    	}
    	}
    
		return $dados;
	}
    
    /**
     * Recupera os limites gerais para a tela
     * @author Silas Matheus
     * @access public
     * @name pegarDados
     * @param int $entid - Código da Entidade
     * return array
     */
    public function pegarDados($entid){

        $dados = array();

        $sql = "SELECT 
                    *
                FROM
                    $this->stNomeTabela
                WHERE
                    entid=$entid";

        $limites = $this->carregar($sql);

        if($limites){
            
            foreach($limites as $limite){

                $dados[$limite['nvcid']] = array(
                    '2008' => $limite['ltg2008'],
                    '2009' => $limite['ltg2009'],
                    '2010' => $limite['ltg2010'],
                    '2011' => $limite['ltg2011'],
                    '2012' => $limite['ltg2012'],
                );

            }
        }

        return $dados;

    }


    public function valorDisponivelIndividual($lctano, $nvcid, $entid){

        $sql = "SELECT
                    SUM(ltg$lctano)
                FROM
                    $this->stNomeTabela
                WHERE
                    nvcid=$nvcid
                AND
                    entid=$entid";

        return $this->pegaUm($sql);
    }

    
    public function valorDisponivel($lctano, $nvcid){
    
    	$sql = "SELECT
    	SUM(ltg$lctano)
    	FROM
    	$this->stNomeTabela
    	WHERE
    	nvcid=$nvcid
    	AND
    	entid is null";
    
    	return $this->pegaUm($sql);
    }
}