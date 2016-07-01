<?php

/**
 * Class Ted_Model_Coordenacao
 */
class Ted_Model_Coordenacao extends Modelo
{
    /**
     * @var array|void
     */
    protected $_situacao;

    /**
     * @var
     */
    protected $_politica;

    /**
     * @var
     */
    protected $_tcpid;

    /**
     * @var array
     */
    protected $_perfis = array(
        PERFIL_SECRETARIA,
        PERFIL_SUPER_USUARIO
    );

    /**
     *
     */
    public function __construct()
    {
        $this->_tcpid = Ted_Utils_Model::capturaTcpid();
        if (!$this->_tcpid) {
            throw new Exception('Tcpid is null');
        }
        $this->_situacao = Ted_Utils_Model::pegaSituacaoTed();
        $this->_getDadosPolitica();
    }

    /**
     * @return $this
     */
    protected function _getDadosPolitica()
    {
        $strSQL = sprintf("
            select
                dircod, ungcodconcedente, cooid, dircodpoliticafnde, ungcodpoliticafnde
            from ted.termocompromisso
            where tcpid = %d
        ", $this->_tcpid);

        $this->_politica = $this->pegalinha($strSQL);

        return $this;
    }

    /**
     * @return string
     */
    public function getSqlCoordenacao()
    {
        if ($this->_situacao['esdid'] == EM_ANALISE_DA_SECRETARIA
            && Ted_Utils_Model::possuiPerfil($this->_perfis)) {

            if ($this->_politica['ungcodconcedente'] == UG_FNDE && !empty($this->_politica['dircodpoliticafnde'])) {
                $strSQL = "
                    select
                        cooid, coodsc
                    from ted.coordenacao
                    where dircod = '{$this->_politica['dircodpoliticafnde']}'
                    order by coodsc
                ";
            } else if ($this->_politica['ungcodconcedente'] == UG_FNDE && !empty($this->_politica['ungcodpoliticafnde'])) {
                $strSQL = "
                    select
                        cooid, coodsc
                    from ted.coordenacao
                    where ungcodconcedente = '{$this->_politica['ungcodpoliticafnde']}'
                    order by coodsc
                ";
            } else {
                $strSQL = "
                    select
                        cooid, coodsc
                    from ted.coordenacao
                    where ungcodconcedente = '{$this->_politica['ungcodconcedente']}'
                    order by coodsc
                ";
            }

            $collection = $this->carrecar($strSQL);
            $options = array();
            foreach ($collection as $row) {
                $options[$row['cooid']] = $row['coodsc'];
            }

            return $options;
        }
    }

    /**
     *
     */
    public function core()
    {
        if ($this->pegaUm($sql)) {
            echo '<b>Selecione uma Coordenação </b><br/>';
            $db->monta_combo('cooid',$sql,'S', 'Selecione','salvaCoordenacao','','','200','N','dircod','',$dado['cooid']);
            echo "<br><br>";
        } else {
            echo '<b><font color=red>É necessário preencher a aba Concedente para selecionar uma Coordenação.</font></b><br><br>';
            echo '<b>Selecione uma Coordenação </b><br/>';
            $db->monta_combo('cooid',$sql,'S', 'Selecione','salvaCoordenacao','','','200','N','dircod','',$dado['cooid']);
            echo "<br><br>";
        }
    }

}