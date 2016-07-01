<?php

/**
 * Classe responsavel pela manipulacao dos dados da tabela de periodo de Liberacao do curso de pos graduação
 * @author Silas Matheus
 * @name Liberacao
 *
 */
class Liberacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.liberacao";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'libdtinicio' => null,
        'libdtfinal' => null
    );

    /**
     * Salva os dados no banco
     * @author Silas Matheus
     * @access public
     * @name salvar
     * @param string $libdtinicio - Data inicial de liberação
     * @param string $libdtfinal - Data final de liberação
     * return void
     */
    public function salvar($libdtinicio, $libdtfinal){

        $libdtinicio = formata_data_sql($libdtinicio);
        $libdtfinal = formata_data_sql($libdtfinal);

        $sql = "UPDATE
                    $this->stNomeTabela
                SET
                    libdtinicio='$libdtinicio',
                    libdtfinal='$libdtfinal'";

        $this->executar($sql);

    }

    /**
     * Lista períodos de liberação
     * @author Rondomar França
     * @access public
     * @name listarPeriodos
     * return array
     */
    public function listarPeriodos(){
    
    	$sql = "SELECT 
    				A.libdtinicio , A.libdtfinal , B.mescod , B.mesdsc 
    			FROM ". $this->stNomeTabela . " as A 
    			INNER JOIN public.meses as B ON(A.mescod=B.mescod)
    			ORDER BY B.mescod ASC
    			";
    	

    	return $this->carregar( $sql );
    }
    
    /**
     * Recupera o periodo de liberação
     * @author Silas Matheus
     * @access public
     * @name pegarDados
     * return array
     */
    public function pegarDados( $ordenar=null ){

        $sql = "SELECT * FROM $this->stNomeTabela";
        
        if($ordenar)
        $sql.=" WHERE libdtfinal is not null ORDER BY libdtfinal DESC";

        return $this->pegaLinha($sql);

    }

    /**
     * Verifica se a data atual esta n prazo de liberacao
     * @author Silas Matheus
     * @access public
     * @name noPrazo
     * return bool
     */
    public function noPrazo(){
        
        $arrDados = $this->pegarDados( true );

        $dataAtual = date('Y-m-d');

        return (
                    ( strtotime($arrDados['libdtinicio']) < strtotime($dataAtual) ) &&
                    ( strtotime($dataAtual) <= strtotime($arrDados['libdtfinal']) )
               );

    }

}