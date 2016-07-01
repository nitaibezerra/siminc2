<?php

/**
 * @author Jair Foro <jairforo@gmail.com>
 * Trata as restrições de entidade
 */
class RestricaoEntidade extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $stNomeTabela = "par.restricaoentidade";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $arChavePrimaria = array("renid");

    /**
     * Atributos
     * @var array
     * @access protected
     * @author Jair Foro <jairforo@gmail.com>
     */
    protected $arAtributos = array(
        'renid' => null,
        'resid' => null,
        'inuid' => null,
        'tprid' => null,
        'docid' => null,
    );

    public function inserirRestricaoEntidade($arrPost, $resid) {
        
        switch ($arrPost['esfera']) {
            case 'T':
                # Salva lista de documentos PAR
                if ($arrPost['listaTermo']) {
                    return $this->insertRestricaoEntidadeTermo($arrPost, $resid);
                }
                break;
            case 'E':
                # Salva lista de Estados
                if (!empty($arrPost['estuf'])) {
                    return $this->insertRestricaoEntidadeUf($arrPost, $resid);
                }
                break;
            case 'M':
                # Salva lista de Municipios
                if (!empty($arrPost['listaMunicipio'])) {
                    return $this->insertRestricaoEntidadeMunicipio($arrPost, $resid);
                }
                break;
        }
    }

    /**
     * Insere Restrições de entidade vinculada a um Termo
     * @param array $arrPost
     * @param int $resid
     * @return bool
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function insertRestricaoEntidadeTermo(array $arrPost, $resid) {
        try {
            $coluna = '';
            foreach ($arrPost['listaTermo'] as $numeroTermo) {
                if ($arrPost['tipoTermo'][$numeroTermo] == 'OBRASPAC') {
                    $coluna = 'proidpac'; #processoobra
                } else if ($arrPost['tipoTermo'][$numeroTermo] == 'OBRASPAR') {
                    $coluna = 'proidpar'; #Obras do PAR - processoobraspar
                } else {
                    $coluna = 'prpid'; #processopar
                }

                $sql = "
                    INSERT INTO 
                        {$this->stNomeTabela}(
                            resid, 
                            docid, 
                            " . $coluna . " 
                        )
                        VALUES (
                            " . $resid . ", 
                            " . wf_cadastrarDocumento(TPDID_RESTRICAO_PAR, 'Restrição Termo', ESDID_EM_CADASTRAMENTO) . ",
                            " . $numeroTermo . " 
                        )
                    ";

                $this->executar($sql);
            }
            return;
        } catch (Exception $exc) {
            $this->rollback();
            echo $exc->getTraceAsString();
        }
    }

    /**
     * Insere Restrições de entidade vinculada a um Município
     * @param array $arrPost
     * @param int $resid
     * @return bool
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function insertRestricaoEntidadeMunicipio(array $arrPost, $resid) {
        try {
            $sqlMunicipio = "
                SELECT DISTINCT
                   muncod,
                   inuid
                FROM
                    par.instrumentounidade i
                WHERE
                    i.muncod IN('" . join("', '", $arrPost['listaMunicipio']) . "')";

            $arrMunicipio = $this->carregar($sqlMunicipio);
            foreach ($arrMunicipio as $municipio) {
                $sql = "
                    INSERT INTO 
                        {$this->stNomeTabela}(
                            resid, 
                            inuid, 
                            docid
                        )
                    VALUES(
                       " . $resid . ",
                       " . $municipio['inuid'] . ",
                        " . wf_cadastrarDocumento(TPDID_RESTRICAO_PAR, "Restrição #{$resid}para o Município: {$municipio['listaMunicipio']}", ESDID_EM_CADASTRAMENTO) . "
                    )
                ";
                $this->executar($sql);
            }
            return;
        } catch (Exception $exc) {
            $this->rollback();
            echo $exc->getTraceAsString();
        }
    }

    /**
     * Insere Restrições de entidade vinculada a uma UF
     * @param array $arrPost
     * @param int $resid
     * @return bool
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function insertRestricaoEntidadeUf(array $arrPost, $resid) {
        try {
            $sqlUf = "
            SELECT DISTINCT
               estuf,
               inuid
            FROM
                par.instrumentounidade i
            WHERE
                i.estuf IN('" . join("', '", $arrPost['estuf']) . "')";

            $arrUf = $this->carregar($sqlUf);
            foreach ($arrUf as $uf) {
                $sql = "
                    INSERT INTO 
                        {$this->stNomeTabela}(
                            resid, 
                            inuid, 
                            docid
                        )
                    VALUES(
                       " . $resid . ",
                       " . $uf['inuid'] . ",
                        " . wf_cadastrarDocumento(TPDID_RESTRICAO_PAR, "Restrição #{$resid}para o Estado: {$uf['estuf']}", ESDID_EM_CADASTRAMENTO) . "
                    )
                ";
                $this->executar($sql);
            }
            return;
        } catch (Exception $exc) {
            $this->rollback();
            echo $exc->getTraceAsString();
        }
    }

    /**
     * Deleta a relacao da entidade com a restricao
     * @param int $renid
     * @return bool
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function delete($resid) {
        $sql = "
            DELETE FROM 
                {$this->stNomeTabela}
            WHERE resid = {$resid}";

        return $this->executar($sql);
    }

    /**
     * Seleciona Restricoes Entidade pro $resid
     * @param int $resid
     * @return bool
     * @author Jair Foro <jairforo@gmail.com>
     */
    public function buscarPorResid($resid, $campo = '*') {
        $sql = "
            SELECT 
            {$campo} 
            FROM {$this->stNomeTabela}
            WHERE resid = {$resid}
        ";

        return $this->carregar($sql);
    }

    public function atualizarRestricaoEntidade($arrPost, $resid) {
        $this->delete($resid);
        switch ($arrPost['esfera']) {
            case 'T':
                # Salva lista de documentos PAR
                if ($arrPost['listaTermo']) {
                    return $this->insertRestricaoEntidadeTermo($arrPost, $resid);
                }
                break;
            case 'E':
                # Salva lista de Estados
                if (!empty($arrPost['estuf'])) {
                    return $this->insertRestricaoEntidadeUf($arrPost, $resid);
                }
                break;
            case 'M':
                # Manter lista de Municipios
                if (!empty($arrPost['listaMunicipio'])) {
                    return $this->insertRestricaoEntidadeMunicipio($arrPost, $resid);
                }
                break;
        }
    }

}
