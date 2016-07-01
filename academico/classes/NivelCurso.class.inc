<?php

/**
 * Classe responsavel pela manipulacao dos dados da tabela de nivel de curso de pos graduacao / schema academico
 * @author Silas Matheus
 * @name NivelCurso
 *
 */
class NivelCurso extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.nivelcurso";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("nvcid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'nvcid' => null,
        'nvcdsc' => null
    );

    /**
     * Monta o grid de pesquisa de cursos de pos graduacao
     * @author Silas Matheus
     * @access public
     * @name montaCombo
     * @param int $nvcid - Nível do Curso
     * @param string $habil - Se campo é habilitado ou nao
     * return void
     */
    public function montaCombo($nvcid, $habil = 'S'){

        $sql = "SELECT
                    nvcid codigo,
                    nvcdsc descricao
                FROM
                    academico.nivelcurso nvc
                ORDER BY nvcdsc";

        $this->monta_combo('nvcid', $sql, $habil, " -- Selecione -- ", "", "", "", "", "S", "nvcid", "", $nvcid);
        
    }

}