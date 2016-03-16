<?php

require_once APPRAIZ . "academico/classes/LimiteGeral.class.inc";

/**
 * Classe responsavel pela manipulacao dos dados da tabela de Lancamento dos cursso de pos graduação
 * @author Silas Matheus
 * @name Liberacao
 *
 */
class Lancamento extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.lancamento";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("lctid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cpgid' => null,
        'usucpf' => null,
        'lctano' => null,
        'lctvalor' => null,
        'lctdtinclusao' => null,
        'lctstatus' => null
    );

    /**
     * Atributos
     * @var object
     * @access protected
     */
    protected $oLimiteGeral = null;

    /**
     * Verifica se a data atual esta n prazo de liberacao
     * @author Silas Matheus
     * @access public
     * @name pegarLancamentosPorNivel
     * @param $nvcid - Código do Nível
     * @param $entid - Código da Entidade
     * return bool
     */
    public function pegarLancamentosPorNivel($nvcid, $entid){

        $sql = "SELECT 
                    DISTINCT cpg.cpgdsc, cpg.cpgid
                FROM 
                    $this->stNomeTabela lct
                INNER JOIN
                    academico.cursoposgraduacao cpg
                    ON cpg.cpgid=lct.cpgid
                WHERE
                    nvcid=$nvcid
                AND
                    entid=$entid";
        
        $arrDados = $this->carregar($sql);

        return  $arrDados ? $arrDados : array();

    }

    /**
     * Pega o valor do lancamento
     * @author Silas Matheus
     * @access public
     * @name pegarValorLancamento
     * @param $lctano - Ano do Lançamento
     * @param $cpgid - Código do Curso
     * return bool
     */
    public function pegarValorLancamento($lctano, $cpgid){

        $sql = "SELECT 
                    SUM(lctvalor) soma
                FROM
                    $this->stNomeTabela
                WHERE
                    lctano='$lctano'
                AND
                    cpgid=$cpgid";

        return $this->pegaUm($sql);

    }

    /**
     * Pega o valor total de lancamento
     * @author Silas Matheus
     * @access public
     * @name pegarValorLancamentoTotal
     * @param $lctano - Ano do Lançamento
     * @param $nvcid - Código do Nível
     * @param $entid - Código da Entidade
     * return bool
     */
    public function pegarValorLancamentoTotal($lctano, $nvcid, $entid){

        $sql = "SELECT
                    SUM(lct.lctvalor) soma
                FROM
                    $this->stNomeTabela lct
                INNER JOIN
                    academico.cursoposgraduacao cpg
                    ON cpg.cpgid=lct.cpgid
                WHERE
                    lct.lctano='$lctano'
                AND
                    cpg.nvcid=$nvcid
                AND
                    cpg.entid=$entid";

        $valor = $this->pegaUm($sql);
        
        return $valor > 0 ? $valor : 0;

    }

    /**
     * Pega o valor disponvivel para o lancamento
     * @author Silas Matheus
     * @access public
     * @name pegarValorLancamentoDisponivel
     * @param $lctano - Ano do Lançamento
     * @param $nvcid - Código do Nível
     * @param $valorTotal - Valor atual do Lançamento
     * @param $entid - Código da Entidade
     * return bool
     */
    public function pegarValorLancamentoDisponivel($lctano, $nvcid, $valorTotal, $entid){

        $this->oLimiteGeral = new LimiteGeral();
        $valor = $this->oLimiteGeral->valorDisponivelIndividual($lctano, $nvcid, $entid);
        
        // Pega valor geral caso não tenha valor individual
        if( empty($valor) ){
        	$valor = $this->oLimiteGeral->valorDisponivel($lctano, $nvcid) - $valorTotal;
        }

        $valor = $valor - $valorTotal;        

        return $valor > 0 ? $valor : 0;

    }

    public function existeLancamento($nvcid, $lctano){

        $sql = "SELECT
                    COUNT(*)
                FROM
                    $this->stNomeTabela lct
                INNER JOIN
                    academico.cursoposgraduacao cpg
                    ON cpg.cpgid=lct.cpgid
                WHERE
                    lctano='$lctano'
                AND
                    nvcid=$nvcid
                ";

        return $this->pegaUm($sql);

    }

    /**
     * Salva e Atualiza o lancamento
     * @author Silas Matheus
     * @access public
     * @name salvar
     * @param $arrDados - Dados a serem gravados
     * @param $nvcid - Código do Nível
     * @param $entid - Código da Entidade
     * return bool
     */
    public function salvar($arrDados, $nvcid, $entid){


        $usucpf = $_SESSION['usucpf'];
        $cpgid = $arrDados['cpgid'];
        // elimina o campo de codigo de curso do array para gravar apenas os valores
        unset($arrDados['cpgid']);

        foreach($arrDados as $lctano=>$lctvalor){

            if(!empty($lctvalor)){

                $valorLancamento = $this->pegarValorLancamento($lctano,  $cpgid);
                
                if($valorLancamento){

                    $somaLancamento = $lctvalor;

                    $sql = "UPDATE 
                                $this->stNomeTabela
                            SET
                                lctvalor=$somaLancamento
                            WHERE
                                cpgid=$cpgid
                            AND
                                lctano='$lctano'
                            RETURNING
                                lctid";

                }else{

                    $sql = "INSERT INTO $this->stNomeTabela
                                (cpgid, usucpf, lctano, lctvalor)
                            VALUES
                                ($cpgid, '$usucpf', '$lctano', $lctvalor)
                            RETURNING
                                lctid";

                    $somaLancamento = $lctvalor;

                }

                $this->oLimiteGeral = new LimiteGeral();
                if(($this->pegarValorLancamentoTotal($lctano, $nvcid, $entid) - $valorLancamento + $somaLancamento) > $this->oLimiteGeral->valorDisponivel($lctano, $nvcid, $entid)){

                    $lctid = null;

                }else{

                    $lctid = $this->pegaUm($sql);
                    $this->commit();

                }

                if(!$lctid)
                    break;
                
            }
            
        }

        return $lctid;

    }

    /**
     * Exclui o lançamento
     * @author Silas Matheus
     * @access public
     * @name excluir
     * @param $cpgid - Código do curso
     * return bool
     */
    public function excluir($cpgid){

        $sql = "DELETE FROM
                    $this->stNomeTabela
                WHERE
                    cpgid=$cpgid";

        return $this->pegaUm($sql);
        
    }

    /**
     * Recupera os dados do lancamento para edicao
     * @author Silas Matheus
     * @access public
     * @name excluir
     * @param $cpgid - Código do curso
     * return bool
     */
    public function editar($cpgid){

        $sql = "SELECT
                    cpg.nvcid,
                    lct.lctano,
                    lct.lctvalor
                FROM
                    $this->stNomeTabela lct
                INNER JOIN
                    academico.cursoposgraduacao cpg
                    ON cpg.cpgid=lct.cpgid
                WHERE
                    cpg.cpgid=$cpgid";

        $lancamentos = $this->carregar($sql);

        foreach($lancamentos as $lancamento){

            $arrDados['lct'][$lancamento['nvcid']][$lancamento['lctano']] = $lancamento['lctvalor'];

        }

        return $arrDados;

    }

    /**
     * Recupera o nome da entidade
     * @author Silas Matheus
     * @access public
     * @name excluir
     * @param $entid - Código da Entidade
     * return bool
     */
    public function nomeEntidade($entid){

        $sql = "SELECT entnome FROM entidade.entidade WHERE entid=$entid";

        return $this->pegaUm($sql);


    }


    
}