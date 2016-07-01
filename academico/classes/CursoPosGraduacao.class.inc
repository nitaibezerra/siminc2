<?php

/**
 * Classe responsavel pela manipulacao dos dados da tabela de curso de pos graduacao / schema academico
 * @author Silas Matheus
 * @name CursoPosGraduacao
 *
 */
class CursoPosGraduacao extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "academico.cursoposgraduacao";

    /**
     * Chave primaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cpgid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'cpgid' => null,
        'nvcid' => null,
        'entid' => null,
        'usucpf' => null,
        'cpgdsc' => null,
        'cpgcodigo' => null,
        'cpgstatus' => null,
        'cpgdtinclusao' => null
    );

    /**
     * Monta o grid de pesquisa de cursos de pos graduacao
     * @author Silas Matheus
     * @access public
     * @name montaGrid
     * @param int $entid - Entidade Selecionada
     * @param int $nvcid - Nível do Curso
     * @param int $cpgcodigo - Código do Curso
     * @param string $cpgdsc - Descrição do Curso
     * return void
     */
    public function montaGrid($entid, $nvcid, $cpgcodigo, $cpgdsc){

        $sql = "SELECT
                    '<a style=\"cursor:pointer;\" onclick=\"editarCurso(\'' || cpgid || '\'); \">
                        <img src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar\" />
                    </a>' ||

                        CASE WHEN 
                            ((SELECT COUNT(*) FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid) = 0)
                        THEN

                            '<a style=\"cursor:pointer;\" onclick=\"excluirCurso(\'' || cpgid || '\'); \">
                                <img src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir\" />
                            </a>'
                            
                        ELSE
                            ''
                        END

                    acao,
                    (cpg.cpgcodigo || ' ') cpgcodigo,
                    (cpg.cpgdsc || ' ') cpgdsc,
                    COALESCE((SELECT lctvalor FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid AND lct.lctano='2008'),0) a2008,
                    COALESCE((SELECT lctvalor FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid AND lct.lctano='2009'),0) a2009,
                    COALESCE((SELECT lctvalor FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid AND lct.lctano='2010'),0) a2010,
                    COALESCE((SELECT lctvalor FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid AND lct.lctano='2011'),0) a2011,
                    COALESCE((SELECT lctvalor FROM academico.lancamento lct WHERE lct.cpgid=cpg.cpgid AND lct.lctano='2012'),0) a2012
                FROM
                    $this->stNomeTabela cpg
                WHERE
                    cpg.entid=$entid ";
         
        if(!empty($nvcid))
                $sql .= " AND cpg.nvcid=$nvcid ";

        if(!empty($cpgcodigo))
                $sql .= " AND cpg.cpgcodigo='$cpgcodigo' ";
        
        if(!empty($cpgdsc))
                $sql .= " AND cpg.cpgdsc LIKE '%$cpgdsc%' ";

        $cabecalho =  array('A&ccedil;&atilde;o', 'Código', 'Nome', '2008', '2009', '2010', '2011', '2012');
        $alinhameto =  array('center','center','left', 'center', 'center', 'center', 'center', 'center');
        
        $this->monta_lista($sql, $cabecalho, 20, 50, false, "center", 'S', '', '', $alinhameto);
        
    }

    /**
     * Insere o curso no banco
     * @author Silas Matheus
     * @access private
     * @name gravar
     * @param array $arrDados - Array dos dados para salvar
     * return int
     */
    private function gravar($arrDados){

        extract($arrDados);
        $usucpf = $_SESSION['usucpf'];
        
        $sql = "INSERT INTO $this->stNomeTabela
                    (nvcid, entid, usucpf, cpgdsc, cpgcodigo)
                VALUES
                    ($nvcid, $entid, '$usucpf', '$cpgdsc', '$cpgcodigo')
                RETURNING
                    cpgid";

        return $cpgid = $this->pegaUm($sql);

    }

    /**
     * Atualiza o curso no banco
     * @author Silas Matheus
     * @access private
     * @name editar
     * @param array $arrDados - Array dos dados para salvar
     * return int
     */
    private function editar($arrDados){

        extract($arrDados);
        $usucpf = $_SESSION['usucpf'];

        $sql = "UPDATE
                    $this->stNomeTabela
                SET 
                    nvcid=$nvcid,
                    entid=$entid,
                    usucpf='$usucpf',
                    cpgdsc='$cpgdsc',
                    cpgcodigo='$cpgcodigo'
                WHERE
                    cpgid=$cpgid
                RETURNING
                    cpgid";

        return $cpgid = $this->pegaUm($sql);

    }

    /**
     * Salva os dados no banco
     * @author Silas Matheus
     * @access public
     * @name salvar
     * @param array $arrDados - Array dos dados para salvar
     * return int
     */
    public function salvar($arrDados){

        if($this->validar($arrDados)){

            if(!empty($arrDados['cpgid']))
                $cpgid = $this->editar($arrDados);
            else
                $cpgid = $this->gravar($arrDados);
            
        }

        return $cpgid;

    }

    /**
     * Valida se os dados estao repetidos no banco
     * @author Silas Matheus
     * @access private
     * @name validar
     * @param array $arrDados - Array dos dados para salvar
     * return int
     */
    private function validar($arrDados){

        extract($arrDados);

        $sql = "SELECT 
                    COUNT(*)
                FROM
                    $this->stNomeTabela
                WHERE
                    (cpgdsc='$cpgdsc'
                OR
                    cpgcodigo='$cpgcodigo') ";

        if(!empty($cpgid)){
                
                $sql .= " AND cpgid<>$cpgid";
        }

        return ($this->pegaUm($sql) == 0);
        
    }

    /**
     * Monta o combo de curso por nivel e entidade
     * @author Silas Matheus
     * @access private
     * @name montaComboPorNivel
     * @param int $entid - Código da Entidade
     * @param int $nvcid - Código do Nível
     * @param string $name - Nome do campo
     * @param string $id - Id do campo
     * @param string $valor - Valor selected
     * return void
     */
    public function montaComboPorNivel($entid, $nvcid, $name = 'cpgid', $id = 'cpgid', $valor = ""){

        $sql = "SELECT
            cpgid codigo,
            cpgid || ' - ' || cpgdsc descricao
        FROM
            academico.cursoposgraduacao cpg
        WHERE
            nvcid=$nvcid
        AND
            entid=$entid
        ORDER BY
            cpgdsc";

        $this->monta_combo($name, $sql, 'S', " -- Selecione -- ", "", "", "", "200", "N", $id, "", $valor);

    }
    

}